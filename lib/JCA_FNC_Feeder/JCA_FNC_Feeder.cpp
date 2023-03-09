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

namespace JCA {
  namespace FNC {
    const char *Feeder::FeedingHour_Name = "FeedingHour";
    const char *Feeder::FeedingHour_Text = "Fütterung Stunde";
    const char *Feeder::FeedingHour_Unit = "h";
    const char *Feeder::FeedingHour_Comment = nullptr;
    const char *Feeder::FeedingMinute_Name = "FeedingMinute";
    const char *Feeder::FeedingMinute_Text = "Fütterung Minute";
    const char *Feeder::FeedingMinute_Unit = "m";
    const char *Feeder::FeedingMinute_Comment = nullptr;
    const char *Feeder::SteppsPerRotation_Name = "SteppsPerRotation";
    const char *Feeder::SteppsPerRotation_Text = "Schritte pro Umdrehung";
    const char *Feeder::SteppsPerRotation_Unit = "st/rot";
    const char *Feeder::SteppsPerRotation_Comment = nullptr;
    const char *Feeder::FeedingRotations_Name = "FeedingRotations";
    const char *Feeder::FeedingRotations_Text = "Umdrehungen je Fütterung";
    const char *Feeder::FeedingRotaions_Unit = "rot";
    const char *Feeder::FeedingRotaions_Comment = nullptr;
    const char *Feeder::Acceleration_Name = "Acceleration";
    const char *Feeder::Acceleration_Text = "Beschleuningung";
    const char *Feeder::Acceleration_Unit = "st/s2";
    const char *Feeder::Acceleration_Comment = nullptr;
    const char *Feeder::MaxSpeed_Name = "MaxSpeed";
    const char *Feeder::MaxSpeed_Text = "Maximale Geschwindigkeit";
    const char *Feeder::MaxSpeed_Unit = "st/s";
    const char *Feeder::MaxSpeed_Comment = nullptr;
    const char *Feeder::ConstSpeed_Name = "ConstSpeed";
    const char *Feeder::ConstSpeed_Text = "Konstant Geschwindigkeit";
    const char *Feeder::ConstSpeed_Unit = "st/s";
    const char *Feeder::ConstSpeed_Comment = nullptr;
    const char *Feeder::Feeding_Name = "Feeding";
    const char *Feeder::Feeding_Text = "Fütterung aktiv";
    const char *Feeder::Feeding_Comment = nullptr;
    const char *Feeder::Feeding_TextOn = nullptr;
    const char *Feeder::Feeding_TextOff = nullptr;
    const char *Feeder::DistanceToGo_Name = "DistanceToGo";
    const char *Feeder::DistanceToGo_Text = "Verbleibende Schritte";
    const char *Feeder::DistanceToGo_Unit = "st";
    const char *Feeder::DistanceToGo_Comment = nullptr;
    const char *Feeder::RunConst_Name = "RunConst";
    const char *Feeder::RunConst_Text = "Konstante Drehung aktiv";
    const char *Feeder::RunConst_Comment = nullptr;
    const char *Feeder::RunConst_TextOn = nullptr;
    const char *Feeder::RunConst_TextOff = nullptr;
    const char *Feeder::Speed_Name = "Speed";
    const char *Feeder::Speed_Text = "aktuelle Geschwindigkeit";
    const char *Feeder::Speed_Unit = "st/s";
    const char *Feeder::Speed_Comment = nullptr;
    const char *Feeder::CmdDoFeed_Name = "doFeed";
    const char *Feeder::CmdDoFeed_Text = "Jetzt Füttern";
    const char *Feeder::CmdDoFeed_Comment = nullptr;
    const char *Feeder::CmdDoFeed_Type = "bool";
    const char *Feeder::CmdDoFeed_BtnText = "GO";

    /**
     * @brief Construct a new Feeder::Feeder object
     *
     * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
     * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
     * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
     * @param _Name Element Name inside the Communication
     */
    Feeder::Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char *_Name)
        : Parent (_Name), Stepper (AccelStepper::DRIVER, _PinStep, _PinDir) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

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
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void Feeder::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[FeedingHour_Name] = FeedingHour;
      _Values[FeedingMinute_Name] = FeedingMinute;
      _Values[SteppsPerRotation_Name] = SteppsPerRotation;
      _Values[FeedingRotations_Name] = FeedingRotations;
      _Values[Acceleration_Name] = Acceleration;
      _Values[MaxSpeed_Name] = MaxSpeed;
      _Values[ConstSpeed_Name] = ConstSpeed;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void Feeder::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Feeding_Name] = Feeding;
      _Values[DistanceToGo_Name] = Stepper.distanceToGo ();
      _Values[RunConst_Name] = RunConst;
      _Values[Speed_Name] = Stepper.speed ();
      _Values[CmdDoFeed_Name] = false;
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
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == RunConst_Name) {
          RunConst = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, RunConst_Name)) {
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
     * @brief Execute the Commands
     *
     * @param _Tags Array of Commands ("cmd": [])
     */
    void Feeder::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void Feeder::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (FeedingHour_Name, FeedingHour_Text, FeedingHour_Comment, false, FeedingHour_Unit, FeedingHour) + "}");
      _SetupFile.println (",{" + createSetupTag (FeedingMinute_Name, FeedingMinute_Text, FeedingMinute_Comment, false, FeedingMinute_Unit, FeedingMinute) + "}");
      _SetupFile.println (",{" + createSetupTag (SteppsPerRotation_Name, SteppsPerRotation_Text, SteppsPerRotation_Comment, false, SteppsPerRotation_Unit, SteppsPerRotation) + "}");
      _SetupFile.println (",{" + createSetupTag (FeedingRotations_Name, FeedingRotations_Text, FeedingRotaions_Comment, false, FeedingRotaions_Unit, FeedingRotations) + "}");
      _SetupFile.println (",{" + createSetupTag (Acceleration_Name, Acceleration_Text, Acceleration_Comment, false, Acceleration_Unit, Acceleration) + "}");
      _SetupFile.println (",{" + createSetupTag (MaxSpeed_Name, MaxSpeed_Text, MaxSpeed_Comment, false, MaxSpeed_Unit, MaxSpeed) + "}");
      _SetupFile.println (",{" + createSetupTag (ConstSpeed_Name, ConstSpeed_Text, ConstSpeed_Comment, false, ConstSpeed_Unit, ConstSpeed) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void Feeder::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (Feeding_Name, Feeding_Text, Feeding_Comment, true, Feeding_TextOn, Feeding_TextOff, Feeding) + "}");
      _SetupFile.println (",{" + createSetupTag (DistanceToGo_Name, DistanceToGo_Text, DistanceToGo_Comment, true, DistanceToGo_Unit, Stepper.distanceToGo ()) + "}");
      _SetupFile.println (",{" + createSetupTag (RunConst_Name, RunConst_Text, RunConst_Comment, false, RunConst_TextOn, RunConst_TextOff, RunConst) + "}");
      _SetupFile.println (",{" + createSetupTag (Speed_Name, Speed_Text, Speed_Comment, true, Speed_Unit, Stepper.speed ()) + "}");
      _SetupFile.println (",{" + createSetupCmdInfo (CmdDoFeed_Name, CmdDoFeed_Text, CmdDoFeed_Comment, CmdDoFeed_Type, CmdDoFeed_BtnText) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void Feeder::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
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
    }
  }
}
