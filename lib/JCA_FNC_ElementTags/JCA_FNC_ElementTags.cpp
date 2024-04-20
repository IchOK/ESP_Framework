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
    ElementTag::ElementTag (String _Name, String _Text, String _Comment, bool _ReadOnly, void *_Value, ElementTagTypes_T _Type, ElementTagUsage_T _Usage, SetCallback _CB) {
      Type = _Type;
      Usage = _Usage;
      Name = _Name;
      Text = _Text;
      Comment = _Comment;
      ReadOnly = _ReadOnly;
      Value = _Value;
      afterSetCB = _CB;
    }

    ElementTag::ElementTag (String _Name, String _Text, String _Comment, bool _ReadOnly, void *_Value, ElementTagTypes_T _Type, ElementTagUsage_T _Usage) {
      Type = _Type;
      Usage = _Usage;
      Name = _Name;
      Text = _Text;
      Comment = _Comment;
      ReadOnly = _ReadOnly;
      Value = _Value;
    }

    String ElementTag::createSetupTagBase () {
      String SetupTag;
      SetupTag += "\"" + String (JCA_FNC_ELEMENTTAGS_JsonName) + "\":\"" + Name + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonText) + "\":\"" + Text + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonType) + "\":" + String(Type);
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonReadOnly) + "\":" + String(ReadOnly);
      if (Comment.length() > 0) {
        SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonComment) + "\":\"" + String (Comment) + "\"";
      }
      return SetupTag;
    }

    void ElementTag::getBaseObject (JsonObject &_Tag) {
      _Tag[JCA_FNC_ELEMENTTAGS_JsonName] = Name;
      _Tag[JCA_FNC_ELEMENTTAGS_JsonText] = Text;
      _Tag[JCA_FNC_ELEMENTTAGS_JsonType] = Type;
      _Tag[JCA_FNC_ELEMENTTAGS_JsonReadOnly] = ReadOnly;
      _Tag[JCA_FNC_ELEMENTTAGS_JsonComment] = Comment;
    }
  }
}