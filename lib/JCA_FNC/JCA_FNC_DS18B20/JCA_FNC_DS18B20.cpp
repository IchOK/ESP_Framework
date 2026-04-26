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

#include "JCA_FNC_DS18B20.h"
#include "JCA_SYS_DebugOut.h"
#include "JCA_TAG_TagArrayUInt8.h"
#include "JCA_TAG_TagBool.h"
#include "JCA_TAG_TagFloat.h"
#include "JCA_TAG_TagUInt16.h"
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
      Tags.push_back (new TagFloat ("Filter", "Filterkonstante", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &Filter, "s"));
      Tags.push_back (new TagArrayUInt8 ("Addr", "Sensoradresse", "Sensoradress HEX Codiert, ohne führende Fomatkennzeichnung", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &Addr[0], 8, std::bind (&DS18B20::addrChanged, this)));
      Tags.push_back (new TagUInt16 ("ReadInterval", "Leseintervall", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &ReadInterval, "s"));

      // Zwei-Punkt-Kalibrierung
      Tags.push_back (new TagBool ("CalEnable", "Kalibrierung aktiv", "Aus: Rohwert wird ausgegeben. Ein: lineare 2-Punkt-Kalibrierung wird angewendet.", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &CalEnable, "EIN", "AUS"));
      Tags.push_back (new TagFloat ("CalRawLow", "Kalibr. Rohwert unten", "Vom Sensor gemessener Rohwert beim unteren Referenzpunkt", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &CalRawLow, "°C"));
      Tags.push_back (new TagFloat ("CalRefLow", "Kalibr. Sollwert unten", "Tatsächliche Temperatur am unteren Referenzpunkt", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &CalRefLow, "°C"));
      Tags.push_back (new TagFloat ("CalRawHigh", "Kalibr. Rohwert oben", "Vom Sensor gemessener Rohwert beim oberen Referenzpunkt", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &CalRawHigh, "°C"));
      Tags.push_back (new TagFloat ("CalRefHigh", "Kalibr. Sollwert oben", "Tatsächliche Temperatur am oberen Referenzpunkt", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &CalRefHigh, "°C"));

      Tags.push_back (new TagFloat ("Temp", "Temperatur", "Kalibrierter Messwert (oder Rohwert wenn Kalibrierung aus)", TagAccessType_T::Read, TagUsage_T::UseData, &Value, "°C"));
      Tags.push_back (new TagFloat ("RawTemp", "Roh-Temperatur", "Unkalibrierter Messwert direkt vom Sensor", TagAccessType_T::Read, TagUsage_T::UseData, &RawValue, "°C"));
      // Init Data
      Wire = _Wire;
      for (uint8_t i = 0; i < ONEWIRE_ADDRSIZE; i++) {
        Addr[i] = 0;
      }
      AddrIsValid = false;
      ReadInterval = 1;
      Filter = 5.0;
      CalEnable = false;
      CalRawLow = 0.0f;
      CalRefLow = 0.0f;
      CalRawHigh = 100.0f;
      CalRefHigh = 100.0f;
      Value = 0.0;
      RawValue = 0.0;
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

      // if the Address is not Valid, the first Sensor will be selected
      if (!AddrIsValid) {
        if (DiffMillis < ReadInterval * 1000) {
          return;
        }
        LastMillis = millis ();

        uint32_t FoundValid = 0;
        uint8_t SearchAddr[ONEWIRE_ADDRSIZE];
        Wire->reset_search ();
        while (Wire->search (SearchAddr)) {
          Debug.print (FLAG_SETUP, false, Name, __func__, "Found : ");
          Debug.println (FLAG_SETUP, false, Name, __func__, SearchAddr, ONEWIRE_ADDRSIZE, HEX);
          if (validAddr (SearchAddr)) {
            FoundValid++;
            if (validFamily (SearchAddr)) {
              memcpy (Addr, SearchAddr, ONEWIRE_ADDRSIZE);
              AddrIsValid = true;
              break;
            }
          }
        }
        if (!AddrIsValid) {
          Debug.println (FLAG_ERROR, false, Name, __func__, "No valid DS18B20 found");
          return;
        } else {
          Debug.print (FLAG_SETUP, false, Name, __func__, "Found DS18B20: ");
          Debug.println (FLAG_SETUP, false, Name, __func__, Addr, ONEWIRE_ADDRSIZE, HEX);
        }
        return;
      }

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
            RawValue = (float)raw / 16.0;
            Value = applyCalibration (RawValue);
            if (Debug.print (FLAG_LOOP, false, Name, __func__, "Temp")) {
              Debug.print (FLAG_LOOP, false, Name, __func__, DebugSeparator);
              Debug.print (FLAG_LOOP, false, Name, __func__, Value);
              Debug.print (FLAG_LOOP, false, Name, __func__, " (raw ");
              Debug.print (FLAG_LOOP, false, Name, __func__, RawValue);
              Debug.println (FLAG_LOOP, false, Name, __func__, ")");
            }
            this->ReadData = false;
          }
        }
        this->Resend -= DiffMillis;
      } else {
        this->Resend -= DiffMillis;
      }
    }

    void DS18B20::addrChanged () {
      AddrIsValid = validFamily (Addr) && validAddr (Addr);
    }

    /**
     * @brief Apply linear two-point calibration to a raw temperature value.
     *
     * Falls back to the raw value if calibration is disabled or the two raw
     * reference points are equal (which would yield a division by zero).
     *
     * @param _Raw uncalibrated temperature value (°C)
     * @return calibrated temperature value (°C)
     */
    float DS18B20::applyCalibration (float _Raw) const {
      if (!CalEnable) {
        return _Raw;
      }
      const float Span = CalRawHigh - CalRawLow;
      if (fabsf (Span) < 1e-6f) {
        return _Raw;
      }
      return CalRefLow + (_Raw - CalRawLow) * (CalRefHigh - CalRefLow) / Span;
    }

    bool DS18B20::validFamily (uint8_t _Addr[ONEWIRE_ADDRSIZE]) {
      // Check if the address family-code matchs
      switch (_Addr[0]) {
      case DS18B20_Type_T::TYPE_S:
      case DS18B20_Type_T::TYPE_B:
      case DS18B20_Type_T::TYPE_22:
      case DS18B20_Type_T::TYPE_25:
      case DS18B20_Type_T::TYPE_DS28:
        return true;
      default:
        return false;
      }
    }

    bool DS18B20::validAddr (uint8_t _Addr[ONEWIRE_ADDRSIZE]) {
      return (Wire->crc8 (_Addr, 7) == _Addr[7]);
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void DS18B20::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
      _Handler.FunctionSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> (SetupTagType, GetSetupSchema));
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
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      String OneWireName;
      OneWire *OneWireRef = static_cast<OneWire *> (GetSetupHardwareRef(SetupTagRefName, OneWireName, Done, _Setup, Log, _Hardware));

      if (Done) {
        _Functions.push_back (new DS18B20(OneWireRef, Name));
        Log["done"] = Name + " (OneWire: " + OneWireName + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }

    void DS18B20::GetSetupSchema(JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray>();
      
      // name parameter
      JsonObject NameParam = Parameters.add<JsonObject>();
      NameParam["name"] = JCA_IOT_FUNCHANDLER_SETUP_NAME;
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Funktion für die Kommunikation";

      // refName parameter
      JsonObject RefNameParam = Parameters.add<JsonObject>();
      RefNameParam["name"] = SetupTagRefName;
      RefNameParam["type"] = "string";
      RefNameParam["comment"] = "Name der Hardware-Instanz";
    }
  }
}
