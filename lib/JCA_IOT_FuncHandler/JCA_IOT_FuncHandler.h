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
#include <map>
#include <vector>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace IOT {
    class FuncHandler {
    protected:
      // Json Tags
      static const char *JsonTagHardware;
      static const char *JsonTagFunctions;
      static const char *JsonTagLinks;
      // Intern
      String SetupFilePath;

      // Dataconfig
      std::vector<JCA::FNC::FuncParent *> Functions;

    public:
      FuncHandler (String _SetupFilePath);
      // Map with the initialisation callbacks for all functions
      std::map<String, std::function<bool(JsonObject, std::vector<JCA::FNC::FuncParent *>&)> > FunctionList;

      void update (struct tm &_Time);
    };
  }
}

#endif