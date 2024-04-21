/**
 * @file JCA_FNC_INA219.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to get Data from INA219 Sensor
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#ifndef _JCA_FNC_INA219_
#define _JCA_FNC_INA219_

#include <ArduinoJson.h>
#include <INA219_WE.h>

#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagUInt16.h>
#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class INA219 : public FuncParent {
    private:
      // Hardware
      INA219_WE Sensor;

      // Konfig
      uint16_t ReadInterval;

      // Daten
      float PowerPlus;
      float VoltagePlus;
      float PowerMinus;
      float VoltageMinus;
      float Current;

      // Intern
      uint32_t LastMillis;
      uint32_t UpdateMillis;
      float ShuntVoltage_mV;
      float BusVoltage_V;
      float Current_mA;
      void createTags();

    public:
      INA219 (TwoWire *_Wire, const uint8_t _Addr, const char *_Name);
      INA219 (const uint8_t _Addr, const char *_Name);
      INA219 (const char *_Name);
      bool init();
      void update (struct tm &_Time);
      void setInterval (uint16_t _ReadInterval);
      float getPowerPlus ();
      float getVoltagePlus ();
      float getPowerMinus ();
      float getVoltageMinus ();
      float getCurrent ();
    };
  }
}

#endif