/**
 * @file JCA_FNC_ValueAnalog.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a ValueAnalog by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_VALUEANALOG_
#define _JCA_FNC_VALUEANALOG_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagFloat.h>

namespace JCA {
  namespace FNC {
    class ValueAnalog : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagUnit;

      // Daten
      float Value;

    public:
      ValueAnalog (String _Name, String _Unit);
      ~ValueAnalog () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif