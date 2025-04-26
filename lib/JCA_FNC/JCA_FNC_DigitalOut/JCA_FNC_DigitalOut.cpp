/**
 * @file JCA_FNC_DigitalOut.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a DigitalOut by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_DigitalOut.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *DigitalOut::ClassName = "DigitalOut";
    const char *DigitalOut::SetupTagType = "digitalOut";
    const char *DigitalOut::SetupTagOutputPin = "pinOutput";
    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    DigitalOut::DigitalOut (uint8_t _Pin, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("DelayAutoOff", "Verzögerung Auto-OFF", "Wird der Wert auf 0 gesetzt ist die Finktion inaktiv", false, TagUsage_T::UseConfig, &DelayAutoOff, "Min", nullptr));

      Tags.push_back (new TagBool ("Value", "Eingeschaltet", "", false, TagUsage_T::UseData, &Value, "EIN", "AUS"));
      Tags.push_back(new TagUInt16("DelayCounter", "Verzögerung Zähler", "", true, TagUsage_T::UseData, &DelayCounter, "Min"));
      // Init Data
      DelayAutoOff = 0;
      Pin = _Pin;
      pinMode(Pin, OUTPUT);
      Value = false;
      DelayCounter = 0;
      LastMillis = millis ();
      DelayMillis = 0;
    }

    /**
     * @brief Handling DigitalOut
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void DigitalOut::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");

      // Get Millis for AutoOff
      if (DelayAutoOff > 0 && Value) {
        unsigned long ActMillis = millis ();
        unsigned long DiffMillis = ActMillis - LastMillis;
        LastMillis = ActMillis;
        DelayMillis += DiffMillis;
        if (DelayMillis >= 60000) {
          DelayCounter++;
          DelayMillis -= 60000;
          if (DelayCounter >= DelayAutoOff) {
            Value = 0;
          }
        }
      } else {
        LastMillis = millis();
        DelayCounter = 0;
        DelayMillis = 0;
      }

      digitalWrite(Pin, Value);
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     * 
     * @param _Handler Function Handler
     */
    void DigitalOut::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
    }

    /**
     * @brief Create a new Instanz of the Class using the JSON-Configdata and add it to the Functions-List
     * 
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the Instanz to
     * @param _Hardware List of knowen Hardware-References
     * @return true 
     * @return false 
     */
    bool DigitalOut::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t PinOutput = GetSetupValueUINT8(SetupTagOutputPin, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new DigitalOut(PinOutput, Name));
        Log["done"] = Name + " (OutputPin:" + String (PinOutput) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
