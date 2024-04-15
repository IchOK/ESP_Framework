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
    /**
     * @brief Construct a new DS18B20::DS18B20 object
     *
     * @param _Wire Pointer to the OneWire Interface
     * @param _Name Element Name inside the Communication
     */
    DS18B20::DS18B20 (OneWire *_Wire, const char *_Name)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new ElementTagFloat ("Filter", "Filterkonstante", "", false, ElementTagUsage_T::UseConfig, &Filter, "s"));
      Tags.push_back (new ElementTagArrayUInt8 ("Addr", "Sensoradresse", "Sensoradress HEX Codiert, ohne führende Fomatkennzeichnung", false, ElementTagUsage_T::UseConfig, &Addr[0], 8));
      Tags.push_back (new ElementTagUInt16 ("ReadInterval", "Leseintervall", "", false, ElementTagUsage_T::UseConfig, &ReadInterval, "s"));

      Tags.push_back (new ElementTagFloat ("Temp", "Temperatur", "", true, ElementTagUsage_T::UseData, &Value, "°C"));
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
     * @brief Get the scaled DS18B20 Value
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current DS18B20 (0-100%)
     */
    float DS18B20::getValue () {
      return Value;
    }
  }
}
