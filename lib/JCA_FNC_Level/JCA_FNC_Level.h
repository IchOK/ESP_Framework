
#ifndef _JCA_FNC_LEVEL_
#define _JCA_FNC_LEVEL_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Level : public Parent{
    private:
      // Datapoint description
      static const char *NameRawEmpty;
      static const char *NameRawFull;
      static const char *NameAlarmLevel;
      static const char *NameReadInterval;
      static const char *UnitRawEmpty;
      static const char *UnitRawFull;
      static const char *UnitAlarmLevel;
      static const char *UnitReadInterval;
      static const char *NameLevel;
      static const char *NameAlarm;
      static const char *NameRawValue;
      static const char *UnitLevel;
      static const char *UnitAlarm;
      static const char *UnitRawValue;

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
      void setConfig (JsonObject _Data);
      void setData (JsonObject _Data);
      void createConfig (JsonObject &_Data);
      void createData (JsonObject &_Data);

    public:
      Level (uint8_t _Pin, const char* _Name);
      void update (struct tm &_Time);
      float getValue();
      bool getAlarm();
    };
  }
}

#endif