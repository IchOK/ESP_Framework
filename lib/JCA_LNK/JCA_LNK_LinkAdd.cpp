/**
 * @file JCA_LNK_LinkAdd.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Add link type implementation
 * @version 1.0
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef JCA_IGNORE_LINK_ADD

#include "JCA_LNK_LinkAdd.h"

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkAdd::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Der Input wird zum Output addiert";
      _Schema["fromMin"] = 2;
      _Schema["fromMax"] = -1;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1; // -1 means unlimited
      
      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Inputs: Werte, die addiert werden");
      
      
      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Outputs: Werte die beschrieben werden");
    }
    
    bool LinkAdd::Update(JsonArray _Inputs, JsonVariant &_Output) {
      if (_Inputs.size() >= 2) {
        if (_Inputs[0].is<float>()) {
          float Sum = _Inputs[0].as<float>();
          for (uint8_t i = 1; i < _Inputs.size(); i++) {
            Sum = Sum + _Inputs[i].as<float>();
          }
          _Output.set<float>(Sum);
          return true;
        } else if (_Inputs[0].is<int>()) {
          int Sum = _Inputs[0].as<int>();
          for (uint8_t i = 1; i < _Inputs.size(); i++) {
            Sum = Sum + _Inputs[i].as<int>();
          }
          _Output.set<int>(Sum);
          return true;
        }
      }
      return false;
    }

    void LinkAdd::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      // Register link type in mapping
      _Handler.LinkMapping["add"] = Update;
      
      // Register schema function
      _Handler.LinkSchemaList["add"] = GetSetupSchema;
    }
  }
}

#endif
