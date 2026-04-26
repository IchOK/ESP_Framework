/**
 * @file JCA_FNC_ButtonValue.cpp
 * @brief Two-button up/down value adjuster with debouncing and auto-repeat.
 *
 * @version 0.1
 * @date 2026-04-19
 *
 * Apache License
 */

#include "JCA_FNC_ButtonValue.h"
#include "JCA_SYS_DebugOut.h"
#include "JCA_TAG_TagBool.h"
#include "JCA_TAG_TagFloat.h"
#include "JCA_TAG_TagUInt16.h"
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *ButtonValue::ClassName = "ButtonValue";
    const char *ButtonValue::SetupTagType = "buttonValue";
    const char *ButtonValue::SetupTagPinUp = "pinUp";
    const char *ButtonValue::SetupTagPinDown = "pinDown";
    const char *ButtonValue::SetupTagPullup = "pullup";
    const char *ButtonValue::SetupTagActiveLow = "activeLow";
    const char *ButtonValue::SetupTagUnit = "unit";

    static bool isUsableButtonPin (uint8_t _Pin) {
#if defined(digitalPinIsValid)
      if (!digitalPinIsValid (_Pin)) {
        return false;
      }
#endif
#if defined(CONFIG_IDF_TARGET_ESP32C6)
      // Generic ESP32-C6 variants mark all SoC GPIOs valid, but GPIO24+
      // are not exposed as regular pins on SuperMini-style boards.
      return _Pin <= 23;
#else
      return true;
#endif
    }

    /**
     * @brief Construct a new ButtonValue object
     *
     * @param _PinUp     Digital input pin for the increment button
     * @param _PinDown   Digital input pin for the decrement button
     * @param _Pullup    Pin pull mode ("up", "down", "none")
     * @param _ActiveLow true if a pressed button reads LOW (typical with INPUT_PULLUP)
     * @param _Unit      Unit string for the float value (e.g. "°C", "%")
     * @param _Name      Element name inside the FuncHandler
     */
    ButtonValue::ButtonValue (uint8_t _PinUp, uint8_t _PinDown, String _Pullup, bool _ActiveLow, String _Unit, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      Pins[BTN_UP] = _PinUp;
      Pins[BTN_DOWN] = _PinDown;
      ActiveLow = _ActiveLow;

      // Configuration tags (saved to setup file)
      Tags.push_back (new TagFloat ("StepSize", "Schrittweite", "Wert pro Tastendruck bzw. pro Wiederholung", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &StepSize, _Unit));
      Tags.push_back (new TagUInt16 ("DebounceMs", "Entprellzeit", "Stabile Zeit eines Tastersignals bevor es uebernommen wird", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &DebounceMs, "ms"));
      Tags.push_back (new TagUInt16 ("HoldDelayMs", "Hold-Verzoegerung", "Wartezeit nach erstem Druck bis Auto-Wiederholung startet", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &HoldDelayMs, "ms"));
      Tags.push_back (new TagUInt16 ("RepeatIntervalMs", "Wiederhol-Intervall", "Zeitabstand zwischen Wert-Aenderungen bei gehaltenem Taster", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &RepeatIntervalMs, "ms"));
      Tags.push_back (new TagFloat ("MinValue", "Minimaler Wert", "Untere Begrenzung des Wertes", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &MinValue, _Unit));
      Tags.push_back (new TagFloat ("MaxValue", "Maximaler Wert", "Obere Begrenzung des Wertes", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &MaxValue, _Unit));
      Tags.push_back (new TagBool ("WrapAround", "Umlaufend", "Aus: an Min/Max anschlagen. Ein: an Min/Max ueberlaufen lassen.", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &WrapAround, "EIN", "AUS"));

      // Process value (writeable so it can also be set via Link or external request)
      Tags.push_back (new TagFloat ("Value", "Wert", "Aktueller Wert", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &Value, _Unit));
      Tags.push_back (new TagBool ("UpPressed", "Taster + gedrueckt", "", TagAccessType_T::Read, TagUsage_T::UseData, &Pressed[BTN_UP], "EIN", "AUS"));
      Tags.push_back (new TagBool ("DownPressed", "Taster - gedrueckt", "", TagAccessType_T::Read, TagUsage_T::UseData, &Pressed[BTN_DOWN], "EIN", "AUS"));

      // Default configuration
      StepSize = 1.0f;
      DebounceMs = 30;
      HoldDelayMs = 500;
      RepeatIntervalMs = 200;
      MinValue = 0.0f;
      MaxValue = 100.0f;
      WrapAround = false;
      Value = 0.0f;

      for (uint8_t i = 0; i < BTN_COUNT; i++) {
        Pressed[i] = false;
        RawState[i] = false;
        StableState[i] = false;
        LastChangeMs[i] = 0;
        NextRepeatMs[i] = 0;
      }

      // Init hardware
      if (_Pullup == "up") {
        pinMode (Pins[BTN_UP], INPUT_PULLUP);
        pinMode (Pins[BTN_DOWN], INPUT_PULLUP);
      }
#if !defined(ESP8266)
      else if (_Pullup == "down") {
        pinMode (Pins[BTN_UP], INPUT_PULLDOWN);
        pinMode (Pins[BTN_DOWN], INPUT_PULLDOWN);
      }
#endif
      else {
        pinMode (Pins[BTN_UP], INPUT);
        pinMode (Pins[BTN_DOWN], INPUT);
      }
    }

    /**
     * @brief Apply one step in the requested direction, respecting limits / wrap-around.
     *
     * @param _Direction +1 to add StepSize, -1 to subtract
     */
    void ButtonValue::applyStep (int8_t _Direction) {
      if (_Direction == 0 || StepSize <= 0.0f || MaxValue <= MinValue) {
        return;
      }
      float NewValue = Value + static_cast<float> (_Direction) * StepSize;
      const float Span = MaxValue - MinValue;
      if (WrapAround) {
        if (NewValue > MaxValue) {
          NewValue = MinValue + fmodf (NewValue - MinValue, Span);
        } else if (NewValue < MinValue) {
          float Below = MinValue - NewValue;
          NewValue = MaxValue - fmodf (Below, Span);
        }
      } else {
        if (NewValue > MaxValue) {
          NewValue = MaxValue;
        } else if (NewValue < MinValue) {
          NewValue = MinValue;
        }
      }
      Value = NewValue;
    }

    /**
     * @brief Update one button: software debouncing + auto repeat trigger.
     *
     * @param _Idx       BTN_UP or BTN_DOWN
     * @param _Direction +1 (up) or -1 (down)
     * @param _NowMs     Current millis() snapshot for the cycle
     */
    void ButtonValue::handleButton (uint8_t _Idx, int8_t _Direction, unsigned long _NowMs) {
      bool RawLevel = digitalRead (Pins[_Idx]);
      bool Raw = ActiveLow ? (RawLevel == LOW) : (RawLevel == HIGH);

      if (Raw != RawState[_Idx]) {
        RawState[_Idx] = Raw;
        LastChangeMs[_Idx] = _NowMs;
      }

      if ((_NowMs - LastChangeMs[_Idx]) >= static_cast<unsigned long> (DebounceMs)) {
        if (Raw != StableState[_Idx]) {
          StableState[_Idx] = Raw;
          if (Raw) {
            // Pressed edge: apply one step, then schedule first auto-repeat after HoldDelayMs
            applyStep (_Direction);
            NextRepeatMs[_Idx] = _NowMs + HoldDelayMs;
          } else {
            NextRepeatMs[_Idx] = 0;
          }
        } else if (Raw && RepeatIntervalMs > 0 && _NowMs >= NextRepeatMs[_Idx]) {
          // Held: every RepeatIntervalMs apply one more step
          applyStep (_Direction);
          NextRepeatMs[_Idx] = _NowMs + RepeatIntervalMs;
        }
      }

      Pressed[_Idx] = StableState[_Idx];
    }

    /**
     * @brief Cyclic update: poll both buttons.
     */
    void ButtonValue::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      const unsigned long NowMs = millis ();
      handleButton (BTN_UP, +1, NowMs);
      handleButton (BTN_DOWN, -1, NowMs);
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     */
    void ButtonValue::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
      _Handler.FunctionSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> (SetupTagType, GetSetupSchema));
    }

    /**
     * @brief Create a new instance of the class using the JSON-Configdata
     */
    bool ButtonValue::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t PinUp = GetSetupValueUINT8 (SetupTagPinUp, Done, _Setup, Log);
      uint8_t PinDown = GetSetupValueUINT8 (SetupTagPinDown, Done, _Setup, Log);
      String Pullup = _Setup[SetupTagPullup].is<JsonVariant> () ? _Setup[SetupTagPullup].as<String> () : String ("up");
      String Unit = _Setup[SetupTagUnit].is<JsonVariant> () ? _Setup[SetupTagUnit].as<String> () : String ("");

      // ActiveLow defaults to true if pullup="up" (typical button to GND), else false
      bool ActiveLow = (Pullup == "up");
      if (_Setup[SetupTagActiveLow].is<bool> ()) {
        ActiveLow = _Setup[SetupTagActiveLow].as<bool> ();
      } else if (_Setup[SetupTagActiveLow].is<int> ()) {
        ActiveLow = _Setup[SetupTagActiveLow].as<int> () != 0;
      }

#if defined(ESP8266)
      if (Pullup == "down") {
        Log["error"] = "Invalid Pullup value 'down' on ESP8266";
        Done = false;
      }
#endif
      if (!isUsableButtonPin (PinUp)) {
        Log["error"] = "Invalid pinUp GPIO " + String (PinUp);
        Done = false;
      }
      if (!isUsableButtonPin (PinDown)) {
        Log["error"] = "Invalid pinDown GPIO " + String (PinDown);
        Done = false;
      }

      if (Done) {
        _Functions.push_back (new ButtonValue (PinUp, PinDown, Pullup, ActiveLow, Unit, Name));
        Log["done"] = Name + " (PinUp:" + String (PinUp) + " ,PinDown:" + String (PinDown) + " ,Pullup:" + Pullup + " ,ActiveLow:" + String (ActiveLow) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }

    void ButtonValue::GetSetupSchema (JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray> ();

      JsonObject NameParam = Parameters.add<JsonObject> ();
      NameParam["name"] = JCA_IOT_FUNCHANDLER_SETUP_NAME;
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Funktion fuer die Kommunikation";

      JsonObject PinUpParam = Parameters.add<JsonObject> ();
      PinUpParam["name"] = SetupTagPinUp;
      PinUpParam["type"] = "uint8";
      PinUpParam["comment"] = "Pin des Plus-Tasters (Wert erhoehen)";

      JsonObject PinDownParam = Parameters.add<JsonObject> ();
      PinDownParam["name"] = SetupTagPinDown;
      PinDownParam["type"] = "uint8";
      PinDownParam["comment"] = "Pin des Minus-Tasters (Wert verringern)";

      JsonObject PullupParam = Parameters.add<JsonObject> ();
      PullupParam["name"] = SetupTagPullup;
      PullupParam["type"] = "string";
      PullupParam["comment"] = "Pullup-Mode (up, down, none) - Standard: up";

      JsonObject ActiveLowParam = Parameters.add<JsonObject> ();
      ActiveLowParam["name"] = SetupTagActiveLow;
      ActiveLowParam["type"] = "bool";
      ActiveLowParam["comment"] = "Taster gedrueckt = LOW (Standard: true bei pullup=up)";

      JsonObject UnitParam = Parameters.add<JsonObject> ();
      UnitParam["name"] = SetupTagUnit;
      UnitParam["type"] = "string";
      UnitParam["comment"] = "Einheit des Wertes (z.B. '°C', '%')";
    }
  }
}
