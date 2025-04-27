/**
 * @file JCA_FNC_DigitalIn.cpp
 * @brief Framework Element to count changes on a digital input using a hardware interrupt.
 * @version 1.1
 * @date 2023-10-10
 * @changelog
 * - [1.0] 2023-10-10: First Version
 * - [1.0] 2025-04-13: Renamed to DigitalIn and add mode to select between count and input
 *
 * Apache License
 */

#include <JCA_FNC_DigitalIn.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *DigitalIn::ClassName = "DigitalIn";
    const char *DigitalIn::SetupTagType = "digitalIn";
    const char *DigitalIn::SetupTagInputPin = "pin";
    const char *DigitalIn::SetupTagPullup = "pullup";
    const char *DigitalIn::SetupTagMode = "mode";

    /**
     * @brief Construct a new DigitalIn::DigitalIn object
     *
     * @param _Pin Digital Pin connected to the input
     * @param _Name Element Name inside the Communication
     * @param _Pullup Pullup-Mode (up, down, none)
     * @param _Mode Mode of the Input (count, input)
     */
    DigitalIn::DigitalIn (uint8_t _Pin, String _Pullup, String _Mode, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagUInt32 ("DebounceTime", "Entpressel Zeit", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &DebounceTime, "us"));

      // Init Hardware
      if (_Mode == "count") {
        Mode = MODE_COUNT;
        Tags.push_back (new TagUInt16 ("Value", "Zähler", "", TagAccessType_T::Read, TagUsage_T::UseData, &ValueCount, ""));
      } else {
        Mode = MODE_INPUT;
        Tags.push_back (new TagBool ("Value", "Eingang", "", TagAccessType_T::Read, TagUsage_T::UseData, &ValueInput, "EIN", "AUS"));

      }
      Pin = _Pin;
      Counter = 0;
      DebounceTime = 100000; // 100ms
      LastDebounceTime = 0;
      LastStableState = LOW;
      if (_Pullup == "up") {
        pinMode (Pin, INPUT_PULLUP);
      }
      #if !defined(ESP8266)
        else if (_Pullup == "down") {
          pinMode (Pin, INPUT_PULLDOWN);
        }
      #endif
      else {
        pinMode (Pin, INPUT);
      }
      if (Mode == MODE_COUNT) {
        attachInterruptArg (digitalPinToInterrupt (Pin), handleInterrupt, this, CHANGE);
      }
    }

    /**
     * @brief Handling DigitalIn
     * No additional tasks needed in update for now
     * @param time Current Time to check the Samplerate
     */
    void DigitalIn::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      if (Mode == MODE_INPUT) {
        ValueInput = digitalRead (Pin);
      } else if (Mode == MODE_COUNT) {
        ValueCount = Counter;
      }
    }

    /**
     * @brief Interrupt Service Routine to count changes on the digital input
     *
     * @param arg Pointer to the DigitalIn instance
     */
    void IRAM_ATTR DigitalIn::handleInterrupt (void *arg) {
      DigitalIn *counterInstance = static_cast<DigitalIn *> (arg);
      uint32_t currentTime = micros ();
      bool currentState = digitalRead (counterInstance->Pin);

      if (currentState != counterInstance->LastStableState) {
        if (currentTime - counterInstance->LastDebounceTime > counterInstance->DebounceTime) {
          // Change after debounce time
          if (currentState == HIGH) {
            counterInstance->Counter++;
          }
        }
        counterInstance->LastStableState = currentState;
        counterInstance->LastDebounceTime = currentTime;
      }
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void DigitalIn::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
    }

    /**
     * @brief Create a new instance of the class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the instance to
     * @param _Hardware List of known Hardware-References
     * @return true if creation was successful
     * @return false if creation failed
     */
    bool DigitalIn::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t PinInput = GetSetupValueUINT8 (SetupTagInputPin, Done, _Setup, Log);
      String Pullup = GetSetupValueString (SetupTagPullup, Done, _Setup, Log);
      String Mode = GetSetupValueString (SetupTagMode, Done, _Setup, Log);

      #if defined(ESP8266)
        if (Pullup == "down") {
          Log["error"] = "Invalid Pullup value: " + Pullup;
        }
      #endif

      if (Done) {
        _Functions.push_back (new DigitalIn (PinInput, Pullup, Mode, Name));
        Log["done"] = Name + " (InputPin:" + String (PinInput) + " ,Pullup:" + Pullup + " ,Mode:" + Mode + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}