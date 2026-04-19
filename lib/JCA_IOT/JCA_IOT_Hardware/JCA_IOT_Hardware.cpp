/**
 * @file JCA_IOT_Hardware.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Collection of creation methods for Hardware-Interfaces, the creation methos has to be added to the HardwareList in Main.cpp
 * @version 1.0
 * @date 2025-07-23
 *
 * Copyright Jochen Cabrera 2025
 * Apache License
 *
 */

#include <JCA_IOT_Hardware.h>

using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    /**
     * @brief Add the OneWire Interface to the HardwareList inside the FunctionHandler
     *
     * @param _Handler Function Handler
     */
    void AddOneWire (FuncHandler &_Handler) {
      _Handler.HardwareList.insert(std::pair<String, std::function<bool (JsonObject, JsonObject, std::map<String, void *>&)>>("onewire", CreateOneWire));
      _Handler.HardwareSchemaList.insert(std::pair<String, std::function<void (JsonObject &)>>("onewire", GetOneWireSchema));
    }

    /**
     * @brief Create a OneWire Interface and add it to the HardwareMapping
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param HardwareMapping Map to add the created Hardware-Interface
     * @return true if created
     * @return false if not created
     */
    bool CreateOneWire(JsonObject _Setup, JsonObject _Log, std::map<String, void *> &HardwareMapping) {
      Debug.println(FLAG_SETUP, true, "Hardware", __func__, "Start");
      bool Done = false;
      String HwName = _Setup["name"].as<String>();
      if (HwName.length() > 0) {
        HardwareMapping[HwName] = new OneWire (_Setup["pin"].as<uint8_t> ());
        Done = true;
        Debug.println (FLAG_SETUP, true, "Hardware", __func__, "Created");
      } else {
        Debug.println (FLAG_ERROR, true, "Hardware", __func__, "No Name given for OneWire Interface");
        _Log["Fault"] = "No Name given for OneWire Interface";
      }
      return Done;
    }

    /**
     * @brief Add the PwmOutput Interface to the HardwareList inside the FunctionHandler
     *
     * @param _Handler Function Handler
     */
    void AddPwmOutput(FuncHandler &_Handler) {
      _Handler.HardwareList.insert(std::pair<String, std::function<bool (JsonObject, JsonObject, std::map<String, void *>&)>>("pwmoutput", CreatePwmOutput));
      _Handler.HardwareSchemaList.insert(std::pair<String, std::function<void (JsonObject &)>>("pwmoutput", GetPwmOutputSchema));
    }

    /**
     * @brief Create a PwmOutput Interface and add it to the HardwareMapping
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param HardwareMapping Map to add the created Hardware-Interface
     * @return true if created
     * @return false if not created
     */
    bool CreatePwmOutput(JsonObject _Setup, JsonObject _Log, std::map<String, void *> &HardwareMapping) {
      Debug.println(FLAG_SETUP, true, "Hardware", __func__, "Start");
      bool Done = false;
      String HwName = _Setup["name"].as<String>();
      if (HwName.length() > 0) {
        HardwareMapping[HwName] = new JCA::SYS::PwmOutput();
        Done = true;
        Debug.println(FLAG_SETUP, true, "Hardware", __func__, "Created");
      } else {
        Debug.println(FLAG_ERROR, true, "Hardware", __func__, "No Name given for PwmOutput Interface");
        _Log["Fault"] = "No Name given for PwmOutput Interface";
      }
      return Done;
    }

    /**
     * @brief Get setup schema for OneWire hardware
     * 
     * @param _Schema JSON object to fill with schema information
     */
    void GetOneWireSchema (JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray>();
      
      // name parameter
      JsonObject NameParam = Parameters.add<JsonObject>();
      NameParam["name"] = "name";
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Hardware-Instanz";
      
      // pin parameter
      JsonObject PinParam = Parameters.add<JsonObject>();
      PinParam["name"] = "pin";
      PinParam["type"] = "uint8";
      PinParam["comment"] = "GPIO-Pin für OneWire-Bus";
    }

    /**
     * @brief Get setup schema for PwmOutput hardware
     * 
     * @param _Schema JSON object to fill with schema information
     */
    void GetPwmOutputSchema (JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray>();
      
      // name parameter
      JsonObject NameParam = Parameters.add<JsonObject>();
      NameParam["name"] = "name";
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Hardware-Instanz";
    }

    /**
     * @brief Add the TwoWire (I2C) Interface to the HardwareList inside the FunctionHandler
     *
     * @param _Handler Function Handler
     */
    void AddTwoWire (FuncHandler &_Handler) {
      _Handler.HardwareList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::map<String, void *>&)>> ("twowire", CreateTwoWire));
      _Handler.HardwareSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> ("twowire", GetTwoWireSchema));
    }

    /**
     * @brief Create a TwoWire (I2C) Interface and add it to the HardwareMapping.
     *
     * Setup keys:
     *  - name (string, required) : key in the HardwareMapping
     *  - bus  (uint, optional)   : I2C bus index (ESP32: 0 or 1; ignored on ESP8266)
     *  - sda  (int,  optional)   : SDA pin (-1 = use board default)
     *  - scl  (int,  optional)   : SCL pin (-1 = use board default)
     *  - freq (uint, optional)   : I2C clock frequency in Hz (default 100000)
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param HardwareMapping Map to add the created Hardware-Interface
     * @return true if created
     * @return false if not created
     */
    bool CreateTwoWire (JsonObject _Setup, JsonObject _Log, std::map<String, void *> &HardwareMapping) {
      Debug.println (FLAG_SETUP, true, "Hardware", __func__, "Start");
      bool Done = false;
      String HwName = _Setup["name"].as<String> ();
      if (HwName.length () == 0) {
        Debug.println (FLAG_ERROR, true, "Hardware", __func__, "No Name given for TwoWire Interface");
        _Log["Fault"] = "No Name given for TwoWire Interface";
        return false;
      }

      int Sda = _Setup["sda"].is<int> () ? _Setup["sda"].as<int> () : -1;
      int Scl = _Setup["scl"].is<int> () ? _Setup["scl"].as<int> () : -1;
      uint32_t Freq = _Setup["freq"].is<uint32_t> () ? _Setup["freq"].as<uint32_t> () : 100000UL;

      TwoWire *Wire_Ref = nullptr;
#if defined(ESP32)
      uint8_t Bus = _Setup["bus"].is<int> () ? _Setup["bus"].as<uint8_t> () : 0;
      if (Bus > 1) {
        Bus = 0;
      }
      Wire_Ref = new TwoWire (Bus);
      if (Sda >= 0 && Scl >= 0) {
        Wire_Ref->begin (Sda, Scl, Freq);
      } else {
        Wire_Ref->begin ();
        Wire_Ref->setClock (Freq);
      }
#else
      // ESP8266 / AVR: only one TwoWire instance, no bus index
      Wire_Ref = &Wire;
      if (Sda >= 0 && Scl >= 0) {
        Wire_Ref->begin (Sda, Scl);
      } else {
        Wire_Ref->begin ();
      }
      Wire_Ref->setClock (Freq);
#endif

      HardwareMapping[HwName] = Wire_Ref;
      Done = true;
      Debug.print (FLAG_SETUP, true, "Hardware", __func__, "Created (sda=");
      Debug.print (FLAG_SETUP, true, "Hardware", __func__, Sda);
      Debug.print (FLAG_SETUP, true, "Hardware", __func__, ", scl=");
      Debug.print (FLAG_SETUP, true, "Hardware", __func__, Scl);
      Debug.print (FLAG_SETUP, true, "Hardware", __func__, ", freq=");
      Debug.print (FLAG_SETUP, true, "Hardware", __func__, Freq);
      Debug.println (FLAG_SETUP, true, "Hardware", __func__, ")");
      return Done;
    }

    /**
     * @brief Get setup schema for TwoWire (I2C) hardware
     *
     * @param _Schema JSON object to fill with schema information
     */
    void GetTwoWireSchema (JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray> ();

      JsonObject NameParam = Parameters.add<JsonObject> ();
      NameParam["name"] = "name";
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Hardware-Instanz";

#if defined(ESP32)
      JsonObject BusParam = Parameters.add<JsonObject> ();
      BusParam["name"] = "bus";
      BusParam["type"] = "uint8";
      BusParam["comment"] = "I2C-Bus (0 oder 1, optional, Default 0)";
#endif

      JsonObject SdaParam = Parameters.add<JsonObject> ();
      SdaParam["name"] = "sda";
      SdaParam["type"] = "int8";
      SdaParam["comment"] = "SDA-Pin (optional, -1 = Board-Standard)";

      JsonObject SclParam = Parameters.add<JsonObject> ();
      SclParam["name"] = "scl";
      SclParam["type"] = "int8";
      SclParam["comment"] = "SCL-Pin (optional, -1 = Board-Standard)";

      JsonObject FreqParam = Parameters.add<JsonObject> ();
      FreqParam["name"] = "freq";
      FreqParam["type"] = "uint32";
      FreqParam["comment"] = "Bus-Frequenz in Hz (optional, Default 100000)";
    }
  }
}
