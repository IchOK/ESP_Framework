/**
 * @file JCA_FNC_Feeder.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element for a daily Feeder. With variable Time and Amount
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_Feeder.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *Feeder::ClassName = "Feeder";
    const char *Feeder::SetupTagType = "feeder";
    const char *Feeder::SetupTagEnablePin = "pinEnable";
    const char *Feeder::SetupTagStepPin = "pinStep";
    const char *Feeder::SetupTagDirPin = "pinDir";
    const char *Feeder::SetupTagInvertEnable = "invertEnable";
    const char *Feeder::SetupTagInvertStep = "invertStep";
    const char *Feeder::SetupTagInvertDir = "invertDir";
    /**
     * @brief Construct a new Feeder::Feeder object
     *
     * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
     * @param _InvertEnable True if the Enable is inverted
     * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
     * @param _InvertStep True if the Step is inverted
     * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
     * @param _InvertDir True if the Direction is inverted
     * @param _Name Element Name inside the Communication
     */
    Feeder::Feeder (uint8_t _PinEnable, bool _InvertEnable, uint8_t _PinStep, bool _InvertStep, uint8_t _PinDir, bool _InvertDir, String _Name)
        : FuncParent (_Name), Stepper (AccelStepper::DRIVER, _PinStep, _PinDir) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      Tags.push_back (new TagInt16 ("FeedingHour", "Fütterung Stunde", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &FeedingHour, "h"));
      Tags.push_back (new TagInt16 ("FeedingMinute", "Fütterung Minute", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &FeedingMinute, "m"));
      Tags.push_back (new TagFloat ("SteppsPerRotation", "Schritte pro Umdrehung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &SteppsPerRotation, "st/rot"));
      Tags.push_back (new TagFloat ("FeedingRotations", "Umdrehungen je Fütterung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &FeedingRotations, "rot"));
      Tags.push_back (new TagFloat ("Acceleration", "Beschleuningung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &Acceleration, "st/s2", std::bind (&Feeder::accelerationCB, this)));
      Tags.push_back (new TagFloat ("MaxSpeed", "Maximale Geschwindigkeit", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &MaxSpeed, "st/s", std::bind (&Feeder::maxSpeedCB, this)));
      Tags.push_back (new TagFloat ("ConstSpeed", "Konstant Geschwindigkeit", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &ConstSpeed, "st/s", std::bind (&Feeder::constSpeedCB, this)));

      Tags.push_back (new TagBool ("Feeding", "Fütterung aktiv", "", TagAccessType_T::ReadWrite, TagUsage_T::UseData, &Feeding, "EIN", "AUS"));
      Tags.push_back (new TagInt32 ("DistanceToGo", "Verbleibende Schritte", "", TagAccessType_T::Read, TagUsage_T::UseData, &DistanceToGo, "st"));
      Tags.push_back (new TagBool ("RunConst", "Konstante Drehung aktiv", "", TagAccessType_T::ReadWrite, TagUsage_T::UseData, &RunConst, "EIN", "AUS"));
      Tags.push_back (new TagFloat ("Speed", "aktuelle Geschwindigkeit", "", TagAccessType_T::Read, TagUsage_T::UseData, &Speed, "st/s"));

      Tags.push_back (new TagBool ("CmdDoFeed", "Jetzt Füttern", "", TagAccessType_T::ReadWrite, TagUsage_T::UseData, &DoFeed, "Aktiv", "Start", std::bind (&Feeder::doFeedCB, this)));

      // Intern
      DoFeed = false;
      AutoFeedDone = false;
      DistanceToGo = 0;
      Speed = 0.0;

      // Konfig
      FeedingHour = -1;
      FeedingMinute = -1;
      SteppsPerRotation = 0.0;
      FeedingRotations = 0.0;
      Acceleration = 0.0;
      MaxSpeed = 0.0;
      ConstSpeed = 0.0;

      // Daten
      RunConst = false;
      Feeding = false;

      // Hardware
      Stepper.setEnablePin (_PinEnable);
      Stepper.setPinsInverted (_InvertDir, _InvertStep, _InvertEnable);
      Stepper.disableOutputs ();
    }

    void Feeder::doFeedCB() {
      if (DoFeed) {
        RunConst = false;
      }
    }

    void Feeder::accelerationCB() {
      Stepper.setAcceleration (Acceleration);
    }

    void Feeder::maxSpeedCB() {
      Stepper.setMaxSpeed (MaxSpeed);
    }

    void Feeder::constSpeedCB() {
      Stepper.setSpeed (ConstSpeed);
    }

    /**
     * @brief Handling the Feeder
     * Handling the Stepper-Functions and check if Feeding is requested
     * @param _Time Current Time to check automated feeding
     */
    void Feeder::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      bool AutoFeed = FeedingHour == _Time.tm_hour && FeedingMinute == _Time.tm_min && _Time.tm_year > 100;

      // Run const Speed
      if (RunConst) {
        // Constant Mode
        Stepper.setSpeed (ConstSpeed);
        Stepper.enableOutputs ();
        Stepper.runSpeed ();
        DoFeed = false;
        Feeding = false;
      } else {
        // Dosing Mode
        if ((AutoFeed && !AutoFeedDone) || DoFeed) {
          Debug.println (FLAG_LOOP, false, Name, __func__, "Start Feeding");
          Stepper.move ((long)(SteppsPerRotation * FeedingRotations));
          Stepper.enableOutputs ();
          Feeding = true;
          DoFeed = false;
        }
        if (Feeding) {
          if (Stepper.distanceToGo () == 0) {
            Debug.println (FLAG_LOOP, false, Name, __func__, "Done Feeding");
            Feeding = false;
          }
        } else {
          Stepper.stop ();
          Stepper.disableOutputs ();
        }

        Stepper.run ();
      }
      AutoFeedDone = AutoFeed;
      DistanceToGo = Stepper.distanceToGo ();
      Speed = Stepper.speed ();
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void Feeder::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
      _Handler.FunctionSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> (SetupTagType, GetSetupSchema));
    }

    /**
     * @brief Create a new Instanz of the Class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the Instanz to
     * @param _Hardware List of knowen Hardware-References
     * @return true
     * @return false
     */
    bool Feeder::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t PinEnable = GetSetupValueUINT8 (SetupTagEnablePin, Done, _Setup, Log);
      uint8_t PinStep = GetSetupValueUINT8 (SetupTagStepPin, Done, _Setup, Log);
      uint8_t PinDir = GetSetupValueUINT8 (SetupTagDirPin, Done, _Setup, Log);
      bool InvertEnable = GetSetupValueBOOL (SetupTagInvertEnable, Done, _Setup, Log);
      bool InvertStep = GetSetupValueBOOL (SetupTagInvertStep, Done, _Setup, Log);
      bool InvertDir = GetSetupValueBOOL (SetupTagInvertDir, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new Feeder (PinEnable, InvertEnable, PinStep, InvertStep, PinDir, InvertDir, Name));
        Log["done"] = Name + " (EnablePin:" + String (PinEnable) + ", InvertEnable:" + String (InvertEnable) + ", StepPin: " + String (PinStep) + ", InvertStep:" + String (InvertStep) + ", DirPin: " + String (PinDir) + ", InvertDir:" + String (InvertDir) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }

    void Feeder::GetSetupSchema(JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray>();
      
      // name parameter
      JsonObject NameParam = Parameters.add<JsonObject>();
      NameParam["name"] = JCA_IOT_FUNCHANDLER_SETUP_NAME;
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Funktion für die Kommunikation";

      // enablePin parameter
      JsonObject EnablePinParam = Parameters.add<JsonObject>();
      EnablePinParam["name"] = SetupTagEnablePin;
      EnablePinParam["type"] = "uint8";
      EnablePinParam["comment"] = "Pin für den Enable";

      // invertEnable parameter
      JsonObject InvertEnableParam = Parameters.add<JsonObject>();
      InvertEnableParam["name"] = SetupTagInvertEnable;
      InvertEnableParam["type"] = "bool";
      InvertEnableParam["comment"] = "Invert Enable";

      // stepPin parameter
      JsonObject StepPinParam = Parameters.add<JsonObject>();
      StepPinParam["name"] = SetupTagStepPin;
      StepPinParam["type"] = "uint8";
      StepPinParam["comment"] = "Pin für den Step";

      // invertStep parameter
      JsonObject InvertStepParam = Parameters.add<JsonObject>();
      InvertStepParam["name"] = SetupTagInvertStep;
      InvertStepParam["type"] = "bool";
      InvertStepParam["comment"] = "Invert Step";

      // dirPin parameter
      JsonObject DirPinParam = Parameters.add<JsonObject>();
      DirPinParam["name"] = SetupTagDirPin;
      DirPinParam["type"] = "uint8";
      DirPinParam["comment"] = "Pin für den Direction";

      // invertDir parameter
      JsonObject InvertDirParam = Parameters.add<JsonObject>();
      InvertDirParam["name"] = SetupTagInvertDir;
      InvertDirParam["type"] = "bool";
      InvertDirParam["comment"] = "Invert Direction";
    }
  }
}
