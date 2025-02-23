#include <JCA_IOT_FuncHandler.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace IOT {
    const char *FuncHandler::JsonTagHardware = "hardware";
    const char *FuncHandler::JsonTagFunctions = "functions";
    const char *FuncHandler::JsonTagLinks = "links";

    FuncLink::FuncLink(FuncLinkType_T _Type) {
      Input = std::vector<FuncLinkPair_T>();
      Output = std::vector<FuncLinkPair_T>();
      Type = _Type;
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

    FuncHandler::FuncHandler (String _Name, String _SetupFilePath, String _FuncFilePath, String _ValueFilePath, String _LogFilePath) {
      SetupFilePath = _SetupFilePath;
      FuncFilePath = _FuncFilePath;
      ValueFilePath = _ValueFilePath;
      LogFilePath = _LogFilePath;
      Name = _Name;
      LinkMapping["direct"] = FuncLinkType_T::LinkDirect;
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
          JsonObject Log = _LogArray.createNestedObject ();
          Log["func"] = _FuncName;
          Log["tag"] = _TagName;
          return false;
        }
      } else {
        _Tag = -1;
        Debug.print (FLAG_ERROR, true, Name, __func__, "Function to link not found : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, _FuncName);
        JsonObject Log = _LogArray.createNestedObject ();
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
      DynamicJsonDocument SetupDoc (10000);
      DynamicJsonDocument LogDoc(10000);

      if (!LittleFS.exists(SetupFilePath)) {
        JsonObject LogObj = LogDoc.createNestedObject ("File");
        LogObj["Name"] = SetupFilePath;
        LogObj["Error"] = "not found";
        RetValue = FuncPatchRet_T::fileMissing;
      } else {
        // Open Setup File
        File SetupFile = LittleFS.open (SetupFilePath, FILE_READ);
        DeserializationError Error = deserializeJson (SetupDoc, SetupFile);

        if (Error) {
          Debug.print (FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
          Debug.println (FLAG_ERROR, true, Name, __func__, Error.c_str());
          JsonObject LogObj = LogDoc.createNestedObject ("File");
          LogObj["Name"] = SetupFilePath;
          LogObj["Error"] = Error.c_str ();
          RetValue = FuncPatchRet_T::jsonSyntax;
        } else {
          JsonObject SetupObj = SetupDoc.as<JsonObject>();

          // First have to delete alle Functions and Links to rebuild the Vectors
          deleteFunctions();

          //-------------------------------------------------------
          // HardwareMapping 
          //-------------------------------------------------------
          if (SetupObj.containsKey (JsonTagHardware)) {
            Debug.println (FLAG_SETUP, true, Name, __func__, "Found Hardware");
            JsonArray LogArray = LogDoc.createNestedArray ("Hardware");
            JsonArray SetupHwArr = SetupObj[JsonTagHardware].as<JsonArray>();
            for (JsonObject SetupHwObj : SetupHwArr) {
              Debug.println (FLAG_SETUP, true, Name, __func__, SetupHwObj["type"].as<String>());
              JsonObject Log = LogArray.createNestedObject ();
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
          if (SetupObj.containsKey (JsonTagFunctions)) {
            Debug.println (FLAG_SETUP, true, Name, __func__, "Found Functions");
            JsonArray LogArray = LogDoc.createNestedArray ("Functions");
            JsonArray SetupFuncArr = SetupObj[JsonTagFunctions].as<JsonArray> ();
            for (JsonObject SetupFuncObj : SetupFuncArr) {
              Debug.println (FLAG_SETUP, true, Name, __func__, SetupFuncObj["type"].as<String> ());
              JsonObject Log = LogArray.createNestedObject ();
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
          if (SetupObj.containsKey (JsonTagLinks)) {
            Debug.println (FLAG_SETUP, true, Name, __func__, "Found Links");
            JsonArray LogArray = LogDoc.createNestedArray ("Links");
            JsonArray SetupLinkArr = SetupObj[JsonTagLinks].as<JsonArray> ();
            for (JsonObject SetupLinkObj : SetupLinkArr) {
              Debug.println (FLAG_SETUP, true, Name, __func__, SetupLinkObj["type"].as<String> ());
              JsonObject Log = LogArray.createNestedObject ();
              if (LinkMapping.count (SetupLinkObj["type"]) == 1) {
                // Create Link
                Links.push_back (new FuncLink (LinkMapping[SetupLinkObj["type"].as<String> ()]));
                Log["Type"] = SetupLinkObj["type"].as<String> ();
                size_t Link = Links.size() - 1;
                int16_t FuncIndex;
                int16_t TagIndex;


                // Add all From Pointer
                JsonArray FromArr = SetupLinkObj["from"].as<JsonArray> ();
                JsonArray LogFrom = Log.createNestedArray("IN");
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
                JsonArray LogTo = Log.createNestedArray ("OUT");
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

          // write Functions File to used by Webpage
          saveFunctions ();
        }
        SetupFile.close ();
      }

      // Write Logfile
      File LogFile = LittleFS.open (LogFilePath, FILE_WRITE);
      serializeJson (LogDoc, LogFile);
      LogFile.close ();

      Debug.println (FLAG_SETUP, true, Name, __func__, "Done");
      return RetValue;
    }

    /**
     * @brief clear all Setup-Data and delete the Functio
     * 
     */
    FuncPatchRet_T FuncHandler::remove () {
      FuncPatchRet_T RetValue = FuncPatchRet_T::done;
      deleteFunctions ();
      LittleFS.remove(FuncFilePath);
      return RetValue;
    }

    /**
     * @brief Save the FunctionList in JSON to Setup the Webpage
     * (Default: usrFunctions.json)
     */
    FuncPatchRet_T FuncHandler::saveFunctions () {
      FuncPatchRet_T RetValue = FuncPatchRet_T::done;
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      // Open Output File
      File FuncFile = LittleFS.open (FuncFilePath, FILE_WRITE, true);
      if (!FuncFile) {
        RetValue = FuncPatchRet_T::fileOpen;
      } else {
        bool InitDone = false;
        // Create Object with all Functions inside
        FuncFile.println ("{");
        for (int16_t i = 0; i < Functions.size (); i++) {
          Functions[i]->writeFunction (FuncFile, InitDone);
        }
        FuncFile.println ("}");
        // Close File
        FuncFile.close ();
      }
      if (Debug.println (FLAG_DATA, true, "FuncHandler", __func__, "Functions-JSON")) {
        File FuncFile = LittleFS.open (FuncFilePath, FILE_READ);
        while (FuncFile.available ()) {
          Debug.print (FLAG_DATA, true, "FuncHandler", __func__, (char)FuncFile.read ());
        }
        FuncFile.close ();
      }
      return RetValue;
    }

    /**
     * @brief get all Values from Functions and store it to File
     * (Default: usrValues.json)
     */
    FuncPatchRet_T FuncHandler::saveValues () {
      FuncPatchRet_T RetValue = FuncPatchRet_T::done;
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      DynamicJsonDocument ValueDoc (10000);
      JsonObject Values = ValueDoc.createNestedObject (JCA::FNC::FuncParent::JsonTagElements); //.as<JsonObject>();
      getValues (Values);
      File ValuesFile = LittleFS.open (ValueFilePath, FILE_WRITE);
      if (!ValuesFile) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "Failed to open File for write : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, ValueFilePath);
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
      DynamicJsonDocument ValueDoc (10000);
      if (!LittleFS.exists (ValueFilePath)) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "File not found : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, ValueFilePath);
        RetValue = FuncPatchRet_T::fileMissing;
      } else {
        File ValuesFile = LittleFS.open (ValueFilePath, FILE_READ);
        DeserializationError Error = deserializeJson (ValueDoc, ValuesFile);
        if (Error) {
          Debug.print (FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
          Debug.println (FLAG_ERROR, true, Name, __func__, Error.c_str ());
          RetValue = FuncPatchRet_T::jsonSyntax;
        } else if (ValueDoc.containsKey (JCA::FNC::FuncParent::JsonTagElements)) {
          JsonObject Values = ValueDoc[JCA::FNC::FuncParent::JsonTagElements].as<JsonObject> ();
          setValues (Values);
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
      DynamicJsonDocument LinkDoc (1000);

      // Update Links
      for (FuncLink *Link : Links) {

        switch (Link->Type) {
        case FuncLinkType_T::LinkDirect:
          // Direkt Link always read the first Input-Link and set it to all Output Links
          if (Link->getInputCount () > 0 && Link->getOutputCount () > 0) {
            JsonVariant Value = LinkDoc.to<JsonVariant> ();
            FuncLinkPair_T Input = Link->getInput(0);

            if (Functions[Input.Func]->getTagValueByIndex (Input.Tag, Value)) {
              for (uint8_t OutputIndex = 0; OutputIndex < Link->getInputCount(); OutputIndex++) {
                FuncLinkPair_T Output = Link->getOutput(0);
                Functions[Output.Func]->setTagValueByIndex (Output.Tag, Value);
              }
            }
          }
          break;

        default:
          break;
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
      if (_Command == "saveconfig") {
        RetValue = saveFunctions ();
      } else if (_Command == "savevalues") {
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
      } else if (_Command == "delete") {
        RetValue = remove ();
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
      for (int16_t i = 0; i < Functions.size (); i++) {
        if (Functions[i]->getName () == _Name) {
          return i;
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
    void FuncHandler::setValues( JsonObject &_Functions) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      for (JsonPair Function : _Functions) {
        int16_t FuncIndex = getFuncIndex (Function.key ().c_str ());
        if (FuncIndex >= 0) {
          JsonObject FuncValues = Function.value ().as<JsonObject> ();
          Functions[FuncIndex]->setValues(FuncValues);
        }
      }
    }

    /**
     * @brief returns a Values-Object of all the Functions Tags
     * 
     * @param _Functions REF where the data will returned
     */
    void FuncHandler::getValues (JsonObject &_Functions) {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      for (int16_t i = 0; i < Functions.size (); i++) {
        JsonObject Function = _Functions.createNestedObject(Functions[i]->getName());
        Functions[i]->addValues(Function);
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
  }
}