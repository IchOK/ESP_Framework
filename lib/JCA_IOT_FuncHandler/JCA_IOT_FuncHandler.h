/**
 * @file JCA_IOT_Handler.h
 * @author JCA (https://github.com/ichok)
 * @brief Handling class to create an handle functions.
 * @version 1.0
 * @date 2024-04-21
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

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

#define JCA_IOT_FUNCHANDLER_SETUP_NAME "name"

namespace JCA {
  namespace IOT {
    typedef struct {
      int16_t Func;
      int16_t Tag;
    } FuncLinkPair_T;
    enum FuncLinkType_T : uint8_t {
      LinkNone = 0,
      LinkDirect = 1
    };
    typedef struct {
      std::vector<FuncLinkPair_T> Input;
      std::vector<FuncLinkPair_T> Output;
      FuncLinkType_T Type;
    } FuncLink_T;
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
    class FuncHandler {
    protected:
      // Json Tags
      static const char *JsonTagHardware;
      static const char *JsonTagFunctions;
      static const char *JsonTagLinks;
      String Name;
      // Setup
      String SetupFilePath;
      String FuncFilePath;
      String ValueFilePath;
      String LogFilePath;

      // LoopData
      unsigned long LastUpdate;

      // Controller Setup
      std::vector<FuncLink_T> Links;
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

      FuncHandler (String _Name, String _SetupFilePath = "/usrSetup.json", String _FuncFilePath = "/usrFunctions.json", String _ValueFilePath = "/usrValues.json", String _LogFilePath = "/usrLog.json");
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