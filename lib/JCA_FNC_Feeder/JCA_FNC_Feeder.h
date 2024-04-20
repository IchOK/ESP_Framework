/**
 * @file JCA_FNC_Feeder.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element for a daily Feeder. With variable Time and Amount
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_FEEDER_
#define _JCA_FNC_FEEDER_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_FNC_ElementTagInt16.h>
#include <JCA_FNC_ElementTagFloat.h>
#include <JCA_FNC_ElementTagBool.h>
#include <JCA_FNC_ElementTagInt32.h>


namespace JCA {
  namespace FNC {
    class Feeder : public Parent {
    private:
      // Hardware
      AccelStepper Stepper;

      // Konfig
      int16_t FeedingHour;
      int16_t FeedingMinute;
      float SteppsPerRotation;
      float FeedingRotations;
      float Acceleration;
      float MaxSpeed;
      float ConstSpeed;

      // Daten
      bool RunConst;
      bool Feeding;

      // Intern
      bool DoFeed;
      bool AutoFeedDone;
      int32_t DistanceToGo;
      float Speed;
      void doFeedCB();

    public:
      Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char *_Name);
      void update (struct tm &_Time);
    };
  }
}

#endif