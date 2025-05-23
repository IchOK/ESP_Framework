/**
 * @file JCA_FNC_Feeder.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element for a daily Feeder. With variable Time and Amount
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_FEEDER_
#define _JCA_FNC_FEEDER_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagInt16.h>
#include <JCA_TAG_TagInt32.h>

namespace JCA {
  namespace FNC {
    class Feeder : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagEnablePin;
      static const char *SetupTagStepPin;
      static const char *SetupTagDirPin;

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
      int32_t DistanceToGo;
      float Speed;
      void doFeedCB();

    public:
      Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, String _Name);
      ~Feeder () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif