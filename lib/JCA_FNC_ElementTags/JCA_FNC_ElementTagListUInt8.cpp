/**
 * @file JCA_FNC_ElementTagListUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagListUInt8.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /**
     * @brief Construct a new ElementTagListUInt8::ElementTagListUInt8 object
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
    ElementTagListUInt8::ElementTagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value, SetCallback _CB)
    : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagListUInt8, _Usage, _CB) {
    }

    ElementTagListUInt8::ElementTagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value)
    : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagListUInt8, _Usage) {
    }

    /**
     * @brief Get the Value into an JsonVariant
     *
     * @param _Value Reference to the JsonVariant to which the value is to be written
     * @return true Value was successfully written to _Value
     * @return false something failed
     */
    bool ElementTagListUInt8::getValue (JsonVariant _Value) {
      return _Value.set (*(static_cast<uint8_t *> (Value)));
    }

    /**
     * @brief Set the value of the Tag
     *
     * @param _Value Value that should be set (Intager or String matching the List are possible)
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool ElementTagListUInt8::setValue (JsonVariant _Value) {
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
     * @brief Create the complete Json-String of the Tag-Data
     *
     * @return String Json-String
     */
    String ElementTagListUInt8::createSetupTag () {
      String SetupTag = createSetupTagBase ();
      int16_t Counter = 0;
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":" + String (*(static_cast<uint8_t *> (Value)));
      // for each Listmap
      for (const auto &[Index, Text] : List) {
        if (Counter == 0) {
          SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonList) + "\": [";
          SetupTag += "{\"" + String (JCA_FNC_ELEMENTTAGS_JsonListIndex) + "\":\"" + String(Index) + "\",\"" + String (JCA_FNC_ELEMENTTAGS_JsonListValue) + "\":\"" + Text + "\"}";
        } else {
          SetupTag += ",{\"" + String (JCA_FNC_ELEMENTTAGS_JsonListIndex) + "\":\"" + String (Index) + "\",\"" + String (JCA_FNC_ELEMENTTAGS_JsonListValue) + "\":\"" + Text + "\"}";
        }
        Counter++;
      }
      if (Counter > 0) {
        SetupTag += "]";
      }
      return SetupTag;
    }

    /**
     * @brief Create JsonObject with all Tag informations
     *
     * @param _Tag Reference to tha JsonObject
     */
    void ElementTagListUInt8::getTagObject (JsonObject &_Tag) {
      JsonArray JsonList;
      JsonObject ListElement;
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<uint8_t *> (Value));
      JsonList = _Tag.createNestedArray (JCA_FNC_ELEMENTTAGS_JsonList);
      for (const auto &[Index, Text] : List) {
        ListElement = JsonList.createNestedObject();
        ListElement[JCA_FNC_ELEMENTTAGS_JsonListIndex] = Index;
        ListElement[JCA_FNC_ELEMENTTAGS_JsonListValue] = Text;
      }
    }

    /**
     * @brief Create a key-value-pair of the Tag inside an JsonObject
     *
     * @param _Values Reference to tha JsonObject
     */
    void ElementTagListUInt8::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<uint8_t *> (Value));
    }
  }
}