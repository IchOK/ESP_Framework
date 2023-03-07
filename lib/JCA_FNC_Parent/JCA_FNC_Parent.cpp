/**
 * @file JCA_FNC_Parent.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Parent Class of all Framework Elements.
 * @version 1.0
 * @date 2022-12-10
 * 
 * Copyright Jochen Cabrera 2022
 * Apache License
 * 
 */

#include <JCA_FNC_Parent.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Parent::JsonTagElements = "elements";
    const char *Parent::JsonTagConfig = "config";
    const char *Parent::JsonTagData = "data";
    const char *Parent::JsonTagCmdInfo = "cmdInfo";
    const char *Parent::JsonTagCmd = "cmd";
    const char *Parent::JsonTagName = "name";
    const char *Parent::JsonTagText = "text";
    const char *Parent::JsonTagComment = "comment";
    const char *Parent::JsonTagValue = "value";
    const char *Parent::JsonTagUnit = "unit";
    const char *Parent::JsonTagOn = "on";
    const char *Parent::JsonTagOff = "off";
    const char *Parent::JsonTagType = "type";
    const char *Parent::JsonTagReadOnly = "readOnly";
    const char *Parent::BtnOnDefault = "ON";
    const char *Parent::BtnOffDefault = "OFF";
    const char *Parent::DebugSeparator = " - ";
    
    /**
     * @brief Construct a new Parent::Parent object
     * 
     * @param _Name Element Name inside the Communication
     * @param _Comment Comment if requested
     */
    Parent::Parent (String _Name, String _Comment) {
      Name = _Name;
      Comment = _Comment;
    }

    /**
     * @brief Construct a new Parent::Parent object
     * 
     * @param _Name Element Name inside the Communication
     */
    Parent::Parent (String _Name) : Parent (_Name, "") {
    }

    /**
     * @brief Search the Config-Block ("config": []) of the Element
     * The Functions first search the Element inside the Elements-Array by Name,
     * Then returns the Config-Tags Array if extist
     * @param _Elements Array of Elements ("elements": [])
     * @return JsonVariant if not found returns an empty Variant
     */
    JsonVariant Parent::findConfig (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      for (JsonObject Element : _Elements) {
        if (Element[JsonTagName] == Name) {
          if (Element.containsKey (JsonTagConfig)) {
            return Element[JsonTagConfig];
          }
        }
      }
      return JsonVariant ();
    }

    /**
     * @brief Search the Data-Block ("data": []) of the Element
     * The Functions first search the Element inside the Elements-Array by Name,
     * Then returns the Data-Tags Array if extist
     * @param _Elements Array of Elements ("elements": [])
     * @return JsonVariant if not found returns an empty Variant
     */
    JsonVariant Parent::findData (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      for (JsonObject Element : _Elements) {
        if (Element[JsonTagName] == Name) {
          if (Element.containsKey (JsonTagData)) {
            return Element[JsonTagData];
          }
        }
      }
      return JsonVariant ();
    }

    /**
     * @brief Search the Command-Block ("cmd": []) of the Element
     * The Functions first search the Element inside the Elements-Array by Name,
     * Then returns the Command-Tags Array if extist
     * @param _Elements Array of Elements ("elements": [])
     * @return JsonVariant if not found returns an empty Variant
     */
    JsonVariant Parent::findCmd (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      for (JsonObject Element : _Elements) {
        if (Element[JsonTagName] == Name) {
          if (Element.containsKey (JsonTagCmd)) {
            return Element[JsonTagCmd];
          }
        }
      }
      return JsonVariant ();
    }

    String Parent::createDefaultTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly) {
      String SetupTag;
      SetupTag += "\"" + String(JsonTagName) + "\":\"" + String(_Name) + "\"";
      SetupTag += ",\"" + String(JsonTagText) + "\":\"" + String(_Text) + "\"";
      if (_Comment != nullptr) {
        SetupTag += ",\"" + String(JsonTagComment) + "\":\"" + String(_Comment) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagReadOnly) + "\":" + String(_ReadOnly);
      return SetupTag;
    }

    /**
     * @brief Add a bool-/Button-Tag to the Array
     * use createEmptyTag to create the Body and attach the Button-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _BtnOnText Buttontext if Value is True, if not defined (nullptr) set to "ON"
     * @param _BntOffText Buttontext if Value is False, if not defined (nullptr) set to "OFF"
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_BtnOnText, const char *_BtnOffText, bool _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_BtnOnText != nullptr) {
        SetupTag += ",\"" + String(JsonTagOn) + "\":\"" + String(_BtnOnText) + "\"";
      } else {
        SetupTag += ",\"" + String(JsonTagOn) + "\":\"" + String(BtnOnDefault) + "\"";
      }
      if (_BtnOffText != nullptr) {
        SetupTag += ",\"" + String(JsonTagOff) + "\":\"" + String(_BtnOffText) + "\"";
      } else {
        SetupTag += ",\"" + String(JsonTagOff) + "\":\"" + String(BtnOffDefault) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":";
      if (_Value) {
        SetupTag += "true";
      } else {
        SetupTag += "false";
      }
      return SetupTag;
    }

    /**
     * @brief Add a Value-Tag to the Array
     * use createEmptyTag to create the Body and attach the Value-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Unit Unit, if not used set nullptr
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, float _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_Unit != nullptr) {
        SetupTag += ",\"" + String(JsonTagUnit) + "\":\"" + String(_Unit) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":" + String(_Value);
      return SetupTag;
    }

    /**
     * @brief Add a Value-Tag to the Array
     * use createEmptyTag to create the Body and attach the Value-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Unit Unit, if not used set nullptr
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int16_t _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_Unit != nullptr) {
        SetupTag += ",\"" + String(JsonTagUnit) + "\":\"" + String(_Unit) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":" + String(_Value);
      return SetupTag;
    }

    /**
     * @brief Add a Value-Tag to the Array
     * use createEmptyTag to create the Body and attach the Value-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Unit Unit, if not used set nullptr
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint16_t _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_Unit != nullptr) {
        SetupTag += ",\"" + String(JsonTagUnit) + "\":\"" + String(_Unit) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":" + String(_Value);
      return SetupTag;
    }

    /**
     * @brief Add a Value-Tag to the Array
     * use createEmptyTag to create the Body and attach the Value-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Unit Unit, if not used set nullptr
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int32_t _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_Unit != nullptr) {
        SetupTag += ",\"" + String(JsonTagUnit) + "\":\"" + String(_Unit) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":" + String(_Value);
      return SetupTag;
    }

    /**
     * @brief Add a Value-Tag to the Array
     * use createEmptyTag to create the Body and attach the Value-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Unit Unit, if not used set nullptr
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint32_t _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_Unit != nullptr) {
        SetupTag += ",\"" + String(JsonTagUnit) + "\":\"" + String(_Unit) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":" + String(_Value);
      return SetupTag;
    }

    /**
     * @brief Add a Value-Tag to the Array
     * use createEmptyTag to create the Body and attach the Value-Information
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Unit Unit, if not used set nullptr
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, long _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      if (_Unit != nullptr) {
        SetupTag += ",\"" + String(JsonTagUnit) + "\":\"" + String(_Unit) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagValue) + "\":" + String(_Value);
      return SetupTag;
    }

    /**
     * @brief Add a String-Tag to the Array
     * use createEmptyTag to create the Body and attach the String
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, String _Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      SetupTag += ",\"" + String(JsonTagValue) + "\":\"" + _Value + "\"";
      return SetupTag;
    }

    /**
     * @brief Add a String-Tag to the Array
     * use createEmptyTag to create the Body and attach the String
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @param _Value Current value of the Tag
     */
    String Parent::createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Value) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, _ReadOnly);
      SetupTag += ",\"" + String(JsonTagValue) + "\":\"" + String(_Value) + "\"";
      return SetupTag;
    }

    /**
     * @brief Add a Command-Info-Tag to the Array
     * use createEmptyTag to create the Body and attach the String
     * @param _Tags Array of Command-Inormations ("cmdInfo": [])
     * @param _Name Name of the Command
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _Type Type for the HMI to know what ist requested (boot, in16, string, ...)
     */
    String Parent::createSetupCmdInfo (const char *_Name, const char *_Text, const char *_Comment, const char *_Type) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, false);
      SetupTag += ",\"" + String(JsonTagType) + "\":\"" + String(_Type) + "\"";
      return SetupTag;
    }

    /**
     * @brief Add a Button-Command-Info to the Array
     * contains additional Information for Button dynamic
     * @param _Tags Array of Command-Inormations ("cmdInfo": [])
     * @param _Name Name of the Command
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _Type sould be bool
     * @param _BtnText Buttontext, if not defined (nullptr) set to "ON"
     */
    String Parent::createSetupCmdInfo (const char *_Name, const char *_Text, const char *_Comment, const char *_Type, const char *_BtnText) {
      String SetupTag = createDefaultTag (_Name, _Text, _Comment, false);
      if (_BtnText != nullptr) {
        SetupTag += ",\"" + String(JsonTagOff) + "\":\"" + String(_BtnText) + "\"";
      }
      SetupTag += ",\"" + String(JsonTagType) + "\":\"" + String(_Type) + "\"";
      return SetupTag;
    }

    /**
     * @brief Set Data, Config and execute Commands
     * Check if the Element in the Array and pass the Tag-Arrays to the Element Data
     * @param _Elements Array of Elements that maybe const Tags for the Element
     */
    void Parent::set (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      JsonVariant _Tags;
      _Tags = findConfig (_Elements);
      if (_Tags.is<JsonArray> ()) {
        setConfig(_Tags.as<JsonArray>());
      }
      _Tags = findData (_Elements);
      if (_Tags.is<JsonArray> ()) {
        setData(_Tags.as<JsonArray>());
      }
      _Tags = findCmd (_Elements);
      if (_Tags.is<JsonArray> ()) {
        setCmd (_Tags.as<JsonArray> ());
      }
    }

    void Parent::getValues (JsonObject &_Elements) {
      JsonObject Element = _Elements.createNestedObject (Name);
      JsonObject Values;
      Values = Element.createNestedObject (JsonTagData);
      createDataValues (Values);
      Values = Element.createNestedObject (JsonTagConfig);
      createConfigValues (Values);
    }

    void Parent::writeSetup (File _SetupFile, bool &_ElementInit) {
      if (_ElementInit) {
        _SetupFile.println(",{");
      } else {
        _SetupFile.println ("{");
        _ElementInit = true;
      }
      _SetupFile.println ("\"" + String(JsonTagName) + "\":\"" + Name + "\"");
      if (Comment.length() > 0) {
        _SetupFile.println(",\"" + String(JsonTagComment) + "\":\"" + Comment + "\"");
      }
      writeSetupConfig (_SetupFile);
      writeSetupData (_SetupFile);
      writeSetupCmdInfo (_SetupFile);
      _SetupFile.println ("}");
    }
  }
}
