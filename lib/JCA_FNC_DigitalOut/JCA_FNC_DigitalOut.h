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

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_FNC_ElementTagBool.h>
#include <JCA_FNC_ElementTagUInt16.h>

namespace JCA {
  namespace FNC {
    class DigitalOut : public Parent {
    private:
      // Hardware
      uint8_t Pin;

      // Konfig
      uint16_t DelayAutoOff;

      // Daten
      bool Value;
      uint16_t DelayCounter;

      // Intern
      uint32_t LastMillis;
      uint16_t DelayMillis;

    public:
      DigitalOut (uint8_t _Pin, const char *_Name);
      void update (struct tm &_Time);
      bool getValue ();
    };
  }
}

#endif