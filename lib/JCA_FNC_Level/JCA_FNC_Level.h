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

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagInt16.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    class Level : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagInputPin;

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
      Level (uint8_t _Pin, String _Name);
      ~Level () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif