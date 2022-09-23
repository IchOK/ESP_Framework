
#ifndef _JCA_FNC_LEVEL_
#define _JCA_FNC_LEVEL_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Level {
    private:
      char ObjectName[80];
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
      void set (JsonObject _Collection);
      void getConfig (JsonObject &_Collection);
      void getData (JsonObject &_Collection);
      void getAll (JsonObject &_Collection);
      float getValue();
      bool getAlarm();
    };
  }
}

#endif