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
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    class DigitalOut : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagOutputPin;

      // Hardware
      uint8_t Pin;

      // Konfig
      uint16_t DelayAutoOff;

      // Daten
      bool Value;
      uint16_t DelayCounter;

      // Intern
      unsigned long LastMillis;
      uint16_t DelayMillis;

    public:
      DigitalOut (uint8_t _Pin, String _Name);
      ~DigitalOut () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif