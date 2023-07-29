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
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_PwmOutput.h>

#define CURRENT_AH_STEPS 0.001
#define POWER_AH_STEPS 0.001

namespace JCA {
  namespace FNC {
    typedef enum Charger_State_T {
      IDLE,
      CHARGE_CURRENT,
      CHARGE_VOLTAGE,
      WAIT_CHARGE,
      WAIT_DISCHARCH,
      DISCHARGE,
      FAULT
    } Charger_State;

    class Charger : public Parent {
    private:
      /* #region(collapsed) Datapoint description */
      static const char *AccuVoltageMax_Name;
      static const char *AccuVoltageMax_Text;
      static const char *AccuVoltageMax_Unit;
      static const char *AccuVoltageMax_Comment;
      static const char *AccuVoltageMin_Name;
      static const char *AccuVoltageMin_Text;
      static const char *AccuVoltageMin_Unit;
      static const char *AccuVoltageMin_Comment;
      static const char *AccuChargeCurrent_Name;
      static const char *AccuChargeCurrent_Text;
      static const char *AccuChargeCurrent_Unit;
      static const char *AccuChargeCurrent_Comment;
      static const char *AccuDischargeVoltage_Name;
      static const char *AccuDischargeVoltage_Text;
      static const char *AccuDischargeVoltage_Unit;
      static const char *AccuDischargeVoltage_Comment;
      static const char *AccuDischargeCurrent_Name;
      static const char *AccuDischargeCurrent_Text;
      static const char *AccuDischargeCurrent_Unit;
      static const char *AccuDischargeCurrent_Comment;
      static const char *ChargeEndCurrent_Name;
      static const char *ChargeEndCurrent_Text;
      static const char *ChargeEndCurrent_Unit;
      static const char *ChargeEndCurrent_Comment;
      static const char *WaitDischarge_Name;
      static const char *WaitDischarge_Text;
      static const char *WaitDischarge_Unit;
      static const char *WaitDischarge_Comment;
      static const char *RechargeVoltage_Name;
      static const char *RechargeVoltage_Text;
      static const char *RechargeVoltage_Unit;
      static const char *RechargeVoltage_Comment;
      static const char *DoCheck_Name;
      static const char *DoCheck_Text;
      static const char *DoCheck_Comment;
      static const char *DoCheck_TextOn;
      static const char *DoCheck_TextOff;
      static const char *DoCharge_Name;
      static const char *DoCharge_Text;
      static const char *DoCharge_Comment;
      static const char *DoCharge_TextOn;
      static const char *DoCharge_TextOff;
      static const char *Fault_Name;
      static const char *Fault_Text;
      static const char *Fault_Comment;
      static const char *Fault_TextOn;
      static const char *Fault_TextOff;
      static const char *AccuVoltage_Name;
      static const char *AccuVoltage_Text;
      static const char *AccuVoltage_Unit;
      static const char *AccuVoltage_Comment;
      static const char *Current_Name;
      static const char *Current_Text;
      static const char *Current_Unit;
      static const char *Current_Comment;
      static const char *ChargedAH_Name;
      static const char *ChargedAH_Text;
      static const char *ChargedAH_Unit;
      static const char *ChargedAH_Comment;
      static const char *ChargedWH_Name;
      static const char *ChargedWH_Text;
      static const char *ChargedWH_Unit;
      static const char *ChargedWH_Comment;
      static const char *DischargedAH_Name;
      static const char *DischargedAH_Text;
      static const char *DischargedAH_Unit;
      static const char *DischargedAH_Comment;
      static const char *DischargedWH_Name;
      static const char *DischargedWH_Text;
      static const char *DischargedWH_Unit;
      static const char *DischargedWH_Comment;
      static const char *ChargeSP_Name;
      static const char *ChargeSP_Text;
      static const char *ChargeSP_Unit;
      static const char *ChargeSP_Comment;
      static const char *DischargeSP_Name;
      static const char *DischargeSP_Text;
      static const char *DischargeSP_Unit;
      static const char *DischargeSP_Comment;
      static const char *ChargeState_Name;
      static const char *ChargeState_Text;
      static const char *ChargeState_Comment;
      static const char *ChargeState_Case_Undef;
      static const char *ChargeState_Case_Idle;
      static const char *ChargeState_Case_ChargeCurrent;
      static const char *ChargeState_Case_ChargeVoltage;
      static const char *ChargeState_Case_WaitCharge;
      static const char *ChargeState_Case_WaitDischarge;
      static const char *ChargeState_Case_Discharge;
      static const char *ChargeState_Case_Fault;
      static const char *DischargeEndCurrent_Name;
      static const char *DischargeEndCurrent_Text;
      static const char *DischargeEndCurrent_Unit;
      static const char *DischargeEndCurrent_Comment;
      static const char *DischargeVoltage_Name;
      static const char *DischargeVoltage_Text;
      static const char *DischargeVoltage_Unit;
      static const char *DischargeVoltage_Comment;
      /* #endregion */

      static const float CurrentHyst;
      static const float VoltageHyst;
      static const float OutputStep;
      static const uint16_t UpdateInterval;

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
      INA219 *Sensor;
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
      float AccuVoltage;
      float Current;
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
      uint32_t LastMillis;
      uint32_t UpdateMillis;
      uint32_t StepDelay;
      float CurrentStep;
      float PowerStep;
      bool DischargeHold;
      float DischargeSave;

    public:
      Charger (INA219 *_Sensor, uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name, JCA::SYS::PwmOutput *_Output);
      bool init ();
      void update (struct tm &_Time);
    };
  }
}

#endif