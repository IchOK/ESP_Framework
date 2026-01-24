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

#include <JCA_LNK_LinkDirect.h>
#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkDirect::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      // Register link type in mapping
      _Handler.LinkMapping["direct"] = JCA::LNK::FuncLinkType_T::LinkDirect;
      
      // Register schema function
      _Handler.LinkSchemaList["direct"] = GetSetupSchema;
      
      // Register update function
      _Handler.LinkUpdateList[JCA::LNK::FuncLinkType_T::LinkDirect] = Update;
    }
    
    void LinkDirect::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Direkter Link: Wert von einem From-Tag wird an alle To-Tags weitergegeben";
      _Schema["fromMin"] = 1;
      _Schema["fromMax"] = 1;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1; // -1 means unlimited
      
      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Quell-Funktion und Tag, von dem der Wert gelesen wird");
      
      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Ziel-Funktion(en) und Tag(s), an die der Wert geschrieben wird");
    }
    
    void LinkDirect::Update(JCA::LNK::FuncLink *_Link, std::vector<JCA::FNC::FuncParent *> &_Functions, JsonDocument &_LinkDoc) {
      // Direct Link always read the first Input-Link and set it to all Output-Links
      if (_Link->getInputCount() > 0 && _Link->getOutputCount() > 0) {
        JsonVariant Value = _LinkDoc.to<JsonVariant>();
        JCA::LNK::FuncLinkPair_T Input = _Link->getInput(0);
        
        if (_Functions[Input.Func]->getTagValueByIndex(Input.Tag, Value, TAG::TagAccessType_T::Read)) {
          for (uint8_t OutputIndex = 0; OutputIndex < _Link->getOutputCount(); OutputIndex++) {
            JCA::LNK::FuncLinkPair_T Output = _Link->getOutput(OutputIndex);
            _Functions[Output.Func]->setTagValueByIndex(Output.Tag, Value, TAG::TagAccessType_T::Write);
          }
        }
      }
    }
  }
}
