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
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *DS18B20::ClassName = "DS18B20";
    const char *DS18B20::SetupTagType = "ds18b20";
    const char *DS18B20::SetupTagRefName = "refName";
    /**
     * @brief Construct a new DS18B20::DS18B20 object
     *
     * @param _Wire Pointer to the OneWire Interface
     * @param _Name Element Name inside the Communication
     */
    DS18B20::DS18B20 (OneWire *_Wire, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagFloat ("Filter", "Filterkonstante", "", false, TagUsage_T::UseConfig, &Filter, "s"));
      Tags.push_back (new TagArrayUInt8 ("Addr", "Sensoradresse", "Sensoradress HEX Codiert, ohne führende Fomatkennzeichnung", false, TagUsage_T::UseConfig, &Addr[0], 8));
      Tags.push_back (new TagUInt16 ("ReadInterval", "Leseintervall", "", false, TagUsage_T::UseConfig, &ReadInterval, "s"));

      Tags.push_back (new TagFloat ("Temp", "Temperatur", "", true, TagUsage_T::UseData, &Value, "°C"));
      // Init Data
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
            if (Debug.print (FLAG_LOOP, false, Name, __func__, "Temp")) {
              Debug.print (FLAG_LOOP, false, Name, __func__, DebugSeparator);
              Debug.println (FLAG_LOOP, false, Name, __func__, Value);
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
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void DS18B20::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
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
    bool DS18B20::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log.createNestedObject (SetupTagType);

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, _Log);
      String OneWireName;
      OneWire *OneWireRef = static_cast<OneWire *> (GetSetupHardwareRef(SetupTagRefName, OneWireName, Done, _Setup, _Log, _Hardware));

      if (Done) {
        _Functions.push_back (new DS18B20(OneWireRef, Name));
        Log["done"] = Name + " (OneWire: " + OneWireName + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
