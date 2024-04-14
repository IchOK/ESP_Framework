/**
 * @file JCA_FNC_ElementTagBool.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_FNC_ElementTagBool.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /**
     * @brief Construct a new ElementTagBool::ElementTagBool object
     *
     * @param _Name Name of the Element (in JSON)
     * @param _Text Text showen on the website
     * @param _Comment Comment showen on the website if nedded
     * @param _ReadOnly set the Tag to read only, can only write by the Function-Object
     * @param _Usage Usage-Type to sort the Tag on teh website
     * @param _Value Pointer to the Value-Datapoint inside the Function-Object
     * @param _BtnOnText Button-Text if the tag is true
     * @param _BtnOffText Button-Text ift the tag is false
     */
    ElementTagBool::ElementTagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText)
        : ElementTag (_Name, _Text, _Comment, _ReadOnly, _Value, ElementTagTypes_T::TagBool, _Usage) {
      BtnOnText = _BtnOnText;
      BtnOffText = _BtnOffText;
    }

    /**
     * @brief Get the Value into an JsonVariant
     *
     * @param _Value Reference to the JsonVariant to which the value is to be written
     * @return true Value was successfully written to _Value
     * @return false something failed
     */
    bool ElementTagBool::getValue (JsonVariant _Value) {
      return _Value.set (*(static_cast<bool *> (Value)));
    }

    /**
     * @brief Set the value of the Tag
     *
     * @param _Value Value that should be set
     * @return true Tag-Value was successfully set
     * @return false something failed
     */
    bool ElementTagBool::setValue (JsonVariant _Value) {
      *(static_cast<bool *> (Value)) = _Value.as<bool> ();
      return true;
    }

    /**
     * @brief Create the complete Json-String of the Tag-Data
     *
     * @return String Json-String
     */
    String ElementTagBool::createSetupTag () {
      String SetupTag = createSetupTagBase();
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonValue) + "\":\"" + String (*(static_cast<bool *>(Value))) + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonOn) + "\":\"" + BtnOnText + "\"";
      SetupTag += ",\"" + String (JCA_FNC_ELEMENTTAGS_JsonOff) + "\":\"" + BtnOffText + "\"";
      return SetupTag;
    }

    /**
     * @brief Create JsonObject with all Tag informations
     *
     * @param _Tag Reference to tha JsonObject
     */
    void ElementTagBool::getTagObject (JsonObject &_Tag) {
      getBaseObject (_Tag);
      _Tag[JCA_FNC_ELEMENTTAGS_JsonValue] = *(static_cast<bool *> (Value));
      _Tag[JCA_FNC_ELEMENTTAGS_JsonOn] = BtnOnText;
      _Tag[JCA_FNC_ELEMENTTAGS_JsonOff] = BtnOffText;
    }

    /**
     * @brief Create a key-value-pair of the Tag inside an JsonObject
     *
     * @param _Values Reference to tha JsonObject
     */
    void ElementTagBool::addTagValue (JsonObject &_Values) {
      _Values[Name] = *(static_cast<bool *> (Value));
    }
  }
}