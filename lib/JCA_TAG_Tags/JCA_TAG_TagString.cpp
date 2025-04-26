/**
 * @file JCA_TAG_TAGString.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.1
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_TAG_TagString.h>
using namespace JCA::SYS;

namespace JCA {
  namespace TAG {
    /**
     * @brief Construct a new TagString::TagString object
     *
     * @param _Name Name of the Element (in JSON)
     * @param _Text Text showen on the website
     * @param _Comment Comment showen on the website if nedded
     * @param _ReadOnly set the Tag to read only, can only write by the Function-Object
     * @param _Usage Usage-Type to sort the Tag on teh website
     * @param _Value Pointer to the Value-Datapoint inside the Function-Object
     * @param _CB Optional Callback-Function, if defined it will execute after setting the new Value
     */
    TagString::TagString (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, String *_Value, SetCallback _CB, TagTypes_T _Type)
        : TagParent (_Name, _Text, _Comment, _ReadOnly, _Value, _Type, _Usage, _CB) {
    }

    TagString::TagString (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, String *_Value, TagTypes_T _Type)
        : TagParent (_Name, _Text, _Comment, _ReadOnly, _Value, _Type, _Usage) {
    }

    /**
     * @brief Create the complete Json-String of the Tag-Data
     *
     * @return String Json-String
     */
    String TagString::writeTag () {
      String SetupTag = writeTagBase ();
      return SetupTag;
    }

    /**
     * @brief Get the Value into an JsonVariant
     *
     * @param _Value Reference to the JsonVariant to which the value is to be written
     * @return true Value was successfully written to _Value
     * @return false something failed
     */
    bool TagString::getValue (JsonVariant _Value) {
      return _Value.set (*(static_cast<String *> (Value)));
    }

    /**
     * @brief Set the value of the Tag
     *
     * @param _Value Value that should be set
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool TagString::setValue (JsonVariant _Value) {
      *(static_cast<String *> (Value)) = _Value.as<String> ();
      if (afterSetCB) {
        afterSetCB ();
      }
      return true;
    }

    /**
     * @brief Create a key-value-pair of the Tag inside an JsonObject
     *
     * @param _Values Reference to tha JsonObject
     */
    void TagString::addValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<String *> (Value));
    }
  }
}