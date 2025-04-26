/**
 * @file JCA_IOT_Webserver.h
 * @author JCA (https://github.com/ichok)
 * @brief The Class contains a Server (with WebSocket)
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
 * - UdpListener
 *   - Listen to UDP-Packets in JSON for Timesync and maybe more some times
 * - LocaltimeZone
 *   - Get Epoch of local Timezone with Daylight Saving Time
 * @version 1.0
 * @date 2022-09-04
 * @changelog
 * - [0.1] 2022-09-04: First Version
 * - [1.0] 2025-04-12: UdpListener added, Localtime Zone added
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#ifndef _JCA_IOT_SERVER_
#define _JCA_IOT_SERVER_
#include "FS.h"
#include <Arduino.h>
#include <ArduinoJson.h>

#ifdef ESP32
  #include <AsyncTCP.h>
  #include <AsyncUDP.h>
  #include <ESPmDNS.h>
  #include <FS.h>
  #include <SPIFFS.h>
  #include <Update.h>
  #include <WiFi.h>
#elif defined(ESP8266)
  #define SPIFFS LittleFS
  #include "ESPAsyncUDP.h"
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESPAsyncTCP.h>
#elif defined(TARGET_RP2040)
  #include <WebServer.h>
  #include <WiFi.h>
#endif

#include <ESPAsyncWebServer.h>

#if __has_include("ArduinoJson.h")
  #include <ArduinoJson.h>
  #include <AsyncJson.h>
  #include <AsyncMessagePack.h>
#endif

#include <LittleFS.h>

#include <ESP32Time.h>

#include <JCA_IOT_Server_Boardinfo.h>
#include <JCA_IOT_Server_WebSVGs.h>
#include <JCA_IOT_Server_WebSites.h>
#include <JCA_IOT_WiFiConnect.h>
#include <JCA_SYS_DebugOut.h>

// Manual setting Firmware withpout Git
#ifndef AUTO_VERSION
  #define AUTO_VERSION "V1.0.0"
#endif

// Default Config if not passt other Data to Contructor
#define JCA_IOT_SERVER_DEFAULT_HOSTNAMEPREFIX "JCA_IOT_NODE_"
#define JCA_IOT_SERVER_DEFAULT_WEBSERVERPORT 80
#define JCA_IOT_SERVER_DEFAULT_UDPLISTENERPORT 81
#define JCA_IOT_SERVER_DEFAULT_CONF_USER "Admin"
#define JCA_IOT_SERVER_DEFAULT_CONF_PASS "Admin"
// JSON Files used by Server for Config and Data-Storage, only if not defines in main.cpp or somewhere else
#ifndef JCA_IOT_FILE_WIFICONFIG
  #define JCA_IOT_FILE_WIFICONFIG "/sysWifi.json"
#endif
#ifndef JCA_IOT_FILE_SYSTEMCONFIG
  #define JCA_IOT_FILE_SYSTEMCONFIG "/sysConfig.json"
#endif
#ifndef JCA_IOT_FILE_FUNCTIONS
  #define JCA_IOT_FILE_FUNCTIONS "/usrFunctions.json"
#endif
// JSON Keys for WiFi Config
#define JCA_IOT_SERVER_CONFKEY_WIFI "wifi"
#define JCA_IOT_SERVER_CONFKEY_WIFI_SSID "ssid"
#define JCA_IOT_SERVER_CONFKEY_WIFI_PASS "pass"
#define JCA_IOT_SERVER_CONFKEY_WIFI_IP "ip"
#define JCA_IOT_SERVER_CONFKEY_WIFI_GATEWAY "gateway"
#define JCA_IOT_SERVER_CONFKEY_WIFI_SUBNET "subnet"
#define JCA_IOT_SERVER_CONFKEY_WIFI_DHCP "dhcp"
#define JCA_IOT_SERVER_CONFKEY_WIFI_STATPIN "statpin"
// JSON Keys for Server Config
#define JCA_IOT_SERVER_CONFKEY_HOSTNAME "hostname"
#define JCA_IOT_SERVER_CONFKEY_PORT "port"
#define JCA_IOT_SERVER_CONFKEY_UDPPORT "udtPort"
#define JCA_IOT_SERVER_CONFKEY_LOCALTIMEZONE "localTimeZone"
#define JCA_IOT_SERVER_CONFKEY_DAYLIGHTSAVING "dayLightSaving"
#define JCA_IOT_SERVER_CONFKEY_REBOOTCOUNTER "rebootCounter"
// JSON Keys for Web-Socket Config
#define JCA_IOT_SERVER_CONFKEY_SOCKETUPDATE "wsUpdate"
// Website Config
#define JCA_IOT_SERVER_PATH_CONNECT "/connect"
#define JCA_IOT_SERVER_PATH_SYS "/sys"
#define JCA_IOT_SERVER_PATH_SYS_UPLOAD "/upload"
#define JCA_IOT_SERVER_PATH_SYS_UPDATE "/update"
#define JCA_IOT_SERVER_PATH_SYS_RESET "/reset"
#define JCA_IOT_SERVER_PATH_HOME "/home.htm"
#define JCA_IOT_SERVER_PATH_CONFIG "/config.htm"
#define JCA_IOT_SERVER_PATH_CONFIGSAVE "/configSave"
// Time settings
#define JCA_IOT_SERVER_TIME_OFFSET 3600
#define JCA_IOT_SERVER_TIME_VALID 1609459200
#define JCA_IOT_SERVER_TIME_DATETIMEFORMAT "%d.%m.%G %H:%M:%S"
#define JCA_IOT_SERVER_TIME_DATEFORMAT "%d.%m.%G"
#define JCA_IOT_SERVER_TIME_TIMEFORMAT "%H:%M:%S"

namespace JCA {
  namespace IOT {
    typedef std::function<void (JsonVariant &_In, JsonVariant &_Out)> JsonVariantCallback;
    typedef std::function<void (void)> SimpleCallback;

    class Server {
    private:
      // ...Webserver_System.cpp
      static const char *ElementName;
      char ConfUser[80];
      char ConfPassword[80];
      char Firmware[80];
      const char *ObjectName = "IOT::Server";
      bool Reboot;
      WiFiConnect Connector;
      AsyncWebServer WebServerObject;
      AsyncWebSocket WebSocketObject;
      AsyncUDP UpdListenerObject;
      ESP32Time Rtc;
      String WebConfigFile;

      SimpleCallback onSystemResetCB;
      SimpleCallback onSaveConfigCB;
      bool readConfig ();
      int getLastSunday(int _Year, int _Month);

      // ...UdpListener.cpp
      void udpPacketHandler (AsyncUDPPacket _Packet);
    
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
      unsigned long WsLastUpdate;
      JsonVariantCallback wsDataCB;
      JsonVariantCallback wsUpdateCB;
      void onWsEvent (AsyncWebSocket *_Server, AsyncWebSocketClient *_Client, AwsEventType _Type, void *_Arg, uint8_t *_Data, size_t _Len);
      void wsHandleData (AsyncWebSocketClient *_Client, void *_Arg, uint8_t *_Data, size_t _Len);
      bool doWsUpdate (AsyncWebSocketClient *_Client);

    public:
      // ...Webserver_System.cpp
      String Hostname;
      uint32_t LocalTimeZone;
      bool DaylightSavingTime;
      uint16_t WebServerPort;
      uint16_t UdpListenerPort;
      uint16_t RebootCounter;
      Server (const char *_HostnamePrefix, uint16_t _WebServerPort, uint16_t _UdpListenerPort, const char *_ConfUser, const char *_ConfPassword, unsigned long _Offset, bool _DayLightSaving);
      Server (const char *_HostnamePrefix, uint16_t _WebServerPort, uint16_t _UdpListenerPort, const char *_ConfUser, const char *_ConfPassword);
      Server (const char *_HostnamePrefix, uint16_t _WebServerPort, uint16_t _UdpListenerPort, unsigned long _Offset, bool _DayLightSaving);
      Server (const char *_HostnamePrefix, uint16_t _WebServerPort, uint16_t _UdpListenerPort);
      Server (unsigned long _Offset, bool _DayLightSaving);
      Server ();
      bool init ();
      bool handle ();
      void onSystemReset (SimpleCallback _CB);
      void onSaveConfig (SimpleCallback _CB);
      void setTime (unsigned long _Epoch = 1609459200, int _Millis = 0); // default (1609459200) = 1st Jan 2021
      void setTime (int _Second, int _Minute, int _Hour, int _Day, int _Month, int _Year, int _Millis = 0);
      void setStatePin (int8_t _Pin);
      void setTimeStruct (tm _Time);
      void setWebConfigFile (String _WebConfigFile);
      bool writeSystemConfig ();
      bool timeIsValid ();
      tm getSystemTimeStruct ();
      tm getLocalTimeStruct ();
      String getTimeString ();
      String getDateString ();
      String getTimeFormated (String _Format, bool _LocalTime = false);
      uint32_t getLocalTime();
      uint32_t getSystemTime();

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
      uint32_t WsUpdateCycle;
      void onWsData (JsonVariantCallback _CB);
      void onWsUpdate (JsonVariantCallback _CB);
      bool doWsUpdate ();
    };
  }
}
#endif