/**
 * @file JCA_FNC_AcDimmers.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a AcDimmers by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_AcDimmers.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const uint8_t AcDimmers::CalibrationLoops = 100;

    /**
     * @brief Construct a new AcDimmers::AcDimmers object
     *
     * @param _Pin Analog Pin conected to the AcDimmers-Sensor
     * @param _Name Element Name inside the Communication
     */
    AcDimmers::AcDimmers (uint8_t _PinZeroDetection, uint8_t *_PinsOutputs, uint8_t _CountOutputs, uint8_t _TimerIndex, char *_Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Output-Arrays
      if (_CountOutputs > 0) {
        PinsOutputs = new uint8_t[_CountOutputs];
        Delays = new uint16_t[_CountOutputs];
        Values = new uint8_t[_CountOutputs];
        CalcTriggers = new AcDimmersTriggers_T;
        CalcTriggers->Triggers = new AcDimmersTriggerPair_T[_CountOutputs];
        IntTriggers = new AcDimmersTriggers_T;
        IntTriggers->Triggers = new AcDimmersTriggerPair_T[_CountOutputs];

        for (size_t i = 0; i < _CountOutputs; i++) {
          // Init Data
          PinsOutputs[i] = _PinsOutputs[i];
          Delays[i] = 0;
          Values[i] = 0;

          // Create Tag-List
          String NumStr = String (i + 1);
          Tags.push_back (new TagUInt16 ("Delay" + NumStr, "Verzögerung " + NumStr, "", true, TagUsage_T::UseConfig, &(Delays[i]), "us"));
          Tags.push_back (new TagUInt8 ("Value" + NumStr, "Wert " + NumStr, "", false, TagUsage_T::UseData, &(Values[i]), "%", std::bind (&AcDimmers::calc, this)));
        }
      }
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("ZeroWidth", "Nullpunkt länge", "", true, TagUsage_T::UseConfig, &ZeroWidth, "us"));
      Tags.push_back (new TagUInt16 ("Period", "Dauer einer Sinuswelle", "", true, TagUsage_T::UseConfig, &Period, "us"));

      // Init Data
      CountOutputs = _CountOutputs;
      PinZeroDetection = _PinZeroDetection;
      ZeroWidth = 0;
      Period = 0;
      LastMicros = 0;
      InitDone = false;
      CalibrationDone = false;
      CalibrationCount = 0;
      CalSumPeriod = 0;
      CalSumZeroWidth = 0;
      CalcUpdate = false;
      CalcTriggers->Count = 0;
      IntTriggers->Count = 0;

      // define Hardware interrupt
      attachInterrupt (digitalPinToInterrupt (PinZeroDetection), std::bind (&AcDimmers::zeroInterrupt, this), CHANGE);
      OutputTimer = TimerESP32_Handler.addTimer();
      IntTriggers->Timer = OutputTimer;
      if (OutputTimer >= 0) {
        TimerESP32_Handler.addCallback (OutputTimer, 2 ^ 60, AcDimmers::timerInterrupt, IntTriggers);
      }
    }

    /**
     * @brief Handling AcDimmers
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void AcDimmers::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
    }

    void AcDimmers::calc () {
      CalcTriggers->Count = 0;
      for (size_t i = 0; i < CountOutputs; i++) {
        float Value = static_cast<float> (Values[i]) / 100.0;
        // Convert Value to Time-Delay
        if (Value < 0.02) {
          Delays[i] = 0;
          digitalWrite(PinsOutputs[i], false);
        } else if (Value > 0.98) {
          Delays[i] = 0;
          digitalWrite (PinsOutputs[i], true);
        } else {
          Delays[i] = static_cast<int16_t> (asin (Value) / (PI / 2.0) * (Period / 2)) + (ZeroWidth / 2);
          CalcTriggers->Triggers[CalcTriggers->Count].Delay = Delays[i];
          CalcTriggers->Triggers[CalcTriggers->Count].Pin = PinsOutputs[i];
          CalcTriggers->Count++;
        }
      }

      // Reset not used Triggers
      for (size_t i = CalcTriggers->Count; i < CountOutputs; i++) {
        CalcTriggers->Triggers[i].Delay = 0;
        CalcTriggers->Triggers[i].Pin = 0;
      }

      // Sort Interrupts
      if (CalcTriggers->Count > 0) {
        // Sort Interrupts
        std::sort (CalcTriggers->Triggers, CalcTriggers->Triggers + CalcTriggers->Count, [] (const AcDimmersTriggerPair_T &a, const AcDimmersTriggerPair_T &b) {
          return a.Delay < a.Delay;
        });
      }
      CalcUpdate = true;
    }

    void AcDimmers::zeroInterrupt () {
      uint32_t ActMicros = micros();
      bool ZeroOn = digitalRead(PinZeroDetection);
      if (CalibrationDone) {
        if (CalcUpdate) {
          IntTriggers->Count = CalcTriggers->Count;
          for(size_t i = 0; i<CountOutputs; i++) {
            IntTriggers->Triggers[i].Delay = CalcTriggers->Triggers[i].Delay;
            IntTriggers->Triggers[i].Pin = CalcTriggers->Triggers[i].Pin;
          }
          CalcUpdate = false;
        }
        IntTriggers->Index = 0;
        if (IntTriggers->Count > 0) {
          TimerESP32_Handler.restartTimer (OutputTimer, IntTriggers->Triggers[0].Delay);
        } else {
          TimerESP32_Handler.pauseTimer (OutputTimer);
        };
      } else {
        // Periode and ZeroImpulse-Width have to be calibrated
        if (InitDone) {
          if (ZeroOn) {
            CalSumPeriod += ActMicros - LastMicros;
            LastMicros = ActMicros;
          } else {
            CalSumZeroWidth += ActMicros - LastMicros;
            CalibrationCount++;
            if (CalibrationCount >= CalibrationLoops) {
              Period = static_cast<int16_t> (CalSumPeriod / CalibrationCount);
              ZeroWidth = static_cast<int16_t> (CalSumZeroWidth / CalibrationCount);
              CalibrationDone = true;
              detachInterrupt (digitalPinToInterrupt (PinZeroDetection));
              attachInterrupt (digitalPinToInterrupt (PinZeroDetection), std::bind (&AcDimmers::zeroInterrupt, this), RISING);
            }
          }
        } else {
          if (ZeroOn) {
            LastMicros = ActMicros;
          } else if (LastMicros > 0) {
            InitDone = true;
          }
        }
      }
    }

    bool AcDimmers::timerInterrupt (void *_Args) {
      AcDimmersTriggers_T *Triggers = static_cast<AcDimmersTriggers_T *> (_Args);
      digitalWrite (Triggers->Triggers[Triggers->Index].Pin, true);
      Triggers->Index++;
      if (Triggers->Index < Triggers->Count) {
        TimerESP32_Handler.setMicros (Triggers->Timer, Triggers->Triggers[Triggers->Index].Delay);
        return false;
      } else {
        TimerESP32_Handler.pauseTimer (Triggers->Timer);
        return true;
      }
    }
  }
}
