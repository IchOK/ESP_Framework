/**
 * @file JCA_FNC_Level.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a Level by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_LEVEL_
#define _JCA_FNC_LEVEL_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_ElementTagBool.h>
#include <JCA_FNC_ElementTagFloat.h>
#include <JCA_FNC_ElementTagInt16.h>
#include <JCA_FNC_ElementTagUInt16.h>
#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Level : public Parent {
    private:
      // Hardware
      uint8_t Pin;

      // Konfig
      int16_t RawEmpty;
      int16_t RawFull;
      uint16_t ReadInterval;
      float AlarmLevel;

      // Daten
      float Value;
      bool Alarm;
      int16_t RawValue;

      // Intern
      int8_t LastSeconds;
      uint16_t IntervalCount;

    public:
      Level (uint8_t _Pin, const char *_Name);
      void update (struct tm &_Time);
      float getValue ();
      bool getAlarm ();
    };
  }
}

#endif