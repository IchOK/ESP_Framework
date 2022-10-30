
#include <JCA_FNC_Parent.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Protocol::JsonTagElements = "elements";
    const char *Protocol::JsonTagConfig = "config";
    const char *Protocol::JsonTagData = "data";
    const char *Protocol::JsonTagCmdInfo = "cmdInfo";
    const char *Protocol::JsonTagCmd = "cmd";
    const char *Protocol::JsonTagName = "name";
    const char *Protocol::JsonTagText = "text";
    const char *Protocol::JsonTagComment = "comment";
    const char *Protocol::JsonTagValue = "value";
    const char *Protocol::JsonTagUnit = "unit";
    const char *Protocol::JsonTagOn = "on";
    const char *Protocol::JsonTagOff = "off";
    const char *Protocol::JsonTagType = "type";
    const char *Protocol::JsonTagReadOnly = "readOnly";
    const char *Protocol::BtnOnDefault = "ON";
    const char *Protocol::BtnOffDefault = "OFF";
    const char *Protocol::DebugSeparator = " - ";
    
    /**
     * @brief Construct a new Protocol::Protocol object
     * 
     * @param _Name Element Name inside the Communication
     * @param _Comment Comment if requested
     */
    Protocol::Protocol (String _Name, String _Comment) {
      Name = _Name;
      Comment = _Comment;
    }

    /**
     * @brief Construct a new Protocol::Protocol object
     * 
     * @param _Name Element Name inside the Communication
     */
    Protocol::Protocol (String _Name) : Protocol (_Name, "") {
    }

    /**
     * @brief Search the Config-Block ("config": []) of the Element
     * The Functions first search the Element inside the Elements-Array by Name,
     * Then returns the Config-Tags Array if extist
     * @param _Elements Array of Elements ("elements": [])
     * @return JsonVariant if not found returns an empty Variant
     */
    JsonVariant Protocol::findConfig (JsonArray &_Elements) {
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
    JsonVariant Protocol::findData (JsonArray &_Elements) {
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
    JsonVariant Protocol::findCmd (JsonArray &_Elements) {
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

    /**
     * @brief Add the Elemet-Body to the Array
     * Contains Element Name and comment if defined
     * @param _Elements Array of Elements ("elements": [])
     * @return JsonObject Element to attach data, config, ...
     */
    JsonObject Protocol::createEmptyElement (JsonArray &_Elements) {
      JsonObject Element = _Elements.createNestedObject ();
      Element[JsonTagName] = Name;
      if (Comment.length () > 0) {
        Element[JsonTagComment] = Comment;
      }
      return Element;
    }

    /**
     * @brief Add a Default-Tag-Structur to the Array
     * Contains Element Name and comment if defined
     * @param _Tags Array of Tags like Data ("data": []) or Config
     * @param _Name Name of the Tag to add
     * @param _Text Text for Website Lable
     * @param _Comment Comment, if not used set nullptr
     * @param _ReadOnly Disable Input on Website
     * @return JsonObject Tag to attach value, unit, ...
     */
    JsonObject Protocol::createEmptyTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly) {
      JsonObject Tag = _Tags.createNestedObject ();
      Tag[JsonTagName] = _Name;
      Tag[JsonTagText] = _Text;
      Tag[JsonTagReadOnly] = _ReadOnly;
      if (_Comment != nullptr) {
        Tag[JsonTagComment] = _Comment;
      }
      return Tag;
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_BtnOnText, const char *_BntOffText, bool _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_BtnOnText != nullptr) {
        Tag[JsonTagOn] = _BtnOnText;
      } else {
        Tag[JsonTagOn] = BtnOnDefault;
      }
      if (_BntOffText != nullptr) {
        Tag[JsonTagOff] = _BntOffText;
      } else {
        Tag[JsonTagOff] = BtnOffDefault;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, float _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_Unit != nullptr) {
        Tag[JsonTagUnit] = _Unit;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int16_t _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_Unit != nullptr) {
        Tag[JsonTagUnit] = _Unit;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint16_t _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_Unit != nullptr) {
        Tag[JsonTagUnit] = _Unit;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int32_t _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_Unit != nullptr) {
        Tag[JsonTagUnit] = _Unit;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint32_t _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_Unit != nullptr) {
        Tag[JsonTagUnit] = _Unit;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, long _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
      if (_Unit != nullptr) {
        Tag[JsonTagUnit] = _Unit;
      }
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, String _Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
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
    void Protocol::createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Value) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, _ReadOnly);
      Tag[JsonTagValue] = _Value;
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
    void Protocol::createCmdInfo (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, const char *_Type) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, false);
      Tag[JsonTagType] = _Type;
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
    void Protocol::createCmdInfo (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, const char *_Type, const char *_BtnText) {
      JsonObject Tag = createEmptyTag (_Tags, _Name, _Text, _Comment, false);
      Tag[JsonTagType] = _Type;
      if (_BtnText != nullptr) {
        Tag[JsonTagOff] = _BtnText;
      } else {
        Tag[JsonTagOff] = BtnOffDefault;
      }
    }

    /**
     * @brief Add the Elemet with all Config-Tags to the Array
     *
     * @param _Elements Array of Elements ("elements": []) the Element have to add
     */
    void Protocol::getConfig (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      JsonObject Element = createEmptyElement (_Elements);
      JsonArray Tags;
      Tags = Element.createNestedArray (JsonTagConfig);
      createConfigTags (Tags);
    }

    /**
     * @brief Add the Element with all Data-Tags to the Array
     *
     * @param _Elements Array of Elements ("elements": []) the Element have to add
     */
    void Protocol::getData (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      JsonObject Element = createEmptyElement (_Elements);
      JsonArray Tags;
      Tags = Element.createNestedArray (JsonTagData);
      createDataTags (Tags);
    }

    /**
     * @brief Add the Element with all Command-Infos-Tags to the Array
     *
     * @param _Elements Array of Elements ("elements": []) the Element have to add
     */
    void Protocol::getCmdInfo (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      JsonObject Element = createEmptyElement (_Elements);
      JsonArray Tags;
      Tags = Element.createNestedArray (JsonTagCmdInfo);
      createCmdInfoTags (Tags);
    }

    /**
     * @brief Add the complete Element to the Array
     *
     * @param _Elements Array of Elements ("elements": []) the Element have to add
     */
    void Protocol::getAll (JsonArray &_Elements) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      JsonObject Element = createEmptyElement (_Elements);
      JsonArray Tags;
      Tags = Element.createNestedArray (JsonTagData);
      createDataTags (Tags);
      Tags = Element.createNestedArray (JsonTagConfig);
      createConfigTags (Tags);
      Tags = Element.createNestedArray (JsonTagCmdInfo);
      createCmdInfoTags (Tags);
    }

    /**
     * @brief Set Data, Config and execute Commands
     * Check if the Element in the Array and pass the Tag-Arrays to the Element Data
     * @param _Elements Array of Elements that maybe const Tags for the Element
     */
    void Protocol::set (JsonArray &_Elements) {
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
  }
}
