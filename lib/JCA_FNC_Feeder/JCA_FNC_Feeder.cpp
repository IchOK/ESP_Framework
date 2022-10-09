
#include <JCA_FNC_Feeder.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Feeder::NameFeedingHour = "FeedingHour";
    const char *Feeder::NameFeedingMinute = "FeedingMinute";
    const char *Feeder::NameSteppsPerRotation = "SteppsPerRotation";
    const char *Feeder::NameFeedingRotations = "FeedingRotations";
    const char *Feeder::NameAcceleration = "Acceleration";
    const char *Feeder::NameMaxSpeed = "MaxSpeed";
    const char *Feeder::NameConstSpeed = "ConstSpeed";
    const char *Feeder::UnitFeedingHour = "h";
    const char *Feeder::UnitFeedingMinute = "m";
    const char *Feeder::UnitSteppsPerRotation = "st/rot";
    const char *Feeder::UnitFeedingRotaions = "rot";
    const char *Feeder::UnitAcceleration = "st/s2";
    const char *Feeder::UnitMaxSpeed = "st/s";
    const char *Feeder::UnitConstSpeed = "st/s";
    const char *Feeder::NameFeeding = "Feeding";
    const char *Feeder::NameDistanceToGo = "DistanceToGo";
    const char *Feeder::NameRunConst = "RunConst";
    const char *Feeder::NameSpeed = "Speed";
    const char *Feeder::UnitFeeding = "";
    const char *Feeder::UnitDistanceToGo = "st";
    const char *Feeder::UnitRunConst = "";
    const char *Feeder::UnitSpeed = "st/s";
    const char *Feeder::CmdRunConst = "runConst";
    const char *Feeder::CmdDoFeed = "doFeed";

    Feeder::Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char *_Name)
        : Parent(_Name)
        , Stepper (AccelStepper::DRIVER, _PinStep, _PinDir) {

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
          Debug.println (FLAG_LOOP, false, ObjectName, __func__, "Start Feeding");
          Stepper.move ((long)(SteppsPerRotation * FeedingRotations));
          Stepper.enableOutputs ();
          Feeding = true;
          DoFeed = false;
        }
        if (Stepper.distanceToGo () == 0 && Feeding) {
          Debug.println (FLAG_LOOP, false, ObjectName, __func__, "Done Feeding");
          Stepper.disableOutputs ();
          Feeding = false;
        }

        Stepper.run ();
      }
      AutoFeedDone = AutoFeed;
    }

    void Feeder::createConfig (JsonObject &_Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Get");
      createTag (_Data, NameFeedingHour, UnitFeedingHour, FeedingHour);
      createTag (_Data, NameFeedingMinute, UnitFeedingMinute, FeedingHour);
      createTag (_Data, NameSteppsPerRotation, UnitSteppsPerRotation, SteppsPerRotation);
      createTag (_Data, NameFeedingRotations, UnitFeedingRotaions, FeedingRotations);
      createTag (_Data, NameAcceleration, UnitAcceleration, Acceleration);
      createTag (_Data, NameMaxSpeed, UnitMaxSpeed, MaxSpeed);
      createTag (_Data, NameConstSpeed, UnitConstSpeed, ConstSpeed);
    }

    void Feeder::createData (JsonObject &_Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Get");
      createTag (_Data, NameFeeding, UnitFeeding, Feeding);
      createTag (_Data, NameDistanceToGo, UnitDistanceToGo, Stepper.distanceToGo ());
      createTag (_Data, NameRunConst, UnitRunConst, RunConst);
      createTag (_Data, NameSpeed, UnitSpeed, Stepper.speed ());
    }

    void Feeder::setConfig (JsonObject _Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Set");
      if (_Data.containsKey (NameFeedingHour)) {
        FeedingHour = _Data[NameFeedingHour].as<int16_t> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameFeedingHour)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, FeedingHour);
        }
      }
      if (_Data.containsKey (NameFeedingMinute)) {
        FeedingMinute = _Data[NameFeedingMinute].as<int16_t> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameFeedingMinute)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, FeedingMinute);
        }
      }
      if (_Data.containsKey (NameSteppsPerRotation)) {
        SteppsPerRotation = _Data[NameSteppsPerRotation].as<float> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameSteppsPerRotation)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, SteppsPerRotation);
        }
      }
      if (_Data.containsKey (NameFeedingRotations)) {
        FeedingRotations = _Data[NameFeedingRotations].as<float> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameFeedingRotations)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, FeedingRotations);
        }
      }
      if (_Data.containsKey (NameAcceleration)) {
        Acceleration = _Data[NameAcceleration].as<float> ();
        Stepper.setAcceleration (Acceleration);
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameAcceleration)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, Acceleration);
        }
      }
      if (_Data.containsKey (NameMaxSpeed)) {
        MaxSpeed = _Data[NameMaxSpeed].as<float> ();
        Stepper.setMaxSpeed (MaxSpeed);
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameMaxSpeed)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, MaxSpeed);
        }
      }
      if (_Data.containsKey (NameConstSpeed)) {
        ConstSpeed = _Data[NameConstSpeed].as<float> ();
        Stepper.setSpeed (ConstSpeed);
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameConstSpeed)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ConstSpeed);
        }
      }
    }

    void Feeder::setData (JsonObject _Data) {
      if (_Data.containsKey (CmdRunConst)) {
        RunConst = _Data[CmdRunConst].as<bool> ();
        if (Debug.print (FLAG_LOOP, false, ObjectName, __func__, CmdRunConst)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_LOOP, false, ObjectName, __func__, RunConst);
        }
        if (RunConst) {
          Stepper.enableOutputs ();
        } else {
          Stepper.disableOutputs ();
        }
      }

      if (_Data.containsKey (CmdDoFeed)) {
        DoFeed = _Data[CmdDoFeed].as<bool> ();
        if (Debug.print (FLAG_LOOP, false, ObjectName, __func__, CmdDoFeed)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_LOOP, false, ObjectName, __func__, DoFeed);
        }
        if (DoFeed) {
          RunConst = false;
        }
      }
    }
  }
}
