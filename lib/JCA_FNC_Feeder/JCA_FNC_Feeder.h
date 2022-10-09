
#ifndef _JCA_FNC_FEEDER_
#define _JCA_FNC_FEEDER_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_SYS_DebugOut.h>
#include <JCA_FNC_Parent.h>

namespace JCA {
  namespace FNC {
    class Feeder : public Parent{
    private:
      // Datapoint description
      static const char *NameFeedingHour;
      static const char *NameFeedingMinute;
      static const char *NameSteppsPerRotation;
      static const char *NameFeedingRotations;
      static const char *NameAcceleration;
      static const char *NameMaxSpeed;
      static const char *NameConstSpeed;
      static const char *UnitFeedingHour;
      static const char *UnitFeedingMinute;
      static const char *UnitSteppsPerRotation;
      static const char *UnitFeedingRotaions;
      static const char *UnitAcceleration;
      static const char *UnitMaxSpeed;
      static const char *UnitConstSpeed;
      static const char *NameFeeding;
      static const char *NameDistanceToGo;
      static const char *NameRunConst;
      static const char *NameSpeed;
      static const char *UnitFeeding;
      static const char *UnitDistanceToGo;
      static const char *UnitRunConst;
      static const char *UnitSpeed;
      static const char *CmdRunConst;
      static const char *CmdDoFeed;

      // Hardware
      AccelStepper Stepper;

      // Konfig
      int16_t FeedingHour;
      int16_t FeedingMinute;
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
      Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char *_Name);
      void update (struct tm &_Time);
    };
  }
}

#endif