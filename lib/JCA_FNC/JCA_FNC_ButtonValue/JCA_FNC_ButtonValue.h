/**
 * @file JCA_FNC_ButtonValue.h
 * @brief Framework Element to adjust a float value via two debounced buttons (Up/Down).
 *
 * - Two digital input pins (pinUp, pinDown).
 * - Each press changes the value by StepSize (configurable at runtime).
 * - Software debouncing with configurable DebounceMs.
 * - When a button is held longer than HoldDelayMs, the value is changed
 *   automatically every RepeatIntervalMs (configurable at runtime).
 * - Value is clamped to [MinValue, MaxValue]; if WrapAround is true the
 *   value wraps around at the limits.
 *
 * @version 0.1
 * @date 2026-04-19
 *
 * Apache License
 */

#ifndef _JCA_FNC_BUTTONVALUE_
#define _JCA_FNC_BUTTONVALUE_

#include <ArduinoJson.h>
#include <time.h>

#include "JCA_FNC_Parent.h"
#include "JCA_IOT_FuncHandler.h"

namespace JCA {
  namespace FNC {
    class ButtonValue : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagPinUp;
      static const char *SetupTagPinDown;
      static const char *SetupTagPullup;
      static const char *SetupTagActiveLow;
      static const char *SetupTagUnit;

      enum ButtonIdx_T : uint8_t {
        BTN_UP = 0,
        BTN_DOWN = 1,
        BTN_COUNT = 2
      };

      // Hardware
      uint8_t Pins[BTN_COUNT];
      bool ActiveLow;

      // Konfig
      float StepSize;
      uint16_t DebounceMs;
      uint16_t HoldDelayMs;
      uint16_t RepeatIntervalMs;
      float MinValue;
      float MaxValue;
      bool WrapAround;

      // Daten
      float Value;
      bool Pressed[BTN_COUNT];

      // Intern: Debounce + Auto-Repeat State
      bool RawState[BTN_COUNT];
      bool StableState[BTN_COUNT];
      unsigned long LastChangeMs[BTN_COUNT];
      unsigned long NextRepeatMs[BTN_COUNT];

      void applyStep (int8_t _Direction);
      void handleButton (uint8_t _Idx, int8_t _Direction, unsigned long _NowMs);

    public:
      ButtonValue (uint8_t _PinUp, uint8_t _PinDown, String _Pullup, bool _ActiveLow, String _Unit, String _Name);
      ~ButtonValue () { ; }

      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
      static void GetSetupSchema (JsonObject &_Schema);
    };
  }
}

#endif
