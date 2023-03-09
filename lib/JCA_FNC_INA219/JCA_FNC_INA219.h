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

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class INA219 : public Parent {
    private:
      // Datapoint description
      static const char *ReadInterval_Name;
      static const char *ReadInterval_Text;
      static const char *ReadInterval_Unit;
      static const char *ReadInterval_Comment;
      static const char *PowerPlus_Name;
      static const char *PowerPlus_Text;
      static const char *PowerPlus_Unit;
      static const char *PowerPlus_Comment;
      static const char *VoltagePlus_Name;
      static const char *VoltagePlus_Text;
      static const char *VoltagePlus_Unit;
      static const char *VoltagePlus_Comment;
      static const char *PowerMinus_Name;
      static const char *PowerMinus_Text;
      static const char *PowerMinus_Unit;
      static const char *PowerMinus_Comment;
      static const char *VoltageMinus_Name;
      static const char *VoltageMinus_Text;
      static const char *VoltageMinus_Unit;
      static const char *VoltageMinus_Comment;
      static const char *Current_Name;
      static const char *Current_Text;
      static const char *Current_Unit;
      static const char *Current_Comment;

      // Parent Functions
      void createConfigValues (JsonObject &_Values);
      void createDataValues (JsonObject &_Values);
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);

      void writeSetupConfig (File _SetupFile);
      void writeSetupData (File _SetupFile);
      void writeSetupCmdInfo (File _SetupFile);

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
      int32_t Resend;
      uint32_t LastMillis;
      float ShuntVoltage_mV;
      float BusVoltage_V;
      float Current_mA;

    public:
      INA219 (TwoWire *_Wire, const uint8_t _Addr, const char *_Name);
      INA219 (const uint8_t _Addr, const char *_Name);
      INA219 (const char *_Name);
      bool init();
      void update (struct tm &_Time);
      float getPowerPlus ();
      float getVoltagePlus ();
      float getPowerMinus ();
      float getVoltageMinus ();
      float getCurrent ();
    };
  }
}

#endif