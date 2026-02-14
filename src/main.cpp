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
#include <JCA_IOT_FuncHandler.h>
#include <JCA_IOT_Server.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_PwmOutput.h>

// Project Hardware
#include <JCA_IOT_Hardware.h>

// Project Links
#include <JCA_LNK_LinkDirect.h>
#include <JCA_LNK_LinkMove.h>

// Project function - conditional compilation for memory-constrained targets
// Define JCA_IGNORE_xxx build flags in platformio.ini to exclude specific modules.
// By default all modules are included. Use -DJCA_IGNORE_xxx to exclude heavy modules on ESP8266.
#ifdef ESP32
  #include <JCA_FNC_AcDimmers.h>
#endif
#ifndef JCA_IGNORE_CHARGER
  #include <JCA_FNC_Charger.h>
#endif
#ifndef JCA_IGNORE_CLOCKVALUES
  #include <JCA_FNC_ClockValues.h>
#endif
#ifndef JCA_IGNORE_DIGITALIN
  #include <JCA_FNC_DigitalIn.h>
#endif
#ifndef JCA_IGNORE_DIGITALOUT
  #include <JCA_FNC_DigitalOut.h>
#endif
#ifndef JCA_IGNORE_DS18B20
  #include <JCA_FNC_DS18B20.h>
#endif
#ifndef JCA_IGNORE_FEEDER
  #include <JCA_FNC_Feeder.h>
#endif
#ifndef JCA_IGNORE_INA219
  #include <JCA_FNC_INA219.h>
#endif
#ifndef JCA_IGNORE_LEDSTRIP
  #include <JCA_FNC_LedStrip.h>
#endif
#ifndef JCA_IGNORE_LEVEL
  #include <JCA_FNC_Level.h>
#endif
#ifndef JCA_IGNORE_SERVERLINK
  #include <JCA_FNC_ServerLink.h>
#endif
#ifndef JCA_IGNORE_VALUEANALOG
  #include <JCA_FNC_ValueAnalog.h>
#endif
#ifndef JCA_IGNORE_VALUEDIGITAL
  #include <JCA_FNC_ValueDigital.h>
#endif
#ifndef JCA_IGNORE_DAYSELECT
  #include <JCA_FNC_DaySelect.h>
#endif
#ifndef JCA_IGNORE_PIDCONTROLLER
  #include <JCA_FNC_PIDController.h>
#endif
#ifndef JCA_IGNORE_VALVE2DPOSIMP
  #include <JCA_FNC_Valve2DPosImp.h>
#endif

using namespace JCA::IOT;
using namespace JCA::SYS;
using namespace JCA::FNC;
using namespace JCA::TAG;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Custom Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define STATE_LED_PIN -1           // disable Status-LED
//#define STATE_LED_PIN LED_BUILTIN  // set Status to onborad LED

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// JCA IOT Functions
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
JCA::IOT::Server IotServer;
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
void addHardwareToHandler() {
  // Link const Hardware
  Handler.HardwareMapping.insert (std::pair<String, void *> ("IotServer", &IotServer));

  // Add Hardware Interfaces
  JCA::IOT::AddOneWire (Handler);
  JCA::IOT::AddPwmOutput (Handler);
  //HwTwoWire.setPins(TwoWireSDA,TwoWireSCL);
  //Handler.HardwareMapping.insert (std::pair<String, void *> ("TwoWire", &HwTwoWire));
}

//-------------------------------------------------------
// Links
//-------------------------------------------------------
void addLinksToHandler() {
  JCA::LNK::LinkDirect::AddToHandler(Handler);
  JCA::LNK::LinkMove::AddToHandler(Handler);
}

//-------------------------------------------------------
// Functions
//-------------------------------------------------------
void addFunctionsToHandler () {
  #ifdef ESP32
    AcDimmers::AddToHandler (Handler);
  #endif
  #ifndef JCA_IGNORE_CHARGER
    Charger::AddToHandler (Handler);
  #endif
  #ifndef JCA_IGNORE_CLOCKVALUES
    ClockValues::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_DIGITALIN
    DigitalIn::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_DIGITALOUT
    DigitalOut::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_DS18B20
    DS18B20::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_FEEDER
    Feeder::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_INA219
    INA219::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_LEDSTRIP
    LedStrip::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_LEVEL
    Level::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_SERVERLINK
    ServerLink::AddToHandler (Handler);
  #endif
  #ifndef JCA_IGNORE_VALUEANALOG
    ValueAnalog::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_VALUEDIGITAL
    ValueDigital::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_DAYSELECT
    DaySelect::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_PIDCONTROLLER
    PIDController::AddToHandler(Handler);
  #endif
  #ifndef JCA_IGNORE_VALVE2DPOSIMP
    Valve2DPosImp::AddToHandler(Handler);
  #endif
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
  JsonObject Elements = _Out[FuncParent::JsonTagElements].to<JsonObject>();
  Handler.getValues (Elements, TagAccessType_T::Read);
}

void setAll (JsonVariant &_In) {
  if (_In[FuncParent::JsonTagElements].is<JsonObject>()) {
    JsonObject Elements = _In[FuncParent::JsonTagElements].as<JsonObject>();
    Handler.setValues (Elements, TagAccessType_T::Write);
  }
  if (_In["mode"].is<JsonVariant> ()) {
    Handler.patch (_In["mode"].as<String> ());
  }
}
//-------------------------------------------------------
// Website Functions
//-------------------------------------------------------
String createConfigHRef (const String FilePath, const String LinkText) {
  return String ("<a href = \"") + FilePath + String ("\" target=\"_blank\">") + LinkText + String("</a><br/>");
}
String cbWebUserReplace (const String &var) {
#ifdef JCA_IOT_FILE_SYSTEMCONFIG
  if (var == "SYSTEMCONFIG_LINK") {
    return createConfigHRef (JCA_IOT_FILE_SYSTEMCONFIG, "System Config");
  }
#endif
#ifdef JCA_IOT_FILE_VALUES
  if (var == "VALUES_LINK") {
    return createConfigHRef (JCA_IOT_FILE_VALUES, "Value Listing");
  }
#endif
#ifdef JCA_IOT_FILE_LOG
  if (var == "LOG_LINK") {
    return createConfigHRef (JCA_IOT_FILE_LOG, "Log File");
  }
#endif
#ifdef JCA_IOT_FILE_WIFICONFIG
  if (var == "WIFICONFIG_LINK") {
    return createConfigHRef (JCA_IOT_FILE_WIFICONFIG, "WiFi Config");
  }
#endif
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
  _Out["links"] = Handler.getLinkCount();
}

void cbRestApiTags (JsonVariant &_In, JsonVariant &_Out) {
  // Write tag structures directly to _Out without nested "elements" object
  JsonObject Elements = _Out.to<JsonObject>();
  Handler.getTagStructures (Elements, TagUsage_T::GetAll);
}

void cbRestApiPatch (JsonVariant &_In, JsonVariant &_Out) {
  if (_In["mode"].is<JsonVariant> ()) {
    String Mode = _In["mode"].as<String> ();
    _Out["mode"] = Mode;
    _Out["ret"] = Handler.patch (Mode);
  } else {
    _Out["ret"] = "mode Missing";
  }
  if (_In["reboot"].is<JsonVariant> ()) {
    if (_In["reboot"].as<bool>()) {
      ESP.restart ();
    }
  }
}

void cbRestApiDelete (JsonVariant &_In, JsonVariant &_Out) {
  String Mode = "delete";
  _Out["mode"] = Mode;
  _Out["ret"] = Handler.patch (Mode);
}

void cbRestApiSetupMetadata (JsonVariant &_In, JsonVariant &_Out) {
  JsonObject OutObj = _Out.to<JsonObject>();
  Handler.getSetupMetadata(OutObj);
}

void cbRestApiSetupGet (JsonVariant &_In, JsonVariant &_Out) {
  JsonObject OutObj = _Out.to<JsonObject>();
  if (!Handler.loadSetup(OutObj)) {
    // Error already set in loadSetup
  }
}

void cbRestApiSetupPut (JsonVariant &_In, JsonVariant &_Out) {
  JsonObject InObj = _In.as<JsonObject>();
  if (Handler.saveSetup(InObj)) {
    _Out["success"] = true;
  } else {
    _Out["success"] = false;
    _Out["error"] = "Failed to save setup file";
  }
}

