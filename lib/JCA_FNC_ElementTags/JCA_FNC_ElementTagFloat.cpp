/**
 * @file JCA_FNC_ElementTagFloat.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagFloat.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTagFloat::ElementTagFloat (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, float *_Value, String _Unit)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagFloat, _Usage) {
      Unit = _Unit;
    }

    String ElementTagFloat::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<float *> (Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonUnit) + "\":\"" + Unit + "\"";
      return SetupTag;
    }

    void ElementTagFloat::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<float *> (Value));
      _Tag[JCA_FNC_ELEMENTTAGS_JsonUnit] = Unit;
    }

    void ElementTagFloat::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<float *> (Value));
    }
  }
}