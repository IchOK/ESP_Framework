/**
 * @file JCA_TAG_TAGArrayUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.2
 * @date 2024-04-14
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_TAG_TagArrayUInt8.h>
using namespace JCA::SYS;

namespace JCA {
  namespace TAG {
    /**
     * @brief Construct a new TagArrayUInt8::TagArrayUInt8 object
     *
     * @param _Name Name of the Element (in JSON)
     * @param _Text Text showen on the website
     * @param _Comment Comment showen on the website if nedded
     * @param _Access Access-Type of the Tag, combination auf TagAccessTypt_T enum
     * @param _Usage Usage-Type to sort the Tag on teh website
     * @param _Value Pointer to the Value-Array inside the Function-Object
     * @param _Length Length of the Value-Array
     * @param _CB Optional Callback-Function, if defined it will execute after setting the new Value
     */
    TagArrayUInt8::TagArrayUInt8 (String _Name, String _Text, String _Comment, TagAccessType_T _Access, TagUsage_T _Usage, uint8_t *_Value, uint8_t _Length, SetCallback _CB)
        : TagParent (_Name, _Text, _Comment, _Access, _Value, TagTypes_T::TypeArrayUInt8, _Usage, _CB) {
      Length = _Length;
    }

    TagArrayUInt8::TagArrayUInt8 (String _Name, String _Text, String _Comment, TagAccessType_T _Access, TagUsage_T _Usage, uint8_t *_Value, uint8_t _Length)
        : TagParent (_Name, _Text, _Comment, _Access, _Value, TagTypes_T::TypeArrayUInt8, _Usage) {
      Length = _Length;
    }

    /**
     * @brief Create the config Json-String of the Tag-Data
     *
     * @return String Json-String
     */
    String TagArrayUInt8::writeTag () {
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
    bool TagArrayUInt8::getValue (JsonVariant _Value, TagAccessType_T _Access) {
      if ((Access & ~TagAccessType_T::Write & _Access) == 0) {
        // Access-Type is not allowed
        return false;
      }
      return _Value.set (JCA::SYS::ByteArrayToHexString (static_cast<uint8_t *> (Value), Length));
    }
    
    /**
     * @brief Set the value of the Tag
     * 
     * @param _Value Value that should be set
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool TagArrayUInt8::setValue (JsonVariant _Value, TagAccessType_T _Access) {
      if ((Access & ~TagAccessType_T::Read & _Access) == 0) {
        // Access-Type is not allowed
        return false;
      }
      bool RetValue = false;
      if (_Value.is<JsonArray> ()) {
        JsonArray ValueArray = _Value.as<JsonArray>();
        if (ValueArray.size() >= Length) {
          uint8_t *OutValue = static_cast<uint8_t *> (Value);
          for (uint8_t i=0; i<Length; i++) {
            OutValue[i] = ValueArray[i].as<uint8_t>();
          }
          RetValue = true;
        }
      } else if (_Value.is<const char *> ()) {
        RetValue = JCA::SYS::HexStringToByteArray (_Value.as<String> (), static_cast<uint8_t *> (Value), Length);
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
    void TagArrayUInt8::addValue (JsonObject &_Values, TagAccessType_T _Access) {
      if ((Access & ~TagAccessType_T::Read & _Access) == 0) {
        // Access-Type is not allowed
        return;
      }
      _Values[Name] = JCA::SYS::ByteArrayToHexString (static_cast<uint8_t *> (Value), Length);
    }
  }
}