/**
 * @file JCA_IOT_Webserver_System.cpp
 * @author JCA (https://github.com/ichok)
 * @brief System-Functions of the Webserver
 * @version 0.1
 * @date 2022-09-07
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Webserver.h>

namespace JCA {
  namespace IOT {
    /**
     * @brief Construct a new Webserver::Webserver object
     *
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     * @param _ConfUser Username for the System Sites
     * @param _ConfPassword Password for the System Sites
     * @param _Offset RTC Timeoffset in seconds
     */
    Webserver::Webserver (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword, unsigned long _Offset)
        : Server (_Port), Websocket ("/ws"), Rtc (_Offset) {
      sprintf (Hostname, "%s_%08X", _HostnamePrefix, ESP.getChipId ());
      Port = _Port;
      Reboot = false;
      strncpy (ConfUser, _ConfUser, sizeof (ConfUser));
      strncpy (ConfPassword, _ConfPassword, sizeof (ConfPassword));
    }

    /**
     * @brief Construct a new Webserver::Webserver object
     * Username and Password for System Sites is set to Default
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     * @param _ConfUser Username for the System Sites
     * @param _ConfPassword Password for the System Sites
     */
    Webserver::Webserver (const char *_HostnamePrefix, uint16_t _Port, const char *_ConfUser, const char *_ConfPassword) : Webserver (_HostnamePrefix, _Port, _ConfUser, _ConfPassword, JCA_IOT_WEBSERVER_TIME_OFFSET) {
    }

    /**
     * @brief Construct a new Webserver::Webserver object
     * Username and Password for System Sites is set to Default
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     * @param _Offset RTC Timeoffset in seconds
     */
    Webserver::Webserver (const char *_HostnamePrefix, uint16_t _Port, unsigned long _Offset) : Webserver (_HostnamePrefix, _Port, JCA_IOT_WEBSERVER_DEFAULT_CONF_USER, JCA_IOT_WEBSERVER_DEFAULT_CONF_PASS, _Offset) {
    }

    /**
     * @brief Construct a new Webserver::Webserver object
     * Username and Password for System Sites is set to Default
     * @param _HostnamePrefix Prefix for default Hostname ist not defined in Config
     * @param _Port Port of the Webserver if not defined in Config
     */
    Webserver::Webserver (const char *_HostnamePrefix, uint16_t _Port) : Webserver (_HostnamePrefix, _Port, JCA_IOT_WEBSERVER_DEFAULT_CONF_USER, JCA_IOT_WEBSERVER_DEFAULT_CONF_PASS) {
    }

    /**
     * @brief Construct a new Webserver::Webserver object
     * Username and Password for System Sites is set to Default
     * Hostname-Prefix and Webserver-Port ist set to Default
     * @param _Offset RTC Timeoffset in seconds
     */
    Webserver::Webserver (unsigned long _Offset) : Webserver (JCA_IOT_WEBSERVER_DEFAULT_HOSTNAMEPREFIX, JCA_IOT_WEBSERVER_DEFAULT_PORT, _Offset) {
    }

    /**
     * @brief Construct a new Webserver::Webserver object
     * Username and Password for System Sites is set to Default
     * Hostname-Prefix and Webserver-Port ist set to Default
     */
    Webserver::Webserver () : Webserver (JCA_IOT_WEBSERVER_DEFAULT_HOSTNAMEPREFIX, JCA_IOT_WEBSERVER_DEFAULT_PORT) {
    }

    /**
     * @brief Read the Config-JSON and store the Data
     *
     * @return true Config-File exists and is valid
     * @return false Config-File didn't exists or is not a valid JSON File
     */
    bool Webserver::readConfig () {
      const char *FunctionName = "readConfig";
      // Get Wifi-Config from File5
      File ConfigFile = LittleFS.open (JCA_IOT_WEBSERVER_CONFIGPATH, "r");
      ConfigFile.sendAll (Serial);
      if (ConfigFile) {
        Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "Config File Found");
        Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, ConfigFile.readString ());
        DeserializationError Error = deserializeJson (JsonDoc, ConfigFile);
        if (!Error) {
          Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "Deserialize Done");
          JsonObject Config = JsonDoc.as<JsonObject> ();
          if (Config.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI)) {
            Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "Config contains WiFi");
            JsonObject WiFiConfig = Config[JCA_IOT_WEBSERVER_CONFKEY_WIFI].as<JsonObject> ();
            // Get Network Config
            if (WiFiConfig.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI_SSID)) {
              Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "[WiFi] Found SSID");
              if (!Connector.setSsid (WiFiConfig[JCA_IOT_WEBSERVER_CONFKEY_WIFI_SSID].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[WiFi] SSID invalid");
              }
            }
            if (WiFiConfig.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI_PASS)) {
              Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "[WiFi] Found Password");
              if (!Connector.setPassword (WiFiConfig[JCA_IOT_WEBSERVER_CONFKEY_WIFI_PASS].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[WiFi] Password invalid");
              }
            }
            if (WiFiConfig.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI_IP)) {
              Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "[WiFi] Found IP");
              if (!Connector.setIP (WiFiConfig[JCA_IOT_WEBSERVER_CONFKEY_WIFI_IP].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[WiFi] IP invalid");
              }
            }
            if (WiFiConfig.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI_GATEWAY)) {
              Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "[WiFi] Found Gateway");
              if (!Connector.setGateway (WiFiConfig[JCA_IOT_WEBSERVER_CONFKEY_WIFI_GATEWAY].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[WiFi] Gateway invalid");
              }
            }
            if (WiFiConfig.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI_SUBNET)) {
              Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "[WiFi] Found Subnet");
              if (!Connector.setSubnet (WiFiConfig[JCA_IOT_WEBSERVER_CONFKEY_WIFI_SUBNET].as<const char *> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[WiFi] Subnet invalid");
              }
            }
            if (WiFiConfig.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI_DHCP)) {
              Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "[WiFi] Found DHCP");
              if (!Connector.setDHCP (WiFiConfig[JCA_IOT_WEBSERVER_CONFKEY_WIFI_DHCP].as<bool> ())) {
                Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[WiFi] DHCP invalid");
              }
            }
          }

          if (Config.containsKey (JCA_IOT_WEBSERVER_CONFKEY_HOSTNAME)) {
            Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "Config contains Hostname");
            strncpy (Hostname, Config[JCA_IOT_WEBSERVER_CONFKEY_HOSTNAME].as<const char *> (), sizeof (Hostname));
          }

          if (Config.containsKey (JCA_IOT_WEBSERVER_CONFKEY_PORT)) {
            Debug.println (FLAG_CONFIG, true, ObjectName, FunctionName, "Config contains Serverport");
            Port = Config[JCA_IOT_WEBSERVER_CONFKEY_PORT].as<uint16_t> ();
          }
        } else {
          Debug.print (FLAG_ERROR, true, ObjectName, FunctionName, "deserializeJson() failed: ");
          Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, Error.c_str ());
          return false;
        }
        ConfigFile.close ();
      } else {
        Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "Config File NOT found");
        return false;
      }
      return true;
    }

    /**
     * @brief Define all Default Web-Requests and init the Webserver
     *
     * @return true Controller is connected to WiFI
     * @return false Controller runs as AP
     */
    bool Webserver::init () {
      const char *FunctionName = "init";
      // WiFi Connection - Init First
      Connector.init ();

      // WebSocket - Init
      // Server.addHandle(&Websocket);

      // Webserver - WiFi Config
      Server.on (JCA_IOT_WEBSERVER_PATH_CONNECT, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebConnectGet (_Request); });
      Server.on (JCA_IOT_WEBSERVER_PATH_CONNECT, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { this->onWebConnectPost (_Request); });
      if (Connector.isConnected ()) {
        Server.on ("/", HTTP_GET, [] (AsyncWebServerRequest *request) { request->redirect (JCA_IOT_WEBSERVER_PATH_HOME); });
      } else {
        Server.on ("/", HTTP_GET, [] (AsyncWebServerRequest *request) { request->redirect (JCA_IOT_WEBSERVER_PATH_CONNECT); });
      }

      // Webserver - System Config
      Server.on (JCA_IOT_WEBSERVER_PATH_SYS, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebSystemGet (_Request); });
      Server.on (
          JCA_IOT_WEBSERVER_PATH_SYS_UPLOAD, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { _Request->redirect (JCA_IOT_WEBSERVER_PATH_SYS); },
          [this] (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) { this->onWebSystemUploadData (_Request, _Filename, _Index, _Data, _Len, _Final); });
      Server.on (
          JCA_IOT_WEBSERVER_PATH_SYS_UPDATE, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { this->onWebSystemUpdate (_Request); },
          [this] (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) { this->onWebSystemUpdateData (_Request, _Filename, _Index, _Data, _Len, _Final); });
      Server.on (JCA_IOT_WEBSERVER_PATH_SYS_RESET, HTTP_POST, [this] (AsyncWebServerRequest *_Request) { this->onWebSystemReset (_Request); });

      // Webserver - Custom Pages
      Server.on (JCA_IOT_WEBSERVER_PATH_HOME, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebHomeGet (_Request); });
      Server.on (JCA_IOT_WEBSERVER_PATH_CONFIG, HTTP_GET, [this] (AsyncWebServerRequest *_Request) { this->onWebConfigGet (_Request); });

      // RestAPI
      Server.on (
          "/api", HTTP_ANY,
          [this] (AsyncWebServerRequest *_Request) {
            Debug.println (FLAG_TRAFFIC, true, this->ObjectName, "RestAPI", "Request");
            DynamicJsonDocument jsonBuffer (1000);
            JsonVariant InData;

            DeserializationError Error = deserializeJson (jsonBuffer, (char *)(_Request->_tempObject));
            if (Error) {
              if (Debug.print (FLAG_ERROR, true, ObjectName, "RestAPI", "+ deserializeJson() failed: ")) {
                Debug.println (FLAG_ERROR, true, ObjectName, "RestAPI", Error.c_str ());
                Debug.print (FLAG_ERROR, true, ObjectName, "RestAPI", "+ Body:");
                Debug.println (FLAG_ERROR, true, ObjectName, "RestAPI", (char *)(_Request->_tempObject));
              }
              jsonBuffer.clear ();
            }
            InData = jsonBuffer.as<JsonVariant> ();
            this->onRestApiRequest (_Request, InData);
          },
          [this] (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) {
            Debug.println (FLAG_TRAFFIC, true, this->ObjectName, "RestAPI", "File");
          },
          [this] (AsyncWebServerRequest *_Request, uint8_t *_Data, size_t _Len, size_t _Index, size_t _Total) {
            Debug.println (FLAG_TRAFFIC, true, this->ObjectName, "RestAPI", "Data");

            if (_Total > 0 && _Request->_tempObject == NULL) {
              _Request->_tempObject = malloc (_Total);
            }
            if (_Request->_tempObject != NULL) {
              memcpy ((uint8_t *)(_Request->_tempObject) + _Index, _Data, _Len);
            }
          });

      // Webserver - If not defined
      Server.serveStatic ("/", LittleFS, "/")
          .setDefaultFile (JCA_IOT_WEBSERVER_PATH_HOME);
      Server.onNotFound ([] (AsyncWebServerRequest *_Request) { _Request->send (404); });
      Server.begin ();

      Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "Done");
      return Connector.isConnected ();
    }

    /**
     * @brief Loop Function to handle Connection State
     *
     * @return true Controller is connected to WiFI
     * @return false Controller runs as AP
     */
    bool Webserver::handle () {
      Connector.handle ();
      return Connector.isConnected ();
    }

    void Webserver::onSystemReset (SimpleCallback _CB) {
      onSystemResetCB = _CB;
    }

    void Webserver::setTime (unsigned long _Epoch, int _Millis) {
      Rtc.setTime (_Epoch, _Millis);
    }
    void Webserver::setTime (int _Second, int _Minute, int _Hour, int _Day, int _Month, int _Year, int _Millis) {
      Rtc.setTime (_Second, _Minute, _Hour, _Day, _Month, _Year, _Millis);
    }
    void Webserver::setTimeStruct (tm _Time) {
      Rtc.setTimeStruct (_Time);
    }
    bool Webserver::timeIsValid () {
      return Rtc.getEpoch () > JCA_IOT_WEBSERVER_TIME_VALID;
    }
    tm Webserver::getTimeStruct () {
      return Rtc.getTimeStruct ();
    }
    String Webserver::getTime () {
      return Rtc.getTime ();
    }
    String Webserver::getDate (){
      return Rtc.getTime (JCA_IOT_WEBSERVER_TIME_DATEFORMAT);
    }
    String Webserver::getTimeString (String _Format) {
      if (_Format.length() == 0){
        return Rtc.getTime (JCA_IOT_WEBSERVER_TIME_TIMEFORMAT);
      } else {
        return Rtc.getTime (_Format);
      }
    }
  }
}