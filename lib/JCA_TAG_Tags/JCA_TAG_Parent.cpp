/**
 * @file JCA_TAG_TAGs.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of TagParent-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_TAG_Parent.h>
using namespace JCA::SYS;

namespace JCA {
  namespace TAG {
    TagParent::TagParent (String _Name, String _Text, String _Comment, bool _ReadOnly, void *_Value, TagTypes_T _Type, TagUsage_T _Usage, SetCallback _CB) {
      Type = _Type;
      Usage = _Usage;
      Name = _Name;
      Text = _Text;
      Comment = _Comment;
      ReadOnly = _ReadOnly;
      Value = _Value;
      afterSetCB = _CB;
    }

    TagParent::TagParent (String _Name, String _Text, String _Comment, bool _ReadOnly, void *_Value, TagTypes_T _Type, TagUsage_T _Usage) {
      Type = _Type;
      Usage = _Usage;
      Name = _Name;
      Text = _Text;
      Comment = _Comment;
      ReadOnly = _ReadOnly;
      Value = _Value;
    }

    /**
     * @brief Create a basic TagParent-Config-String with al default informations
     * 
     * @return String JsonString with default informations
     */
    String TagParent::writeTagBase () {
      String SetupTag;
      SetupTag += "\"" + String (JCA_TAG_TAGS_JsonName) + "\":\"" + Name + "\"";
      SetupTag += ",\"" + String (JCA_TAG_TAGS_JsonText) + "\":\"" + Text + "\"";
      SetupTag += ",\"" + String (JCA_TAG_TAGS_JsonType) + "\":" + String(Type);
      SetupTag += ",\"" + String (JCA_TAG_TAGS_JsonReadOnly) + "\":" + String(ReadOnly);
      if (Comment.length() > 0) {
        SetupTag += ",\"" + String (JCA_TAG_TAGS_JsonComment) + "\":\"" + String (Comment) + "\"";
      }
      return SetupTag;
    }
  }
}