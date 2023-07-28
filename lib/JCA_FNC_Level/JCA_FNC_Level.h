/**
 * @file JCA_FNC_Level.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a Level by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_LEVEL_
#define _JCA_FNC_LEVEL_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Level : public Parent {
    private:
      // Datapoint description
      static const char *RawEmpty_Name;
      static const char *RawEmpty_Text;
      static const char *RawEmpty_Unit;
      static const char *RawEmpty_Comment;
      static const char *RawFull_Name;
      static const char *RawFull_Text;
      static const char *RawFull_Unit;
      static const char *RawFull_Comment;
      static const char *AlarmLevel_Name;
      static const char *AlarmLevel_Text;
      static const char *AlarmLevel_Unit;
      static const char *AlarmLevel_Comment;
      static const char *ReadInterval_Name;
      static const char *ReadInterval_Text;
      static const char *ReadInterval_Unit;
      static const char *ReadInterval_Comment;
      static const char *Level_Name;
      static const char *Level_Text;
      static const char *Level_Unit;
      static const char *Level_Comment;
      static const char *Alarm_Name;
      static const char *Alarm_Text;
      static const char *Alarm_Comment;
      static const char *Alarm_TextOn;
      static const char *Alarm_TextOff;
      static const char *RawValue_Name;
      static const char *RawValue_Text;
      static const char *RawValue_Unit;
      static const char *RawValue_Comment;

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
      int RawEmpty;
      int RawFull;
      uint16_t ReadInterval;
      float AlarmLevel;

      // Daten
      float Value;
      bool Alarm;
      int RawValue;

      // Intern
      int8_t LastSeconds;
      uint16_t IntervalCount;

    public:
      Level (uint8_t _Pin, const char *_Name);
      void update (struct tm &_Time);
      float getValue ();
      bool getAlarm ();
    };
  }
}

#endif