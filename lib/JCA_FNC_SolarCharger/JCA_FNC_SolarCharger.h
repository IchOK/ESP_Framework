/**
 * @file JCA_FNC_SolarCharger.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to charge accu with Solarpower using MPPT Tacking
 * @version 0.1
 * @date 2023-07-28
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#ifndef _JCA_FNC_SOLARCHARGER_
#define _JCA_FNC_SOLARCHARGER_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_PwmOutput.h>

#define CURRENT_AH_STEPS 0.001
#define POWER_AH_STEPS 0.001

namespace JCA {
  namespace FNC {
    typedef std::function<float (void)> ValueCallback;
    typedef enum SolarCharger_State_T {
      IDLE,
      CHARGE_MPPT,
      CHARGE_CURRENT,
      CHARGE_VOLTAGE,
      TRACK_MPPT,
      FAULT
    } SolarCharger_State;

    class SolarCharger : public Parent {
    private:
      /* #region(collapsed) Datapoint description */
      static const char *AccuVoltageMax_Name;
      static const char *AccuVoltageMax_Text;
      static const char *AccuVoltageMax_Unit;
      static const char *AccuVoltageMax_Comment;
      static const char *AccuCurrentMax_Name;
      static const char *AccuCurrentMax_Text;
      static const char *AccuCurrentMax_Unit;
      static const char *AccuCurrentMax_Comment;
      static const char *AccuChargeEndCurrent_Name;
      static const char *AccuChargeEndCurrent_Text;
      static const char *AccuChargeEndCurrent_Unit;
      static const char *AccuChargeEndCurrent_Comment;
      static const char *AccuRechargeVoltage_Name;
      static const char *AccuRechargeVoltage_Text;
      static const char *AccuRechargeVoltage_Unit;
      static const char *AccuRechargeVoltage_Comment;
      static const char *AccuVoltage_Name;
      static const char *AccuVoltage_Text;
      static const char *AccuVoltage_Unit;
      static const char *AccuVoltage_Comment;
      static const char *AccuCurrent_Name;
      static const char *AccuCurrent_Text;
      static const char *AccuCurrent_Unit;
      static const char *AccuCurrent_Comment;
      static const char *AccuPower_Name;
      static const char *AccuPower_Text;
      static const char *AccuPower_Unit;
      static const char *AccuPower_Comment;
      static const char *AccuEnergie15m_Name;
      static const char *AccuEnergie15m_Text;
      static const char *AccuEnergie15m_Unit;
      static const char *AccuEnergie15m_Comment;
      static const char *AccuEnergie1h_Name;
      static const char *AccuEnergie1h_Text;
      static const char *AccuEnergie1h_Unit;
      static const char *AccuEnergie1h_Comment;
      static const char *AccuEnergie1d_Name;
      static const char *AccuEnergie1d_Text;
      static const char *AccuEnergie1d_Unit;
      static const char *AccuEnergie1d_Comment;
      static const char *SolarVoltageMin_Name;
      static const char *SolarVoltageMin_Text;
      static const char *SolarVoltageMin_Unit;
      static const char *SolarVoltageMin_Comment;
      static const char *MpptInterval_Name;
      static const char *MpptInterval_Text;
      static const char *MpptInterval_Unit;
      static const char *MpptInterval_Comment;
      static const char *SolarVoltage_Name;
      static const char *SolarVoltage_Text;
      static const char *SolarVoltage_Unit;
      static const char *SolarVoltage_Comment;
      static const char *SolarCurrent_Name;
      static const char *SolarCurrent_Text;
      static const char *SolarCurrent_Unit;
      static const char *SolarCurrent_Comment;
      static const char *SolarPower_Name;
      static const char *SolarPower_Text;
      static const char *SolarPower_Unit;
      static const char *SolarPower_Comment;
      static const char *SolarEnergie15m_Name;
      static const char *SolarEnergie15m_Text;
      static const char *SolarEnergie15m_Unit;
      static const char *SolarEnergie15m_Comment;
      static const char *SolarEnergie1h_Name;
      static const char *SolarEnergie1h_Text;
      static const char *SolarEnergie1h_Unit;
      static const char *SolarEnergie1h_Comment;
      static const char *SolarEnergie1d_Name;
      static const char *SolarEnergie1d_Text;
      static const char *SolarEnergie1d_Unit;
      static const char *SolarEnergie1d_Comment;
      static const char *Fault_Name;
      static const char *Fault_Text;
      static const char *Fault_Comment;
      static const char *Fault_TextOn;
      static const char *Fault_TextOff;
      static const char *ChargeState_Name;
      static const char *ChargeState_Text;
      static const char *ChargeState_Comment;
      static const char *ChargeState_Case_Undef;
      static const char *ChargeState_Case_Idle;
      static const char *ChargeState_Case_ChargeMppt;
      static const char *ChargeState_Case_ChargeCurrent;
      static const char *ChargeState_Case_ChargeVoltage;
      static const char *ChargeState_Case_TrackMppt;
      static const char *ChargeState_Case_Fault;
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
      JCA::SYS::PwmOutput *Output;
      uint8_t PinBoost;
      uint8_t PinBuck;
      ValueCallback getAccuVoltageCB;
      ValueCallback getAccuCurrentCB;
      ValueCallback getSolarVoltageCB;
      ValueCallback getSolarCurrentCB;

      // Konfig
      float AccuVoltageMax;
      float AccuCurrentMax;
      float AccuChargeEndCurrent;
      float AccuRechargeVoltage;
      float SolarVoltageMin;
      uint32_t MpptInterval;

      // Daten
      float AccuVoltage;
      float AccuCurrent;
      float AccuPower;
      float SolarVoltage;
      float SolarCurrent;
      float SolarPower;
      SolarCharger_State_T ChargeState;

      // Intern
      uint32_t MpptDelay;
      uint32_t FaultDelay;
      SolarCharger_State_T FaultState;
      uint8_t Resolution;
      uint32_t Frequency;
      float DutyScale;
      uint32_t LastMillis;
      uint32_t UpdateMillis;
      uint32_t StepDelay;
      float CurrentStep;
      float PowerStep;
      float BoostDuty;
      float BuckDuty;
      bool BoostMode;

    public:
      SolarCharger (uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name, JCA::SYS::PwmOutput *_Output);
      bool init ();
      void update (struct tm &_Time);
    };
  }
}

#endif