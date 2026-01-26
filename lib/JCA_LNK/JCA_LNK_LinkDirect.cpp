/**
 * @file JCA_LNK_LinkDirect.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Direct link type implementation
 * @version 1.0
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#include "JCA_LNK_LinkDirect.h"
#include "ArduinoJson/Array/JsonArray.hpp"

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkDirect::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Der Wert wird unverändert weitergegeben";
      _Schema["fromMin"] = 1;
      _Schema["fromMax"] = 1;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1; // -1 means unlimited
      
      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Input: Wert, der gelesen wird");
      
      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Outputs: Werte die beschrieben werden");
    }
    
    bool LinkDirect::Update(JsonArray _Inputs, JsonVariant &_Output) {
      if (_Inputs.size() == 1) {
        _Output = _Inputs[0];
        return true;
      }
      return false;
    }

    void LinkDirect::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      // Register link type in mapping
      _Handler.LinkMapping["direct"] = Update;
      
      // Register schema function
      _Handler.LinkSchemaList["direct"] = GetSetupSchema;
    }
    
  }
}
