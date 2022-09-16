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
    WiFiConnect::WiFiConnect (const char *_SsidPrefix, const char *_Password, const char *_ApIP, const char *_ApGateway, const char *_APSubnet) {
      sprintf (ApSsid, "%s_%08X", _SsidPrefix, ESP.getChipId ());
      strncpy (ApPassword, _Password, sizeof (ApPassword));
      ApIP.fromString (_ApIP);
//      ApGateway.fromString (_ApGateway);
//      ApGateway.fromString ("0.0.0.0");
      ApSubnet.fromString (_APSubnet);
      State = Init;
      DHCP = true;
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
     * @brief Set the Station SSID
     * 
     * @param _Ssid SSID auf the Ap to connect to
     * @return true SSID is valid
     * @return false SSID is invalid
     */
    bool WiFiConnect::setSsid (const char *_Ssid) {
      if (_Ssid == NULL) {
        return false;
      } else if (strlen (_Ssid) > sizeof (Ssid)) {
        strncpy (Ssid, _Ssid, sizeof (Ssid));
        return false;
      } else {
        strcpy (Ssid, _Ssid);
        return true;
      }
    }

    /**
     * @brief Set the Station Password
     *
     * @param _Password Password auf the Ap to connect to
     * @return true Password is valid
     * @return false Password is invalid
     */
    bool WiFiConnect::setPassword (const char *_Password) {
      if (_Password == NULL) {
        return false;
      } else if (strlen (_Password) > sizeof (Password)) {
        strncpy (Password, _Password, sizeof (Password));
        return false;
      } else {
        strcpy (Password, _Password);
        return true;
      }
    }

    /**
     * @brief Set the Station IP
     * 
     * @param _IP IP-Address if Station use fix Address
     * @return true IP is valid
     * @return false IP is invalid
     */
    bool WiFiConnect::setIP (const char *_IP) {
      if (_IP == NULL) {
        return false;
      } else if (IP.fromString (_IP)) {
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Set the Station Gateway
     * 
     * @param _Gateway Gateway-Address if Station use fix Address
     * @return true Gateway is valid
     * @return false Dateway is invalid
     */
    bool WiFiConnect::setGateway (const char *_Gateway) {
      if (_Gateway == NULL) {
        return false;
      } else if (Gateway.fromString (_Gateway)) {
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Set the Station Subnet-Mask
     * 
     * @param _Subnet Subnet-Mask if Station use fix Address
     * @return true Subnet-Mask is valid
     * @return false Subnet-Mask is invalid
     */
    bool WiFiConnect::setSubnet (const char *_Subnet) {
      if (_Subnet == NULL) {
        return false;
      } else if (Subnet.fromString (_Subnet)) {
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Set the Station DHCP mode
     * 
     * @param _DHCP use DHCP or fix Address
     * @return true always 
     * @return false never
     */
    bool WiFiConnect::setDHCP (bool _DHCP) {
      DHCP = _DHCP;
      return true;
    }

    /**
     * @brief initialize the WiFi Connection and pass the data for Station mode
     * 
     * @param _Ssid SSID of remote AP
     * @param _Password Password of remote AP
     * @param _IP IP if use fix Address
     * @param _Gateway Gateway if use fix Address
     * @param _Subnet Subnet if use fix Address
     * @param _DHCP use DHCP or fix Address
     * @return true Data is valid and Station ist connected to AP
     * @return false Data is invalid or can't connect to AP
     */
    bool WiFiConnect::init (const char *_Ssid, const char *_Password, const char *_IP, const char *_Gateway, const char *_Subnet, bool _DHCP) {
      const char *FunctionName = "init";
      Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "Started");
      bool DataValid = true;
      if (!setSsid (_Ssid)) {
        DataValid = false;
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "SSID Invalid");
      }
      if (!setPassword (_Password)) {
        DataValid = false;
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "Pasword Invalid");
      }
      if (!setIP (_IP)) {
        DataValid = false;
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "IP Invalid");
      }
      if (!setGateway (_Gateway)) {
        DataValid = false;
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "Gateway Invalid");
      }
      if (!setSubnet (_Subnet)) {
        DataValid = false;
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "Subnet Invalid");
      }
      if (!setDHCP (_DHCP)) {
        DataValid = false;
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "DHCP Invalid");
      }
      if (DataValid) {
        handle ();
        return isConnected ();
      } else {
        return false;
      }
    }

    /**
     * @brief WiFi Connection data unknown, not connection to AP
     * Connection Data has to bee passed later.
     * @return true always
     * @return false never
     */
    bool WiFiConnect::init () {
      const char *FunctionName = "init";
      Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "Not initialized");
      return false;
    }

    /**
     * @brief Handle the Connection State and WatchDogs
     * 
     * @return true if connected to AP
     * @return false if in AP-Mode or trying to connect to AP
     */
    bool WiFiConnect::handle () {
      const char *FunctionName = "handle";
      switch (State) {
      case Init:
        //-----------------------------
        // Init Connection
        //-----------------------------
        WiFi.persistent (true);
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "[Init] Started");
        if (isConfigured ()) {
          int ConnectCounter = 0;
          // Set static IP
          if (!DHCP) {
            Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "[Init] Set static IP");
            if (!WiFi.config (IP, Gateway, Subnet)) {
              Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[Init] Static IP failed");
            }
          }

          // Connect to Network
          Debug.print (FLAG_SETUP, true, ObjectName, FunctionName, "[Init] Connect ");
          WiFi.mode (WIFI_STA);
          WiFi.begin (Ssid, Password);
          while (WiFi.status () != WL_CONNECTED && ConnectCounter < 10) {
            Debug.print (FLAG_SETUP, true, ObjectName, FunctionName, "0");
            delay (500);
            ConnectCounter++;
          }

          if (WiFi.status () == WL_CONNECTED) {
            Debug.print (FLAG_SETUP, true, ObjectName, FunctionName, " DONE : ");
            Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, WiFi.localIP().toString());
            State = STA;
          } else {
            Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, " FAILED");
            BusyTimer = millis ();
            State = Failed;
          }
        } else {
          BusyTimer = millis ();
          State = Failed;
        }
        break;

      case Connect:
        //-----------------------------
        // Connect to new SSID
        //-----------------------------
        // Disable AP to change to Station
        WiFi.softAPdisconnect (true);
        WiFi.disconnect ();
        delay (1000);

        // Set static IP
        if (!DHCP) {
          Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "[Connect] Set static IP");
          if (!WiFi.config (IP, Gateway, Subnet)) {
            Debug.println (FLAG_ERROR, true, ObjectName, FunctionName, "[Connect] Static IP failed");
          }
        }

        // Connect to Network
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "[Connect] Connect ");
        WiFi.mode (WIFI_STA);
        WiFi.begin (Ssid, Password);
        BusyTimer = millis ();
        State = Busy;
        break;

      case Busy:
        //-----------------------------
        // Wait for Connection
        //-----------------------------
        if (WiFi.status () == WL_CONNECTED && WiFi.getMode () == WIFI_STA) {
          Debug.print (FLAG_SETUP, true, ObjectName, FunctionName, "[Connect] Connect DONE : ");
          Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, WiFi.localIP ().toString ());
          State = STA;
        } else {
          digitalWrite (LED_BUILTIN, !digitalRead (LED_BUILTIN));
          if (millis () - BusyTimer > JCA_IOT_WIFICONNECT_DELAY_FAILED) {
            Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, "[Connect] Connect FAILED");
            State = Failed;
          }
        }
        break;

      case Failed:
        //-----------------------------
        // Connection Failt, setup AP
        //-----------------------------
        Debug.print (FLAG_SETUP, true, ObjectName, FunctionName, "[Failed] Start AP: ");
        Debug.println (FLAG_SETUP, true, ObjectName, FunctionName, ApSsid);
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
          BusyTimer = millis ();
          State = Connect;
        }
        break;
      }
      return isConnected();
    }

    /**
     * @brief Trigger a reconnect to AP
     * 
     * @return true Station alreadxy was connected, reconnect
     * @return false connect to Station in progress
     */
    bool WiFiConnect::doConnect() {
      if (isConfigured ()) {
        State = Connect;
        handle ();
        return true;
      }
      return false;
    }
    
    /**
     * @brief Check if the Station Config is valid
     * 
     * @return true valid
     * @return false invalid
     */
    bool WiFiConnect::isConfigured () {
      if (Ssid == NULL) {
        return false;
      }
      if (Password == NULL) {
        return false;
      }
      if (!DHCP) {
        if (!IP.isSet ()) {
          return false;
        }
        if (!Gateway.isSet ()) {
          return false;
        }
        if (!Subnet.isSet ()) {
          return false;
        }
      }
      return true;
    }

    /**
     * @brief Check if the Controller is connected to AP
     * 
     * @return true connected
     * @return false disconnected
     */
    bool WiFiConnect::isConnected () {
      return State == STA;
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
      if (var == "SSID") {
        return String (Ssid);
      }
      if (var == "DHCP" && DHCP) {
        return "checked";
      }
      if (var == "IP") {
        return IP.toString ();
      }
      if (var == "GATEWAY") {
        return Gateway.toString ();
      }
      if (var == "SUBNET") {
        return Gateway.toString ();
      }
      if (var == "STYLE") {
        return F (":root{--ColorWiFi:var(--contrast)}");
      }
      return String();
    }

  } // namespace IOT
} // namespace JCA