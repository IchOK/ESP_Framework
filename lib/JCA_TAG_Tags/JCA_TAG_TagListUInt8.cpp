/**
 * @file JCA_TAG_TAGListUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_TAG_TagListUInt8.h>
using namespace JCA::SYS;

namespace JCA {
  namespace TAG {
    /**
     * @brief Construct a new TagListUInt8::TagListUInt8 object
     *
     * @param _Name Name of the Element (in JSON)
     * @param _Text Text showen on the website
     * @param _Comment Comment showen on the website if nedded
     * @param _ReadOnly set the Tag to read only, can only write by the Function-Object
     * @param _Usage Usage-Type to sort the Tag on teh website
     * @param _Value Pointer to the Value-Datapoint inside the Function-Object
     * @param _Unit Unit of the Tag, showen on the website
     * @param _CB Optional Callback-Function, if defined it will execute after setting the new Value
     */
    TagListUInt8::TagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint8_t *_Value, SetCallback _CB)
    : TagParent (_Name, _Text, _Comment, _ReadOnly, _Value, TagTypes_T::TypeListUInt8, _Usage, _CB) {
    }

    TagListUInt8::TagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint8_t *_Value)
    : TagParent (_Name, _Text, _Comment, _ReadOnly, _Value, TagTypes_T::TypeListUInt8, _Usage) {
    }

    TagListUInt8::~TagListUInt8() {
      List.clear();
    }

    /**
     * @brief Create the complete Json-String of the Tag-Data
     *
     * @return String Json-String
     */
    String TagListUInt8::writeTag () {
      String SetupTag = writeTagBase ();
      int16_t Counter = 0;
      // for each Listmap
      for (const auto &[Index, Text] : List) {
        if (Counter == 0) {
          SetupTag += ",\"" + String (JCA_TAG_TAGS_JsonList) + "\": [";
          SetupTag += "{\"" + String (JCA_TAG_TAGS_JsonListIndex) + "\":\"" + String (Index) + "\",\"" + String (JCA_TAG_TAGS_JsonListValue) + "\":\"" + Text + "\"}";
        } else {
          SetupTag += ",{\"" + String (JCA_TAG_TAGS_JsonListIndex) + "\":\"" + String (Index) + "\",\"" + String (JCA_TAG_TAGS_JsonListValue) + "\":\"" + Text + "\"}";
        }
        Counter++;
      }
      if (Counter > 0) {
        SetupTag += "]";
      }
      return SetupTag;
    }

    /**
     * @brief Get the Value into an JsonVariant
     *
     * @param _Value Reference to the JsonVariant to which the value is to be written
     * @return true Value was successfully written to _Value
     * @return false something failed
     */
    bool TagListUInt8::getValue (JsonVariant _Value) {
      return _Value.set (*(static_cast<uint8_t *> (Value)));
    }

    /**
     * @brief Set the value of the Tag
     *
     * @param _Value Value that should be set (Intager or String matching the List are possible)
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool TagListUInt8::setValue (JsonVariant _Value) {
      bool RetValue = false;
      if (_Value.is<const char *> ()) {
        String ValueSet = _Value.as<String>();
        for (const auto &[Index, Text] : List) {
          if (Text == ValueSet) {
            *(static_cast<uint8_t *> (Value)) = Index;
            RetValue = true;
          }
        }
      } else {
        *(static_cast<uint8_t *> (Value)) = _Value.as<uint8_t> ();
        RetValue = true;
      }
      if (afterSetCB && RetValue) {
        afterSetCB ();
      }
      return RetValue;
    }

    /**
     * @brief Create a key-value-pair of the Tag inside an JsonObject
     *
     * @param _Values Reference to tha JsonObject
     */
    void TagListUInt8::addValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<uint8_t *> (Value));
    }
  }
}