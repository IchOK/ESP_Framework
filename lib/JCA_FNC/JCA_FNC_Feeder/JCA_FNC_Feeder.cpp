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
    /**
     * @brief Construct a new Feeder::Feeder object
     *
     * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
     * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
     * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
     * @param _Name Element Name inside the Communication
     */
    Feeder::Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, String _Name)
        : FuncParent (_Name), Stepper (AccelStepper::DRIVER, _PinStep, _PinDir) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      Tags.push_back (new TagInt16 ("FeedingHour", "Fütterung Stunde", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &FeedingHour, "h"));
      Tags.push_back (new TagInt16 ("FeedingMinute", "Fütterung Minute", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &FeedingMinute, "m"));
      Tags.push_back (new TagFloat ("SteppsPerRotation", "Schritte pro Umdrehung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &SteppsPerRotation, "st/rot"));
      Tags.push_back (new TagFloat ("FeedingRotations", "Umdrehungen je Fütterung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &FeedingRotations, "rot"));
      Tags.push_back (new TagFloat ("Acceleration", "Beschleuningung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &Acceleration, "st/s2"));
      Tags.push_back (new TagFloat ("MaxSpeed", "Maximale Geschwindigkeit", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &MaxSpeed, "st/s"));
      Tags.push_back (new TagFloat ("ConstSpeed", "Konstant Geschwindigkeit", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &ConstSpeed, "st/s"));

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
      Stepper.setPinsInverted (false, false, true);
      Stepper.disableOutputs ();
    }

    void Feeder::doFeedCB() {
      if (DoFeed) {
        RunConst = false;
      }
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
        Stepper.runSpeed ();
        DoFeed = false;
      } else {
        // Dosing Mode
        if ((AutoFeed && !AutoFeedDone) || DoFeed) {
          Debug.println (FLAG_LOOP, false, Name, __func__, "Start Feeding");
          Stepper.move ((long)(SteppsPerRotation * FeedingRotations));
          Stepper.enableOutputs ();
          Feeding = true;
          DoFeed = false;
        }
        if (Stepper.distanceToGo () == 0 && Feeding) {
          Debug.println (FLAG_LOOP, false, Name, __func__, "Done Feeding");
          Stepper.disableOutputs ();
          Feeding = false;
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

      if (Done) {
        _Functions.push_back (new Feeder (PinEnable, PinStep, PinDir, Name));
        Log["done"] = Name + " (EnablePin:" + String (PinEnable) + ", StepPin: " + String (PinStep) + ", DirPin: " + String (PinDir) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
