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

//-------------------------------------------------------
// Stepper
//-------------------------------------------------------
#define EN_PIN D1   // Enable
#define STEP_PIN D2 // Step
#define DIR_PIN D3  // Direction
#define STAT_PIN LED_BUILTIN
#define SW_RX D7      // TMC2208/TMC2224 SoftwareSerial receive pin
#define SW_TX D8      // TMC2208/TMC2224 SoftwareSerial transmit pin
#define R_SENSE 0.11f // Match to your driver

constexpr uint32_t steps_per_mm = 80;

// TMC2208Stepper driver = TMC2208Stepper(SW_RX, SW_TX, R_SENSE); // Software serial
// TMC2660Stepper driver = TMC2660Stepper(CS_PIN, R_SENSE); // Hardware SPI
AccelStepper stepper = AccelStepper (stepper.DRIVER, STEP_PIN, DIR_PIN);

//-------------------------------------------------------
// JCA IOT Functions
//-------------------------------------------------------
Webserver Server;

//#######################################################
// Setup
//#######################################################
void setup () {
  // SPI.begin();

  pinMode (STAT_PIN, OUTPUT);
  digitalWrite (STAT_PIN, LOW);

  Debug.init (FLAG_ERROR | FLAG_SETUP);

  //-------------------------------------------------------
  // Filesystem
  //-------------------------------------------------------
  if (!LittleFS.begin ()) {
#if defined(DEBUG_SETUP)
    Serial.println ("LITTLEFS Mount Failed");
    return;
#endif
  }

  //-------------------------------------------------------
  // JCA IOT Functions - WiFiConnect
  //-------------------------------------------------------
  Server.init ();

  //-------------------------------------------------------
  // Stepper
  //-------------------------------------------------------
  //  driver.begin();             // Initiate pins and registeries
  //  driver.rms_current(600);    // Set stepper current to 600mA. The command is the same as command TMC2130.setCurrent(600, 0.11, 0.5);
  ////  driver.en_pwm_mode(1);      // Enable extremely quiet stepping
  //  driver.pwm_autoscale(1);
  //  driver.microsteps(16);

  stepper.setMaxSpeed (50 * steps_per_mm);       // 100mm/s @ 80 steps/mm
  stepper.setAcceleration (1000 * steps_per_mm); // 2000mm/s^2
  stepper.setEnablePin (EN_PIN);
  stepper.setPinsInverted (false, false, true);
  stepper.enableOutputs ();

  //-------------------------------------------------------
  // Webserver
  //-------------------------------------------------------
}

//#######################################################
// Loop
//#######################################################
void loop () {
  Server.handle();
  /*
    if (stepper.distanceToGo() == 0) {
      digitalWrite(STAT_PIN, LOW);
      stepper.disableOutputs();
      delay(100);
      digitalWrite(STAT_PIN, LOW);
      stepper.move(100*steps_per_mm); // Move 100mm
      stepper.enableOutputs();
    }
    stepper.run();
  */
  stepper.setSpeed (stepper.maxSpeed ());
  stepper.runSpeed ();
}