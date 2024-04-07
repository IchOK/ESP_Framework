/**
 * @file main.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Ceiling Light with WS2812 Stripe and LED Spot
 * @version 0.1
 * @date 2023-06.11
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

// Firmware
#include "FS.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <Wire.h>
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
#include <JCA_SYS_PwmOutput.h>

// Project function
#include <JCA_FNC_DigitalOut.h>
#include <JCA_FNC_LedStrip.h>
#include <JCA_FNC_Parent.h>

using namespace JCA::IOT;
using namespace JCA::SYS;
using namespace JCA::FNC;
#define STAT_PIN LED_BUILTIN

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Custom Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define CONFIGPATH "/usrConfig.json"
#include <JCA_FNC_ElementTagBool.h>
#include <JCA_FNC_ElementTagFloat.h>
#include <vector>
std::vector<ElementTag *> Config;

bool ValueBool;
float ValueFloat;
void setupConfig () {
  Config.push_back (new ElementTagBool ("Name", "Text", "", false, &ValueBool, "", ""));
  Config.push_back (new ElementTagFloat ("Name", "Text", "", false, &ValueFloat, ""));
}
void loopConfig () {
  DynamicJsonDocument doc (1024);
  JsonObject Data;
  String Output;
  ValueFloat += 1;
  ValueBool = !ValueBool;

  Data = doc.createNestedObject("BOOL");
  Config[0]->getJsonObject (Data);
  Data = doc.createNestedObject ("FLOAT");
  Config[1]->getJsonObject (Data);
  serializeJsonPretty(doc, Output);
  Debug.println (FLAG_SETUP, false, "main", "loop", Output);
}
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
  ConfigFile.println ("]}");
  ConfigFile.close ();
}

void getAllValues (JsonVariant &_Out) {
  JsonObject Elements = _Out.createNestedObject (Parent::JsonTagElements);
  Server.getValues (Elements);
}

void setAll (JsonVariant &_In) {
  if (_In.containsKey (Parent::JsonTagElements)) {
    JsonArray Elements = (_In.as<JsonObject> ())[Parent::JsonTagElements].as<JsonArray> ();
    Server.set (Elements);
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
  DebugFlags |= FLAG_ERROR;
  DebugFlags |= FLAG_SETUP;
  DebugFlags |= FLAG_CONFIG;
  // DebugFlags |= FLAG_TRAFFIC;
  // DebugFlags |= FLAG_LOOP;
  // DebugFlags |= FLAG_PROTOCOL;
  // DebugFlags |= FLAG_DATA;
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

  setupConfig();
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

    loopConfig();
  }
}