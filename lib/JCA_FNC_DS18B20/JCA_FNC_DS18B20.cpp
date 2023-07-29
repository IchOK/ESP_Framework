/**
 * @file JCA_FNC_DS18B20.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to get Data from DS18B20 Sensor
 * @version 1.0
 * @date 2023-01-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_DS18B20.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /* #region(collapsed) Datapoint description */
    const char *DS18B20::Filter_Name = "Filter";
    const char *DS18B20::Filter_Text = "Filterkonstante";
    const char *DS18B20::Filter_Unit = "s";
    const char *DS18B20::Filter_Comment = nullptr;
    const char *DS18B20::Addr_Name = "Addr";
    const char *DS18B20::Addr_Text = "Sensoradresse";
    const char *DS18B20::Addr_Unit = "X";
    const char *DS18B20::Addr_Comment = "Sensoradress HEX Codiert, ohne führende Fomatkennzeichnung";
    const char *DS18B20::ReadInterval_Name = "ReadInterval";
    const char *DS18B20::ReadInterval_Text = "Leseintervall";
    const char *DS18B20::ReadInterval_Unit = "s";
    const char *DS18B20::ReadInterval_Comment = nullptr;
    const char *DS18B20::Temp_Name = "Temp";
    const char *DS18B20::Temp_Text = "Temperatur";
    const char *DS18B20::Temp_Unit = "°C";
    const char *DS18B20::Temp_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new DS18B20::DS18B20 object
     *
     * @param _Wire Pointer to the OneWire Interface
     * @param _Name Element Name inside the Communication
     */
    DS18B20::DS18B20 (OneWire *_Wire, const char *_Name)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      Wire = _Wire;
      Addr[0] = 0;
      Addr[1] = 0;
      Addr[2] = 0;
      Addr[3] = 0;
      Addr[4] = 0;
      Addr[5] = 0;
      Addr[6] = 0;
      Addr[7] = 0;
      ReadInterval = 1;
      Filter = 5.0;
      Value = 0.0;
      Resend = 0;
      ReadData = false;
      LastMillis = millis ();
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void DS18B20::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Filter_Name] = Filter;
      _Values[Addr_Name] = ByteArrayToHexString (Addr, 8);
      _Values[ReadInterval_Name] = ReadInterval;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void DS18B20::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Temp_Name] = Value;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void DS18B20::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == Filter_Name) {
          Filter = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, Filter_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, Filter);
          }
        }
        if (Tag[JsonTagName] == Addr_Name) {
          String AddrHex = Tag[JsonTagValue].as<String> ();
          // Convert HEX-String to Byte-Array
          HexStringToByteArray (AddrHex, Addr, 8);
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, Addr_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AddrHex);
          }
        }
        if (Tag[JsonTagName] == ReadInterval_Name) {
          ReadInterval = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ReadInterval_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ReadInterval);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void DS18B20::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Execute the Commands
     * currently not used
     * @param _Tags Array of Commands ("cmd": [])
     */
    void DS18B20::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void DS18B20::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (Filter_Name, Filter_Text, Filter_Comment, false, Filter_Unit, Filter) + "}");
      _SetupFile.println (",{" + createSetupTag (Addr_Name, Addr_Text, Addr_Comment, false, ByteArrayToHexString (Addr, 8)) + "}");
      _SetupFile.println (",{" + createSetupTag (ReadInterval_Name, ReadInterval_Text, ReadInterval_Comment, false, ReadInterval_Unit, ReadInterval) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void DS18B20::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (Temp_Name, Temp_Text, Temp_Comment, true, Temp_Unit, Value) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void DS18B20::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Handling DS18B20-Sensor
     * Read and Skale the Temperature
     * @param time Current Time to check the Samplerate
     */
    void DS18B20::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      int16_t raw;
      uint32_t DiffMillis = millis () - LastMillis;

      // If Resend counts to 0 resend convertion Request
      if (this->Resend <= 0) {
        // OneWire Bus is free to write Data
        if (Wire->reset ()) {
          Wire->select (this->Addr);
          Wire->write (DS18B20_Cmd_T::CONV);
          this->Resend = (uint32_t)(this->ReadInterval);
          this->ReadData = true;
        } else {
          this->Resend = 100;
        }
      }
      // checking if Convetion is Done
      else if (this->ReadData) {
        // OneWire Bus is free to write Data
        if (Wire->reset ()) {
          // send Data Request
          Wire->select (this->Addr);
          Wire->write (DS18B20_Cmd_T::READ);
          Wire->read_bytes (this->Raw, 9);
          // check data Consistens
          if (OneWire::crc8 (this->Raw, 8) == this->Raw[8]) {
            raw = (this->Raw[1] << 8) | Raw[0];
            if (Addr[0] == DS18B20_Type_T::TYPE_S) {
              // Type DS18S20 has special Data-Setup, allways use 9 bit resolition
              raw = raw << 3;
              if (this->Raw[7] == 0x10) {
                raw = (raw & 0xFFF0) + 12 - this->Raw[6];
              }
            } else {
              byte cfg = (this->Raw[4] & 0x60);
              switch (cfg) {
              case 0x00:
                // 9 bit resolution, 93.75 ms
                raw = raw & ~7;
                break;
              case 0x20:
                // 10 bit res, 187.5 ms
                raw = raw & ~3;
                break;
              case 0x40:
                // 11 bit res, 375 ms
                raw = raw & ~1;
              }
            }
            Value = (float)raw / 16.0;
            if (Debug.print (FLAG_LOOP, false, Name, __func__, Temp_Name)) {
              Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
              Debug.println (FLAG_CONFIG, false, Name, __func__, Value);
            }
            this->ReadData = false;
          }
        }
        this->Resend -= DiffMillis;
      } else {
        this->Resend -= DiffMillis;
      }
    }

    /**
     * @brief Get the scaled DS18B20 Value
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current DS18B20 (0-100%)
     */
    float DS18B20::getValue () {
      return Value;
    }
  }
}
