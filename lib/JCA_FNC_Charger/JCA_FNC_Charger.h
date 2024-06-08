/**
 * @file JCA_FNC_Charger.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to charge and discharge an Accu with Capacity calculation
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#ifndef _JCA_FNC_CHARGER_
#define _JCA_FNC_CHARGER_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_INA219.h>
#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_PwmOutput.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagListUInt8.h>
#include <JCA_TAG_TagUInt16.h>

#define CURRENT_AH_STEPS 0.001
#define POWER_AH_STEPS 0.001

namespace JCA {
  namespace FNC {
    enum Charger_State_T : uint8_t {
      IDLE,
      CHARGE_CURRENT,
      CHARGE_VOLTAGE,
      WAIT_CHARGE,
      WAIT_DISCHARCH,
      DISCHARGE,
      FAULT
    };

    class Charger : public FuncParent {
    private:
      static const char *ClassName;
      static const char *SetupTagType;
      static const char *SetupTagChargePin;
      static const char *SetupTagDischargePin;
      static const char *SetupTagOutputPWN;

      static const float CurrentHyst;
      static const float VoltageHyst;
      static const float OutputStep;
      static const uint16_t UpdateInterval;

      // Hardware
      JCA::SYS::PwmOutput *Output;
      uint8_t PinCharge;
      uint8_t PinDischarge;

      // Konfig
      float AccuVoltageMax;
      float AccuVoltageMin;
      float AccuChargeCurrent;
      float AccuDischargeVoltage;
      float AccuDischargeCurrent;
      float ChargeEndCurrent;
      float DischargeEndCurrent;
      uint16_t WaitDischarge;
      float RechargeVoltage;

      // Daten
      bool DoCharge;
      bool DoCheck;
      bool Fault;
      float AccuVoltage;
      float Current;
      float Power;
      float ChargedAH;
      float ChargedWH;
      float DischargedAH;
      float DischargedWH;
      float DischargeVoltage;
      float ChargeSP;
      float DischargeSP;
      Charger_State_T ChargeState;
      uint32_t FaultDelay;
      Charger_State_T FaultState;

      // Intern
      uint8_t Resolution;
      uint32_t Frequency;
      float DutyScale;
      unsigned long LastMillis;
      unsigned long UpdateMillis;
      uint32_t StepDelay;
      float CurrentStep;
      float PowerStep;
      bool DischargeHold;
      float DischargeSave;

    public:
      Charger (uint8_t _PinCharge, uint8_t _PinDischarge, String _Name, JCA::SYS::PwmOutput *_Output);
      bool init ();
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif