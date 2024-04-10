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
    ElementTagListUInt8::ElementTagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value, String _Default)
    : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagListUInt8, _Usage) {
      Default = _Default;
    }

    String ElementTagListUInt8::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      int16_t Counter = 0;
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<uint8_t *> (Value))) + "\"";
      // for each Listmap
      for (const auto &[Index, Text] : List) {
        if (Counter == 0) {
          SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonList) + "\": [";
          SetupTag += "{\"" + String (JCA_FNC_ELEMENTTAGS_JsonListIndex) + "\":\"" + String(Index) + "\",\"" + String (JCA_FNC_ELEMENTTAGS_JsonListValue) + "\":\"" + Text + "\"}";
        } else {
          SetupTag += ",{\"" + String (JCA_FNC_ELEMENTTAGS_JsonListIndex) + "\":\"" + String (Index) + "\",\"" + String (JCA_FNC_ELEMENTTAGS_JsonListValue) + "\":\"" + Text + "\"}";
        }
        Counter++;
      }
      if (Counter > 0) {
        SetupTag += "]";
      }
      return SetupTag;
    }

    void ElementTagListUInt8::getTagObject (JsonObject &_Tag) {
      JsonArray JsonList;
      JsonObject ListElement;
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<uint8_t *> (Value));
      JsonList = _Tag.createNestedArray (JCA_FNC_ELEMENTTAGS_JsonList);
      for (const auto &[Index, Text] : List) {
        ListElement = JsonList.createNestedObject();
        ListElement[JCA_FNC_ELEMENTTAGS_JsonListIndex] = Index;
        ListElement[JCA_FNC_ELEMENTTAGS_JsonListValue] = Text;
      }
    }

    void ElementTagListUInt8::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<uint8_t *> (Value));
    }
  }
}