/**
 * @file JCA_IOT_WiFiConnect.cpp
 * @brief WiFi Connection Manager
 * It contains the following Moduls
 * - Check Connection State
 * - Create AP if not possible to connect to a WiFi
 * - Check configured WiFi after WatchDog is in AP Mode
 * @version 0.1
 * @date 2022-09-03
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_WiFiConnect.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    /**
     * @brief Construct a new WiFiConnect::WiFiConnect object
     *
     * @param _SsidPrefix SSID Prefix for AP, extend with ChipID
     * @param _Password Password for AP
     * @param _ApIP AP IP-Adress
     * @param _ApGateway AP Gateway-Adress
     * @param _APSubnet AP Subnet-Mask
     */
    WiFiConnect::WiFiConnect (const char *_SsidPrefix, const char *_Password, const char *_ApIP, const char *_ApGateway, const char *_ApSubnet) : Config (10000) {
#ifdef ESP8266
      sprintf (ApSsid, "%s_%08X", _SsidPrefix, ESP.getChipId ());
#elif ESP32
      uint32_t ESP32ChipId = 0;
      for (int i = 0; i < 17; i = i + 8) {
        ESP32ChipId |= ((ESP.getEfuseMac () >> (40 - i)) & 0xff) << i;
      }
      sprintf (ApSsid, "%s_%08X", _SsidPrefix, ESP32ChipId);
#endif
      strncpy (ApPassword, _Password, sizeof (ApPassword));
      ApIP.fromString (_ApIP);
      ApSubnet.fromString (_ApSubnet);
      State = Init;
      CurrentConfig = 0;
    }

    /**
     * @brief Construct a new WiFiConnect::WiFiConnect object
     * IP-Settings used default Vaules
     * @param _SsidPrefix SSID Prefix for AP, extend with ChipID
     * @param _Password Password for AP
     */
    WiFiConnect::WiFiConnect (const char *_SsidPrefix, const char *_Password) : WiFiConnect (_SsidPrefix, _Password, JCA_IOT_WIFICONNECT_AP_IP, JCA_IOT_WIFICONNECT_AP_GATEWAY, JCA_IOT_WIFICONNECT_AP_SUBNET) {
    }

    /**
     * @brief Construct a new WiFiConnect::WiFiConnect object
     * IP-Settings used default Vaules
     * WiFi AP Settings used default Values
     */
    WiFiConnect::WiFiConnect () : WiFiConnect (JCA_IOT_WIFICONNECT_DEFAULT_SSID_PREFIX, JCA_IOT_WIFICONNECT_DEFAULT_PASSWORD) {
    }

    /**
     * @brief Destroy the WiFiConnect::WiFiConnect object
     *
     */
    WiFiConnect::~WiFiConnect () {
    }

    /**
     * @brief Read the WiFi Config-File and store the Date in Config-Array
     *
     * @return true file and config found
     * @return false any fault
     */
    bool WiFiConnect::readConfig () {
      Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Start");
      //-------------------------------------------------------
      // Read Config File
      //-------------------------------------------------------
      File ConfigFile = LittleFS.open (JCA_IOT_WIFICONNECT_CONFIGPATH, "r");
      if (ConfigFile) {
        Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config File Found");
        DeserializationError Error = deserializeJson (Config, ConfigFile);
        if (!Error) {
          Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Deserialize Done");
          JsonObject InConfig = Config.as<JsonObject> ();
          if (InConfig.containsKey (JCA_IOT_WIFICONNECT_CONFKEY_WIFI)) {
            Configs = InConfig[JCA_IOT_WIFICONNECT_CONFKEY_WIFI].as<JsonArray> ();
            return true;
          }
        } else {
          Debug.print (FLAG_ERROR, true, ObjectName, __func__, "deserializeJson() failed: ");
          Debug.println (FLAG_ERROR, true, ObjectName, __func__, Error.c_str ());
        }
        ConfigFile.close ();
      } else {
        Debug.println (FLAG_ERROR, true, ObjectName, __func__, "Config File NOT found");
      }

      Configs.clear ();
      Config.clear ();
      return false;
    }

    /**
     * @brief initialize the WiFi Connection and pass the data for Station mode
     *
     * @return Index of connected WiFi, Zero when not connected
     */
    uint8_t WiFiConnect::init () {
      handle ();
      return isConnected ();
    }

    /**
     * @brief Handle the Connection State and WatchDogs
     *
     * @return true if connected to AP
     * @return false if in AP-Mode or trying to connect to AP
     */
    bool WiFiConnect::handle () {
      bool DHCP;
      IPAddress IP;
      IPAddress Gateway;
      IPAddress Subnet;
      JsonObject Config;

      switch (State) {
      case Init:
        //-----------------------------
        // Init Connection
        //-----------------------------
        WiFi.persistent (true);
        Debug.println (FLAG_SETUP, true, ObjectName, __func__, "[Init] Started");

        if (isConfigured ()) {
          for (uint8_t ConfIndex = 0; ConfIndex < Configs.size (); ConfIndex++) {
            int ConnectCounter = 0;
            Config = Configs[ConfIndex].as<JsonObject> ();

            // Check IP Settings
            if (Config.containsKey ("dhcp")) {
              DHCP = Config["dhcp"].as<bool> ();
            } else {
              DHCP = false;
            }
            if (!DHCP) {
              Debug.println (FLAG_SETUP, true, ObjectName, __func__, "[Init] Set static IP");
              IP.fromString (Config["ip"].as<const char *> ());
              Gateway.fromString (Config["gateway"].as<const char *> ());
              Subnet.fromString (Config["subnet"].as<const char *> ());
              if (!WiFi.config (IP, Gateway, Subnet)) {
                Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[Init] Static IP failed");
              }
            }

            // Connect to Network
            Debug.print (FLAG_SETUP, true, ObjectName, __func__, "[Init] Connect ");
            WiFi.mode (WIFI_STA);
            WiFi.begin (Config["ssid"].as<const char *> (), Config["password"].as<const char *> ());
            while (WiFi.status () != WL_CONNECTED && ConnectCounter < 10) {
              Debug.print (FLAG_SETUP, true, ObjectName, __func__, "0");
              delay (500);
              ConnectCounter++;
            }

            if (WiFi.status () == WL_CONNECTED) {
              Debug.print (FLAG_SETUP, true, ObjectName, __func__, " DONE : ");
              Debug.println (FLAG_SETUP, true, ObjectName, __func__, WiFi.localIP ().toString ());
              CurrentConfig = ConfIndex;
              State = STA;
              break;
            } else {
              Debug.println (FLAG_SETUP, true, ObjectName, __func__, " FAILED");
            }
          }
        }
        if (isConnected () == 0) {
          Debug.print (FLAG_SETUP, true, ObjectName, __func__, "[Init] Start AP: ");
          Debug.println (FLAG_SETUP, true, ObjectName, __func__, ApSsid);
          WiFi.mode (WIFI_AP);
          WiFi.softAPConfig (ApIP, ApGateway, ApSubnet);
          WiFi.softAP (ApSsid, ApPassword);
          State = AP;
          ReconnectTimer = millis ();
        }
        break;

      case Connect:
        //-----------------------------
        // Connect to new SSID
        //-----------------------------
        Config = Configs[CurrentConfig].as<JsonObject> ();

        // Check IP Settings
        if (Config.containsKey ("dhcp")) {
          DHCP = Config["dhcp"].as<bool> ();
        } else {
          DHCP = false;
        }
        if (!DHCP) {
          Debug.println (FLAG_SETUP, true, ObjectName, __func__, "[Connect] Set static IP");
          IP.fromString (Config["ip"].as<const char *> ());
          Gateway.fromString (Config["gateway"].as<const char *> ());
          Subnet.fromString (Config["subnet"].as<const char *> ());
          if (!WiFi.config (IP, Gateway, Subnet)) {
            Debug.println (FLAG_ERROR, true, ObjectName, __func__, "[Connect] Static IP failed");
          }
        }

        // Connect to Network
        Debug.print (FLAG_SETUP, true, ObjectName, __func__, "[Connect] Connect ");
        WiFi.mode (WIFI_STA);
        WiFi.begin (Config["ssid"].as<const char *> (), Config["password"].as<const char *> ());
        BusyTimer = millis ();
        State = Busy;
        break;

      case Busy:
        //-----------------------------
        // Wait for Connection
        //-----------------------------
        if (WiFi.status () == WL_CONNECTED && WiFi.getMode () == WIFI_STA) {
          Debug.print (FLAG_SETUP, true, ObjectName, __func__, "[Connect] Connect DONE : ");
          Debug.println (FLAG_SETUP, true, ObjectName, __func__, WiFi.localIP ().toString ());
          State = STA;
        } else {
          digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
          if (millis () - BusyTimer > JCA_IOT_WIFICONNECT_DELAY_FAILED) {
            Debug.println (FLAG_SETUP, true, ObjectName, __func__, "[Connect] Connect FAILED");
            if (CurrentConfig >= Configs.size () - 1) {
              State = Failed;
            } else {
              CurrentConfig++;
              State = Connect;
            }
          }
        }
        break;

      case Failed:
        //-----------------------------
        // Connection Failt, setup AP
        //-----------------------------
        Debug.print (FLAG_SETUP, true, ObjectName, __func__, "[Failed] Start AP: ");
        Debug.println (FLAG_SETUP, true, ObjectName, __func__, ApSsid);
        WiFi.mode (WIFI_AP);
        WiFi.softAPConfig (ApIP, ApGateway, ApSubnet);
        WiFi.softAP (ApSsid, ApPassword);
        State = AP;
        ReconnectTimer = millis ();
        break;

      case STA:
        //-----------------------------
        // Connected to STA
        //-----------------------------
        digitalWrite (LED_BUILTIN, HIGH);
        break;

      case AP:
        //-----------------------------
        // Connected to AP
        //-----------------------------
        digitalWrite (LED_BUILTIN, LOW);
        if (WiFi.softAPgetStationNum () > 0) {
          ReconnectTimer = millis ();
        } else if (millis () - ReconnectTimer > JCA_IOT_WIFICONNECT_DELAY_RECONNECT) {
          // Disable AP to change to Station
          WiFi.softAPdisconnect (true);
          WiFi.disconnect ();
          delay (1000);
          // Try to Connect
          BusyTimer = millis ();
          CurrentConfig = 0;
          State = Connect;
        }
        break;
      }
      return isConnected ();
    }

    /**
     * @brief Trigger a reconnect to AP
     *
     * @return true Station alreadxy was connected, reconnect
     * @return false connect to Station in progress
     */
    bool WiFiConnect::doConnect () {
      if (isConfigured ()) {
        State = Connect;
        handle ();
        return true;
      }
      return false;
    }

    /**
     * @brief Check if the Controller is connected to AP
     *
     * @return Index of connected WiFi, Zero when not connected
     */
    uint8_t WiFiConnect::isConnected () {
      if (State == STA) {
        return CurrentConfig + 1;
      } else {
        return 0;
      }
    }

    /**
     * @brief Replace Connection Wildcard in Website
     *
     * @param var Wildcard
     * @return String Replace String
     */
    String WiFiConnect::replaceWildcards (const String &var) {
      if (var == "NAME") {
        return F ("WiFi Connect");
      }
      if (var == "STYLE") {
        return F (":root{--ColorWiFi:var(--contrast)}");
      }
      return String ();
    }

    bool WiFiConnect::isConfigured () {
      return Configs.size () > 0;
    }
  } // namespace IOT
} // namespace JCA