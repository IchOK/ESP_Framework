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
      Tags.push_back (new ElementTagInt16 ("FeedingHour", "Fütterung Stunde", "", false, ElementTagUsage_T::UseConfig, &FeedingHour, "h"));
      Tags.push_back (new ElementTagInt16 ("FeedingMinute", "Fütterung Minute", "", false, ElementTagUsage_T::UseConfig, &FeedingMinute, "m"));
      Tags.push_back (new ElementTagFloat ("SteppsPerRotation", "Schritte pro Umdrehung", "", false, ElementTagUsage_T::UseConfig, &SteppsPerRotation, "st/rot"));
      Tags.push_back (new ElementTagFloat ("FeedingRotations", "Umdrehungen je Fütterung", "", false, ElementTagUsage_T::UseConfig, &FeedingRotations, "rot"));
      Tags.push_back (new ElementTagFloat ("Acceleration", "Beschleuningung", "", false, ElementTagUsage_T::UseConfig, &Acceleration, "st/s2"));
      Tags.push_back (new ElementTagFloat ("MaxSpeed", "Maximale Geschwindigkeit", "", false, ElementTagUsage_T::UseConfig, &MaxSpeed, "st/s"));
      Tags.push_back (new ElementTagFloat ("ConstSpeed", "Konstant Geschwindigkeit", "", false, ElementTagUsage_T::UseConfig, &ConstSpeed, "st/s"));

      Tags.push_back (new ElementTagBool ("Feeding", "Fütterung aktiv", "", false, ElementTagUsage_T::UseData, &Feeding, "EIN", "AUS"));
      Tags.push_back (new ElementTagInt32 ("DistanceToGo", "Verbleibende Schritte", "", true, ElementTagUsage_T::UseData, &DistanceToGo, "st"));
      Tags.push_back (new ElementTagBool ("RunConst", "Konstante Drehung aktiv", "", false, ElementTagUsage_T::UseData, &RunConst, "EIN", "AUS"));
      Tags.push_back (new ElementTagFloat ("Speed", "aktuelle Geschwindigkeit", "", true, ElementTagUsage_T::UseData, &Speed, "st/s"));

      Tags.push_back (new ElementTagBool ("CmdDoFeed", "Jetzt Füttern", "", false, ElementTagUsage_T::UseData, &DoFeed, "Aktiv", "Start", std::bind (&Feeder::doFeedCB, this)));

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
  }
}
