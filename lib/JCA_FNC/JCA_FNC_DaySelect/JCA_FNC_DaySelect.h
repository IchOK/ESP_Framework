/**
 * @file JCA_FNC_DaySelect.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a DaySelect by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_DAYSELECT_
#define _JCA_FNC_DAYSELECT_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    class DaySelect : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagOutputPin;

      // Konfig
      uint16_t Days;

      // Daten
      bool Value;



    public:
      DaySelect (String _Name);
      ~DaySelect () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif