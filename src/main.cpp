/**
 * @file main.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Akku Manager charging and discharging with Capacity calculation
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

// Firmware
#include "FS.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <time.h>
#include <Wire.h>

#ifdef ESP8266
  #define SPIFFS LittleFS
  #define SERIAL_BAUD 74880
#elif ESP32
  #define SERIAL_BAUD 115200
#endif

// Basics
#include <JCA_IOT_Webserver.h>
#include <JCA_SYS_DebugOut.h>

// Project function
#include <JCA_FNC_Charger.h>
#include <JCA_FNC_DS18B20.h>
#include <JCA_FNC_Feeder.h>
#include <JCA_FNC_INA219.h>
#include <JCA_FNC_Level.h>
#include <JCA_FNC_Parent.h>

using namespace JCA::IOT;
using namespace JCA::SYS;
using namespace JCA::FNC;
#define STAT_PIN LED_BUILTIN

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Custom Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CONFIGPATH "/usrConfig.json"
//-------------------------------------------------------
// INA291 Sensor
//-------------------------------------------------------
#define SDA_PIN 21
#define SCL_PIN 22
#define SENSOR1_ADR 0x40   // Modul 1 INA219-Address (A0=0 / A1=0)
#define SENSOR2_ADR 0x41   // Modul 2 INA219-Address (A0=1 / A1=0)
#define SENSOR3_ADR 0x44   // Modul 3 INA219-Address (A0=0 / A1=1)
#define SENSOR4_ADR 0x45   // Modul 4 INA219-Address (A0=1 / A1=1)

INA219 PowerSensor1 (SENSOR1_ADR, "PowerSensor1");

//-------------------------------------------------------
// Charger
//-------------------------------------------------------
#define CHARGE1_PIN 18     // Modul 1 Charging-PWM
#define CHARGE2_PIN 26     // Modul 2 Charging-PWM
#define CHARGE3_PIN 33     // Modul 3 Charging-PWM
#define CHARGE4_PIN 13     // Modul 4 Charging-PWM
#define DISCHARGE1_PIN 27  // Modul 1 Discharge-PWM
#define DISCHARGE2_PIN 25  // Modul 2 Discharge-PWM
#define DISCHARGE3_PIN 32  // Modul 3 Discharge-PWM
#define DISCHARGE4_PIN 4   // Modul 4 Discharge-PWM

Charger Laderegler1 (&PowerSensor1, CHARGE1_PIN, DISCHARGE1_PIN, "Laderegler1");

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// JCA IOT Functions
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
Webserver Server;
//-------------------------------------------------------
// System Functions
//-------------------------------------------------------
void cbSystemReset () {
  ESP.restart ();
}
void cbSaveConfig () {
  File ConfigFile = LittleFS.open (CONFIGPATH, "w");
  bool ElementInit = false;
  ConfigFile.println ("{\"elements\":[");
  Server.writeSetup (ConfigFile, ElementInit);
  PowerSensor1.writeSetup (ConfigFile, ElementInit);
  Laderegler1.writeSetup (ConfigFile, ElementInit);
  ConfigFile.println ("]}");
  ConfigFile.close ();
}

void getAllValues (JsonVariant &_Out) {
  JsonObject Elements = _Out.createNestedObject (Parent::JsonTagElements);
  Server.getValues (Elements);
  PowerSensor1.getValues (Elements);
  Laderegler1.getValues (Elements);
}

void setAll (JsonVariant &_In) {
  if (_In.containsKey (Parent::JsonTagElements)) {
    JsonArray Elements = (_In.as<JsonObject> ())[Parent::JsonTagElements].as<JsonArray> ();
    Server.set (Elements);
    PowerSensor1.set (Elements);
    Laderegler1.set (Elements);
  }
}
//-------------------------------------------------------
// Website Functions
//-------------------------------------------------------
String cbWebHomeReplace (const String &var) {
  return String ();
}
String cbWebConfigReplace (const String &var) {
  return String ();
}
//-------------------------------------------------------
// RestAPI Functions
//-------------------------------------------------------
void cbRestApiGet (JsonVariant &_In, JsonVariant &_Out) {
  getAllValues (_Out);
}

void cbRestApiPost (JsonVariant &_In, JsonVariant &_Out) {
  setAll (_In);
}

void cbRestApiPut (JsonVariant &_In, JsonVariant &_Out) {
  _Out["message"] = "PUT not Used";
}

void cbRestApiPatch (JsonVariant &_In, JsonVariant &_Out) {
  cbSaveConfig ();
}

void cbRestApiDelete (JsonVariant &_In, JsonVariant &_Out) {
  _Out["message"] = "DELETE not used";
}

//-------------------------------------------------------
// Websocket Functions
//-------------------------------------------------------
void cbWsUpdate (JsonVariant &_In, JsonVariant &_Out) {
  getAllValues (_Out);
}
void cbWsData (JsonVariant &_In, JsonVariant &_Out) {
  setAll (_In);

  // Return Value update
  getAllValues (_Out);
}

// #######################################################
//  Setup
// #######################################################
void setup () {
  DynamicJsonDocument JDoc (10000);

  pinMode (STAT_PIN, OUTPUT);
  digitalWrite (STAT_PIN, LOW);

  // Config Debug-Output
  uint16_t DebugFlags = FLAG_NONE;
  //DebugFlags |= FLAG_ERROR;
  //DebugFlags |= FLAG_SETUP;
  //DebugFlags |= FLAG_CONFIG;
  //DebugFlags |= FLAG_TRAFFIC;
  //DebugFlags |= FLAG_LOOP;
  //DebugFlags |= FLAG_PROTOCOL;
  //DebugFlags |= FLAG_DATA;
  Debug.init (DebugFlags, SERIAL_BAUD);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Filesystem
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef ESP32
  if (!LittleFS.begin (true)) {
#else
  if (!LittleFS.begin ()) {
#endif
    Debug.println (FLAG_ERROR, false, "root", "setup", "LITTLEFS Mount Failed");
    return;
  } else {
    Debug.println (FLAG_SETUP, false, "root", "setup", "LITTLEFS Mount Done");
  }

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // JCA IOT Functions - WiFiConnect
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // System
  Server.init ();
  Server.onSystemReset (cbSystemReset);
  Server.onSaveConfig (cbSaveConfig);
  // Web
  Server.onWebHomeReplace (cbWebHomeReplace);
  Server.onWebConfigReplace (cbWebConfigReplace);
  // RestAPI
  Server.onRestApiGet (cbRestApiGet);
  Server.onRestApiPost (cbRestApiPost);
  Server.onRestApiPut (cbRestApiPut);
  Server.onRestApiPatch (cbRestApiPatch);
  // Web-Socket
  Server.onWsData (cbWsData);
  Server.onWsUpdate (cbWsUpdate);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Custom Code
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //-------------------------------------------------------
  // Init Elements
  //-------------------------------------------------------
  Wire.begin(SDA_PIN, SCL_PIN);
  if(!PowerSensor1.init()){
    Debug.println (FLAG_SETUP, false, "main", "setup", "Power Sensor not connected");
  }
  Laderegler1.init();
  //-------------------------------------------------------
  // Read Config File
  //-------------------------------------------------------
  File ConfigFile = LittleFS.open (CONFIGPATH, "r");
  if (ConfigFile) {
    Debug.println (FLAG_CONFIG, false, "main", "setup", "Config File Found");
    DeserializationError Error = deserializeJson (JDoc, ConfigFile);
    if (!Error) {
      Debug.println (FLAG_CONFIG, false, "main", "setup", "Deserialize Done");
      JsonVariant InConfig = JDoc.as<JsonVariant> ();
      setAll (InConfig);
    } else {
      Debug.print (FLAG_ERROR, false, "main", "setup", "deserializeJson() failed: ");
      Debug.println (FLAG_ERROR, false, "main", "setup", Error.c_str ());
    }
    ConfigFile.close ();
  } else {
    Debug.println (FLAG_ERROR, false, "main", "setup", "Config File NOT found");
  }
}

// #######################################################
//  Loop
// #######################################################
int8_t LastSeconds = 0;
void loop () {
  Server.handle ();
  tm CurrentTime = Server.getTimeStruct ();
  if (LastSeconds != CurrentTime.tm_sec) {
    if (Debug.print (FLAG_LOOP, false, "main", "loop", "Current Time: ")) {
      Debug.println (FLAG_LOOP, false, "main", "loop", Server.getTimeString (""));
    }
    LastSeconds = CurrentTime.tm_sec;
  }

  PowerSensor1.update (CurrentTime);
  Laderegler1.update (CurrentTime);
}