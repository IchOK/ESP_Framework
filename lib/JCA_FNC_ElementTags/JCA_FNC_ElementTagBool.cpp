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
    ElementTagBool::ElementTagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagBool, _Usage) {
      BtnOnText = _BtnOnText;
      BtnOffText = _BtnOffText;
    }

    String ElementTagBool::createSetupTag () {
      String SetupTag = createSetupTagBase();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<bool *>(Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonOn) + "\":\"" + BtnOnText + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonOff) + "\":\"" + BtnOffText + "\"";
      return SetupTag;
    }

    void ElementTagBool::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<bool *> (Value));
      _Tag[JCA_FNC_ELEMENTTAGS_JsonOn] = BtnOnText;
      _Tag[JCA_FNC_ELEMENTTAGS_JsonOff] = BtnOffText;
    }

    void ElementTagBool::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<bool *> (Value));
    }
  }
}