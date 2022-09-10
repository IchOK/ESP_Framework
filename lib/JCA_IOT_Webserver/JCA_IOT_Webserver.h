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
 * - ResrAPI
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
#include <AsyncJson.h>
#include <ArduinoJson.h>
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

#include <JCA_IOT_DebugOut.h>
#include <JCA_IOT_Webserver_Boardinfo.h>
#include <JCA_IOT_Webserver_SVGs.h>
#include <JCA_IOT_Webserver_Sites.h>
#include <JCA_IOT_WiFiConnect.h>

// Default Config if not passt other Data to Contructor
#define JCA_IOT_WEBSERVER_DEFAULT_HOSTNAMEPREFIX "JCA_IOT_NODE_"
#define JCA_IOT_WEBSERVER_DEFAULT_PORT 80
#define JCA_IOT_WEBSERVER_DEFAULT_CONF_USER "Admin"
#define JCA_IOT_WEBSERVER_DEFAULT_CONF_PASS "Admin"
// Config File is readen on Init
#define JCA_IOT_WEBSERVER_CONFIGPATH "/sysConfig.json"
// JSON Keys for Config-Post
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI "wifi"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_SSID "ssid"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_PASS "pass"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_IP "ip"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_GATEWAY "gateway"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_SUBNET "subnet"
#define JCA_IOT_WEBSERVER_CONFKEY_WIFI_DHCP "dhcp"
// JSON Keys for Config-Post
#define JCA_IOT_WEBSERVER_CONFKEY_HOSTNAME "hostname"
#define JCA_IOT_WEBSERVER_CONFKEY_PORT "port"
// Website Config
#define JCA_IOT_WEBSERVER_PATH_CONNECT "/connect"
#define JCA_IOT_WEBSERVER_PATH_SYS "/sys"
#define JCA_IOT_WEBSERVER_PATH_SYS_UPLOAD "/upload"
#define JCA_IOT_WEBSERVER_PATH_SYS_UPDATE "/update"
#define JCA_IOT_WEBSERVER_PATH_SYS_RESET "/reset"
#define JCA_IOT_WEBSERVER_PATH_HOME "/home.htm"
#define JCA_IOT_WEBSERVER_PATH_CONFIG "/config.htm"

namespace JCA {
  namespace IOT {
    typedef std::function<JsonVariant (JsonVariant &)> JsonVariantCallback;
    typedef std::function<void (void)> SimpleCallback;

    class Webserver {
    private:
      // ...Webserver_System.cpp
      char Hostname[80];
      char ConfUser[80];
      char ConfPassword[80];
      char FirmwareVersion[10];
      char FirmwareBuild[10];
      const char *ObjectName = "Webserver";
      bool Reboot;
      WiFiConnect Connector;
      AsyncWebServer Server;
      AsyncWebSocket Websocket;
      uint16_t Port;
      StaticJsonDocument<1000> JsonDoc;
      SimpleCallback onSystemResetCB;
      bool readConfig ();

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

    public:
      // ...Webserver_System.cpp
      Webserver (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword);
      Webserver (const char *_HostnamePrefix, uint16_t _Port);
      Webserver ();
      bool init ();
      bool handle ();
      void onSystemReset(SimpleCallback _CB);

      // ...Webserver_Web.cpp
      void onWebHomeReplace (AwsTemplateProcessor _CB);
      void onWebConfigReplace (AwsTemplateProcessor _CB);

      // ...Webserver_RestApi.cpp
      void onRestApiGet (JsonVariantCallback _CB);
      void onRestApiPost (JsonVariantCallback _CB);
      void onRestApiPut (JsonVariantCallback _CB);
      void onRestApiPatch (JsonVariantCallback _CB);
      void onRestApiDelete (JsonVariantCallback _CB);

      //
    };
  }
}
#endif