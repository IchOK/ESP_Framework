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
    const char *FuncParent::JsonTagElements = "elements";
    const char *FuncParent::JsonTagConfig = "config";
    const char *FuncParent::JsonTagData = "data";
    const char *FuncParent::JsonTagCmdInfo = "cmdInfo";
    const char *FuncParent::JsonTagAll = "all";
    const char *FuncParent::JsonTagCmd = "cmd";
    const char *FuncParent::JsonTagName = "name";
    const char *FuncParent::JsonTagText = "text";
    const char *FuncParent::JsonTagComment = "comment";
    const char *FuncParent::JsonTagValue = "value";
    const char *FuncParent::JsonTagUnit = "unit";
    const char *FuncParent::JsonTagOn = "on";
    const char *FuncParent::JsonTagOff = "off";
    const char *FuncParent::JsonTagType = "type";
    const char *FuncParent::JsonTagReadOnly = "readOnly";

    const char *FuncParent::ClassName = "FuncParent";
    const char *FuncParent::DebugSeparator = " - ";

    /**
     * @brief Construct a new Parent::Parent object
     *
     * @param _Name Element Name inside the Communication
     * @param _Comment Comment if requested
     */
    FuncParent::FuncParent (String _Name, String _Comment) {
      Name = _Name;
      Comment = _Comment;
    }

    /**
     * @brief Construct a new Parent::Parent object
     *
     * @param _Name Element Name inside the Communication
     */
    FuncParent::FuncParent (String _Name) : FuncParent (_Name, "") {
    }

    String FuncParent::getName () {
      return Name;
    }
    
    /**
     * @brief Destroy the FuncParent::FuncParent object
     * including the Tags-Vector
     */
    FuncParent::~FuncParent () {
      for (TagParent *Tag : Tags) {
        delete Tag;
      }
      Tags.clear();
    }

    /**
     * @brief 
     * 
     * @param _FuncFile 
     * @param _FilterUsage 
     */
    bool FuncParent::writeFunctionTags (File _FuncFile, TagUsage_T _FilterUsage) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      int16_t Counter = 0;
      String ObjectKey;
      switch (_FilterUsage)
      {
      case TagUsage_T::GetWebData:
        ObjectKey = String (JsonTagData);
        break;
      case TagUsage_T::GetWebConfig:
        ObjectKey = String (JsonTagConfig);
        break;

      default:
        ObjectKey = "";
        break;
      }

      if (ObjectKey.length () > 0) {
        for (int16_t i = 0; i < Tags.size (); i++) {
          if (Tags[i]->Usage & _FilterUsage) {
            if (Counter == 0) {
              _FuncFile.println ("\"" + ObjectKey + "\":[");
              _FuncFile.println ("{" + Tags[i]->writeTag () + "}");
            } else {
              _FuncFile.println (",{" + Tags[i]->writeTag () + "}");
            }
            Counter++;
          }
        }
      }
      if (Counter > 0) {
        _FuncFile.println ("]");
      }
      return Counter > 0;
    }

    uint8_t FuncParent::GetSetupValueUINT8 (const char *_TagName, bool &_Done, JsonObject _Setup, JsonObject _Log) {
      if (_Setup[_TagName].is<int> ()) {
        uint8_t Value = _Setup[_TagName].as<uint8_t> ();
        Debug.println (FLAG_SETUP, true, ClassName, __func__, String (_TagName) + " > " + String (Value));
        return Value;
      } else {
        _Log[_TagName] = "wrong datatype or missing";
        Debug.println (FLAG_ERROR, true, ClassName, __func__, String (_TagName) + " > wrong datatype");
      }
      _Done = false;
      return 0;
    }

    uint16_t FuncParent::GetSetupValueUINT16 (const char *_TagName, bool &_Done, JsonObject _Setup, JsonObject _Log) {
      if (_Setup[_TagName].is<int> ()) {
        uint16_t Value = _Setup[_TagName].as<uint16_t> ();
        Debug.println (FLAG_SETUP, true, ClassName, __func__, String (_TagName) + " > " + String (Value));
        return Value;
      } else {
        _Log[_TagName] = "wrong datatype or missing";
        Debug.println (FLAG_ERROR, true, ClassName, __func__, String (_TagName) + " > wrong datatype");
      }
      _Done = false;
      return 0;
    }

    uint8_t FuncParent::GetSetupValueUINT8Arr (const char *_TagName, uint8_t *&_Values, bool &_Done, JsonObject _Setup, JsonObject _Log) {
      uint8_t Count;
      if (_Setup[_TagName].is<JsonArray> ()) {
        JsonArray OutputArray = _Setup[_TagName].as<JsonArray> ();
        Count = OutputArray.size ();
        _Values = new uint8_t[Count];
        Debug.print (FLAG_SETUP, true, ClassName, __func__, String (_TagName) + " > " + String (Count) + " [ ");
        for (uint8_t i = 0; i < Count; i++) {
          _Values[i] = OutputArray[i].as<uint8_t> ();
          Debug.print (FLAG_SETUP, true, ClassName, __func__, String (_Values[i]) + " ");
        }
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "]");
        return Count;
      } else {
        _Log[_TagName] = "wrong datatype or missing";
        Debug.println (FLAG_ERROR, true, ClassName, __func__, String (_TagName) + " > wrong datatype");
      }
      _Done = false;
      return 0;
    }

    String FuncParent::GetSetupValueString (const char *_TagName, bool &_Done, JsonObject _Setup, JsonObject _Log) {
      if (_Setup[_TagName].is<JsonVariant> ()) {
        String Value = _Setup[_TagName].as<String> ();
        Debug.println (FLAG_SETUP, true, ClassName, __func__, String (_TagName) + " > " + Value);
        return Value;
      } else {
        _Log[_TagName] = "missing";
        Debug.println (FLAG_ERROR, true, ClassName, __func__, String (_TagName) + " > Missing");
      }
      _Done = false;
      return "";
    }

    void* FuncParent::GetSetupHardwareRef (const char *_TagName, String &_HwName, bool &_Done, JsonObject _Setup, JsonObject _Log, std::map<String, void *> _Hardware) {
      void *HwRef;
      if (_Setup[_TagName].is<JsonVariant> ()) {
        _HwName = _Setup[_TagName].as<String> ();
        if (_Hardware.count (_HwName) == 1) {
          HwRef = _Hardware[_HwName];
          Debug.println (FLAG_SETUP, true, ClassName, __func__, String (_TagName) + " > " + String (_HwName));
          return HwRef;
        } else {
          _Log[_TagName] = "hardware " + _HwName + " not Found";
          Debug.println (FLAG_ERROR, true, ClassName, __func__, String (_TagName) + " > Hardware not listed");
        }
      } else {
        _Log[_TagName] = "missing";
        Debug.println (FLAG_ERROR, true, ClassName, __func__, String (_TagName) + " > Missing");
      }
      _Done = false;
      return nullptr;
    }

    /**
     * @brief Write Element-Tags to the Functions-File
     *
     * @param _SetupFile File to Write
     * @param _ElementInit First Element added
     */
    void FuncParent::writeFunction (File _FuncFile, bool &_Init) {
      if (_Init) {
        _FuncFile.print (",");
      } else {
        _Init = true;
      }
      _FuncFile.println ("\"" + Name + "\":{");
      if (Comment.length () > 0) {
        _FuncFile.println ("\"" + String (JsonTagComment) + "\":\"" + Comment + "\"");
      }
      if (writeFunctionTags (_FuncFile, TagUsage_T::GetWebConfig)) {
        _FuncFile.println (",");
      }
      writeFunctionTags (_FuncFile, TagUsage_T::GetWebData);
      _FuncFile.println ("}");
    }

    /**
     * @brief Set Data, Config and execute Commands
     * Check if the Element in the Array and pass the Tag-Arrays to the Element Data
     * @param _Elements Array of Elements that maybe const Tags for the Element
     */
    void FuncParent::setValues (JsonObject &_Function) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Start");
      for (JsonPair JsonTag : _Function) {
        setTagValueByIndex(getTagIndex(JsonTag.key().c_str()), JsonTag.value());
      }
    }

    /**
     * @brief Add key-value-pairs for all tags to a JsonObject
     * @param _Values Object the to add the tags
     */
    void FuncParent::addValues (JsonObject &_Function) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      for (int16_t i = 0; i < Tags.size (); i++) {
        Tags[i]->addValue (_Function);
      }
    }

    /**
     * @brief Returns the position of a tag inside the Tags-Vector
     *
     * @param _Name Name of the searched tag
     * @return int16_t position of the tag or -1 if not found
     */
    int16_t FuncParent::getTagIndex (String _Name) {
      for (int16_t i = 0; i < Tags.size (); i++) {
        if (Tags[i]->Name == _Name) {
          return i;
        }
      }
      // Tag was not found
      return -1;
    }

    /**
     * @brief Get the value of a tag inside the Tags-Vector
     *
     * @param _Index Position of the tag
     * @param _Value datapoint to write the value
     * @return true tag get successful
     * @return false something failed
     */
    bool FuncParent::getTagValueByIndex (int16_t _Index, JsonVariant _Value) {
      if (_Index < 0) {
        return false;
      }
      return Tags[_Index]->getValue (_Value);
    }

    /**
     * @brief Set the value of a tag inside the Tags-Vector
     * 
     * @param _Index Position of the tag
     * @param _Value value that should be set
     * @return true tag set successful
     * @return false something failed
     */
    bool FuncParent::setTagValueByIndex (int16_t _Index, JsonVariant _Value) {
      if (_Index < 0) {
        return false;
      }
      if (Tags[_Index]->ReadOnly) {
        return false;
      }
      return Tags[_Index]->setValue (_Value);
    }
  }
}
