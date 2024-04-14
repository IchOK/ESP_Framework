/**
 * @file JCA_FNC_DS18B20.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to get Data from DS18B20 Sensor
 * @version 1.0
 * @date 2023-01-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_DS18B20_
#define _JCA_FNC_DS18B20_

#include <ArduinoJson.h>
#include <OneWire.h>
#include <time.h>

#include <JCA_FNC_ElementTagUInt16.h>
#include <JCA_FNC_ElementTagFloat.h>
#include <JCA_FNC_ElementTagArrayUInt8.h>
#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    typedef enum DS18B20_Type_T {
      TYPE_S = 0x10,
      TYPE_B = 0x28,
      TYPE_22 = 0x22
    } DS18B20_Type;
    typedef enum DS18B20_Cmd_T {
      CONV = 0x44,
      READ = 0xBE,
      WRITE = 0x4E,
      COPY = 0x48,
      RECALL = 0xB8,
      POWER = 0xB4
    } DS18B20_Cmd;

    class DS18B20 : public Parent {
    private:
      // Hardware
      OneWire *Wire;

      // Konfig
      float Filter;
      uint8_t Addr[8];
      uint16_t ReadInterval;

      // Daten
      float Value;

      // Intern
      uint8_t Raw[12];
      int32_t Resend;
      bool ReadData;
      uint32_t LastMillis;

    public:
      DS18B20 (OneWire *_Wire, const char *_Name);
      void update (struct tm &_Time);
      float getValue ();
    };
  }
}

#endif