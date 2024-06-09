/**
 * @file JCA_FNC_AcDimmers.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get creates an Array of AC Dimmers by using one Zero-Cross detection Pin
 * The Element callibrate the AC Periode and the Zreo-Cross-Pulse on startup.
 * @version 1.0
 * @date 2024-05-22
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ACDIMMERS_
#define _JCA_FNC_ACDIMMERS_

#include "FunctionalInterrupt.h"
#include <ArduinoJson.h>
#include <algorithm>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_TimerESP32.h>
#include <JCA_TAG_TagUInt16.h>
#include <JCA_TAG_TagUInt8.h>
#include <JCA_TAG_TagInt32.h>

namespace JCA {
  namespace FNC {
    typedef struct {
      int32_t Delay;
      uint8_t Pin;
    } AcDimmersTriggerPair_T;

    typedef struct {
      AcDimmersTriggerPair_T *Pairs;
      unsigned long ZeroCross;
      uint8_t Count;
    } AcDimmersTriggers_T;

    class AcDimmers : public FuncParent {
    private:
      static const char *ClassName;
      static const uint8_t CalibrationLoops;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagZeroPin;
      static const char *SetupTagOutputPins;

      // Hardware
      uint8_t PinZeroDetection;
      uint8_t TimerIndex;

      // Daten
      uint16_t ZeroWidth;
      uint8_t *Values;
      uint16_t Period;

      // Intern
      unsigned long LastMicros;
      bool InitDone;
      bool CalibrationDone;
      uint8_t CalibrationCount;
      unsigned long CalSumPeriod;
      unsigned long CalSumZeroWidth;
      AcDimmersTriggers_T *Triggers;

    public :
      static portMUX_TYPE PortMux;

      AcDimmers (uint8_t _PinZeroDetection, uint8_t *_PinsOutputs, uint8_t _CountOutputs, String _Name);
      ~AcDimmers();
      void update (struct tm &_Time);

      // Interrput Functions
      void calc ();
      void IRAM_ATTR isrZero ();
      static bool IRAM_ATTR isrTimer (void *_Args);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}
#endif