/**
 * @file JCA_FNC_ElementTagString.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagString.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagString::ElementTagString (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, String *_Value)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagFloat, _Usage) {
    }

    String ElementTagString::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<String *> (Value))) + "\"";
      return SetupTag;
    }

    void ElementTagString::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<String *> (Value));
    }

    void ElementTagString::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<String *> (Value));
    }
  }
}