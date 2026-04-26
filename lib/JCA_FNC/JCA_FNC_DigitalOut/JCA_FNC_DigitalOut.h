/**
 * @file JCA_FNC_DigitalOut.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a DigitalOut by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_DIGITALOUT_
#define _JCA_FNC_DIGITALOUT_

#include <ArduinoJson.h>
#include <time.h>

#include "JCA_FNC_Parent.h"
#include "JCA_IOT_FuncHandler.h"

namespace JCA {
  namespace FNC {
    class DigitalOut : public FuncParent {
    private:
      enum DelayAutoOffUnit_T : uint8_t {
        DELAY_UNIT_H = 0,
        DELAY_UNIT_M = 1,
        DELAY_UNIT_S = 2,
        DELAY_UNIT_MS = 3
      };

      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagOutputPin;

      // Hardware
      uint8_t Pin;

      // Konfig
      uint16_t DelayAutoOff;
      uint8_t DelayAutoOffUnit;

      // Daten
      bool Value;
      uint16_t DelayCounter;

      // Intern
      unsigned long LastMillis;
      uint32_t DelayMillis;
      uint32_t getDelayAutoOffUnitMillis () const;

    public:
      DigitalOut (uint8_t _Pin, String _Name);
      ~DigitalOut () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
      static void GetSetupSchema (JsonObject &_Schema);
    };
  }
}

#endif