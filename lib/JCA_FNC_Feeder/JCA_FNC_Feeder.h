
#ifndef _JCA_FNC_FEEDER_
#define _JCA_FNC_FEEDER_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Feeder {
    private:
      char ObjectName[80];
      // Hardware
      AccelStepper Stepper;

      // Konfig
      int8_t FeedingHour;
      int8_t FeedingMinute;
      float SteppsPerRotation;
      float FeedingRotations;
      float Acceleration;
      float MaxSpeed;
      float ConstSpeed;

      // Daten
      bool RunConst;
      bool Feeding;

      // Intern
      bool DoFeed;
      bool AutoFeedDone;
      void setConfig (JsonObject _Data);
      void setData (JsonObject _Data);
      void createConfig (JsonObject &_Data);
      void createData (JsonObject &_Data);

    public:
      Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char* _Name);
      void update (struct tm &_Time);
      void set (JsonObject _Collection);
      void getConfig (JsonObject &_Collection);
      void getData (JsonObject &_Collection);
      void getAll (JsonObject &_Collection);
    };
  }
}

#endif