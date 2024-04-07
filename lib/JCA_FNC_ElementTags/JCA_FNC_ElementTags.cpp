/**
 * @file JCA_FNC_ElementTags.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTags.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    ElementTag::ElementTag (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagTypes_T _Type) {
      Type = _Type;
      Name = _Name;
      Text = _Text;
      Comment = _Comment;
      ReadOnly = _ReadOnly;
    }

    String ElementTag::createSetupTagBase () {
      String SetupTag;
      SetupTag += "\"" + String (JCA_FNC_ELEMENTTAGS_JsonName) + "\":\"" + Name + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonText) + "\":\"" + Text + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonType) + "\":\"" + String(Type) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonReadOnly) + "\":\"" + String(ReadOnly) + "\"";
      if (Comment.length() > 0) {
        SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonComment) + "\":\"" + String (Comment) + "\"";
      }
      return SetupTag;
    }

    void ElementTag::getJsonObjectBase (JsonObject &_Data) {
      _Data[JCA_FNC_ELEMENTTAGS_JsonName] = Name;
      _Data[JCA_FNC_ELEMENTTAGS_JsonText] = Text;
      _Data[JCA_FNC_ELEMENTTAGS_JsonType] = Type;
      _Data[JCA_FNC_ELEMENTTAGS_JsonReadOnly] = ReadOnly;
      _Data[JCA_FNC_ELEMENTTAGS_JsonComment] = Comment;
    }
  }
}