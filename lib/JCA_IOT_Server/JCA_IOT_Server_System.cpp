/**
 * @file JCA_IOT_Webserver_System.cpp
 * @author JCA (https://github.com/ichok)
 * @brief System-Functions of the Server
 * @version 0.1
 * @date 2022-09-07
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Server.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    const char *Server::ElementName = "System";

    /**
     * @brief Construct a new Server::Server object
     *
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     * @param _ConfUser Username for the System Sites
     * @param _ConfPassword Password for the System Sites
     * @param _Offset RTC Timeoffset in seconds
     */
    Server::Server (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword, unsigned long _Offset)
        : WebServerObject (_Port), WebSocketObject ("/ws"), Rtc (_Offset) {
      Debug.println (FLAG_SETUP, false, ObjectName, __func__, "Create");

      String ChipID;
#ifdef ESP8266
      ChipID = String (ESP.getChipId (), 16);
#elif ESP32
      uint32_t ESP32ChipId = 0;
      for (int i = 0; i < 17; i = i + 8) {
        ESP32ChipId |= ((ESP.getEfuseMac () >> (40 - i)) & 0xff) << i;
      }
      ChipID = String (ESP32ChipId, 16);
#endif
      Hostname = String (_HostnamePrefix) + "_";

      for (int i = ChipID.length (); i < 8; i++) {
        Hostname += "0";
      }
      Hostname += ChipID;
      Port = _Port;
      Reboot = false;
      strncpy (ConfUser, _ConfUser, sizeof (ConfUser));
      strncpy (ConfPassword, _ConfPassword, sizeof (ConfPassword));
      WsUpdateCycle = 1000;
      WsLastUpdate = millis ();
      WebConfigFile = "/usrFunctions.json";
    }

    /**
     * @brief Construct a new Server::Server object
     * Username and Password for System Sites is set to Default
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     * @param _ConfUser Username for the System Sites
     * @param _ConfPassword Password for the System Sites
     */
    Server::Server (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword) : Server (_HostnamePrefix, _Port, _ConfUser, _ConfPassword, JCA_IOT_SERVER_TIME_OFFSET) {
    }

    /**
     * @brief Construct a new Server::Server object
     * Username and Password for System Sites is set to Default
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     * @param _Offset RTC Timeoffset in seconds
     */
    Server::Server (const char *_HostnamePrefix, uint16_t _Port, unsigned long _Offset) : Server (_HostnamePrefix, _Port, JCA_IOT_SERVER_DEFAULT_CONF_USER, JCA_IOT_SERVER_DEFAULT_CONF_PASS, _Offset) {
    }

    /**
     * @brief Construct a new Server::Server object
     * Username and Password for System Sites is set to Default
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     */
    Server::Server (const char *_HostnamePrefix, uint16_t _Port) : Server (_HostnamePrefix, _Port, JCA_IOT_SERVER_DEFAULT_CONF_USER, JCA_IOT_SERVER_DEFAULT_CONF_PASS) {
    }

    /**
     * @brief Construct a new Server::Server object
     * Username and Password for System Sites is set to Default
     * Hostname-Prefix and Webserver-Port ist set to Default
     * @param _Offset RTC Timeoffset in seconds
     */
    Server::Server (unsigned long _Offset) : Server (JCA_IOT_SERVER_DEFAULT_HOSTNAMEPREFIX, JCA_IOT_SERVER_DEFAULT_PORT, _Offset) {
    }

    /**
     * @brief Construct a new Server::Server object
     * Username and Password for System Sites is set to Default
     * Hostname-Prefix and Webserver-Port ist set to Default
     */
    Server::Server () : Server (JCA_IOT_SERVER_DEFAULT_HOSTNAMEPREFIX, JCA_IOT_SERVER_DEFAULT_PORT) {
    }

    /**
     * @brief Read the Config-JSON and store the Data
     *
     * @return true Config-File exists and is valid
     * @return false Config-File didn't exists or is not a valid JSON File
     */
    bool Server::readConfig () {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Read");
      JsonDocument JsonDoc;
      // Get Wifi-Config from File5
      File ConfigFile = LittleFS.open (JCA_IOT_SERVER_CONFIGPATH, "r");
      if (ConfigFile) {
        Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config File Found");
        DeserializationError Error = deserializeJson (JsonDoc, ConfigFile);
        if (!Error) {
          Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Deserialize Done");
          JsonObject Config = JsonDoc.as<JsonObject> ();
          //------------------------------------------------------
          // Read WiFi Config
          //------------------------------------------------------
          if (Config[JCA_IOT_SERVER_CONFKEY_WIFI].is<JsonObject> ()) {
            Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config contains WiFi");
            JsonObject WiFiConfig = Config[JCA_IOT_SERVER_CONFKEY_WIFI].as<JsonObject> ();
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_SSID].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found SSID");
              if (!Connector.setSsid (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_SSID].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] SSID invalid");
              }
            }
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_PASS].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found Password");
              if (!Connector.setPassword (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_PASS].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] Password invalid");
              }
            }
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_DHCP].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found DHCP");
              if (!Connector.setDHCP (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_DHCP].as<bool> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] DHCP invalid");
              }
            }
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_IP].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found IP");
              if (!Connector.setIP (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_IP].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] IP invalid");
              }
            }
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_GATEWAY].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found Gateway");
              if (!Connector.setGateway (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_GATEWAY].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] Gateway invalid");
              }
            }
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_SUBNET].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found Subnet");
              if (!Connector.setSubnet (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_SUBNET].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] Subnet invalid");
              }
            }
            if (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_STATPIN].is<JsonVariant> ()) {
              Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "[WiFi] Found State Pin");
              if (!Connector.setStatePin (WiFiConfig[JCA_IOT_SERVER_CONFKEY_WIFI_STATPIN].as<int8_t> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[WiFi] State Pin invalid");
              }
            }
          }
          //------------------------------------------------------
          // Read Server Config
          //------------------------------------------------------
          if (Config[JCA_IOT_SERVER_CONFKEY_HOSTNAME].is<JsonVariant>()) {
            Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config contains Hostname");
            Hostname = Config[JCA_IOT_SERVER_CONFKEY_HOSTNAME].as<String> ();
          }
          if (Config[JCA_IOT_SERVER_CONFKEY_PORT].is<JsonVariant> ()) {
            Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config contains Serverport");
            Port = Config[JCA_IOT_SERVER_CONFKEY_PORT].as<uint16_t> ();
          }
          if (Config[JCA_IOT_SERVER_CONFKEY_SOCKETUPDATE].is<JsonVariant>()) {
            Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config contains WebSocket Update");
            WsUpdateCycle = Config[JCA_IOT_SERVER_CONFKEY_SOCKETUPDATE].as<uint32_t> ();
          }

        } else {
          Debug.print (FLAG_ERROR, true, ObjectName, __func__, "deserializeJson() failed: ");
          Debug.println (FLAG_ERROR, true, ObjectName, __func__, Error.c_str ());
          return false;
        }
        ConfigFile.close ();
      } else {
        Debug.println (FLAG_ERROR, true, ObjectName, __func__, "Config File NOT found");
        return false;
      }
      return true;
    }

    /**
     * @brief Define all Default Web-Requests and init the Server
     *
     * @return true Controller is connected to WiFI
     * @return false Controller runs as AP
     */
    bool Server::init () {
      Debug.println (FLAG_SETUP, false, ObjectName, __func__, "Init");
      // Read Config
      readConfig ();

      // WiFi Connection
      Connector.init ();

      // WebSocket - Init
      WebSocketObject.onEvent ([this] (AsyncWebSocket *_Server, AsyncWebSocketClient *_Client, AwsEventType _Type, void *_Arg, uint8_t *_Data, size_t _Len) { this->onWsEvent (_Server, _Client, _Type, _Arg, _Data, _Len); });
      WebServerObject.addHandler (&WebSocketObject);

      // Server - WiFi Config
      WebServerObject.on (JCA_IOT_SERVER_PATH_CONNECT, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebConnectGet (_Request); });
      WebServerObject.on (JCA_IOT_SERVER_PATH_CONNECT, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { this->onWebConnectPost (_Request); });

      // Server - System Config
      WebServerObject.on (JCA_IOT_SERVER_PATH_SYS, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebSystemGet (_Request); });
      WebServerObject.on (
          JCA_IOT_SERVER_PATH_SYS_UPLOAD, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { _Request->redirect (JCA_IOT_SERVER_PATH_SYS); },
          [this] (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) { this->onWebSystemUploadData (_Request, _Filename, _Index, _Data, _Len, _Final); });
      WebServerObject.on (
          JCA_IOT_SERVER_PATH_SYS_UPDATE, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { this->onWebSystemUpdate (_Request); },
          [this] (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) { this->onWebSystemUpdateData (_Request, _Filename, _Index, _Data, _Len, _Final); });
      WebServerObject.on (JCA_IOT_SERVER_PATH_SYS_RESET, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { this->onWebSystemReset (_Request); });

      // Server - Custom Pages
      WebServerObject.on ("/", HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebHomeGet (_Request); });
      WebServerObject.on (JCA_IOT_SERVER_PATH_HOME, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebHomeGet (_Request); });
      WebServerObject.on (JCA_IOT_SERVER_PATH_CONFIG, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebConfigGet (_Request); });

      // Save WebConfig
      WebServerObject.on (JCA_IOT_SERVER_PATH_CONFIGSAVE, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onSaveConfigCB(); this->onWebConfigGet (_Request); });

      // RestAPI
      WebServerObject.on (
          "/api", HTTP_ANY,
          [this] (AsyncWebServerRequest *_Request) {
            Debug.println (FLAG_TRAFFIC, true, this->ObjectName, "RestAPI", "Request");
            JsonDocument JBuffer;
            JsonVariant InData;

            DeserializationError Error = deserializeJson (JBuffer, (char *)(_Request->_tempObject));
            if (Error) {
              if (Debug.print (FLAG_ERROR, true, ObjectName, "RestAPI", "+ deserializeJson() failed: ")) {
                Debug.println (FLAG_ERROR, true, ObjectName, "RestAPI", Error.c_str ());
                Debug.print (FLAG_ERROR, true, ObjectName, "RestAPI", "+ Body:");
                Debug.println (FLAG_ERROR, true, ObjectName, "RestAPI", (char *)(_Request->_tempObject));
              }
              JBuffer.clear ();
            }
            InData = JBuffer.as<JsonVariant> ();
            this->onRestApiRequest (_Request, InData);
          },
          [this] (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) {
            Debug.println (FLAG_TRAFFIC, true, this->ObjectName, "RestAPI", "File");
          },
          [this] (AsyncWebServerRequest *_Request, uint8_t *_Data, size_t _Len, size_t _Index, size_t _Total) {
            Debug.println (FLAG_TRAFFIC, true, this->ObjectName, "RestAPI", "Data");

            if (_Total > 0 && _Request->_tempObject == nullptr) {
              _Request->_tempObject = malloc (_Total + 10);
            }
            if (_Request->_tempObject != nullptr) {
              memcpy ((uint8_t *)(_Request->_tempObject) + _Index, _Data, _Len);
            }
          });

      // Server - If not defined
      WebServerObject.serveStatic ("/", LittleFS, "/")
          .setDefaultFile (JCA_IOT_SERVER_PATH_HOME);
      WebServerObject.onNotFound ([] (AsyncWebServerRequest *_Request) { _Request->redirect (JCA_IOT_SERVER_PATH_SYS); });
      WebServerObject.begin ();

      Debug.println (FLAG_SETUP, true, ObjectName, __func__, "Done");
      return Connector.isConnected ();
    }

    /**
     * @brief Loop Function to handle Connection State
     *
     * @return true Controller is connected to WiFI
     * @return false Controller runs as AP
     */
    bool Server::handle () {
      Debug.println (FLAG_LOOP, false, ObjectName, __func__, "Run");
      unsigned long ActMillis = millis ();
      // Update Cycle WebSocket
      if (ActMillis - WsLastUpdate >= WsUpdateCycle && WsUpdateCycle > 0) {
        doWsUpdate (nullptr);
        WsLastUpdate = ActMillis;
      }
      // Check WiFi Connection
      Connector.handle ();
      return Connector.isConnected ();
    }

    void Server::onSystemReset (SimpleCallback _CB) {
      onSystemResetCB = _CB;
    }

    void Server::onSaveConfig (SimpleCallback _CB) {
      onSaveConfigCB = _CB;
    }

    void Server::setTime (unsigned long _Epoch, int _Millis) {
      Rtc.setTime (_Epoch, _Millis);
    }
    void Server::setTime (int _Second, int _Minute, int _Hour, int _Day, int _Month, int _Year, int _Millis) {
      Rtc.setTime (_Second, _Minute, _Hour, _Day, _Month, _Year, _Millis);
    }
    void Server::setStatePin (int8_t _Pin) {
      Connector.setStatePin (_Pin);
    }

    void Server::setTimeStruct (tm _Time) {
      Rtc.setTimeStruct (_Time);
    }
    void Server::setWebConfigFile (String _WebConfigFile) {
      WebConfigFile = _WebConfigFile;
    }
    bool Server::timeIsValid () {
      return Rtc.getEpoch () > JCA_IOT_SERVER_TIME_VALID;
    }
    tm Server::getTimeStruct () {
      return Rtc.getTimeStruct ();
    }
    String Server::getTime () {
      return Rtc.getTime ();
    }
    String Server::getDate () {
      return Rtc.getTime (JCA_IOT_SERVER_TIME_DATEFORMAT);
    }
    String Server::getTimeString (String _Format) {
      if (_Format.length () == 0) {
        return Rtc.getTime (JCA_IOT_SERVER_TIME_TIMEFORMAT);
      } else {
        return Rtc.getTime (_Format);
      }
    }
  }
}