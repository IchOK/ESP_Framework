/**
 * @file JCA_FNC_LedStrip.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a LedStrip by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_LEDSTRIP_
#define _JCA_FNC_LEDSTRIP_

#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>

#include <JCA_FNC_ElementTagBool.h>
#include <JCA_FNC_ElementTagUInt32.h>
#include <JCA_FNC_ElementTagUInt16.h>
#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class LedStrip : public Parent {
    private:
      // Hardware
      Adafruit_NeoPixel Strip;

      // Konfig
      uint16_t DelayAutoOff;

      // Daten
      bool OnOff;
      uint32_t Value;
      uint16_t DelayCounter;

      // Intern
      uint32_t LastMillis;
      uint16_t UpdateMillis;
      uint16_t DelayMillis;
      void updateColorCB ();

    public:
      LedStrip (uint8_t _Pin, uint8_t _NumLeds, neoPixelType _Type, const char *_Name);
      bool init();
      void update (struct tm &_Time);
      bool getOnOff ();
    };
  }
}

#endif