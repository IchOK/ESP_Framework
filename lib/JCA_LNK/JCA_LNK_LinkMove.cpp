/**
 * @file JCA_LNK_LinkMove.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Move link type implementation
 * @version 1.0
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#include "JCA_LNK_LinkMove.h"

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkMove::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Input wird auf Output geschrieben, wenn der Selector aktiv ist";
      _Schema["fromMin"] = 2;
      _Schema["fromMax"] = 2;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1; // -1 means unlimited
      
      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Selector: Wert, der entscheidet ob der Link aktiv ist");
      FromDesc.add("Input: Wert, der gelesen wird");
      
      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Outputs: Werte die beschrieben werden");
    }
    
    bool LinkMove::Update(JsonArray _Inputs, JsonVariant &_Output) {
      if (_Inputs.size() == 2) {
        JsonVariant Selector = _Inputs[0];
        JsonVariant Input = _Inputs[1];
        if (Selector.as<bool>()) {
          _Output = Input;
          return true;
        }
      }
      return false;
    }

    void LinkMove::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      // Register link type in mapping
      _Handler.LinkMapping["move"] = Update;
      
      // Register schema function
      _Handler.LinkSchemaList["move"] = GetSetupSchema;
    }
  }
}
