/**
 * @file JCA_FNC_AcDimmers.cpp
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

#include <JCA_FNC_AcDimmers.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *AcDimmers::ClassName = "AcDimmers";
    const char *AcDimmers::SetupTagType = "acDimmer";
    const char *AcDimmers::SetupTagZeroPin = "pinZero";
    const char *AcDimmers::SetupTagOutputPins = "pinsOutput";

    const uint8_t AcDimmers::CalibrationLoops = 100;
    portMUX_TYPE AcDimmers::PortMux = portMUX_INITIALIZER_UNLOCKED;

    /**
     * @brief Construct a new AcDimmers::AcDimmers object
     * The Arrays to store the Dimmer Informations will create on consturtion
     * @param _PinZeroDetection Pin for Zero-Cross detection
     * @param _PinsOutputs Array of AC-Dimmer output Pins
     * @param _CountOutputs Array Length
     * @param _Name Element Name inside the Communication
     */
    AcDimmers::AcDimmers (uint8_t _PinZeroDetection, uint8_t *_PinsOutputs, uint8_t _CountOutputs, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Output-Arrays
      if (_CountOutputs > 0) {
        Values = new uint8_t[_CountOutputs];
        Triggers = new AcDimmersTriggers_T;
        Triggers->Pairs = new AcDimmersTriggerPair_T[_CountOutputs];
        Triggers->Count = _CountOutputs;
        Debug.print (FLAG_SETUP, false, Name, __func__, "Create Trigger Done [");
        Debug.print (FLAG_SETUP, false, Name, __func__, _CountOutputs);
        Debug.println (FLAG_SETUP, false, Name, __func__, "]");

        for (size_t i = 0; i < _CountOutputs; i++) {
          Debug.print (FLAG_SETUP, false, Name, __func__, i);
          // Init Data
          Values[i] = 0;
          Triggers->Pairs[i].Delay = 0;
          Triggers->Pairs[i].Pin = _PinsOutputs[i];
          Debug.print (FLAG_SETUP, false, Name, __func__, " > Pair Done [");
          Debug.print (FLAG_SETUP, false, Name, __func__, Triggers->Pairs[i].Pin);
          Debug.print (FLAG_SETUP, false, Name, __func__, "]");

          // Init digital Output
          pinMode (Triggers->Pairs[i].Pin, OUTPUT);
          digitalWrite (Triggers->Pairs[i].Pin, LOW);
          Debug.print (FLAG_SETUP, false, Name, __func__, " > Mode Done");

          // Create Tag-List
          String NumStr = String (i + 1);
          Tags.push_back (new TagUInt16 ("Delay" + NumStr, "Verzögerung " + NumStr, "", true, TagUsage_T::UseConfig, &(Triggers->Pairs[i].Delay), "us"));
          Tags.push_back (new TagUInt8 ("Value" + NumStr, "Wert " + NumStr, "", false, TagUsage_T::UseData, &(Values[i]), "%", std::bind (&AcDimmers::calc, this)));
          Debug.println (FLAG_SETUP, false, Name, __func__, " > Tags Done");
        }
      }
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("ZeroWidth", "Nullpunkt länge", "", true, TagUsage_T::UseConfig, &ZeroWidth, "us"));
      Tags.push_back (new TagUInt16 ("Period", "Dauer einer Sinuswelle", "", true, TagUsage_T::UseConfig, &Period, "us"));
      Debug.println (FLAG_SETUP, false, Name, __func__, "General Tags Done");

      // Init Data
      PinZeroDetection = _PinZeroDetection;
      ZeroWidth = 0;
      Period = 0;
      LastMicros = 0;
      InitDone = false;
      CalibrationDone = false;
      CalibrationCount = 0;
      CalSumPeriod = 0;
      CalSumZeroWidth = 0;

      // define Hardware interrupt
      attachInterrupt (digitalPinToInterrupt (PinZeroDetection), std::bind (&AcDimmers::isrZero, this), CHANGE);
      Debug.println (FLAG_SETUP, false, Name, __func__, "Interrupt Done");
      TimerIndex = TimerESP32_Handler.addTimer ();
      Debug.println (FLAG_SETUP, false, Name, __func__, "Timer Done");
      if (TimerIndex >= 0) {
        TimerESP32_Handler.isrCallbackAdd (TimerIndex, AcDimmers::isrTimer, Triggers);
        Debug.println (FLAG_SETUP, false, Name, __func__, "Callback Done");
        TimerESP32_Handler.autoReload (TimerIndex, true);
        Debug.println (FLAG_SETUP, false, Name, __func__, "Reload Done");
      }
    }

    AcDimmers::~AcDimmers() {
      delete[] Values;
      delete[] Triggers->Pairs;
      delete Triggers;
    }

    /**
     * @brief Handling AcDimmers
     * Only to support the Element Defaults. The Magic happens in the ISRs
     * @param time Current Time to check the Samplerate
     */
    void AcDimmers::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
    }

    /**
     * @brief Calculate the Delays for each Dimmer-Value
     * 
     */
    void AcDimmers::calc () {
      if (CalibrationDone) {
        // Calculation is only possible after calibation is done
        for (size_t i = 0; i < Triggers->Count; i++) {
          float Value = static_cast<float> (Values[i]) / 100.0;
          // Convert Value to Time-Delay
          if (Value < 0.02) {
            // lower than 2% is OFF
            Triggers->Pairs[i].Delay = Period * 2;
          } else if (Value > 0.98) {
            // higher than 98% is ON
            Triggers->Pairs[i].Delay = 0;
          } else {
            // between calc the ON-Delay and turn off the Output for retrigger
            Triggers->Pairs[i].Delay = static_cast<int16_t> (asin (Value) / (PI / 2.0) * static_cast<float> (Period)) + (ZeroWidth / 2);
            digitalWrite (Triggers->Pairs[i].Pin, LOW);
          }
        }
      }
    }

    /**
     * @brief ISR runs on Zero-Cross-Detection
     * First do calibration: get Period and Zero-Cross-Pulswidth in Microseconds
     * After calibration: Restart the Dimmer-Timer-Interrupt
     */
    void AcDimmers::isrZero () {
      portENTER_CRITICAL_ISR (&PortMux);
      noInterrupts ();
      unsigned long ActMicros = micros ();
      bool ZeroOn = digitalRead(PinZeroDetection);
      if (CalibrationDone) {
        // Start/Restart Timer when calibration ist done
        Triggers->ZeroCross = ActMicros;
        TimerESP32_Handler.restartTimer (TimerIndex, Period / 100);
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
              attachInterrupt (digitalPinToInterrupt (PinZeroDetection), std::bind (&AcDimmers::isrZero, this), RISING);
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
      interrupts ();
      portEXIT_CRITICAL_ISR (&PortMux);
    }

    /**
     * @brief ISR cyclic timer Interrupt, check if some output have to be switched on
     * 
     * @param _Args 
     * @return true 
     * @return false 
     */
    bool AcDimmers::isrTimer (void *_Args) {
      portENTER_CRITICAL_ISR (&PortMux);
      noInterrupts ();
      AcDimmersTriggers_T *Triggers = static_cast<AcDimmersTriggers_T *> (_Args);
      unsigned long ActMicros = micros () - Triggers->ZeroCross;
      for (uint8_t i = 0; i < Triggers->Count; i++) {
        if (Triggers->Pairs[i].Delay <= ActMicros) {
          digitalWrite (Triggers->Pairs[i].Pin, HIGH);
        } else {
          digitalWrite (Triggers->Pairs[i].Pin, LOW);
        }
      }
      interrupts ();
      portEXIT_CRITICAL_ISR (&PortMux);
      return false;
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void AcDimmers::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
    }

    bool AcDimmers::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log.createNestedObject (SetupTagType);

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, _Log);
      uint8_t PinZeroDetection = GetSetupValueUINT8 (SetupTagZeroPin, Done, _Setup, _Log);
      uint8_t *PinsOutput;
      uint8_t CountOutputs = GetSetupValueUINT8Arr(SetupTagOutputPins, PinsOutput, Done, _Setup, _Log);

      if (Done) {
        _Functions.push_back (new AcDimmers (PinZeroDetection, PinsOutput, CountOutputs, Name));
        Log["done"] = Name + " (ZeroPin:" + String (PinZeroDetection) + ", Output Count: " + String (CountOutputs) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      //delete[] PinsOutput;
      return Done;
    }
  }
}
