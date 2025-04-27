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

#ifndef _JCA_FNC_SERVERLINK_
#define _JCA_FNC_SERVERLINK_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_IOT_Server.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagString.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagUInt16.h>
#include <JCA_TAG_TagUInt32.h>

namespace JCA {
  namespace FNC {
    class ServerLink : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagRefName;

      // Hardware
      JCA::IOT::Server *ServerRef;
      void setTimeCB();
      void writeServerConfigCB();
      void setServerDataCB ();
      void getServerDataCB ();

      // Konfig
      String Hostname;
      uint32_t WsUpdateCycle;
      uint16_t WebServerPort;
      uint16_t UdpListenerPort;
      uint32_t LocalTimeZone;
      bool DaylightSavingTime;
      uint16_t RebootCounter;

      // Daten
      bool WriteConfig;
      uint32_t TimeSync;
      String SystemTime;
      String LocalTime;

    public:
      ServerLink (JCA::IOT::Server *_ServerRef, String _Name);
      ~ServerLink () {;};
      void update (struct tm &_Time);
      

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif