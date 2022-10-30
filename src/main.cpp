// Firmware
#include "FS.h"
#include <Arduino.h>
#include <LittleFS.h>
#define SPIFFS LittleFS

// Basics
#include <JCA_IOT_Webserver.h>
#include <JCA_SYS_DebugOut.h>

// Project function
#include <JCA_FNC_Feeder.h>
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
StaticJsonDocument<2000> JDoc;
Webserver Server;
//-------------------------------------------------------
// System Functions
//-------------------------------------------------------
void cbSystemReset () {
  ESP.restart ();
}
void cbSaveConfig () {
  JDoc.clear ();
  JsonArray Elements = JDoc.createNestedArray (Protocol::JsonTagElements);
  Spindel.getConfig (Elements);
  Futter.getConfig (Elements);
  File ConfigFile = LittleFS.open (CONFIGPATH, "w");
  serializeJson (JDoc, ConfigFile);
  ConfigFile.close ();
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
JsonVariant cbRestApiGet (JsonVariant &var) {
  JDoc.clear ();
  JsonArray Elements = JDoc.createNestedArray (Protocol::JsonTagElements);
  Server.getAll(Elements);
  Spindel.getAll (Elements);
  Futter.getAll (Elements);
  return JDoc;
}
JsonVariant cbRestApiPost (JsonVariant &var) {
  if (var.containsKey (Protocol::JsonTagElements)) {
    JsonArray Elements = (var.as<JsonObject> ())[Protocol::JsonTagElements].as<JsonArray> ();
    Spindel.set (Elements);
    Futter.set (Elements);
  }
  JDoc.clear ();
  return JDoc;
}
JsonVariant cbRestApiPut (JsonVariant &var) {
  JDoc.clear ();
  JDoc["message"] = "PUT not Used";
  return JDoc;
}
JsonVariant cbRestApiPatch (JsonVariant &var) {
  cbSaveConfig();
  return JDoc;
}
JsonVariant cbRestApiDelete (JsonVariant &var) {
  JDoc.clear ();
  JDoc["message"] = "DELETE not used";
  return JDoc;
}

//-------------------------------------------------------
// RestAPI Functions
//-------------------------------------------------------
JsonVariant cbWsUpdate (JsonVariant &var) {
  JDoc.clear ();
  JsonArray Elements = JDoc.createNestedArray (Protocol::JsonTagElements);
  Server.getAll (Elements);
  Spindel.getAll (Elements);
  Futter.getAll (Elements);
  return JDoc;
}
JsonVariant cbWsData (JsonVariant &var) {
  if (var.containsKey (Protocol::JsonTagElements)) {
    JsonArray Elements = (var.as<JsonObject> ())[Protocol::JsonTagElements].as<JsonArray> ();
    Spindel.set (Elements);
    Futter.set (Elements);
  }
  JDoc.clear ();
  JsonArray Elements = JDoc.createNestedArray (Protocol::JsonTagElements);
  Server.getAll (Elements);
  Spindel.getAll (Elements);
  Futter.getAll (Elements);
  return JDoc;
}

//#######################################################
// Setup
//#######################################################
void setup () {

  pinMode (STAT_PIN, OUTPUT);
  digitalWrite (STAT_PIN, LOW);

  Debug.init (FLAG_NONE);
  //Debug.init (FLAG_ERROR | FLAG_SETUP | FLAG_CONFIG | FLAG_TRAFFIC);// | FLAG_LOOP);

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Filesystem
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (!LittleFS.begin ()) {
    Debug.println (FLAG_ERROR, false, "root", "setup", "LITTLEFS Mount Failed");
    return;
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
      if (JDoc.containsKey (Protocol::JsonTagElements)) {
        JsonArray Elements = (JDoc.as<JsonObject> ())[Protocol::JsonTagElements].as<JsonArray> ();
        Spindel.set (Elements);
        Futter.set (Elements);
      }
    } else {
      Debug.print (FLAG_ERROR, false, "main", "setup", "deserializeJson() failed: ");
      Debug.println (FLAG_ERROR, false, "main", "setup", Error.c_str ());
    }
    ConfigFile.close ();
  } else {
    Debug.println (FLAG_ERROR, false, "main", "setup", "Config File NOT found");
  }
}

//#######################################################
// Loop
//#######################################################
void loop () {
  Server.handle ();
  tm CurrentTime = Server.getTimeStruct ();

  Spindel.update (CurrentTime);
  Futter.update (CurrentTime);
}