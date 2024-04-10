/**
 * @file JCA_FNC_ElementTagListUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagListUInt8.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagListUInt8::ElementTagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagListUInt8, _Usage) {
    }

    String ElementTagListUInt8::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<uint8_t *> (Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonList) + "\": [";
      // for each Listmap

      SetupTag += "]";
      return SetupTag;
    }

    void ElementTagListUInt8::getTagObject (JsonObject &_Tag) {
      JsonArray jList;
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<uint8_t *> (Value));
      jList = _Tag.createNestedArray (JCA_FNC_ELEMENTTAGS_JsonList);
    }

    void ElementTagListUInt8::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<uint8_t *> (Value));
    }
  }
}