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
#include <FS.h>
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
#include <JCA_IOT_FuncHandler.h>

// Project function
#include <JCA_FNC_AcDimmers.h>
#include <JCA_FNC_Charger.h>

using namespace JCA::IOT;
using namespace JCA::SYS;
using namespace JCA::FNC;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Custom Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// JCA IOT Functions
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
Webserver Server;
FuncHandler Handler ("handler");

//-------------------------------------------------------
// Hardware
//-------------------------------------------------------
PwmOutput OutputPwm;
void linkHardware() {
  Handler.HardwareMapping.insert(std::pair<String, void*> ("PWM", &OutputPwm));
}

//-------------------------------------------------------
// Functions
//-------------------------------------------------------
void addFunctionsToHandler () {
  Charger::AddToHandler (Handler);
  AcDimmers::AddToHandler (Handler);
}

//-------------------------------------------------------
// System Functions
//-------------------------------------------------------
void cbSystemReset () {
  ESP.restart ();
}
void cbSaveConfig () {
  Handler.saveValues();
}

void getAllValues (JsonVariant &_Out) {
  JsonObject Elements = _Out.createNestedObject (FuncParent::JsonTagElements);
  Handler.getValues(Elements);
}

void setAll (JsonVariant &_In) {
  if (_In.containsKey (FuncParent::JsonTagElements)) {
    JsonObject Elements = (_In.as<JsonObject> ())[FuncParent::JsonTagElements].as<JsonObject> ();
    Handler.setValues(Elements);
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

  // Config Debug-Output
  uint16_t DebugFlags = FLAG_NONE;
  DebugFlags |= FLAG_ERROR;
  DebugFlags |= FLAG_SETUP;
  DebugFlags |= FLAG_CONFIG;
  // DebugFlags |= FLAG_TRAFFIC;
  // DebugFlags |= FLAG_LOOP;
  DebugFlags |= FLAG_PROTOCOL;
  DebugFlags |= FLAG_DATA;
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
  // JCA IOT Functions
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // System
  Server.init ();
  Server.onSystemReset (cbSystemReset);
  Server.onSaveConfig (cbSaveConfig);
  Debug.println (FLAG_SETUP, false, "root", __func__, "Server-System Done");
  // Web
  Server.onWebHomeReplace (cbWebHomeReplace);
  Server.onWebConfigReplace (cbWebConfigReplace);
  Debug.println (FLAG_SETUP, false, "root", __func__, "Server-Web Done");
  // RestAPI
  Server.onRestApiGet (cbRestApiGet);
  Server.onRestApiPost (cbRestApiPost);
  Server.onRestApiPut (cbRestApiPut);
  Server.onRestApiPatch (cbRestApiPatch);
  Debug.println (FLAG_SETUP, false, "root", __func__, "Server-RestAPI Done");
  // Web-Socket
  Server.onWsData (cbWsData);
  Server.onWsUpdate (cbWsUpdate);
  Debug.println (FLAG_SETUP, false, "root", __func__, "Server-WebSocket Done");

  // Function-Handler
  addFunctionsToHandler();
  linkHardware();
  Handler.setup ();
  Handler.loadValues ();
  Debug.println (FLAG_SETUP, false, "root", __func__, "FunctionHandler Done");

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Custom Code
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
}

// #######################################################
//  Loop
// #######################################################
int8_t LastSeconds = 0;
void loop () {
  Server.handle ();
  tm CurrentTime = Server.getTimeStruct ();
//  Handler.update(CurrentTime);
}