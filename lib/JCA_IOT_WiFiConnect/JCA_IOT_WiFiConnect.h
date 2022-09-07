/**
 * @file JCA_IOT_WiFiConnect.h
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
#ifndef _JCA_IOT_WIFICONNECT_
#define _JCA_IOT_WIFICONNECT_
#include "FS.h"
#include <Arduino.h>
#define SPIFFS LittleFS
#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
#elif ESP32
  #include <ESPmDNS.h>
  #include <FS.h>
  #include <SPIFFS.h>
  #include <WiFi.h>
#endif
#include <JCA_IOT_DebugOut.h>

// Default Config if not passt other Data to Contructor
#define JCA_IOT_WIFICONNECT_DEFAULT_SSID_PREFIX "JCA_IOT"
#define JCA_IOT_WIFICONNECT_DEFAULT_PASSWORD "JCA_IOT_DEFAULTAP"
#define JCA_IOT_WIFICONNECT_AP_IP "192.168.1.1"
#define JCA_IOT_WIFICONNECT_AP_GATEWAY "192.168.1.1"
#define JCA_IOT_WIFICONNECT_AP_SUBNET "255.255.255.0"
// Watchdog Timer
#define JCA_IOT_WIFICONNECT_DELAY_FAILED 10000
#define JCA_IOT_WIFICONNECT_DELAY_RECONNECT 300000

namespace JCA {
  namespace IOT {
    enum WiFiState {
      Init = 0,
      Connect = 1,
      Busy = 2,
      Failed = 3,
      STA = 4,
      AP = 5
    };

    class WiFiConnect {
    private:
      // Internal
      unsigned long BusyTimer;
      unsigned long ReconnectTimer;
      const char *ObjectName = "WiFiConnect";
      // Defined by Contructor
      enum WiFiState State;
      char ApSsid[80];
      char ApPassword[80];
      IPAddress ApIP;
      IPAddress ApGateway;
      IPAddress ApSubnet;
      // Defined by Init and Set
      char Ssid[80];
      char Password[80];
      bool DHCP;
      IPAddress IP;
      IPAddress Gateway;
      IPAddress Subnet;
      bool isConfigured();

    public:
      // Constuctor/Destructor
      WiFiConnect (const char *_SsidPrefix, const char *_Password, const char *_ApIP, const char *_ApGateway, const char *_APSubnet);
      WiFiConnect (const char *_SsidPrefix, const char *_Password);
      WiFiConnect ();
      ~WiFiConnect ();

      // Setter
      bool setSsid(const char *_Ssid);
      bool setPassword (const char *_Password);
      bool setIP (const char *_IP);
      bool setGateway (const char *_Gateway);
      bool setSubnet (const char *_Subnet);
      bool setDHCP (bool _DHCP);
      bool init (const char *_Ssid, const char *_Password, const char *_IP, const char *_Gateway, const char *_Subnet, bool _DHCP);
      bool init ();

      // Handling
      bool doConnect();
      bool handle ();
      bool isConnected();
      String replaceWildcards (const String &var);
    };
  }
}

#endif