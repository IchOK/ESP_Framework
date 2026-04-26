#include "ArduinoJson/Variant/JsonVariant.hpp"
#include "JCA_SYS_DebugOut.h"
#include "JCA_IOT_FuncHandler.h"
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace IOT {
    const char *FuncHandler::JsonTagHardware = "hardware";
    const char *FuncHandler::JsonTagFunctions = "functions";
    const char *FuncHandler::JsonTagLinks = "links";


    FuncLink::FuncLink(std::function<bool (JsonArray, JsonVariant &)> _UpdateFunction) {
      Input = std::vector<FuncLinkPair_T>();
      Output = std::vector<FuncLinkPair_T>();
      UpdateFunction = _UpdateFunction;
    }

    FuncLink::~FuncLink() {
      Input.clear();
      Output.clear();
    }

    void FuncLink::addInput(FuncLinkPair_T _Input) {
      Input.push_back(_Input);
    }

    void FuncLink::addOutput(FuncLinkPair_T _Output) {
      Output.push_back(_Output);
    }

    FuncLinkPair_T FuncLink::getInput(uint8_t _Index) {
      return Input[_Index];
    }

    FuncLinkPair_T FuncLink::getOutput(uint8_t _Index) {
      return Output[_Index];
    }

    FuncHandler::FuncHandler (String _Name) {
      Name = _Name;
      // Link types are registered via AddToHandler functions (similar to functions)
      // This is done in main.cpp in addLinksToHandler()
    }

    /**
     * @brief Check if a LinkPair is valid and the Function and Tag Index
     * 
     * @param _FuncName Name of the Function to get linked
     * @param _Func REF to the Function-Index
     * @param _TagName Name of the Tag inside the Function to get linked
     * @param _Tag REF to the Tag-Index
     * @param _LogArray Logging Buffer
     * @return true Function with Tag found, Indexs are valid
     * @return false Function or Tag not found
     */
    bool FuncHandler::checkLink(String _FuncName, int16_t &_Func, String _TagName, int16_t &_Tag, JsonArray _LogArray) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      _Func = getFuncIndex (_FuncName);

      if (_Func >= 0) {
        _Tag = Functions[_Func]->getTagIndex (_TagName);
        if (_Tag >= 0) {
          return true;
        } else {
          Debug.print (FLAG_ERROR, true, Name, __func__, "Tag to link not found : ");
          Debug.println (FLAG_ERROR, true, Name, __func__, _TagName);
          JsonObject Log = _LogArray.add<JsonObject>();
          Log["func"] = _FuncName;
          Log["tag"] = _TagName;
          return false;
        }
      } else {
        _Tag = -1;
        Debug.print (FLAG_ERROR, true, Name, __func__, "Function to link not found : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, _FuncName);
        JsonObject Log = _LogArray.add<JsonObject>();
        Log["func"] = _FuncName;
        return false;
      }
    }

    /**
     * @brief delete all Links-Vector entries and LinkData
     * 
     */
    void FuncHandler::deleteLinks() {
      for (FuncLink *Link : Links) {
        delete Link;
      }
      Links.clear();
    }

    /**
     * @brief delete the Functions-Vector and all objects stored inside.
     * Also delete all Links.
     */
    void FuncHandler::deleteFunctions () {
      deleteLinks();
      for (JCA::FNC::FuncParent *Function : Functions) {
        delete Function;
      }
      Functions.clear();
    }

    /**
     * @brief Read the Setup file and create Hardware-, Function- and LinkList
     *
     */
    FuncPatchRet_T FuncHandler::setup () {
      Debug.println (FLAG_SETUP, true, Name, __func__, "Run");
      FuncPatchRet_T RetValue = FuncPatchRet_T::done;
      JsonDocument SetupDoc;
      JsonDocument LogDoc;

      if (!LittleFS.exists(JCA_IOT_FILE_SETUP)) {
        JsonObject LogObj = LogDoc["File"].to<JsonObject>();
        LogObj["Name"] = JCA_IOT_FILE_SETUP;
        LogObj["Error"] = "not found";
        RetValue = FuncPatchRet_T::fileMissing;
      } else {
        // Open Setup File
        File SetupFile = LittleFS.open (JCA_IOT_FILE_SETUP, FILE_READ);
        DeserializationError Error = deserializeJson (SetupDoc, SetupFile);

        if (Error) {
          Debug.print (FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
          Debug.println (FLAG_ERROR, true, Name, __func__, Error.c_str());
          JsonObject LogObj = LogDoc["File"].to<JsonObject>();
          LogObj["Name"] = JCA_IOT_FILE_SETUP;
          LogObj["Error"] = Error.c_str ();
          RetValue = FuncPatchRet_T::jsonSyntax;
        } else {
          JsonObject SetupObj = SetupDoc.as<JsonObject>();

          // First have to delete alle Functions and Links to rebuild the Vectors
          deleteFunctions();

          //-------------------------------------------------------
          // HardwareMapping 
          //-------------------------------------------------------
          if (SetupObj[JsonTagHardware].is<JsonArray>()) {
            Debug.println (FLAG_SETUP, true, Name, __func__, "Found Hardware");
            JsonArray LogArray = LogDoc["Hardware"].to<JsonArray>();
            JsonArray SetupHwArr = SetupObj[JsonTagHardware].as<JsonArray>();
            for (JsonObject SetupHwObj : SetupHwArr) {
              Debug.println (FLAG_SETUP, true, Name, __func__, SetupHwObj["type"].as<String>());
              JsonObject Log = LogArray.add<JsonObject>();
              if (HardwareList.count(SetupHwObj["type"]) == 1) {
                // Hardware found in creator List -> Call Creator and add to HardwareMapping
                if (HardwareMapping.count (SetupHwObj["type"]) == 0) {
                  // Add Hardware only once
                  HardwareList[SetupHwObj["type"].as<String> ()](SetupHwObj, Log, HardwareMapping);
                }
              } else {
                // Hardware not found, log error
                Debug.print (FLAG_ERROR, true, Name, __func__, "Hardware not found in Hardware List : ");
                Debug.println (FLAG_ERROR, true, Name, __func__, SetupHwObj["type"].as<String> ());
                Log["Fault"] = "Type not found" + SetupHwObj["type"].as<String> ();
                if (RetValue > FuncPatchRet_T::hardwareMissing) {
                  RetValue = FuncPatchRet_T::hardwareMissing;
                }
              }
            }
            if (Debug.print (FLAG_SETUP, true, Name, __func__, "Done > HardwareMapping[")) {
              Debug.print (FLAG_SETUP, true, Name, __func__, HardwareMapping.size ());
              Debug.println (FLAG_SETUP, true, Name, __func__, "]");
            }
          }

          //-------------------------------------------------------
          // FunctionList
          //-------------------------------------------------------
          if (SetupObj[JsonTagFunctions].is<JsonArray>()) {
            Debug.println (FLAG_SETUP, true, Name, __func__, "Found Functions");
            JsonArray LogArray = LogDoc["Functions"].to<JsonArray>();
            JsonArray SetupFuncArr = SetupObj[JsonTagFunctions].as<JsonArray> ();
            for (JsonObject SetupFuncObj : SetupFuncArr) {
              Debug.println (FLAG_SETUP, true, Name, __func__, SetupFuncObj["type"].as<String> ());
              JsonObject Log = LogArray.add<JsonObject>();
              if (FunctionList.count (SetupFuncObj["type"]) == 1) {
                // Function found in creator List -> Call Creator and add to Function Vector
                FunctionList[SetupFuncObj["type"].as<String> ()](SetupFuncObj, Log, Functions, HardwareMapping);
              } else {
                // Function not found, log error
                Debug.print (FLAG_ERROR, true, Name, __func__, "Function not found in Function List : ");
                Debug.println (FLAG_ERROR, true, Name, __func__, SetupFuncObj["type"].as<String> ());
                Log["Fault"] = "Type not found" + SetupFuncObj["type"].as<String> ();
                if (RetValue > FuncPatchRet_T::functionMissing) {
                  RetValue = FuncPatchRet_T::functionMissing;
                }
              }
            }
            if (Debug.print (FLAG_SETUP, true, Name, __func__, "Done > Functions[")) {
              Debug.print (FLAG_SETUP, true, Name, __func__, Functions.size ());
              Debug.println (FLAG_SETUP, true, Name, __func__, "]");
            }
          }

          //-------------------------------------------------------
          // Links
          //-------------------------------------------------------
          if (SetupObj[JsonTagLinks].is<JsonArray>()) {
            Debug.println (FLAG_SETUP, true, Name, __func__, "Found Links");
            JsonArray LogArray = LogDoc["Links"].to<JsonArray>();
            JsonArray SetupLinkArr = SetupObj[JsonTagLinks].as<JsonArray> ();
            for (JsonObject SetupLinkObj : SetupLinkArr) {
              Debug.println (FLAG_SETUP, true, Name, __func__, SetupLinkObj["type"].as<String> ());
              JsonObject Log = LogArray.add<JsonObject>();
              if (LinkMapping.count (SetupLinkObj["type"]) == 1) {
                // Create Link
                Links.push_back (new FuncLink (LinkMapping[SetupLinkObj["type"].as<String> ()]));
                Log["Type"] = SetupLinkObj["type"].as<String> ();
                size_t Link = Links.size() - 1;
                int16_t FuncIndex;
                int16_t TagIndex;


                // Add all From Pointer
                JsonArray FromArr = SetupLinkObj["from"].as<JsonArray> ();
                JsonArray LogFrom = Log["IN"].to<JsonArray>();
                for (JsonObject FromObj : FromArr) {
                  if (checkLink (FromObj["func"].as<String> (), FuncIndex, FromObj["tag"].as<String> (), TagIndex, LogArray)) {
                    Links[Link]->addInput ({ FuncIndex, TagIndex });
                    LogFrom.add ("OK: FuncIndex=" + String(FuncIndex) + " TagIndex=" + String(TagIndex));
                  } else {
                    LogFrom.add ("FAIL: " + FromObj["func"].as<String> () + "_" + FromObj["tag"].as<String> ());
                    if (RetValue > FuncPatchRet_T::linkObjMissing) {
                      RetValue = FuncPatchRet_T::linkObjMissing;
                    }
                  }
                }

                // Add all To Pointer
                JsonArray ToArr = SetupLinkObj["to"].as<JsonArray> ();
                JsonArray LogTo = Log["OUT"].to<JsonArray>();
                for (JsonObject ToObj : ToArr) {
                  if (checkLink (ToObj["func"].as<String> (), FuncIndex, ToObj["tag"].as<String> (), TagIndex, LogArray)) {
                    Links[Link]->addOutput ({ FuncIndex, TagIndex });
                    LogTo.add ("OK: FuncIndex=" + String (FuncIndex) + " TagIndex=" + String (TagIndex));
                  } else {
                    LogTo.add ("FAIL: " + ToObj["func"].as<String> () + "_" + ToObj["tag"].as<String> ());
                    if (RetValue > FuncPatchRet_T::linkObjMissing) {
                      RetValue = FuncPatchRet_T::linkObjMissing;
                    }
                  }
                }
              } else {
                // Function not found, log error
                Debug.print (FLAG_ERROR, true, Name, __func__, "Link-Type not defined : ");
                Debug.println (FLAG_ERROR, true, Name, __func__, SetupLinkObj["type"].as<String> ());
                Log["Fault"] = "Type not found" + SetupLinkObj["type"].as<String> ();
                if (RetValue > FuncPatchRet_T::linkTypMissing) {
                  RetValue = FuncPatchRet_T::linkTypMissing;
                }
              }
            }
            if (Debug.print (FLAG_SETUP, true, Name, __func__, "Done > Links[")) {
              Debug.print (FLAG_SETUP, true, Name, __func__, Links.size ());
              Debug.println (FLAG_SETUP, true, Name, __func__, "]");
            }
          }
        }
        SetupFile.close ();
      }

      // Write Logfile
      File LogFile = LittleFS.open (JCA_IOT_FILE_LOG, FILE_WRITE);
      serializeJson (LogDoc, LogFile);
      LogFile.close ();

      Debug.println (FLAG_SETUP, true, Name, __func__, "Done");
      return RetValue;
    }

    /**
     * @brief get all Values from Functions and store it to File
     * (Default: usrValues.json)
     */
    FuncPatchRet_T FuncHandler::saveValues () {
      FuncPatchRet_T RetValue = FuncPatchRet_T::done;
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      JsonDocument ValueDoc;
      JsonObject Values = ValueDoc[JCA::FNC::FuncParent::JsonTagElements].to<JsonObject>(); //.as<JsonObject>();
      getValues (Values, TagAccessType_T::Save);
      File ValuesFile = LittleFS.open (JCA_IOT_FILE_VALUES, FILE_WRITE);
      if (!ValuesFile) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "Failed to open File for write : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, JCA_IOT_FILE_VALUES);
        RetValue = FuncPatchRet_T::fileOpen;
      } else {
        size_t FileSize = serializeJson (ValueDoc, ValuesFile);
        Debug.print (FLAG_PROTOCOL, true, Name, __func__, "File length : ");
        Debug.println (FLAG_PROTOCOL, true, Name, __func__, FileSize);
        ValuesFile.close ();
      }
      return RetValue;
    }

    /**
     * @brief load Values stored in File (Default: usrValues.json) 
     * and set the Functions Values
     */
    FuncPatchRet_T FuncHandler::loadValues () {
      FuncPatchRet_T RetValue = FuncPatchRet_T::done;
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      JsonDocument ValueDoc;
      if (!LittleFS.exists (JCA_IOT_FILE_VALUES)) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "File not found : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, JCA_IOT_FILE_VALUES);
        RetValue = FuncPatchRet_T::fileMissing;
      } else {
        File ValuesFile = LittleFS.open (JCA_IOT_FILE_VALUES, FILE_READ);
        DeserializationError Error = deserializeJson (ValueDoc, ValuesFile);
        if (Error) {
          Debug.print (FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
          Debug.println (FLAG_ERROR, true, Name, __func__, Error.c_str ());
          RetValue = FuncPatchRet_T::jsonSyntax;
        } else if (ValueDoc[JCA::FNC::FuncParent::JsonTagElements].is<JsonObject>()) {
          JsonObject Values = ValueDoc[JCA::FNC::FuncParent::JsonTagElements].as<JsonObject> ();
          setValues (Values, TagAccessType_T::Save);
        }
        ValuesFile.close ();
      }
      return RetValue;
    }

    /**
     * @brief Updates the Links and the Functions
     *
     * @param _Time current Time from RTC
     */
    void FuncHandler::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, true, Name, __func__, "Run");
      // Create JsonDoc to hold variant Data
      JsonDocument LinkDoc;

      // Update Links
      for (FuncLink *Link : Links) {
        // Look up update function in map
        if (Link->UpdateFunction != nullptr) {
          // Collect all Input-Links in one JsonArray
          JsonArray Inputs = LinkDoc["inputs"].to<JsonArray>();
          for (uint8_t i = 0; i < Link->getInputCount(); i++) {
            FuncLinkPair_T LinkPair = Link->getInput(i);
            JsonVariant Input = LinkDoc["input"].to<JsonVariant>();
            if (Functions[LinkPair.Func]->getTagValueByIndex(LinkPair.Tag, Input, TagAccessType_T::Read)) {
              Inputs.add(Input);
            }
          }

          // Call update function
          JsonVariant Output = LinkDoc["output"].to<JsonVariant>();
          if (Link->UpdateFunction(Inputs, Output)) {
            // Write output to Tags
            for (uint8_t i = 0; i < Link->getOutputCount(); i++) {
              FuncLinkPair_T LinkPair = Link->getOutput(i);
              Functions[LinkPair.Func]->setTagValueByIndex(LinkPair.Tag, Output, TagAccessType_T::Write);
            }
          }
        }
      }

      // Update Functions
      for (uint16_t i = 0; i < Functions.size (); i++) {
        Functions[i]->update (_Time);
      }
    }

    String FuncHandler::patch(String _Command) {
      _Command.toLowerCase ();
      FuncPatchRet_T RetValue = FuncPatchRet_T::modeUndef;
      if (_Command == "savevalues") {
        RetValue = saveValues ();
      } else if (_Command == "loadvalues") {
        RetValue = loadValues ();
      } else if (_Command == "init") {
        RetValue = setup ();
        if (RetValue > 0) {
          RetValue = loadValues ();
        }
      } else if (_Command == "reinit") {
        RetValue = saveValues ();
        if (RetValue > 0) {
          RetValue = setup ();
        }
        if (RetValue > 0) {
          RetValue = loadValues ();
        }
      }
      switch (RetValue)
      {
      case FuncPatchRet_T::done:
        return "done";
        break;

      case FuncPatchRet_T::linkObjMissing:
        return "linkObjMissing";
        break;

      case FuncPatchRet_T::linkTypMissing:
        return "linkTypMissing";
        break;

      case FuncPatchRet_T::hardwareMissing:
        return "hardwareMissing";
        break;

      case FuncPatchRet_T::functionMissing:
        return "functionMissing";
        break;

      case FuncPatchRet_T::fileMissing:
        return "fileMissing";
        break;

      case FuncPatchRet_T::jsonSyntax:
        return "jsonSyntax";
        break;

      case FuncPatchRet_T::fileOpen:
        return "fileOpen";
        break;

      case FuncPatchRet_T::modeUndef:
        return "modeUndef";
        break;

      case FuncPatchRet_T::failed:
        return "failed";
        break;

      default:
        return "retUnknown";
        break;
      }
    }

    /**
     * @brief Returns the position of a Function inside the Functionss-Vector
     *
     * @param _Name Name of the searched Function
     * @return int16_t position of the Function or -1 if not found
     */
    int16_t FuncHandler::getFuncIndex (String _Name) {
      for (size_t i = 0; i < Functions.size (); i++) {
        if (Functions[i]->getName () == _Name) {
          return (int16_t)i;
        }
      }
      // Tag was not found
      return -1;
    }

    /**
     * @brief set Function Date from outside the Handler, by Websocket for example
     * 
     * @param _Functions REF to a Values-Object in format like the usrValues.json
     */
    void FuncHandler::setValues (JsonObject &_Functions, TagAccessType_T _Access) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      for (JsonPair Function : _Functions) {
        int16_t FuncIndex = getFuncIndex (Function.key ().c_str ());
        if (FuncIndex >= 0) {
          JsonObject FuncValues = Function.value ().as<JsonObject> ();
          Functions[FuncIndex]->setValues(FuncValues, _Access);
        }
      }
    }

    /**
     * @brief returns a Values-Object of all the Functions Tags
     * 
     * @param _Functions REF where the data will returned
     */
    void FuncHandler::getValues (JsonObject &_Functions, TagAccessType_T _Access) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      for (size_t i = 0; i < Functions.size (); i++) {
        JsonObject Function = _Functions[Functions[i]->getName ()].to<JsonObject> ();
        Functions[i]->addValues(Function, _Access);
      }
    }

    /**
     * @brief returns Tag-Structures (with readOnly) of all the Functions Tags
     * 
     * @param _Functions REF where the tag structures will be returned
     * @param _FilterUsage Filter tags by usage type (GetWebData or GetWebConfig)
     */
    void FuncHandler::getTagStructures (JsonObject &_Functions, TagUsage_T _FilterUsage) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      for (size_t i = 0; i < Functions.size (); i++) {
        // Create or get the function object using modern ArduinoJson API
        JsonObject Function = _Functions[Functions[i]->getName().c_str()].to<JsonObject>();
        Functions[i]->addTagStructures(Function, _FilterUsage);
      }
    }

    /**
     * @brief get the Amount of Links in the Links-Vector
     * 
     * @return int16_t size of the Vector
     */
    int16_t FuncHandler::getLinkCount () {
      return Links.size();
    }

    /**
     * @brief get the Amount of Functions in the Functions-Vector
     *
     * @return int16_t size of the Vector
     */
    int16_t FuncHandler::getFuncCount () {
      return Functions.size ();
    }

    /**
     * @brief Get setup metadata including hardware types, function types with schemas, and link types
     * 
     * @param _Out JSON object to fill with metadata
     */
    void FuncHandler::getSetupMetadata (JsonObject &_Out) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      
      // Return available hardware types with their schemas
      JsonObject HardwareSchemas = _Out["hardwareSchemas"].to<JsonObject>();
      for (auto const& pair : HardwareList) {
        String HwType = pair.first;
        JsonObject HwSchema = HardwareSchemas[HwType].to<JsonObject>();
        
        // Get schema from HardwareSchemaList if available
        if (HardwareSchemaList.count(HwType) == 1) {
          HardwareSchemaList[HwType](HwSchema);
        }
      }
      
      // Also return simple hardware types list for backward compatibility
      JsonArray HardwareTypes = _Out["hardwareTypes"].to<JsonArray>();
      for (auto const& pair : HardwareList) {
        HardwareTypes.add(pair.first);
      }
      
      // Return available function types with their schemas
      JsonObject FunctionSchemas = _Out["functionSchemas"].to<JsonObject>();
      for (auto const& pair : FunctionList) {
        String FuncType = pair.first;
        JsonObject FuncSchema = FunctionSchemas[FuncType].to<JsonObject>();
        
        // Get schema from FunctionSchemaList if available
        if (FunctionSchemaList.count(FuncType) == 1) {
          FunctionSchemaList[FuncType](FuncSchema);
        }
      }
      
      // Also return simple function types list for backward compatibility
      JsonArray FunctionTypes = _Out["functionTypes"].to<JsonArray>();
      for (auto const& pair : FunctionList) {
        FunctionTypes.add(pair.first);
      }
      
      // Return available link types with their schemas
      JsonObject LinkSchemas = _Out["linkSchemas"].to<JsonObject>();
      for (auto const& pair : LinkMapping) {
        String LinkType = pair.first;
        JsonObject LinkSchema = LinkSchemas[LinkType].to<JsonObject>();
        
        // Get schema from LinkSchemaList if available
        if (LinkSchemaList.count(LinkType) == 1) {
          LinkSchemaList[LinkType](LinkSchema);
        }
      }
      
      // Also return simple link types list for backward compatibility
      JsonArray LinkTypes = _Out["linkTypes"].to<JsonArray>();
      LinkTypes.add("direct");
      LinkTypes.add("move");
    }

    /**
     * @brief Load setup configuration from file
     * 
     * @param _Out JSON object to fill with loaded setup data
     * @return true if successful, false otherwise
     */
    bool FuncHandler::loadSetup (JsonObject &_Out) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      
      if (LittleFS.exists(JCA_IOT_FILE_SETUP)) {
        File SetupFile = LittleFS.open(JCA_IOT_FILE_SETUP, FILE_READ);
        if (SetupFile) {
          JsonDocument SetupDoc;
          DeserializationError Error = deserializeJson(SetupDoc, SetupFile);
          SetupFile.close();
          
          if (Error) {
            _Out.clear();
            _Out["error"] = "Failed to parse setup file";
            Debug.print(FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
            Debug.println(FLAG_ERROR, true, Name, __func__, Error.c_str());
            return false;
          }
          
          // Copy data from document to output object
          JsonObject SetupObj = SetupDoc.as<JsonObject>();
          for (JsonPair pair : SetupObj) {
            _Out[pair.key()] = pair.value();
          }
          return true;
        } else {
          _Out["error"] = "Failed to open setup file";
          Debug.println(FLAG_ERROR, true, Name, __func__, "Failed to open setup file");
          return false;
        }
      } else {
        // Return empty structure if file doesn't exist
        _Out["hardware"] = JsonArray();
        _Out["functions"] = JsonArray();
        _Out["links"] = JsonArray();
        return true;
      }
    }

    /**
     * @brief Save setup configuration to file
     * 
     * @param _In JSON object containing setup data to save
     * @return true if successful, false otherwise
     */
    bool FuncHandler::saveSetup (JsonObject &_In) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      
      File SetupFile = LittleFS.open(JCA_IOT_FILE_SETUP, FILE_WRITE);
      if (SetupFile) {
        size_t WrittenBytes = serializeJson(_In, SetupFile);
        SetupFile.close();
        Debug.print(FLAG_SETUP, true, Name, __func__, "Setup file saved, bytes: ");
        Debug.println(FLAG_SETUP, true, Name, __func__, WrittenBytes);
        return true;
      } else {
        Debug.println(FLAG_ERROR, true, Name, __func__, "Failed to open setup file for writing");
        return false;
      }
    }

    /**
     * @brief Get list of all functions with their available tags
     * 
     * @param _Out JSON object to fill with functions and tags
     */
    void FuncHandler::getFunctionsList (JsonObject &_Out) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      
      JsonArray FunctionsArray = _Out["functions"].to<JsonArray>();
      
      for (size_t i = 0; i < Functions.size (); i++) {
        JsonObject FuncObj = FunctionsArray.add<JsonObject>();
        FuncObj["name"] = Functions[i]->getName();
        
        Functions[i]->addTagStructures(FuncObj, TagUsage_T::GetAll);

        /*
        // Get all tag names with access types for this function
        JsonArray TagsArray = FuncObj["tags"].to<JsonArray>();
        
        // Use getTagStructures to get tag info (since Tags is protected)
        JsonDocument TempDoc;
        JsonObject TempFunc = TempDoc.to<JsonObject>();
        Functions[i]->addTagStructures(TempFunc, JCA::TAG::TagUsage_T::GetAll);
        
        // Use a Map to store tag info (name -> accessType) to avoid duplicates
        std::map<String, uint8_t> TagInfoMap;
        
        // Extract tag names and access types from data tags
        if (TempFunc["data"].is<JsonArray>()) {
          JsonArray DataTags = TempFunc["data"].as<JsonArray>();
          for (JsonObject TagObj : DataTags) {
            if (TagObj["name"].is<String>()) {
              String TagName = TagObj["name"].as<String>();
              // Get accessType from readOnly field
              // readOnly: 0 = writable (Access contains Write), 1 = read-only (Access is Read only)
              // readOnly = 0 means (Access & Write) != 0, so accessType includes Write
              // readOnly = 1 means (Access & Write) == 0, so accessType is Read only
              bool ReadOnly = true; // default to read-only if not found
              if (TagObj["readOnly"].is<bool>()) {
                ReadOnly = TagObj["readOnly"].as<bool>();
              } else if (TagObj["readOnly"].is<int>()) {
                ReadOnly = TagObj["readOnly"].as<int>() != 0;
              } else if (TagObj["readOnly"].is<String>()) {
                ReadOnly = TagObj["readOnly"].as<String>() != "0";
              }
              
              // Calculate accessType: ReadOnly=0 means ReadWrite, ReadOnly=1 means Read only
              uint8_t AccessType = ReadOnly ? static_cast<uint8_t>(JCA::TAG::TagAccessType_T::Read) : 
                                   static_cast<uint8_t>(JCA::TAG::TagAccessType_T::ReadWrite);
              TagInfoMap[TagName] = AccessType;
            }
          }
        }
        
        // Extract tag names and access types from config tags
        if (TempFunc["config"].is<JsonArray>()) {
          JsonArray ConfigTags = TempFunc["config"].as<JsonArray>();
          for (JsonObject TagObj : ConfigTags) {
            if (TagObj["name"].is<String>()) {
              String TagName = TagObj["name"].as<String>();
              bool ReadOnly = true; // default to read-only if not found
              if (TagObj["readOnly"].is<bool>()) {
                ReadOnly = TagObj["readOnly"].as<bool>();
              } else if (TagObj["readOnly"].is<int>()) {
                ReadOnly = TagObj["readOnly"].as<int>() != 0;
              } else if (TagObj["readOnly"].is<String>()) {
                ReadOnly = TagObj["readOnly"].as<String>() != "0";
              }
              
              uint8_t AccessType = ReadOnly ? static_cast<uint8_t>(JCA::TAG::TagAccessType_T::Read) : 
                                   static_cast<uint8_t>(JCA::TAG::TagAccessType_T::ReadWrite);
              // If tag already exists, merge access types (use ReadWrite if either is writable)
              if (TagInfoMap.count(TagName) > 0) {
                if (!ReadOnly) {
                  TagInfoMap[TagName] = static_cast<uint8_t>(JCA::TAG::TagAccessType_T::ReadWrite);
                }
              } else {
                TagInfoMap[TagName] = AccessType;
              }
            }
          }
        }
        
        // Add all unique tags with their access types to the array
        for (auto const& pair : TagInfoMap) {
          JsonObject TagInfo = TagsArray.add<JsonObject>();
          TagInfo["name"] = pair.first;
          TagInfo["accessType"] = pair.second;
        }
          */
      }
    }
  }
}