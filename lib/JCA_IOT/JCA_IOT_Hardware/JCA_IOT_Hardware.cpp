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
  }
}
