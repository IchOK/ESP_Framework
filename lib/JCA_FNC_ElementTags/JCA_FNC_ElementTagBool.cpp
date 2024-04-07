/**
 * @file JCA_FNC_ElementTagBool.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagBool.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagBool::ElementTagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, bool *_Value, String _BtnOnText, String _BtnOffText)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, ElementTagTypes_T::TagBool) {
      Value = _Value;
      BtnOnText = _BtnOnText;
      BtnOffText = _BtnOffText;
    }

    String ElementTagBool::createSetupTag () {
      String SetupTag = createSetupTagBase();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*Value) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonOn) + "\":\"" + BtnOnText + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonOff) + "\":\"" + BtnOffText + "\"";
      return SetupTag;
    }

    JsonObject ElementTagBool::getJsonObject () {
      JsonObject Data = getJsonObjectBase ();
      Data[JCA_FNC_ELEMENTTAGS_JsonValue] = *Value;
      Data[JCA_FNC_ELEMENTTAGS_JsonOn] = BtnOnText;
      Data[JCA_FNC_ELEMENTTAGS_JsonOff] = BtnOffText;
      return Data;
    }

    void ElementTagBool::addJsonTag (JsonObject &_Tags) {
      _Tags[Name] = *Value;
    }
  }
}