void cbRestApiSetupFunctions (JsonVariant &_In, JsonVariant &_Out) {
  JsonObject OutObj = _Out.to<JsonObject>();
  Handler.getFunctionsList(OutObj);
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
  // Config Debug-Output
  uint16_t DebugFlags = FLAG_NONE;
  // DebugFlags |= FLAG_ERROR;
  // DebugFlags |= FLAG_SETUP;
  // DebugFlags |= FLAG_CONFIG;
  // DebugFlags |= FLAG_TRAFFIC;
  // DebugFlags |= FLAG_LOOP;
  // DebugFlags |= FLAG_PROTOCOL;
  // DebugFlags |= FLAG_DATA;
  // DebugFlags |= FLAG_SYSTEM;
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
  IotServer.init ();
  IotServer.setStatePin (STATE_LED_PIN);
  IotServer.onSystemReset (cbSystemReset);
  IotServer.onSaveConfig (cbSaveConfig);
  Debug.println (FLAG_SETUP, false, "root", __func__, "IotServer-System Done");
  // Web
  IotServer.onWebUserReplace (cbWebUserReplace);
  Debug.println (FLAG_SETUP, false, "root", __func__, "IotServer-Web Done");
  // RestAPI
  IotServer.onRestApiGet (cbRestApiGet);
  IotServer.onRestApiPost (cbRestApiPost);
  IotServer.onRestApiPut (cbRestApiPut);
  IotServer.onRestApiPatch (cbRestApiPatch);
  IotServer.onRestApiDelete (cbRestApiDelete);
  IotServer.onRestApiTags (cbRestApiTags);
  IotServer.onRestApiSetupMetadata (cbRestApiSetupMetadata);
  IotServer.onRestApiSetupGet (cbRestApiSetupGet);
  IotServer.onRestApiSetupPut (cbRestApiSetupPut);
  IotServer.onRestApiSetupFunctions (cbRestApiSetupFunctions);
  Debug.println (FLAG_SETUP, false, "root", __func__, "IotServer-RestAPI Done");
  // Web-Socket
  IotServer.onWsData (cbWsData);
  IotServer.onWsUpdate (cbWsUpdate);
  Debug.println (FLAG_SETUP, false, "root", __func__, "IotServer-WebSocket Done");

  // Function-Handler
  Debug.print (FLAG_SYSTEM, false, "root", __func__, "[DBG-HEAP] Before handler.patch init: ");
  Debug.println (FLAG_SYSTEM, false, "root", __func__, ESP.getFreeHeap());
  addLinksToHandler();
  addFunctionsToHandler();
  addHardwareToHandler();
  Debug.print (FLAG_SYSTEM, false, "root", __func__, "[DBG-HEAP] After handler.patch init: ");
  Debug.println (FLAG_SYSTEM, false, "root", __func__, ESP.getFreeHeap());
  // #region agent log - Heap monitoring before handler init
  Handler.patch ("init");
  Debug.print (FLAG_SYSTEM, false, "root", __func__, "[DBG-HEAP] After handler.patch init: ");
  Debug.println (FLAG_SYSTEM, false, "root", __func__, ESP.getFreeHeap());
  Debug.println (FLAG_SETUP, false, "root", __func__, "FunctionHandler Done");

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Custom Code
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Debug.print (FLAG_SETUP, false, "root", __func__, "A0: ");
  Debug.println (FLAG_SYSTEM, false, "root", __func__, A0);
  
}

// #######################################################
//  System Analysis
// #######################################################
uint32_t MinHeap = -1;
uint32_t MaxHeap = 0;
uint32_t MinLoopMillis = -1;
uint32_t MaxLoopMillis = 0;
uint32_t LoopCount = 0;
uint32_t LastMillis = 0;
uint32_t LastUpdateMillis = 0;
void printLoopSystemInfo () {

  // Get Heap
  uint32_t CurrentHeap = ESP.getFreeHeap();
  if (CurrentHeap < MinHeap) {
    MinHeap = CurrentHeap;
  }
  if (CurrentHeap > MaxHeap) {
    MaxHeap = CurrentHeap;
  }

  // Get Loop Time
  uint32_t ActMillis = millis();
  if (ActMillis - LastMillis < MinLoopMillis) {
    MinLoopMillis = ActMillis - LastMillis;
  }
  if (ActMillis - LastMillis > MaxLoopMillis) {
    MaxLoopMillis = ActMillis - LastMillis;
  }
  LoopCount++;
  LastMillis = ActMillis;

  // Print System Info
  if (ActMillis - LastUpdateMillis > 1000) {
    Debug.print (FLAG_SYSTEM, false, "root", __func__, "[DBG-HEAP] Loop FreeHeap Act / Min / Max: ");
    Debug.print (FLAG_SYSTEM, false, "root", __func__, CurrentHeap);
    Debug.print (FLAG_SYSTEM, false, "root", __func__, " / ");
    Debug.print (FLAG_SYSTEM, false, "root", __func__, MinHeap);
    Debug.print (FLAG_SYSTEM, false, "root", __func__, " / ");
    Debug.println (FLAG_SYSTEM, false, "root", __func__, MaxHeap);
    MinHeap = -1;
    MaxHeap = 0;
    uint32_t MeanLoopMillis = (ActMillis - LastUpdateMillis) / LoopCount;
    Debug.print (FLAG_SYSTEM, false, "root", __func__, "[DBG-TIME] Loop Time Min / Mean/ Max: ");
    Debug.print (FLAG_SYSTEM, false, "root", __func__, MinLoopMillis);
    Debug.print (FLAG_SYSTEM, false, "root", __func__, " / ");
    Debug.print (FLAG_SYSTEM, false, "root", __func__, MeanLoopMillis);
    Debug.print (FLAG_SYSTEM, false, "root", __func__, " / ");
    Debug.println (FLAG_SYSTEM, false, "root", __func__, MaxLoopMillis);
    MinLoopMillis = -1;
    MaxLoopMillis = 0;
    LoopCount = 0;
    LastUpdateMillis = ActMillis;
  }
}

// #######################################################
//  Loop
// #######################################################
int8_t LastSeconds = 0;
void loop () {
  IotServer.handle ();
  tm CurrentTime = IotServer.getLocalTimeStruct ();
  Handler.update(CurrentTime);
  LastSeconds = CurrentTime.tm_sec;

  if (Debug.isFlagSet(FLAG_SYSTEM)) {
    printLoopSystemInfo();
  }
}

