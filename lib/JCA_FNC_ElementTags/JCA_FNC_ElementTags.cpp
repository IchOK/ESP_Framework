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
    ElementTag::ElementTag (String _Name, String _Text, String _Comment, bool _ReadOnly, void* _Value, ElementTagTypes_T _Type, ElementTagUsage_T _Usage) {
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
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonType) + "\":\"" + String(Type) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonReadOnly) + "\":\"" + String(ReadOnly) + "\"";
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

    bool ElementTag::setValue(JsonVariant _Value) {
      switch (Type)
      {
      case ElementTagTypes_T::TagBool:
        *(static_cast<bool *>(Value)) = _Value.as<bool>();
        break;
      case ElementTagTypes_T::TagFloat:
        *(static_cast<float *> (Value)) = _Value.as<float> ();
        break;
      case ElementTagTypes_T::TagInt16:
        *(static_cast<int16_t *> (Value)) = _Value.as<int16_t> ();
        break;
      case ElementTagTypes_T::TagUInt16:
        *(static_cast<uint16_t *> (Value)) = _Value.as<uint16_t> ();
        break;
      case ElementTagTypes_T::TagInt32:
        *(static_cast<int32_t *> (Value)) = _Value.as<int32_t> ();
        break;
      case ElementTagTypes_T::TagUInt32:
        *(static_cast<uint32_t *> (Value)) = _Value.as<uint32_t> ();
        break;
      case ElementTagTypes_T::TagString:
        *(static_cast<String *> (Value)) = _Value.as<String> ();
        break;

      default:
        return false;
        break;
      }
      return true;
    }
    bool ElementTag::getValue (JsonVariant _Value) {
      switch (Type) {
      case ElementTagTypes_T::TagBool:
         _Value.set(*(static_cast<bool *> (Value)));
        break;
      case ElementTagTypes_T::TagFloat:
        _Value.set(*(static_cast<float *> (Value)));
        break;
      case ElementTagTypes_T::TagInt16:
        _Value.set(*(static_cast<int16_t *> (Value)));
        break;
      case ElementTagTypes_T::TagUInt16:
        _Value.set(*(static_cast<uint16_t *> (Value)));
        break;
      case ElementTagTypes_T::TagInt32:
        _Value.set(*(static_cast<int32_t *> (Value)));
        break;
      case ElementTagTypes_T::TagUInt32:
        _Value.set(*(static_cast<uint32_t *> (Value)));
        break;
      case ElementTagTypes_T::TagString:
        _Value.set(*(static_cast<String *> (Value)));
        break;

      default:
        return false;
        break;
      }
      return true;
    }
  }
}