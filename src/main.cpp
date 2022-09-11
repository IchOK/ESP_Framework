// Firmware
#include "FS.h"
#include <Arduino.h>
#include <LittleFS.h>
#define SPIFFS LittleFS

// External Libraries
#include <AccelStepper.h>
#include <TMCStepper.h>

// Internal Libraries
#include <JCA_IOT_DebugOut.h>
#include <JCA_IOT_Webserver.h>

using namespace JCA::IOT;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Stepper
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
#define EN_PIN D1   // Enable
#define STEP_PIN D2 // Step
#define DIR_PIN D3  // Direction
#define STAT_PIN LED_BUILTIN
#define SW_RX D7      // TMC2208/TMC2224 SoftwareSerial receive pin
#define SW_TX D8      // TMC2208/TMC2224 SoftwareSerial transmit pin
#define R_SENSE 0.11f // Match to your driver

int FeedingHour;
int FeedingMinute;
int FeedingStepps;
bool FeedingExt;
bool FeedingOld;
bool FeederEnable;
bool FeederSpeed;

// TMC2208Stepper Driver = TMC2208Stepper(SW_RX, SW_TX, R_SENSE); // Software serial
// TMC2660Stepper driver = TMC2660Stepper(CS_PIN, R_SENSE); // Hardware SPI

AccelStepper Feeder = AccelStepper (AccelStepper::DRIVER, STEP_PIN, DIR_PIN);
//-------------------------------------------------------
// Feeder Functions
//-------------------------------------------------------
void getFeederData (JsonObject &_FeederObj) {
  _FeederObj["hour"] = FeedingHour;
  _FeederObj["minute"] = FeedingMinute;
  _FeederObj["stepps"] = FeedingStepps;
  _FeederObj["run"] = Feeder.isRunning ();
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
StaticJsonDocument<1000> RestApiReturn;

JsonVariant cbRestApiGet (JsonVariant &var) {
  RestApiReturn.clear ();
  JsonObject RetFeeder = RestApiReturn.createNestedObject ("feeder");
  getFeederData (RetFeeder);
  RestApiReturn["time"] = Server.getTimeString ("");
  return RestApiReturn;
}
JsonVariant cbRestApiPost (JsonVariant &var) {
  if (var.containsKey ("feeder")) {
    JsonObject JFeeder = var["feeder"].as<JsonObject> ();
    if (JFeeder.containsKey ("maxSpeed")) {
      Feeder.setMaxSpeed (JFeeder["maxSpeed"].as<float> ());
    }
    if (JFeeder.containsKey ("speed")) {
      float Speed = JFeeder["speed"].as<float> ();
      Feeder.setSpeed (Speed);
      if (Speed > 0.0) {
        FeederSpeed = true;
        Feeder.enableOutputs ();
      } else {
        FeederSpeed = false;
        Feeder.disableOutputs ();
      }
    }
    if (JFeeder.containsKey ("acceleration")) {
      Feeder.setAcceleration (JFeeder["acceleration"].as<float> ());
    }
    if (JFeeder.containsKey ("hour")) {
      FeedingHour = JFeeder["hour"].as<int> ();
    }
    if (JFeeder.containsKey ("minute")) {
      FeedingMinute = JFeeder["minute"].as<int> ();
    }
    if (JFeeder.containsKey ("stepps")) {
      FeedingStepps = JFeeder["stepps"].as<int> ();
    }
    if (JFeeder.containsKey ("feed")) {
      FeedingExt = JFeeder["feed"].as<bool> ();
    }
  }
  RestApiReturn.clear ();
  JsonObject RetFeeder = RestApiReturn.createNestedObject ("feeder");
  getFeederData (RetFeeder);
  return RestApiReturn;
}
JsonVariant cbRestApiPut (JsonVariant &var) {
  RestApiReturn.clear ();
  RestApiReturn["message"] = "PUT not Used";
  return RestApiReturn;
}
JsonVariant cbRestApiPath (JsonVariant &var) {
  RestApiReturn.clear ();
  RestApiReturn["message"] = "PATCH not used";
  return RestApiReturn;
}
JsonVariant cbRestApiDelete (JsonVariant &var) {
  RestApiReturn.clear ();
  RestApiReturn["message"] = "DELETE not used";
  return RestApiReturn;
}

//#######################################################
// Setup
//#######################################################
void setup () {
  // SPI.begin();

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

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Stepper
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //  driver.begin();             // Initiate pins and registeries
  //  driver.rms_current(600);    // Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  ////  driver.en_pwm_mode(1);      // Enable extremely quiet stepping
  //  driver.pwm_autoscale(1);
  //  driver.microsteps(16);

  Feeder.setMaxSpeed (4000);
  Feeder.setAcceleration (1000);
  Feeder.setEnablePin (EN_PIN);
  Feeder.setPinsInverted (false, false, true);
  Feeder.disableOutputs ();
  FeederEnable = false;
  FeedingExt = false;
  FeederSpeed = false;

  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++
  // Webserver
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++

}

//#######################################################
// Loop
//#######################################################
void loop () {
  Server.handle ();
  tm CurrentTime = Server.getTimeStruct ();
  bool Feeding = CurrentTime.tm_hour == FeedingHour && CurrentTime.tm_min == FeedingMinute && Server.timeIsValid ();
  if (FeederSpeed) {
    FeedingExt = false;
    Feeder.runSpeed ();
  } else {
    if ((Feeding || FeedingExt) && !FeedingOld) {
      Debug.println (FLAG_LOOP, true, "roor", "loop", "Go Feeding");
      FeedingExt = false;
      Feeder.move (FeedingStepps);
      Feeder.enableOutputs ();
      FeederEnable = true;
    }

    if (Feeder.distanceToGo () == 0 && FeederEnable && !FeederSpeed) {
      Debug.println (FLAG_LOOP, true, "roor", "loop", "Done Feeding");
      Feeder.disableOutputs ();
      FeederEnable = false;
    }

    Feeder.run ();
  }

  // Save old states
  FeedingOld = Feeding;
}