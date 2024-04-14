/**
 * @file JCA_FNC_ElementTagArrayUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-14
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagArrayUInt8.h>

namespace JCA {
  namespace FNC {
    /**
     * @brief Construct a new ElementTagArrayUInt8::ElementTagArrayUInt8 object
     * 
     * @param _Name Name of the Element (in JSON)
     * @param _Text Text showen on the website
     * @param _Comment Comment showen on the website if nedded
     * @param _ReadOnly set the Tag to read only, can only write by the Function-Object
     * @param _Usage Usage-Type to sort the Tag on teh website
     * @param _Value Pointer to the Value-Array inside the Function-Object
     * @param _Length Length of the Value-Array
     */
    ElementTagArrayUInt8::ElementTagArrayUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value, uint8_t _Length)
    : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagArrayUInt8, _Usage) {
      Length = _Length;
    }

    /**
     * @brief Get the Value into an JsonVariant
     *
     * @param _Value Reference to the JsonVariant to which the value is to be written
     * @return true Value was successfully written to _Value
     * @return false something failed
     */
    bool ElementTagArrayUInt8::getValue (JsonVariant _Value) {
      return _Value.set (JCA::SYS::ByteArrayToHexString (static_cast<uint8_t *> (Value), Length));
    }
    
    /**
     * @brief Set the value of the Tag
     * 
     * @param _Value Value that should be set
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool ElementTagArrayUInt8::setValue (JsonVariant _Value) {
      if (_Value.is<JsonArray> ()) {
        JsonArray ValueArray = _Value.as<JsonArray>();
        if (ValueArray.size() >= Length) {
          uint8_t *OutValue = static_cast<uint8_t *> (Value);
          for (uint8_t i=0; i<Length; i++) {
            OutValue[i] = ValueArray[i].as<uint8_t>();
          }
          return true;
        } else {
          return false;
        }
      } else if (_Value.is<const char *> ()) {
        return JCA::SYS::HexStringToByteArray (_Value.as<String> (), static_cast<uint8_t *> (Value), Length);
      } else {
        return false;
      }
    }

    /**
     * @brief Create the complete Json-String of the Tag-Data
     * 
     * @return String Json-String
     */
    String ElementTagArrayUInt8::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      int16_t Counter = 0;
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + JCA::SYS::ByteArrayToHexString ((static_cast<uint8_t *> (Value)), Length) + "\"";
      return SetupTag;
    }

    /**
     * @brief Create JsonObject with all Tag informations
     *
     * @param _Tag Reference to tha JsonObject
     */
    void ElementTagArrayUInt8::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = JCA::SYS::ByteArrayToHexString (static_cast<uint8_t *> (Value), Length);
    }

    /**
     * @brief Create a key-value-pair of the Tag inside an JsonObject
     *
     * @param _Values Reference to tha JsonObject
     */
    void ElementTagArrayUInt8::addTagValue (JsonObject &_Values) {
      _Values[Name] = JCA::SYS::ByteArrayToHexString (static_cast<uint8_t *> (Value), Length);
    }
  }
}