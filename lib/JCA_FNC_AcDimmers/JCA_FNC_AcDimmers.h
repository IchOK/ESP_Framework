/**
 * @file JCA_FNC_AcDimmers.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a AcDimmers by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 0.1
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_ACDIMMERS_
#define _JCA_FNC_ACDIMMERS_

//  #include "ESP32TimerInterrupt.h"
  #include "FunctionalInterrupt.h"
  #include <ArduinoJson.h>
  #include <algorithm>
  #include <time.h>

  #include <JCA_FNC_Parent.h>
  #include <JCA_SYS_DebugOut.h>
  #include <JCA_SYS_TimerESP32.h>
  #include <JCA_TAG_TagUInt8.h>
  #include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    typedef struct {
      uint64_t Delay;
      uint8_t Pin;
    } AcDimmersTriggerPair_T;

    typedef struct {
      AcDimmersTriggerPair_T *Triggers;
      uint8_t Count;
      uint8_t Index;
      uint8_t Timer;
    } AcDimmersTriggers_T;

    class AcDimmers : public FuncParent {
    private:
      static const uint8_t CalibrationLoops;

      // Hardware
      uint8_t OutputTimer;
      portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
      uint8_t PinZeroDetection;
      uint8_t *PinsOutputs;
      uint8_t CountOutputs;

      // Konfig
      uint16_t *Delays;

      // Daten
      uint16_t ZeroWidth;
      uint8_t *Values;
      uint16_t Period;

      // Intern
      uint32_t LastMicros;
      bool InitDone;
      bool CalibrationDone;
      bool CalcUpdate;
      uint8_t CalibrationCount;
      uint32_t CalSumPeriod;
      uint32_t CalSumZeroWidth;
      AcDimmersTriggers_T *CalcTriggers;
      AcDimmersTriggers_T *IntTriggers;

    public :
      AcDimmers (uint8_t _PinZeroDetection, uint8_t *_PinsOutputs, uint8_t _CountOutputs, uint8_t _TimerIndex, char *_Name);
      void update (struct tm &_Time);
      void calc();
      void IRAM_ATTR zeroInterrupt ();
      static bool IRAM_ATTR timerInterrupt (void *_Args);
    };
  }
}
#endif