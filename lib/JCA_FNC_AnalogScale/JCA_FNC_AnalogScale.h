/**
 * @file JCA_FNC_AnalogScale.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element Scale a analog Value with filter function
 * @version 0.1
 * @date 2023-07-28
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#ifndef _JCA_FNC_ANALOGSCALE_
#define _JCA_FNC_ANALOGSCALE_

#include <ArduinoJson.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class AnalogScale : public Parent {
    private:
      /* #region(collapsed) Datapoint description */
      static const char *ScaledMin_Name;
      static const char *ScaledMin_Text;
      static const char *ScaledMin_Comment;
      static const char *ScaledMax_Name;
      static const char *ScaledMax_Text;
      static const char *ScaledMax_Comment;
      static const char *Filter_Name;
      static const char *Filter_Text;
      static const char *Filter_Unit;
      static const char *Filter_Comment;
      static const char *Value_Name;
      static const char *Value_Text;
      static const char *Value_Comment;
      static const char *Unit_Name;
      static const char *Unit_Text;
      static const char *Unit_Comment;
      /* #endregion */

      static const uint16_t UpdateInterval;

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
      uint8_t Pin;
      float RawMax;

      // Konfig
      float ScaledMin;
      float ScaledMax;
      float Filter;
      char Unit[10];

      // Daten
      float Value;
      bool InitDone;

      // Intern
      uint32_t LastMillis;
      uint32_t UpdateMillis;

    public:
      AnalogScale (uint8_t _Pin, const char *_Name);
      bool init ();
      void update (struct tm &_Time);
      float getValue ();
    };
  }
}

#endif