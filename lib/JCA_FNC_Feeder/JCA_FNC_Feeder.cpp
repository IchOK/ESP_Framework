
#include <JCA_FNC_Feeder.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Feeder::FeedingHour_Name = "FeedingHour";
    const char *Feeder::FeedingHour_Unit = "h";
    const char *Feeder::FeedingHour_Comment = nullptr;
    const char *Feeder::FeedingMinute_Name = "FeedingMinute";
    const char *Feeder::FeedingMinute_Unit = "m";
    const char *Feeder::FeedingMinute_Comment = nullptr;
    const char *Feeder::SteppsPerRotation_Name = "SteppsPerRotation";
    const char *Feeder::SteppsPerRotation_Unit = "st/rot";
    const char *Feeder::SteppsPerRotation_Comment = nullptr;
    const char *Feeder::FeedingRotations_Name = "FeedingRotations";
    const char *Feeder::FeedingRotaions_Unit = "rot";
    const char *Feeder::FeedingRotaions_Comment = nullptr;
    const char *Feeder::Acceleration_Name = "Acceleration";
    const char *Feeder::Acceleration_Unit = "st/s2";
    const char *Feeder::Acceleration_Comment = nullptr;
    const char *Feeder::MaxSpeed_Name = "MaxSpeed";
    const char *Feeder::MaxSpeed_Unit = "st/s";
    const char *Feeder::MaxSpeed_Comment = nullptr;
    const char *Feeder::ConstSpeed_Name = "ConstSpeed";
    const char *Feeder::ConstSpeed_Unit = "st/s";
    const char *Feeder::ConstSpeed_Comment = nullptr;
    const char *Feeder::Feeding_Name = "Feeding";
    const char *Feeder::Feeding_Comment = nullptr;
    const char *Feeder::Feeding_TextOn = nullptr;
    const char *Feeder::Feeding_TextOff = nullptr;
    const char *Feeder::DistanceToGo_Name = "DistanceToGo";
    const char *Feeder::DistanceToGo_Unit = "st";
    const char *Feeder::DistanceToGo_Comment = nullptr;
    const char *Feeder::RunConst_Name = "RunConst";
    const char *Feeder::RunConst_Comment = nullptr;
    const char *Feeder::RunConst_TextOn = nullptr;
    const char *Feeder::RunConst_TextOff = nullptr;
    const char *Feeder::Speed_Name = "Speed";
    const char *Feeder::Speed_Unit = "st/s";
    const char *Feeder::Speed_Comment = nullptr;
    const char *Feeder::CmdRunConst_Name = "runConst";
    const char *Feeder::CmdRunConst_Comment = nullptr;
    const char *Feeder::CmdRunConst_Type = "bool";
    const char *Feeder::CmdRunConst_TextOn = nullptr;
    const char *Feeder::CmdRunConst_TextOff = nullptr;
    const char *Feeder::CmdDoFeed_Name = "doFeed";
    const char *Feeder::CmdDoFeed_Comment = nullptr;
    const char *Feeder::CmdDoFeed_Type = "bool";
    const char *Feeder::CmdDoFeed_TextOn = nullptr;
    const char *Feeder::CmdDoFeed_TextOff = nullptr;

    /**
     * @brief Construct a new Feeder::Feeder object
     *
     * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
     * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
     * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
     * @param _Name Element Name inside the Communication
     */
    Feeder::Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char *_Name)
        : Protocol (_Name), Stepper (AccelStepper::DRIVER, _PinStep, _PinDir) {

      // Intern
      DoFeed = false;
      AutoFeedDone = false;

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

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void Feeder::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == FeedingHour_Name) {
          FeedingHour = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, FeedingHour_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, FeedingHour);
          }
        }
        if (Tag[JsonTagName] == FeedingMinute_Name) {
          FeedingMinute = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, FeedingMinute_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, FeedingMinute);
          }
        }
        if (Tag[JsonTagName] == SteppsPerRotation_Name) {
          SteppsPerRotation = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, SteppsPerRotation_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, SteppsPerRotation);
          }
        }
        if (Tag[JsonTagName] == FeedingRotations_Name) {
          FeedingRotations = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, FeedingRotations_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, FeedingRotations);
          }
        }
        if (Tag[JsonTagName] == Acceleration_Name) {
          Acceleration = Tag[JsonTagValue].as<float> ();
          Stepper.setAcceleration (Acceleration);
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, Acceleration_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, Acceleration);
          }
        }
        if (Tag[JsonTagName] == MaxSpeed_Name) {
          MaxSpeed = Tag[JsonTagValue].as<float> ();
          Stepper.setMaxSpeed (MaxSpeed);
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MaxSpeed_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MaxSpeed);
          }
        }
        if (Tag[JsonTagName] == ConstSpeed_Name) {
          ConstSpeed = Tag[JsonTagValue].as<float> ();
          Stepper.setSpeed (ConstSpeed);
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ConstSpeed_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ConstSpeed);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void Feeder::setData (JsonArray _Tags) {
    }

    /**
     * @brief Execute the Commands
     * 
     * @param _Tags Array of Commands ("cmd": [])
     */
    void Feeder::setCmd (JsonArray _Tags) {
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == CmdRunConst_Name) {
          RunConst = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, CmdRunConst_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, RunConst);
          }
          if (RunConst) {
            Stepper.enableOutputs ();
          } else {
            Stepper.disableOutputs ();
          }
        }

        if (Tag[JsonTagName] == CmdDoFeed_Name) {
          DoFeed = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, CmdDoFeed_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, DoFeed);
          }
          if (DoFeed) {
            RunConst = false;
          }
        }
      }
    }

    /**
     * @brief Create a list of Config-Tags containing the current Value
     * 
     * @param _Tags Array the Tags have to add
     */
    void Feeder::createConfigTags (JsonArray &_Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      createTag (_Tags, FeedingHour_Name, FeedingHour_Comment, FeedingHour_Unit, FeedingHour);
      createTag (_Tags, FeedingMinute_Name, FeedingMinute_Comment, FeedingMinute_Unit, FeedingMinute);
      createTag (_Tags, SteppsPerRotation_Name, SteppsPerRotation_Comment, SteppsPerRotation_Unit, SteppsPerRotation);
      createTag (_Tags, FeedingRotations_Name, FeedingRotaions_Comment, FeedingRotaions_Unit, FeedingRotations);
      createTag (_Tags, Acceleration_Name, Acceleration_Comment, Acceleration_Unit, Acceleration);
      createTag (_Tags, MaxSpeed_Name, MaxSpeed_Comment, MaxSpeed_Unit, MaxSpeed);
      createTag (_Tags, ConstSpeed_Name, ConstSpeed_Comment, ConstSpeed_Unit, ConstSpeed);
    }

    /**
     * @brief Create a list of Data-Tags containing the current Value
     *
     * @param _Tags Array the Tags have to add
     */
    void Feeder::createDataTags (JsonArray &_Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      createTag (_Tags, Feeding_Name, Feeding_Comment, Feeding_TextOn, Feeding_TextOff, Feeding);
      createTag (_Tags, DistanceToGo_Name, DistanceToGo_Comment, DistanceToGo_Unit, Stepper.distanceToGo ());
      createTag (_Tags, RunConst_Name, RunConst_Comment, RunConst_TextOn, RunConst_TextOff, RunConst);
      createTag (_Tags, Speed_Name, Speed_Comment, Speed_Unit, Stepper.speed ());
    }

    /**
     * @brief Create a list of Command-Informations
     *
     * @param _Tags Array the Command-Infos have to add
     */
    void Feeder::createCmdInfoTags (JsonArray &_Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      createCmdInfo (_Tags, CmdRunConst_Name, CmdRunConst_Comment, CmdRunConst_Type, CmdRunConst_TextOn, CmdRunConst_TextOff);
      createCmdInfo (_Tags, CmdDoFeed_Name, CmdDoFeed_Comment, CmdDoFeed_Type, CmdDoFeed_TextOn, CmdDoFeed_TextOff);
    }

    /**
     * @brief Handling the Feeder
     * Handling the Stepper-Functions and check if Feeding is requested
     * @param _Time Current Time to check automated feeding
     */
    void Feeder::update (struct tm &_Time) {
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
    }
  }
}
