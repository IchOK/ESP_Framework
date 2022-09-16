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
StaticJsonDocument<1000> JDoc;

JsonVariant cbRestApiGet (JsonVariant &var) {
  JDoc.clear ();
  //JsonObject Collection = JDoc.createNestedObject();
  JsonObject Collection = JDoc.to<JsonObject> ();
  Spindel.getAll (Collection);
  Collection["time"] = Server.getTimeString ("");
  return JDoc;
}
JsonVariant cbRestApiPost (JsonVariant &var) {
  Spindel.set (var.as<JsonObject> ());
  JDoc.clear ();
  return JDoc;
}
JsonVariant cbRestApiPut (JsonVariant &var) {
  JDoc.clear ();
  JDoc["message"] = "PUT not Used";
  return JDoc;
}
JsonVariant cbRestApiPatch (JsonVariant &var) {
  JDoc.clear ();
  JsonObject Collection = JDoc.as<JsonObject> ();
  Spindel.getConfig (Collection);
  File ConfigFile = LittleFS.open (CONFIGPATH, "w");
  serializeJson (JDoc, ConfigFile);
  ConfigFile.close ();
  return JDoc;
}
JsonVariant cbRestApiDelete (JsonVariant &var) {
  JDoc.clear ();
  JDoc["message"] = "DELETE not used";
  return JDoc;
}

//#######################################################
// Setup
//#######################################################
void setup () {

  pinMode (STAT_PIN, OUTPUT);
  digitalWrite (STAT_PIN, LOW);

  Debug.init (FLAG_ERROR | FLAG_SETUP | FLAG_CONFIG | FLAG_TRAFFIC | FLAG_LOOP);

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
  // Web
  Server.onWebHomeReplace (cbWebHomeReplace);
  Server.onWebConfigReplace (cbWebConfigReplace);
  // RestAPI
  Server.onRestApiGet (cbRestApiGet);
  Server.onRestApiPost (cbRestApiPost);
  Server.onRestApiPut (cbRestApiPut);
  Server.onRestApiPatch (cbRestApiPatch);

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
      JsonObject Collection = JDoc.as<JsonObject> ();

      // Set custom Objects
      Spindel.set (Collection);

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

  Spindel.update(CurrentTime);
}