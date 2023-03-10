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
// Feeder
//-------------------------------------------------------
#define EN_PIN D1   // Enable
#define STEP_PIN D2 // Step
#define DIR_PIN D3  // Direction

Feeder Spindel (EN_PIN, STEP_PIN, DIR_PIN, "Spindel");

//-------------------------------------------------------
// Level
//-------------------------------------------------------
#define LEVEL_PIN A0 // Levelsensor

Level Futter (LEVEL_PIN, "Futter");

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
  Spindel.writeSetup (ConfigFile, ElementInit);
  Futter.writeSetup (ConfigFile, ElementInit);
  ConfigFile.println ("]}");
  ConfigFile.close ();
}

void getAllValues (JsonVariant &_Out) {
  JsonObject Elements = _Out.createNestedObject (Parent::JsonTagElements);
  Server.getValues (Elements);
  Spindel.getValues (Elements);
  Futter.getValues (Elements);
}

void setAll (JsonVariant &_In) {
  if (_In.containsKey (Parent::JsonTagElements)) {
    JsonArray Elements = (_In.as<JsonObject> ())[Parent::JsonTagElements].as<JsonArray> ();
    Server.set (Elements);
    Spindel.set (Elements);
    Futter.set (Elements);
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

  // Debug.init (FLAG_NONE);
  Debug.init (FLAG_ERROR | FLAG_SETUP | FLAG_CONFIG | FLAG_TRAFFIC, SERIAL_BAUD); // | FLAG_LOOP);

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
void loop () {
  Server.handle ();
  tm CurrentTime = Server.getTimeStruct ();

  Spindel.update (CurrentTime);
  Futter.update (CurrentTime);
}