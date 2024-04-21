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
    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    DigitalOut::DigitalOut (uint8_t _Pin, const char *_Name)
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
        uint32_t ActMillis = millis ();
        uint32_t DiffMillis = ActMillis - LastMillis;
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
     * @brief Get the current output Value
     * just return the last Value
     * @return boot On/Off
     */
    bool DigitalOut::getValue () {
      return Value;
    }
  }
}
