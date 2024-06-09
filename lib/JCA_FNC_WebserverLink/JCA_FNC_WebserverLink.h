/**
 * @file JCA_FNC_WebserverLink.h
 * @author JCA (https://github.com/ichok)
 * @brief Interface to IOT_Webserver class
 * @version 0.1
 * @date 2024-04-22
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_WEBSERVERLINK_
#define _JCA_FNC_WEBSERVERLINK_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_IOT_Webserver.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagString.h>
#include <JCA_TAG_TagUInt32.h>

namespace JCA {
  namespace FNC {
    class WebserverLink : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagRefName;

      // Hardware
      JCA::IOT::Webserver *ServerRef;
      void setTimeCB();

      // Konfig
      uint32_t WsUpdateCycle;
      String Hostname;

      // Daten
      uint32_t TimeSync;
      String ActTime;

    public:
      WebserverLink (JCA::IOT::Webserver *_ServerRef, String _Name);
      ~WebserverLink () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif