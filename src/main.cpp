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
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_TimerESP32.h>

// Project function
#include <JCA_FNC_AcDimmers.h>

using namespace JCA::IOT;
using namespace JCA::SYS;
using namespace JCA::FNC;
#define STAT_PIN LED_BUILTIN

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Custom Code
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++
AcDimmersTriggers_T *IntTriggers;
bool IRAM_ATTR timerInterrupt (void *_Args) {
  AcDimmersTriggers_T *Triggers = static_cast<AcDimmersTriggers_T *> (_Args);
  digitalWrite(STAT_PIN, !digitalRead(STAT_PIN));
  Triggers->Index++;
  if (Triggers->Index < Triggers->Count) {
    TimerESP32_Handler.setAlarmValue (Triggers->Timer, Triggers->Triggers[Triggers->Index].Delay);
    return false;
  } else {
    Triggers->Index = 0;
    TimerESP32_Handler.setCounterValue (Triggers->Timer, 0);
    TimerESP32_Handler.setAlarmValue (Triggers->Timer, Triggers->Triggers[Triggers->Index].Delay);

    //    TimerESP32_Handler.disableIntr (Triggers->Timer);
    //    TimerESP32_Handler.pause (Triggers->Timer);
    return false;
  }
}

void setupConfig () {
  IntTriggers = new AcDimmersTriggers_T;
  IntTriggers->Triggers = new AcDimmersTriggerPair_T[10];
  IntTriggers->Count = 6;
  IntTriggers->Index = 6;
  IntTriggers->Triggers[0].Delay = 1000000;
  IntTriggers->Triggers[1].Delay = 1500000;
  IntTriggers->Triggers[2].Delay = 2000000;
  IntTriggers->Triggers[3].Delay = 3500000;
  IntTriggers->Triggers[4].Delay = 5000000;
  IntTriggers->Triggers[5].Delay = 10000000;
  IntTriggers->Timer = TimerESP32_Handler.addTimer();
  TimerESP32_Handler.isrCallbackAdd (IntTriggers->Timer, timerInterrupt, IntTriggers);

  //  TimerESP32_Handler.setMicros (IntTriggers->Timer, IntTriggers->Triggers[0].Delay);
  //  TimerESP32_Handler.addCallback (IntTriggers->Timer, 2^60, timerInterrupt, IntTriggers);
}
void loopConfig () {
  if (IntTriggers->Index >= IntTriggers->Count) {
//    digitalWrite (STAT_PIN, !digitalRead (STAT_PIN));
//    delay (200);
//    digitalWrite (STAT_PIN, !digitalRead (STAT_PIN));
//    delay (200);
//    digitalWrite (STAT_PIN, !digitalRead (STAT_PIN));
//    delay (200);
//    digitalWrite (STAT_PIN, !digitalRead (STAT_PIN));
    TimerESP32_Handler.setCounterValue (IntTriggers->Timer, 0);
    TimerESP32_Handler.setAlarmValue (IntTriggers->Timer, IntTriggers->Triggers[0].Delay);
    TimerESP32_Handler.enableIntr (IntTriggers->Timer);
    TimerESP32_Handler.start (IntTriggers->Timer);
    IntTriggers->Index = 0;
  }

//  if (TimerESP32_Handler.getCounter (IntTriggers->Timer) > 20000000) {
//    for (int i = 0; i < 20; i++) {
//      delay (200);
//      digitalWrite (STAT_PIN, !digitalRead (STAT_PIN));
//    }
//    TimerESP32_Handler.restart (IntTriggers->Timer, IntTriggers->Triggers[0].Delay);
//  }
}
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
  DebugFlags |= FLAG_PROTOCOL;
  // DebugFlags |= FLAG_DATA;
  Debug.init (DebugFlags, SERIAL_BAUD);

  setupConfig();
}

// #######################################################
//  Loop
// #######################################################
int8_t LastSeconds = 0;
void loop () {
  loopConfig();
}