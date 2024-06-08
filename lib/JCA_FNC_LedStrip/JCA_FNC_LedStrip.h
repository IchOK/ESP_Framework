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

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagUInt16.h>
#include <JCA_TAG_TagUInt32.h>

namespace JCA {
  namespace FNC {
    class LedStrip : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagOutputPin;
      static const char *SetupTagNumLeds;
      static const char *SetupTagPixelType;
      static const char *SetupTagPixelSpeed;

      // Hardware
      Adafruit_NeoPixel Strip;

      // Konfig
      uint16_t DelayAutoOff;

      // Daten
      bool OnOff;
      uint32_t Value;
      uint16_t DelayCounter;

      // Intern
      unsigned long LastMillis;
      uint16_t UpdateMillis;
      uint16_t DelayMillis;
      void updateColorCB ();

    public:
      LedStrip (uint8_t _Pin, uint8_t _NumLeds, neoPixelType _Type, String _Name);
      void update (struct tm &_Time);

      // Additional initialisiations
      bool init ();

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif