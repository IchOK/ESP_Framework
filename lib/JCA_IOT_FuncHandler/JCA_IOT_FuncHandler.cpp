#include <JCA_IOT_FuncHandler.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace IOT {
    const char *FuncHandler::JsonTagHardware = "hardware";
    const char *FuncHandler::JsonTagFunctions = "functions";
    const char *FuncHandler::JsonTagLinks = "links";

    FuncHandler::FuncHandler (String _Name, String _SetupFilePath, String _FuncFilePath, String _ValueFilePath, String _LogFilePath) {
      SetupFilePath = _SetupFilePath;
      FuncFilePath = _FuncFilePath;
      ValueFilePath = _ValueFilePath;
      LogFilePath = _LogFilePath;
      Name = _Name;
      LinkMapping["direct"] = FuncLinkType_T::LinkDirect;
    }

    /**
     * @brief Save the FunctionList in JSON to Setup tha Webpage
     * Default File is usrFunctions.json
     */
    void FuncHandler::saveFunctions(){
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      // Open Output File
      File FuncFile = LittleFS.open (FuncFilePath, FILE_WRITE);
      bool InitDone = false;
      // Create Object with all Functions inside
      FuncFile.println("{");
      for (int16_t i = 0; i < Functions.size(); i++) {
        Functions[i]->writeFunction(FuncFile, InitDone);
      }
      FuncFile.println ("}");
      // Close File
      FuncFile.close();
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
     * @brief Read the Setup file and create Hardware-, Function- and LinkList
     * 
     */
    void FuncHandler::setup(){
      Debug.println (FLAG_SETUP, true, Name, __func__, "Run");
      DynamicJsonDocument SetupDoc (10000);
      DynamicJsonDocument LogDoc(10000);
      // Open Setup File
      File SetupFile = LittleFS.open (SetupFilePath, FILE_READ);

      DeserializationError Error = deserializeJson (SetupDoc, SetupFile);

      if (Error) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
        Debug.println (FLAG_ERROR, true, Name, __func__, Error.c_str());
        JsonObject LogObj = LogDoc.createNestedObject ("File");
        LogObj["Name"] = SetupFilePath;
        LogObj["Error"] = Error.c_str ();
      } else {
        JsonObject SetupObj = SetupDoc.as<JsonObject>();

        //-------------------------------------------------------
        // HardwareMapping 
        //-------------------------------------------------------
        if (SetupObj.containsKey (JsonTagHardware)) {
          Debug.println (FLAG_SETUP, true, Name, __func__, "Found Hardware");
          JsonArray LogArray = LogDoc.createNestedArray ("Hardware");
          JsonArray SetupHwArr = SetupObj[JsonTagHardware].as<JsonArray>();
          for (JsonObject SetupHwObj : SetupHwArr) {
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
            JsonObject Log = LogArray.createNestedObject ();
            if (FunctionList.count (SetupFuncObj["type"]) == 1) {
              // Function found in creator List -> Call Creator and add to Function Vector
              FunctionList[SetupFuncObj["type"].as<String> ()](SetupFuncObj, Log, Functions, HardwareMapping);
            } else {
              // Function not found, log error
              Debug.print (FLAG_ERROR, true, Name, __func__, "Function not found in Function List : ");
              Debug.println (FLAG_ERROR, true, Name, __func__, SetupFuncObj["type"].as<String> ());
              Log["Fault"] = "Type not found" + SetupFuncObj["type"].as<String> ();
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
            JsonObject Log = LogArray.createNestedObject ();
            if (LinkMapping.count (SetupLinkObj["type"]) == 1) {
              // Create Link
              Links.push_back(FuncLink_T());
              size_t Link = Links.size();
              Links[Link].Type = LinkMapping[SetupLinkObj["type"].as<String>()];
              int16_t FuncIndex;
              int16_t TagIndex;


              // Add all From Pointer
              JsonArray FromArr = SetupLinkObj["from"].as<JsonArray> ();
              JsonArray LogFrom = Log.createNestedArray("IN");
              for (JsonObject FromObj : FromArr) {
                if (checkLink (FromObj["func"].as<String> (), FuncIndex, FromObj["tag"].as<String> (), TagIndex, LogArray)) {
                  Links[Link].Input.push_back ({ FuncIndex, TagIndex });
                } else {
                  LogFrom.add ("FAIL: " + FromObj["func"].as<String> () + "_" + FromObj["tag"].as<String> ());
                }
              }

              // Add all To Pointer
              JsonArray ToArr = SetupLinkObj["to"].as<JsonArray> ();
              JsonArray LogTo = Log.createNestedArray ("OUT");
              for (JsonObject ToObj : ToArr) {
                if (checkLink (ToObj["func"].as<String> (), FuncIndex, ToObj["tag"].as<String> (), TagIndex, LogArray)) {
                  Links[Link].Output.push_back ({ FuncIndex, TagIndex });
                } else {
                  LogTo.add ("FAIL: " + ToObj["func"].as<String> () + "_" + ToObj["tag"].as<String> ());
                }
              }
            } else {
              // Function not found, log error
              Debug.print (FLAG_ERROR, true, Name, __func__, "Link-Type not defined : ");
              Debug.println (FLAG_ERROR, true, Name, __func__, SetupLinkObj["type"].as<String> ());
              Log["Fault"] = "Type not found" + SetupLinkObj["type"].as<String> ();
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

      // Write Logfile
      File LogFile = LittleFS.open (LogFilePath, FILE_WRITE);
      serializeJson (LogDoc, LogFile);
      LogFile.close ();

      Debug.println (FLAG_SETUP, true, Name, __func__, "Done");
    }

    /**
     * @brief Updates the Links and the Functions
     * 
     * @param _Time current Time from RTC
     */
    void FuncHandler::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, true, Name, __func__, "Run");
      // Create JsonDoc to hold variant Data
      DynamicJsonDocument LinkDoc(1000);

      // Update Links
      for (FuncLink_T Link : Links) {

        switch (Link.Type)
        {
        case FuncLinkType_T::LinkDirect :
        // Direkt Link always read the first Input-Link and set it to all Output Links
          if (Link.Input.size() > 0 && Link.Output.size() > 0) {
            JsonVariant Value = LinkDoc.to<JsonVariant> ();
            FuncLinkPair_T Input = Link.Input[0];

            if (Functions[Input.Func]->getTagValueByIndex (Input.Tag, Value)){
              for (FuncLinkPair_T Output : Link.Output) {
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
      for (uint16_t i = 0; i < Functions.size(); i++) {
        Functions[i]->update(_Time);
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
     * @brief get all Values and store it to File (Default: usrValues.json)
     * 
     */
    void FuncHandler::saveValues (){
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      DynamicJsonDocument ValueDoc (10000);
      JsonObject Values = ValueDoc.createNestedObject (JCA::FNC::FuncParent::JsonTagElements); //.as<JsonObject>();
      getValues(Values);
      File ValuesFile = LittleFS.open (ValueFilePath, FILE_WRITE);
      if (!ValuesFile) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "Faild to open File for write : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, ValueFilePath);
      }
      size_t FileSize = serializeJson (ValueDoc, ValuesFile);
      Debug.print (FLAG_PROTOCOL, true, Name, __func__, "File length : ");
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, FileSize);
      ValuesFile.close ();
    }

    void FuncHandler::loadValues () {
      Debug.println (FLAG_PROTOCOL, true, Name, __func__, "Run");
      DynamicJsonDocument ValueDoc (10000);
      File ValuesFile = LittleFS.open (ValueFilePath, FILE_READ);
      if (!ValuesFile) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "Faild to open File for read : ");
        Debug.println (FLAG_ERROR, true, Name, __func__, ValueFilePath);
      }
      DeserializationError Error = deserializeJson (ValueDoc, ValuesFile);
      if (Error) {
        Debug.print (FLAG_ERROR, true, Name, __func__, "DeserializeJson failed: ");
        Debug.println (FLAG_ERROR, true, Name, __func__, Error.c_str ());
      } else if (ValueDoc.containsKey (JCA::FNC::FuncParent::JsonTagElements)) {
        JsonObject Values = ValueDoc[JCA::FNC::FuncParent::JsonTagElements].as<JsonObject> ();
        setValues (Values);
      }
      ValuesFile.close ();
    }
  }
}