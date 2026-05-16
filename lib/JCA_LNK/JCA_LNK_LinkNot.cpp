/**
 * @file JCA_LNK_LinkNot.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Not link type implementation
 * @version 1.0
 * @date 2026-05-16
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef JCA_IGNORE_LINK_NOT

#include "JCA_LNK_LinkNot.h"

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkNot::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Logische Negation eines Bool-Eingangs";
      _Schema["fromMin"] = 1;
      _Schema["fromMax"] = 1;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1;

      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Input: Bool-Wert, der negiert wird");

      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Outputs: Bool-Werte, die beschrieben werden");
    }

    bool LinkNot::Update(JsonArray _Inputs, JsonVariant &_Output) {
      if (_Inputs.size() != 1) {
        return false;
      }
      _Output.set<bool>(!_Inputs[0].as<bool>());
      return true;
    }

    void LinkNot::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      _Handler.LinkMapping["not"] = Update;
      _Handler.LinkSchemaList["not"] = GetSetupSchema;
    }
  }
}

#endif
