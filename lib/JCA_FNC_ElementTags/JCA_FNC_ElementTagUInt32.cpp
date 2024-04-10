/**
 * @file JCA_FNC_ElementTagUInt32.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagUInt32.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagUInt32::ElementTagUInt32 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint32_t *_Value, String _Unit)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagFloat, _Usage) {
      Unit = _Unit;
    }

    String ElementTagUInt32::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<uint32_t *> (Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonUnit) + "\":\"" + Unit + "\"";
      return SetupTag;
    }

    void ElementTagUInt32::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<uint32_t *> (Value));
      _Tag[JCA_FNC_ELEMENTTAGS_JsonUnit] = Unit;
    }

    void ElementTagUInt32::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<uint32_t *> (Value));
    }
  }
}