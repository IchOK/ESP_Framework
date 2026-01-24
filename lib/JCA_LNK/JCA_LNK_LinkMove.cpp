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

#include <JCA_LNK_LinkMove.h>
#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>

using namespace JCA::TAG;

namespace JCA {
  namespace LNK {
    void LinkMove::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      // Register link type in mapping
      _Handler.LinkMapping["move"] = JCA::LNK::FuncLinkType_T::LinkMove;
      
      // Register schema function
      _Handler.LinkSchemaList["move"] = GetSetupSchema;
      
      // Register update function
      _Handler.LinkUpdateList[JCA::LNK::FuncLinkType_T::LinkMove] = Update;
    }
    
    void LinkMove::GetSetupSchema(JsonObject &_Schema) {
      _Schema["comment"] = "Move Link: Basierend auf dem ersten From-Tag wird der Wert des zweiten From-Tags an die To-Tags weitergegeben";
      _Schema["fromMin"] = 2;
      _Schema["fromMax"] = 2;
      _Schema["toMin"] = 1;
      _Schema["toMax"] = -1; // -1 means unlimited
      
      JsonArray FromDesc = _Schema["fromDescription"].to<JsonArray>();
      FromDesc.add("Selector: Funktion und Tag, der entscheidet ob der Link aktiv ist (z.B. Kurvenauswahl)");
      FromDesc.add("Input: Funktion und Tag, von dem der Wert gelesen wird (z.B. Verlauf)");
      
      JsonArray ToDesc = _Schema["toDescription"].to<JsonArray>();
      ToDesc.add("Ziel-Funktion(en) und Tag(s), an die der Wert geschrieben wird, wenn Selector aktiv ist");
    }
    
    void LinkMove::Update(JCA::LNK::FuncLink *_Link, std::vector<JCA::FNC::FuncParent *> &_Functions, JsonDocument &_LinkDoc) {
      // Move Link use the first Input-Link to decide if the second Input-Link should be set to all Output-Link
      if (_Link->getInputCount() > 1 && _Link->getOutputCount() > 0) {
        JsonVariant Value = _LinkDoc.to<JsonVariant>();
        JCA::LNK::FuncLinkPair_T Selector = _Link->getInput(0);
        JCA::LNK::FuncLinkPair_T Input = _Link->getInput(1);
        
        if (_Functions[Selector.Func]->getTagValueByIndex(Selector.Tag, Value, TAG::TagAccessType_T::Read)) {
          bool SelectorValue = Value.as<bool>();
          if (SelectorValue) {
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
  }
}
