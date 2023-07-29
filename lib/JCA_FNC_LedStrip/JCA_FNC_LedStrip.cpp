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
    /* #region(collapsed) Datapoint description */
    const char *LedStrip::DelayAutoOff_Name = "DelayAutoOff";
    const char *LedStrip::DelayAutoOff_Text = "Verzögerung Auto-OFF";
    const char *LedStrip::DelayAutoOff_Unit = "Min";
    const char *LedStrip::DelayAutoOff_Comment = "Wird der Wert auf 0 gesetzt ist die Finktion inaktiv";
    const char *LedStrip::OnOff_Name = "OnOff";
    const char *LedStrip::OnOff_Text = "Eingeschaltet";
    const char *LedStrip::OnOff_Comment = nullptr;
    const char *LedStrip::OnOff_TextOn = nullptr;
    const char *LedStrip::OnOff_TextOff = nullptr;
    const char *LedStrip::DelayCounter_Name = "DelayCounter";
    const char *LedStrip::DelayCounter_Text = "Verzögerung Zähler";
    const char *LedStrip::DelayCounter_Unit = "Min";
    const char *LedStrip::DelayCounter_Comment = nullptr;
    const char *LedStrip::Value_Name = "Value";
    const char *LedStrip::Value_Text = "Farbwert";
    const char *LedStrip::Value_Unit = "COLOR";
    const char *LedStrip::Value_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new LedStrip::LedStrip object
     *
     * @param _Pin Analog Pin conected to the LedStrip-Sensor
     * @param _Name Element Name inside the Communication
     */
    LedStrip::LedStrip (uint8_t _Pin, uint8_t _NumLeds, neoPixelType _Type, const char *_Name)
        : Parent (_Name), Strip (_NumLeds, _Pin, _Type) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      DelayAutoOff = 0;
      OnOff = true;
      Value = 4108721;
      DelayCounter = 0;
      LastMillis = millis ();
      DelayMillis = 0;
      UpdateMillis = 0;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current OnOff
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void LedStrip::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[DelayAutoOff_Name] = DelayAutoOff;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current OnOff
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void LedStrip::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[OnOff_Name] = OnOff;
      _Values[Value_Name] = Value;
      _Values[DelayCounter_Name] = DelayCounter;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void LedStrip::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == DelayAutoOff_Name) {
          DelayAutoOff = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, DelayAutoOff_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, DelayAutoOff);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void LedStrip::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == OnOff_Name) {
          OnOff = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, OnOff_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, OnOff);
          }
        }
        if (Tag[JsonTagName] == Value_Name) {
          Value = Tag[JsonTagValue].as<uint32_t> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, Value_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, Value);
          }
        }
      }
    }

    /**
     * @brief Execute the Commands
     * currently not used
     * @param _Tags Array of Commands ("cmd": [])
     */
    void LedStrip::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void LedStrip::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (DelayAutoOff_Name, DelayAutoOff_Text, DelayAutoOff_Comment, false, DelayAutoOff_Unit, DelayAutoOff) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void LedStrip::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (OnOff_Name, OnOff_Text, OnOff_Comment, false, OnOff_TextOn, OnOff_TextOff, OnOff) + "}");
      _SetupFile.println (",{" + createSetupTag (Value_Name, Value_Text, Value_Comment, false, Value_Unit, Value) + "}");
      _SetupFile.println (",{" + createSetupTag (DelayCounter_Name, DelayCounter_Text, DelayCounter_Comment, true, DelayCounter_Unit, DelayCounter) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void LedStrip::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Init the LED-Strip
     */
    bool LedStrip::init () {
      Strip.begin ();
      Strip.show ();
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
        if (OnOff) {
          Strip.fill (Value, 0, 0); //
        } else {
          Strip.clear (); //
        }
        Strip.show ();
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
