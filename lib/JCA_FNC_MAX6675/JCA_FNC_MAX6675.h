/**
 * @file JCA_FNC_MAX6675.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to get Data from MAX6675 Sensor
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#ifndef _JCA_FNC_MAX6675_
#define _JCA_FNC_MAX6675_

#include <ArduinoJson.h>
#include <max6675.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class MAX6675 : public Parent {
    private:
      // Datapoint description
      static const char *ReadInterval_Name;
      static const char *ReadInterval_Text;
      static const char *ReadInterval_Unit;
      static const char *ReadInterval_Comment;
      static const char *Value_Name;
      static const char *Value_Text;
      static const char *Value_Unit;
      static const char *Value_Comment;
      static const char *Filter_Name;
      static const char *Filter_Text;
      static const char *Filter_Unit;
      static const char *Filter_Comment;
      /* #endregion */

      // Parent Functions
      void createConfigValues (JsonObject &_Values);
      void createDataValues (JsonObject &_Values);
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);

      void writeSetupConfig (File _SetupFile);
      void writeSetupData (File _SetupFile);
      void writeSetupCmdInfo (File _SetupFile);

      // Hardware
      ::MAX6675 Sensor;

      // Konfig
      float Filter;
      uint16_t ReadInterval;

      // Daten
      float Value;

      // Intern
      uint32_t LastMillis;
      uint32_t UpdateMillis;

    public:
      MAX6675 (int8_t _SCLK, int8_t _CS, int8_t _MISO, const char *_Name);
      bool init ();
      void update (struct tm &_Time);
      void setInterval (uint16_t _ReadInterval);
      float getValue ();
    };
  }
}

#endif