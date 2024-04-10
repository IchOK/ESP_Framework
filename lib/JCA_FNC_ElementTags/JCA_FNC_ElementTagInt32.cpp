/**
 * @file JCA_FNC_ElementTagInt32.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagInt32.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagInt32::ElementTagInt32 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, int32_t *_Value, String _Unit)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagFloat, _Usage) {
      Unit = _Unit;
    }

    String ElementTagInt32::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<int32_t *> (Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonUnit) + "\":\"" + Unit + "\"";
      return SetupTag;
    }

    void ElementTagInt32::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<int32_t *> (Value));
      _Tag[JCA_FNC_ELEMENTTAGS_JsonUnit] = Unit;
    }

    void ElementTagInt32::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<int32_t *> (Value));
    }
  }
}