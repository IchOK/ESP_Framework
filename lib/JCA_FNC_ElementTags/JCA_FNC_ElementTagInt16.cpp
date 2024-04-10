/**
 * @file JCA_FNC_ElementTagInt16.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagInt16.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagInt16::ElementTagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, int16_t *_Value, String _Unit)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagInt16, _Usage) {
      Unit = _Unit;
    }

    String ElementTagInt16::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<int16_t *> (Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonUnit) + "\":\"" + Unit + "\"";
      return SetupTag;
    }

    void ElementTagInt16::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<int16_t *> (Value));
      _Tag[JCA_FNC_ELEMENTTAGS_JsonUnit] = Unit;
    }

    void ElementTagInt16::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<int16_t *> (Value));
    }
  }
}