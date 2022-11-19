/**
 * @file JCA_IOT_Webserver.h
 * @author JCA (https://github.com/ichok)
 * @brief The Class contains a Webserver (with WebSocket)
 * It contains the folloing Modules
 * - Navigation
 *   - Home [/home.htm], to be define by User (Startpage)
 *   - Config [/config.htm], to be define by User (Application Config Informations)
 *   - Connect [/connect -> PageFrame + SectionConnect], WiFi Connection Settings.
 *   - System [/sys -> PageFrame + SectionSys]
 *     - Download App config [config.json]
 *     - Upload Web-Content [*.json, *.htm, *.html, *.js, *.css]
 *     - Firmware Update [*.bin]
 *     - Reset the controller
 * - Style Sheet
 * - Navigation and Logo Icons
 * - RestAPI
 * - WebSocket
 *   - Websockt use RestAPI Callback-Functions for Events if no other is defined
 *     - onWsEvent : Default = onRestApiPost
 *     - onWsUpdate : Default = onRestApiGet
 * @version 0.1
 * @date 2022-09-04
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#ifndef _JCA_IO_WEBSERVER_
#define _JCA_IO_WEBSERVER_
#include "FS.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <LittleFS.h>

#define SPIFFS LittleFS
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESPAsyncTCP.h>
#elif ESP32
  #include <AsyncTCP.h>
  #include <ESPmDNS.h>
  #include <FS.h>
  #include <SPIFFS.h>
  #include <WiFi.h>
#endif
#include <ESPAsyncWebServer.h>

#include <ESP32Time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_Webserver_Boardinfo.h>
#include <JCA_IOT_Webserver_SVGs.h>
#include <JCA_IOT_Webserver_Sites.h>
#include <JCA_IOT_WiFiConnect.h>
#include <JCA_SYS_DebugOut.h>

// Manual setting Firmware withpout Git
#ifndef AUTO_VERSION
  #define AUTO_VERSION "V1.0.0"
#endif

// Default Config if not passt other Data to Contructor
#define JCA_IOT_WEBSERVER_DEFAULT_HOSTNAMEPREFIX "JCA_IOT_NODE_"
#define JCA_IOT_WEBSERVER_DEFAULT_PORT 80
#define JCA_IOT_WEBSERVER_DEFAULT_CONF_USER "Admin"
#define JCA_IOT_WEBSERVER_DEFAULT_CONF_PASS "Admin"
// Config File is readen on Init
#define JCA_IOT_WEBSERVER_CONFIGPATH "/sysConfig.json"
// JSON Keys for WiFi Config
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI "wifi"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_SSID "ssid"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_PASS "pass"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_IP "ip"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_GATEWAY "gateway"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_SUBNET "subnet"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_DHCP "dhcp"
// JSON Keys for Server Config
#define JCA_IOT_WEBSERVER_CONFKEY_HOSTNAME "hostname"
#define JCA_IOT_WEBSERVER_CONFKEY_PORT "port"
// JSON Keys for Web-Socket Config
#define JCA_IOT_WEBSERVER_CONFKEY_SOCKETUPDATE "wsUpdate"
// Website Config
#define JCA_IOT_WEBSERVER_PATH_CONNECT "/connect"
#define JCA_IOT_WEBSERVER_PATH_SYS "/sys"
#define JCA_IOT_WEBSERVER_PATH_SYS_UPLOAD "/upload"
#define JCA_IOT_WEBSERVER_PATH_SYS_UPDATE "/update"
#define JCA_IOT_WEBSERVER_PATH_SYS_RESET "/reset"
#define JCA_IOT_WEBSERVER_PATH_HOME "/home.htm"
#define JCA_IOT_WEBSERVER_PATH_CONFIG "/config.htm"
// Time settings
#define JCA_IOT_WEBSERVER_TIME_OFFSET 3600
#define JCA_IOT_WEBSERVER_TIME_VALID 1609459200
#define JCA_IOT_WEBSERVER_TIME_TIMEFORMAT "%d.%m.%G %H:%M:%S"
#define JCA_IOT_WEBSERVER_TIME_DATEFORMAT "%d.%m.%G"

namespace JCA {
  namespace IOT {
    typedef std::function<void (JsonVariant &_In, JsonVariant &_Out)> JsonVariantCallback;
    typedef std::function<void (void)> SimpleCallback;

    class Webserver : public JCA::FNC::Protocol {
    private:
      // ...Webserver_System.cpp
      static const char *ElementName;
      static const char *Hostname_Name;
      static const char *Hostname_Text;
      static const char *Hostname_Comment;
      static const char *WsUpdateCycle_Name;
      static const char *WsUpdateCycle_Text;
      static const char *WsUpdateCycle_Unit;
      static const char *WsUpdateCycle_Comment;
      static const char *TimeSync_Name;
      static const char *TimeSync_Text;
      static const char *TimeSync_Type;
      static const char *TimeSync_Comment;
      static const char *SaveConfig_Name;
      static const char *SaveConfig_Text;
      static const char *SaveConfig_Type;
      static const char *SaveConfig_Comment;
      static const char *SaveConfig_BtnText;
      static const char *Time_Name;
      static const char *Time_Text;
      static const char *Time_Comment;
      char Hostname[80];
      char ConfUser[80];
      char ConfPassword[80];
      char Firmware[80];
      const char *ObjectName = "IOT::Webserver";
      bool Reboot;
      WiFiConnect Connector;
      AsyncWebServer Server;
      AsyncWebSocket Websocket;
      ESP32Time Rtc;
      uint16_t Port;
      SimpleCallback onSystemResetCB;
      SimpleCallback onSaveConfigCB;
      bool readConfig ();

      // Protocol Functions
      void createConfigValues (JsonObject &_Values);
      void createDataValues (JsonObject &_Values);
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);

      void writeSetupConfig (File _SetupFile);
      void writeSetupData (File _SetupFile);
      void writeSetupCmdInfo (File _SetupFile);

      // ...Webserver_Web.cpp
      AwsTemplateProcessor replaceHomeWildcardsCB;
      AwsTemplateProcessor replaceConfigWildcardsCB;
      void onWebConnectPost (AsyncWebServerRequest *_Request);
      void onWebConnectGet (AsyncWebServerRequest *_Request);
      void onWebSystemGet (AsyncWebServerRequest *_Request);
      void onWebSystemUploadData (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final);
      void onWebSystemUpdate (AsyncWebServerRequest *_Request);
      void onWebSystemUpdateData (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final);
      void onWebSystemReset (AsyncWebServerRequest *_Request);
      void onWebHomeGet (AsyncWebServerRequest *_Request);
      void onWebConfigGet (AsyncWebServerRequest *_Request);
      String replaceDefaultWildcards (const String &var);
      String replaceHomeWildcards (const String &var);
      String replaceConfigWildcards (const String &var);
      String replaceSystemWildcards (const String &var);
      String replaceConnectWildcards (const String &var);

      // ...Webserver_RestApi.cpp
      JsonVariantCallback restApiGetCB;
      JsonVariantCallback restApiPostCB;
      JsonVariantCallback restApiPutCB;
      JsonVariantCallback restApiPatchCB;
      JsonVariantCallback restApiDeleteCB;
      void onRestApiRequest (AsyncWebServerRequest *_Request, JsonVariant &_Json);

      // ...Webserver_Socket.cpp
      uint32_t WsUpdateCycle;
      uint32_t WsLastUpdate;
      JsonVariantCallback wsDataCB;
      JsonVariantCallback wsUpdateCB;
      void onWsEvent (AsyncWebSocket *_Server, AsyncWebSocketClient *_Client, AwsEventType _Type, void *_Arg, uint8_t *_Data, size_t _Len);
      void wsHandleData (AsyncWebSocketClient *_Client, void *_Arg, uint8_t *_Data, size_t _Len);
      bool doWsUpdate (AsyncWebSocketClient *_Client);

    public:
      // ...Webserver_System.cpp
      Webserver (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword, unsigned long _Offset);
      Webserver (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword);
      Webserver (const char *_HostnamePrefix, uint16_t _Port, unsigned long _Offset);
      Webserver (const char *_HostnamePrefix, uint16_t _Port);
      Webserver (unsigned long _Offset);
      Webserver ();
      bool init ();
      bool handle ();
      void update (struct tm &_Time);
      void onSystemReset (SimpleCallback _CB);
      void onSaveConfig (SimpleCallback _CB);
      void setTime (unsigned long _Epoch = 1609459200, int _Millis = 0); // default (1609459200) = 1st Jan 2021
      void setTime (int _Second, int _Minute, int _Hour, int _Day, int _Month, int _Year, int _Millis = 0);
      void setTimeStruct (tm _Time);
      bool timeIsValid ();
      tm getTimeStruct ();
      String getTime ();
      String getDate ();
      String getTimeString (String _Format);

      // ...Webserver_Web.cpp
      void onWebHomeReplace (AwsTemplateProcessor _CB);
      void onWebConfigReplace (AwsTemplateProcessor _CB);

      // ...Webserver_RestApi.cpp
      void onRestApiGet (JsonVariantCallback _CB);
      void onRestApiPost (JsonVariantCallback _CB);
      void onRestApiPut (JsonVariantCallback _CB);
      void onRestApiPatch (JsonVariantCallback _CB);
      void onRestApiDelete (JsonVariantCallback _CB);

      // ...Webserver_Socket.cpp
      void onWsData (JsonVariantCallback _CB);
      void onWsUpdate (JsonVariantCallback _CB);
      void setWsUpdateCycle (uint32_t _CycleTime);
      bool doWsUpdate ();
    };
  }
}
#endif