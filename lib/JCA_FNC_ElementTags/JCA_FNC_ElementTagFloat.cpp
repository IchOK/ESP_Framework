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
    ElementTagFloat::ElementTagFloat (String _Name, String _Text, String _Comment, bool _ReadOnly, float *_Value, String _Unit)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, ElementTagTypes_T::TagFloat) {
      Value = _Value;
      Unit = _Unit;
    }

    String ElementTagFloat::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*Value) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonUnit) + "\":\"" + Unit + "\"";
      return SetupTag;
    }

    void ElementTagFloat::getJsonObject (JsonObject &_Data) {
      getJsonObjectBase (_Data);
      _Data[JCA_FNC_ELEMENTTAGS_JsonValue] = *Value;
      _Data[JCA_FNC_ELEMENTTAGS_JsonUnit] = Unit;
    }

    void ElementTagFloat::addJsonTag (JsonObject &_Tags) {
      _Tags[Name] = *Value;
    }
  }
}