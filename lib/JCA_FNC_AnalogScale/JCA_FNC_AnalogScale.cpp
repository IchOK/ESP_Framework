/**
 * @file JCA_FNC_AnalogScale.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element Scale a analog Value with filter function
 * @version 0.1
 * @date 2023-07-28
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#include <JCA_FNC_AnalogScale.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /* #region(collapsed) Datapoint description */
    const char *AnalogScale::ScaledMin_Name = "ScaledMin";
    const char *AnalogScale::ScaledMin_Text = "Rohwert Leer";
    const char *AnalogScale::ScaledMin_Unit = "#";
    const char *AnalogScale::ScaledMin_Comment = nullptr;
    const char *AnalogScale::ScaledMax_Name = "ScaledMax";
    const char *AnalogScale::ScaledMax_Text = "Rohwert Voll";
    const char *AnalogScale::ScaledMax_Unit = "#";
    const char *AnalogScale::ScaledMax_Comment = nullptr;
    const char *AnalogScale::Filter_Name = "Filter";
    const char *AnalogScale::Filter_Text = "Alarm Grenzwert";
    const char *AnalogScale::Filter_Unit = "%";
    const char *AnalogScale::Filter_Comment = nullptr;
    const char *AnalogScale::Value_Name = "AnalogScale";
    const char *AnalogScale::Value_Text = "Niveau";
    const char *AnalogScale::Value_Unit = "%";
    const char *AnalogScale::Value_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new AnalogScale::AnalogScale object
     *
     * @param _Pin Analog Pin conected to the AnalogScale-Sensor
     * @param _Name Element Name inside the Communication
     */
    AnalogScale::AnalogScale (uint8_t _Pin, const char *_Name)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      ScaledMin = 0.0;
      ScaledMax = 100.0;
#ifdef ESP32
      RawMax = 4095.0;
#elif ESP8266
      RawMax = 1023.0;
#else
      RawMax = 1023.0;
#endif
      Pin = _Pin;
      Value = 0.0;
      Filter = 0.0;
      InitDone = false;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void AnalogScale::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[ScaledMin_Name] = ScaledMin;
      _Values[ScaledMax_Name] = ScaledMax;
      _Values[Filter_Name] = Filter;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void AnalogScale::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Value_Name] = Value;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void AnalogScale::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == ScaledMin_Name) {
          ScaledMin = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ScaledMin_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ScaledMin);
          }
        }
        if (Tag[JsonTagName] == ScaledMax_Name) {
          ScaledMax = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ScaledMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ScaledMax);
          }
        }
        if (Tag[JsonTagName] == Filter_Name) {
          Filter = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, Filter_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, Filter);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void AnalogScale::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Execute the Commands
     * currently not used
     * @param _Tags Array of Commands ("cmd": [])
     */
    void AnalogScale::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void AnalogScale::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (ScaledMin_Name, ScaledMin_Text, ScaledMin_Comment, false, ScaledMin_Unit, ScaledMin) + "}");
      _SetupFile.println (",{" + createSetupTag (ScaledMax_Name, ScaledMax_Text, ScaledMax_Comment, false, ScaledMax_Unit, ScaledMax) + "}");
      _SetupFile.println (",{" + createSetupTag (Filter_Name, Filter_Text, Filter_Comment, false, Filter_Unit, Filter) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void AnalogScale::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (Value_Name, Value_Text, Value_Comment, true, Value_Unit, Value) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void AnalogScale::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Attahs the pin to ADC
     *
     * @return true configuration successfull
     * @return false configuration failed
     */
    bool AnalogScale::init () {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Setup");
#ifdef ESP32
      InitDone = adcAttachPin (Pin);
#elif ESP8266
      InitDone = Pin == A0;
#else
      InitDone = true;
#endif
      if (ScaledMax == ScaledMin) {
        Debug.println (FLAG_ERROR, false, Name, __func__, "Scale-Data not valid");
        InitDone = false;
      }
      LastMillis = millis ();
      return InitDone;
    }

    /**
     * @brief Handling AnalogScale-Sensor
     * Read and scale the AnalogScale and check if the Alarm value is reached
     * @param time Current Time to check the Samplerate
     */
    void AnalogScale::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      if (InitDone) {
        float RawValue = (float)analogRead (Pin);
        uint32_t ActMillis = millis ();
        float Scaled = RawValue / RawMax * (ScaledMax - ScaledMin) + ScaledMin;
        float FilterFactor = Filter / ((float)(ActMillis - LastMillis) / 1000.0);
        Value = ((Value * FilterFactor) + Scaled) / (FilterFactor + 1.0);
        LastMillis = ActMillis;
      }
    }

    /**
     * @brief Get the scaled AnalogScale Value
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current AnalogScale (0-100%)
     */
    float AnalogScale::getValue () {
      return Value;
    }
  }
}
