
#ifndef _JCA_FNC_LEVEL_
#define _JCA_FNC_LEVEL_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Level : public Protocol{
    private:
      // Datapoint description
      static const char *RawEmpty_Name;
      static const char *RawEmpty_Unit;
      static const char *RawEmpty_Comment;
      static const char *RawFull_Name;
      static const char *RawFull_Unit;
      static const char *RawFull_Comment;
      static const char *AlarmLevel_Name;
      static const char *AlarmLevel_Unit;
      static const char *AlarmLevel_Comment;
      static const char *ReadInterval_Name;
      static const char *ReadInterval_Unit;
      static const char *ReadInterval_Comment;
      static const char *Level_Name;
      static const char *Level_Unit;
      static const char *Level_Comment;
      static const char *Alarm_Name;
      static const char *Alarm_Comment;
      static const char *Alarm_TextOn;
      static const char *Alarm_TextOff;
      static const char *RawValue_Name;
      static const char *RawValue_Unit;
      static const char *RawValue_Comment;

      // Protocol Functions
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);
      void createConfigTags (JsonArray &_Tags);
      void createDataTags (JsonArray &_Tags);
      void createCmdInfoTags (JsonArray &_Tags);

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
      Level (uint8_t _Pin, const char* _Name);
      void update (struct tm &_Time);
      float getValue();
      bool getAlarm();
    };
  }
}

#endif