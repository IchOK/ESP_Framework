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
#include <Arduino.h>
#include <FS.h>
#include <LittleFS.h>
#include <OneWire.h>
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
#include <JCA_FNC_DigitalOut.h>
#include <JCA_FNC_DS18B20.h>
#include <JCA_FNC_Feeder.h>
#include <JCA_FNC_INA219.h>
#include <JCA_FNC_LedStrip.h>
#include <JCA_FNC_Level.h>
#include <JCA_FNC_WebserverLink.h>

using namespace JCA::IOT;
using namespace JCA::SYS;
using namespace JCA::FNC;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Custom Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define STATE_LED_PIN -1           // disable Status-LED
//#define STATE_LED_PIN LED_BUILTIN  // set Status to onborad LED

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// JCA IOT Functions
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
Webserver Server;
FuncHandler Handler ("handler");

//-------------------------------------------------------
// Hardware
//-------------------------------------------------------
//const uint8_t TwoWireNum = 1;
//const int TwoWireSDA = -1;
//const int TwoWireSCL = -1;
//PwmOutput HwPWM;
//OneWire HwOneWire;
//TwoWire HwTwoWire = TwoWire(TwoWireNum);
void linkHardware() {
  //Handler.HardwareMapping.insert (std::pair<String, void *> ("WebServer", &Server));
  //Handler.HardwareMapping.insert (std::pair<String, void *> ("PWM", &HwPWM));
  //Handler.HardwareMapping.insert (std::pair<String, void *> ("OneWire", &HwOneWire));
  //HwTwoWire.setPins(TwoWireSDA,TwoWireSCL);
  //Handler.HardwareMapping.insert (std::pair<String, void *> ("TwoWire", &HwTwoWire));
}

//-------------------------------------------------------
// Functions
//-------------------------------------------------------
void addFunctionsToHandler () {
  AcDimmers::AddToHandler (Handler);
  Charger::AddToHandler (Handler);
  DigitalOut::AddToHandler(Handler);
  DS18B20::AddToHandler(Handler);
  Feeder::AddToHandler(Handler);
  INA219::AddToHandler(Handler);
  LedStrip::AddToHandler(Handler);
  Level::AddToHandler(Handler);
  WebserverLink::AddToHandler (Handler);
}

//-------------------------------------------------------
// System Functions
//-------------------------------------------------------
void cbSystemReset () {
  ESP.restart ();
}
void cbSaveConfig () {
  Handler.patch ("savevalues");
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
  if (_In.containsKey ("mode")) {
    Handler.patch (_In["mode"].as<String> ());
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
  _Out["freeHeap"] = ESP.getFreeHeap ();
  _Out["functions"] = Handler.getFuncCount();
  _Out["links"] = Handler.getFuncCount ();
}

void cbRestApiPatch (JsonVariant &_In, JsonVariant &_Out) {
  if (_In.containsKey ("mode")) {
    String Mode = _In["mode"].as<String> ();
    _Out["mode"] = Mode;
    _Out["ret"] = Handler.patch (Mode);
  } else {
    _Out["ret"] = "mode Missing";
  }
  if (_In.containsKey ("reboot")) {
    if (_In["mode"].as<bool> ()) {
      ESP.restart ();
    }
  }

}

void cbRestApiDelete (JsonVariant &_In, JsonVariant &_Out) {
  String Mode = "delete";
  _Out["mode"] = Mode;
  _Out["ret"] = Handler.patch (Mode);
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
  // DebugFlags |= FLAG_CONFIG;
  // DebugFlags |= FLAG_TRAFFIC;
  // DebugFlags |= FLAG_LOOP;
  // DebugFlags |= FLAG_PROTOCOL;
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
  Server.setStatePin (STATE_LED_PIN);
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
  Server.onRestApiDelete (cbRestApiDelete);
  Debug.println (FLAG_SETUP, false, "root", __func__, "Server-RestAPI Done");
  // Web-Socket
  Server.onWsData (cbWsData);
  Server.onWsUpdate (cbWsUpdate);
  Debug.println (FLAG_SETUP, false, "root", __func__, "Server-WebSocket Done");

  // Function-Handler
  addFunctionsToHandler();
  linkHardware();
  Handler.patch ("init");
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
  Handler.update(CurrentTime);
}