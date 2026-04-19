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
#ifndef ONEWIRE_ADDRSIZE
#define ONEWIRE_ADDRSIZE 8
#endif

#ifndef _JCA_FNC_DS18B20_
#define _JCA_FNC_DS18B20_

#include <ArduinoJson.h>
#include <OneWire.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagArrayUInt8.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    enum DS18B20_Type_T {
      TYPE_S = 0x10,
      TYPE_B = 0x28,
      TYPE_22 = 0x22,
      TYPE_25 = 0x3B,
      TYPE_DS28 = 0x42
    };
    enum DS18B20_Cmd_T {
      CONV = 0x44,
      READ = 0xBE,
      WRITE = 0x4E,
      COPY = 0x48,
      RECALL = 0xB8,
      POWER = 0xB4
    };

    class DS18B20 : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagRefName;

      // Hardware
      OneWire *Wire;

      // Konfig
      float Filter;
      uint8_t Addr[ONEWIRE_ADDRSIZE];
      uint16_t ReadInterval;
      // Zwei-Punkt-Kalibrierung (linear): Value = CalRefLow + (RawValue - CalRawLow) * (CalRefHigh - CalRefLow) / (CalRawHigh - CalRawLow)
      bool CalEnable;
      float CalRawLow;
      float CalRefLow;
      float CalRawHigh;
      float CalRefHigh;

      // Daten
      float Value;
      float RawValue;

      // Intern
      uint8_t Raw[12];
      int32_t Resend;
      bool ReadData;
      bool AddrIsValid;
      unsigned long LastMillis;

      void addrChanged ();
      bool validFamily (uint8_t _Addr[ONEWIRE_ADDRSIZE]);
      bool validAddr (uint8_t _Addr[ONEWIRE_ADDRSIZE]);
      float applyCalibration (float _Raw) const;

    public:
      DS18B20 (OneWire *_Wire, String _Name);
      ~DS18B20 () {;};
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
      static void GetSetupSchema (JsonObject &_Schema);
    };
  }
}

#endif