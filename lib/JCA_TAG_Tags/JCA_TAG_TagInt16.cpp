/**
 * @file JCA_TAG_TAGInt16.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_TAG_TagInt16.h>
using namespace JCA::SYS;

namespace JCA {
  namespace TAG {
    /**
     * @brief Construct a new TagInt16::TagInt16 object
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
    TagInt16::TagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, int16_t *_Value, String _Unit, SetCallback _CB)
        : TagParent (_Name, _Text, _Comment, _ReadOnly, _Value, TagTypes_T::TypeInt16, _Usage, _CB) {
      Unit = _Unit;
    }

    TagInt16::TagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, int16_t *_Value, String _Unit)
        : TagParent (_Name, _Text, _Comment, _ReadOnly, _Value, TagTypes_T::TypeInt16, _Usage) {
      Unit = _Unit;
    }

    /**
     * @brief Create the complete Json-String of the Tag-Data
     *
     * @return String Json-String
     */
    String TagInt16::writeTag () {
      String SetupTag = writeTagBase ();
      SetupTag += ",\"" + String (JCA_TAG_TAGS_JsonUnit) + "\":\"" + Unit + "\"";
      return SetupTag;
    }

    /**
     * @brief Get the Value into an JsonVariant
     *
     * @param _Value Reference to the JsonVariant to which the value is to be written
     * @return true Value was successfully written to _Value
     * @return false something failed
     */
    bool TagInt16::getValue (JsonVariant _Value) {
      return _Value.set (*(static_cast<int16_t *> (Value)));
    }

    /**
     * @brief Set the value of the Tag
     *
     * @param _Value Value that should be set
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool TagInt16::setValue (JsonVariant _Value) {
      *(static_cast<int16_t *> (Value)) = _Value.as<int16_t> ();
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
    void TagInt16::addValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<int16_t *> (Value));
    }
  }
}