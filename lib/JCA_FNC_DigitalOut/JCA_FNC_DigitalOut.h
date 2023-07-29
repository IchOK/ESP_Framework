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

namespace JCA {
  namespace FNC {
    class DigitalOut : public Parent {
    private:
      /* #region(collapsed) Datapoint description */
      static const char *DelayAutoOff_Name;
      static const char *DelayAutoOff_Text;
      static const char *DelayAutoOff_Unit;
      static const char *DelayAutoOff_Comment;
      static const char *Value_Name;
      static const char *Value_Text;
      static const char *Value_Comment;
      static const char *Value_TextOn;
      static const char *Value_TextOff;
      static const char *DelayCounter_Name;
      static const char *DelayCounter_Text;
      static const char *DelayCounter_Unit;
      static const char *DelayCounter_Comment;
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