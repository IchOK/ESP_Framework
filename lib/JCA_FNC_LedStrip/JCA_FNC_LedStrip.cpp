/**
 * @file JCA_FNC_LedStrip.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a LedStrip by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_LedStrip.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /**
     * @brief Construct a new LedStrip::LedStrip object
     *
     * @param _Pin Analog Pin conected to the LedStrip-Sensor
     * @param _Name Element Name inside the Communication
     */
    LedStrip::LedStrip (uint8_t _Pin, uint8_t _NumLeds, neoPixelType _Type, const char *_Name)
        : Parent (_Name), Strip (_NumLeds, _Pin, _Type) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new ElementTagUInt16 ("DelayAutoOff", "Verzögerung Auto-OFF", "Wird der Wert auf 0 gesetzt ist die Finktion inaktiv", false, ElementTagUsage_T::UseConfig, &DelayAutoOff, "Min"));

      Tags.push_back (new ElementTagBool ("OnOff", "Einschalten", "", false, ElementTagUsage_T::UseData, &OnOff, "EIN", "AUS", std::bind (&LedStrip::updateColorCB, this)));
      Tags.push_back (new ElementTagUInt32 ("Value", "Farbwert", "", false, ElementTagUsage_T::UseData, &Value, "COLOR", std::bind (&LedStrip::updateColorCB, this)));
      Tags.push_back (new ElementTagUInt16 ("DelayCounter", "Verzögerung bis AUS", "", false, ElementTagUsage_T::UseData, &DelayCounter, "Min"));

      DelayAutoOff = 0;
      OnOff = true;
      Value = 4108721;
      DelayCounter = 0;
      LastMillis = millis ();
      DelayMillis = 0;
      UpdateMillis = 0;
    }

    void  LedStrip::updateColorCB() {
      if (OnOff) {
        Strip.fill (Value, 0, 0);
      } else {
        Strip.clear ();
      }
      Strip.show ();
    }

    /**
     * @brief Init the LED-Strip
     */
    bool LedStrip::init () {
      Strip.begin ();
      Strip.show();
      return true;
    }

    /**
     * @brief Handling LedStrip
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void LedStrip::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");

      // Get Millis for AutoOff
      uint32_t ActMillis = millis ();
      uint32_t DiffMillis = ActMillis - LastMillis;
      LastMillis = ActMillis;

      if (DelayAutoOff > 0 && OnOff) {
        DelayMillis += DiffMillis;
        if (DelayMillis >= 60000) {
          DelayCounter++;
          DelayMillis -= 60000;
          if (DelayCounter >= DelayAutoOff) {
            OnOff = 0;
          }
        }
      } else {
        DelayCounter = 0;
        DelayMillis = 0;
      }

      UpdateMillis += DiffMillis;
      if (UpdateMillis >= 1000) {
        UpdateMillis -= 1000;
        updateColorCB();
      }
    }

    /**
     * @brief Get the current output OnOff
     * just return the last OnOff
     * @return boot On/Off
     */
    bool LedStrip::getOnOff () {
      return OnOff;
    }
  }
}
