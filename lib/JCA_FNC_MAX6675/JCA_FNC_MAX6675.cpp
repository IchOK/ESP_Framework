/**
 * @file JCA_FNC_MAX6675.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to get Data from MAX6675 Sensor
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#include <JCA_FNC_MAX6675.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /* #region(collapsed) Datapoint description */
    const char *MAX6675::ReadInterval_Name = "ReadInterval";
    const char *MAX6675::ReadInterval_Text = "Leseintervall";
    const char *MAX6675::ReadInterval_Unit = "ms";
    const char *MAX6675::ReadInterval_Comment = nullptr;
    const char *MAX6675::Value_Name = "Value";
    const char *MAX6675::Value_Text = "Temperatur";
    const char *MAX6675::Value_Unit = "°C";
    const char *MAX6675::Value_Comment = nullptr;
    const char *MAX6675::Filter_Name = "Filter";
    const char *MAX6675::Filter_Text = "Filterkonstante";
    const char *MAX6675::Filter_Unit = "s";
    const char *MAX6675::Filter_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new MAX6675::MAX6675 object
     *
     * @param _SCLK PIN of the Clock Signal
     * @param _CS Chip-select PIN for the MAX6675
     * @param _MISO Data input PIN
     * @param _Name Element Name inside the Communication
     */
    MAX6675::MAX6675 (int8_t _SCLK, int8_t _CS, int8_t _MISO, const char *_Name)
        : Parent (_Name), Sensor (_SCLK, _CS, _MISO) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      Value = 0.0;
      Filter = 0.0;
      ReadInterval = 1000;
    }
    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void MAX6675::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[ReadInterval_Name] = ReadInterval;
      _Values[Filter_Name] = Filter;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void MAX6675::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Value_Name] = Value;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void MAX6675::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == ReadInterval_Name) {
          ReadInterval = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ReadInterval_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ReadInterval);
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
    void MAX6675::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Execute the Commands
     * currently not used
     * @param _Tags Array of Commands ("cmd": [])
     */
    void MAX6675::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void MAX6675::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (ReadInterval_Name, ReadInterval_Text, ReadInterval_Comment, false, ReadInterval_Unit, ReadInterval) + "}");
      _SetupFile.println (",{" + createSetupTag (Filter_Name, Filter_Text, Filter_Comment, false, Filter_Unit, Filter) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void MAX6675::writeSetupData (File _SetupFile) {
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
    void MAX6675::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Init the Sensor
     */
    bool MAX6675::init () {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Init");
      LastMillis = millis ();
      return true;
    }

    /**
     * @brief Handling MAX6675-Sensor
     * Read and Skale the Temperature
     * @param time Current Time to check the Samplerate
     */
    void MAX6675::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      // Get Update Intervall
      uint32_t ActMillis = millis ();
      UpdateMillis += (ActMillis - LastMillis);
      LastMillis = ActMillis;

      if (UpdateMillis >= ReadInterval) {
        // Read Values from Sensor
        float InValue = Sensor.readCelsius ();
        float FilterFactor = Filter / ((float)UpdateMillis / 1000.0);
        Value = ((Value * FilterFactor) + InValue) / (FilterFactor + 1.0);

        if (Debug.println (FLAG_DATA, false, Name, __func__, "Values:")) {
          Debug.print (FLAG_DATA, false, Name, __func__, " - Temp. [°C]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, Value);
        }
        UpdateMillis = 0;
      }
    }

    /**
     * @brief Set the Update Interval to sync with other Elements
     *
     * @param _ReadInterval new Update Interval
     */
    void MAX6675::setInterval (uint16_t _ReadInterval) {
      ReadInterval = _ReadInterval;
    }

    /**
     * @brief Get the MAX6675 Temperature
     * just return the last filtered Value, don't read the Hardware-Value
     * @return float Current MAX6675 Temperatur [°C]
     */
    float MAX6675::getValue () {
      return Value;
    }
  }
}
