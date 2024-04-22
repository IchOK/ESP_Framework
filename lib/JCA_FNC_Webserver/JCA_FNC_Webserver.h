/**
 * @file JCA_FNC_Webserver.h
 * @author JCA (https://github.com/ichok)
 * @brief Interface to IOT_Webserver class
 * @version 0.1
 * @date 2024-04-22
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_WEBSERVER_
#define _JCA_FNC_WEBSERVER_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagString.h>
#include <JCA_TAG_TagUInt32.h>
#include <JCA_IOT_Webserver.h>

namespace JCA {
  namespace FNC {
    class Webserver : public FuncParent {
    private:
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
      Webserver (JCA::IOT::Webserver *_ServerRef, const char *_Name);
      void update (struct tm &_Time);
    };
  }
}

#endif