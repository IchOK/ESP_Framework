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

namespace JCA {
  namespace FNC {
    /* #region(collapsed) Datapoint description */
    const char *DigitalOut::DelayAutoOff_Name = "DelayAutoOff";
    const char *DigitalOut::DelayAutoOff_Text = "Verzögerung Auto-OFF";
    const char *DigitalOut::DelayAutoOff_Unit = "Min";
    const char *DigitalOut::DelayAutoOff_Comment = "Wird der Wert auf 0 gesetzt ist die Finktion inaktiv";
    const char *DigitalOut::Value_Name = "Value";
    const char *DigitalOut::Value_Text = "Eingeschaltet";
    const char *DigitalOut::Value_Comment = nullptr;
    const char *DigitalOut::Value_TextOn = nullptr;
    const char *DigitalOut::Value_TextOff = nullptr;
    const char *DigitalOut::DelayCounter_Name = "DelayCounter";
    const char *DigitalOut::DelayCounter_Text = "Verzögerung Zähler";
    const char *DigitalOut::DelayCounter_Unit = "Min";
    const char *DigitalOut::DelayCounter_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    DigitalOut::DigitalOut (uint8_t _Pin, const char *_Name)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      DelayAutoOff = 0;
      Pin = _Pin;
      pinMode (Pin, OUTPUT);
      Value = false;
      DelayCounter = 0;
      LastMillis = millis ();
      DelayMillis = 0;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void DigitalOut::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[DelayAutoOff_Name] = DelayAutoOff;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void DigitalOut::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Value_Name] = Value;
      _Values[DelayCounter_Name] = DelayCounter;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void DigitalOut::setConfig (JsonArray _Tags) {
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
    void DigitalOut::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == Value_Name) {
          Value = Tag[JsonTagValue].as<bool> ();
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
    void DigitalOut::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void DigitalOut::writeSetupConfig (File _SetupFile) {
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
    void DigitalOut::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (Value_Name, Value_Text, Value_Comment, false, Value_TextOn, Value_TextOff, Value) + "}");
      _SetupFile.println (",{" + createSetupTag (DelayCounter_Name, DelayCounter_Text, DelayCounter_Comment, true, DelayCounter_Unit, DelayCounter) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void DigitalOut::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
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
        LastMillis = millis ();
        DelayCounter = 0;
        DelayMillis = 0;
      }

      digitalWrite (Pin, Value);
    }

    /**
     * @brief Get the current output Value
     * just return the last Value
     * @return boot On/Off
     */
    bool DigitalOut::getState () {
      return Value;
    }

    /**
     * @brief Set the current output Value
     * 
     * @param _Value Value to set
     */
    void DigitalOut::setState (bool _Value) {
      Value = _Value;
    }
  }
}
