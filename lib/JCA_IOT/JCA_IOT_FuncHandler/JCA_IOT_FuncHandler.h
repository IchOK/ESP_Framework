/**
 * @file JCA_IOT_Handler.h
 * @author JCA (https://github.com/ichok)
 * @brief Handling class to create an handle functions.
 * @version 1.1
 * @date 2024-04-21
 * @changelog
 * - 1.0 2024-04-21: Initial version
 * - 1.1 2024-04-21: Added new Link-Type Move
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_IOT_FUNCHANDLER_
#define _JCA_IOT_FUNCHANDLER_

#include <ArduinoJson.h>
#include <FS.h>
#include <LittleFS.h>
#include <map>
#include <vector>

// Add LittleFS constants if not exists
#ifndef FILE_READ
  #define FILE_READ "r"
#endif
#ifndef FILE_WRITE
  #define FILE_WRITE "w"
#endif

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

#define JCA_IOT_FUNCHANDLER_SETUP_NAME "name"
// JSON Files used Functionhandler for Config and Data-Storage, only if not defines in main.cpp or somewhere else
#ifndef JCA_IOT_FILE_SETUP
  #define JCA_IOT_FILE_SETUP "/usrSetup.json"
#endif
#ifndef JCA_IOT_FILE_FUNCTIONS
  #define JCA_IOT_FILE_FUNCTIONS "/usrFunctions.json"
#endif
#ifndef JCA_IOT_FILE_VALUES
  #define JCA_IOT_FILE_VALUES "/usrValues.json"
#endif
#ifndef JCA_IOT_FILE_LOG
  #define JCA_IOT_FILE_LOG "/usrLog.json"
#endif

namespace JCA {
  namespace IOT {
    struct FuncLinkPair_T{
      int16_t Func;
      int16_t Tag;
    };
    enum FuncLinkType_T : uint8_t {
      LinkNone = 0,
      LinkDirect = 1,
      LinkMove = 2
    };
    enum FuncPatchRet_T : int8_t {
      done = 127,
      linkObjMissing = 35,
      linkTypMissing = 30,
      hardwareMissing = 20,
      functionMissing = 10,
      fileMissing = -1,
      jsonSyntax = -2,
      fileOpen = -3,
      modeUndef = -4,
      failed = -99
    };
    
    class FuncLink {
    private:
      std::vector<FuncLinkPair_T> Input;
      std::vector<FuncLinkPair_T> Output;

    public:
      FuncLinkType_T Type;

      FuncLink(FuncLinkType_T _Type);
      ~FuncLink();
      void addInput(FuncLinkPair_T _Input);
      void addOutput(FuncLinkPair_T _Output);
      FuncLinkPair_T getInput(uint8_t _Index);
      FuncLinkPair_T getOutput(uint8_t _Index);
      uint8_t getInputCount() { return Input.size(); };
      uint8_t getOutputCount() { return Output.size(); };
    };

    class FuncHandler {
    protected:
      // Json Tags
      static const char *JsonTagHardware;
      static const char *JsonTagFunctions;
      static const char *JsonTagLinks;
      String Name;

      // LoopData
      unsigned long LastUpdate;

      // Controller Setup
      std::vector<FuncLink *> Links;
      std::map<String, FuncLinkType_T> LinkMapping;

      bool checkLink (String _FuncName, int16_t &_Func, String _TagName, int16_t &_Tag, JsonArray _LogArray);
      void deleteLinks();
      void deleteFunctions();
      FuncPatchRet_T setup ();
      FuncPatchRet_T remove ();
      FuncPatchRet_T saveFunctions ();
      FuncPatchRet_T saveValues ();
      FuncPatchRet_T loadValues ();

    public:
      // Map with the initialisation callbacks for all functions
      std::map<String, std::function<bool (JsonObject, JsonObject, std::map<String, void *>&)>> HardwareList;
      std::map<String, void *> HardwareMapping;
      std::map<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> FunctionList;
      std::vector<JCA::FNC::FuncParent *> Functions;

      FuncHandler (String _Name);
      void update (struct tm &_Time);
      String patch(String _Command);

      int16_t getFuncIndex (String _Name);
      void setValues (JsonObject &_Functions);
      void getValues (JsonObject &_Functions);
      int16_t getLinkCount();
      int16_t getFuncCount();
    };
  }
}

#endif