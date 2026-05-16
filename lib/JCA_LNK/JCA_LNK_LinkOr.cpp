/**
 * @file JCA_LNK_LinkOr.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Or link type implementation
 * @version 1.0
 * @date 2026-05-16
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#include "JCA_LNK_LinkOr.h"

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkOr::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Logisches ODER aller Bool-Eingänge";
      _Schema["fromMin"] = 2;
      _Schema["fromMax"] = -1;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1;

      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Inputs: Bool-Werte, die verknüpft werden");

      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Outputs: Bool-Werte, die beschrieben werden");
    }

    bool LinkOr::Update(JsonArray _Inputs, JsonVariant &_Output) {
      if (_Inputs.size() < 2) {
        return false;
      }
      bool Result = _Inputs[0].as<bool>();
      for (uint8_t i = 1; i < _Inputs.size(); i++) {
        Result = Result || _Inputs[i].as<bool>();
      }
      _Output.set<bool>(Result);
      return true;
    }

    void LinkOr::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      _Handler.LinkMapping["or"] = Update;
      _Handler.LinkSchemaList["or"] = GetSetupSchema;
    }
  }
}
