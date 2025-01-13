/**
 * @file JCA_FNC_DigitalCounter.cpp
 * @brief Framework Element to count changes on a digital input using a hardware interrupt.
 * @version 1.0
 * @date 2023-10-10
 *
 * Apache License
 */

#include <JCA_FNC_DigitalCounter.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *DigitalCounter::ClassName = "DigitalCounter";
    const char *DigitalCounter::SetupTagType = "digitalCounter";
    const char *DigitalCounter::SetupTagInputPin = "pinInput";

    /**
     * @brief Construct a new DigitalCounter::DigitalCounter object
     *
     * @param _Pin Digital Pin connected to the input
     * @param _Name Element Name inside the Communication
     */
    DigitalCounter::DigitalCounter (uint8_t _Pin, String _Name)
        : FuncParent (_Name), Pin (_Pin), Counter (0) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("Counter", "Zähler", "", false, TagUsage_T::UseData, const_cast<uint16_t *> (&Counter), "Count"));

      // Init Hardware
      pinMode (Pin, INPUT_PULLUP);
      attachInterruptArg (digitalPinToInterrupt (Pin), handleInterrupt, this, CHANGE);
    }

    /**
     * @brief Handling DigitalCounter
     * No additional tasks needed in update for now
     * @param time Current Time to check the Samplerate
     */
    void DigitalCounter::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      // No additional tasks needed in update for now
    }

    /**
     * @brief Interrupt Service Routine to count changes on the digital input
     *
     * @param arg Pointer to the DigitalCounter instance
     */
    void IRAM_ATTR DigitalCounter::handleInterrupt (void *arg) {
      DigitalCounter *counterInstance = static_cast<DigitalCounter *> (arg);
      counterInstance->Counter++;
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void DigitalCounter::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
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
    bool DigitalCounter::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log.createNestedObject (SetupTagType);

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, _Log);
      uint8_t PinInput = GetSetupValueUINT8 (SetupTagInputPin, Done, _Setup, _Log);

      if (Done) {
        _Functions.push_back (new DigitalCounter (PinInput, Name));
        Log["done"] = Name + " (InputPin:" + String (PinInput) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}