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
#include <JCA_SYS_Functions.h>
#include <JCA_SYS_PwmOutput.h>

#define CURRENT_AH_STEPS 0.001
#define POWER_AH_STEPS 0.001

using namespace JCA::SYS;
namespace JCA {
  namespace FNC {
    typedef enum SolarCharger_State_T {
      IDLE,
      CHARGE_MPPT,
      CHARGE_CURRENT,
      CHARGE_VOLTAGE,
      TRACK_MPPT,
      FULL
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
      static const char *SolarVoltageOn_Name;
      static const char *SolarVoltageOn_Text;
      static const char *SolarVoltageOn_Unit;
      static const char *SolarVoltageOn_Comment;
      static const char *SolarVoltageOff_Name;
      static const char *SolarVoltageOff_Text;
      static const char *SolarVoltageOff_Unit;
      static const char *SolarVoltageOff_Comment;
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
      static const char *AutoStart_Name;
      static const char *AutoStart_Text;
      static const char *AutoStart_Comment;
      static const char *AutoStart_TextOn;
      static const char *AutoStart_TextOff;
      static const char *Charging_Name;
      static const char *Charging_Text;
      static const char *Charging_Comment;
      static const char *Charging_TextOn;
      static const char *Charging_TextOff;
      static const char *MpptVoltage_Name;
      static const char *MpptVoltage_Text;
      static const char *MpptVoltage_Unit;
      static const char *MpptVoltage_Comment;
      static const char *MpptVoltageMin_Name;
      static const char *MpptVoltageMin_Text;
      static const char *MpptVoltageMin_Unit;
      static const char *MpptVoltageMin_Comment;
      static const char *MpptVoltageMax_Name;
      static const char *MpptVoltageMax_Text;
      static const char *MpptVoltageMax_Unit;
      static const char *MpptVoltageMax_Comment;
      static const char *MpptStepVoltage_Name;
      static const char *MpptStepVoltage_Text;
      static const char *MpptStepVoltage_Unit;
      static const char *MpptStepVoltage_Comment;
      static const char *MpptStepTime_Name;
      static const char *MpptStepTime_Text;
      static const char *MpptStepTime_Unit;
      static const char *MpptStepTime_Comment;
      static const char *MpptCheckDelay_Name;
      static const char *MpptCheckDelay_Text;
      static const char *MpptCheckDelay_Unit;
      static const char *MpptCheckDelay_Comment;
      static const char *DutyCycle_Name;
      static const char *DutyCycle_Text;
      static const char *DutyCycle_Unit;
      static const char *DutyCycle_Comment;
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

      static const float MaxOutputDutyCycle; // X < 100.0%
      static const float MinDutyCycle;       // 0.05 < X < 1.0
      static const float MaxDutyCycle;       // 1.0 < X < 2.0
      static const uint32_t WaitStart;       // Start Delay in Milliseconds

      // Parent Functions
      void createConfigValues (JsonObject &_Values);
      void createDataValues (JsonObject &_Values);
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);

      void writeSetupConfig (File _SetupFile);
      void writeSetupData (File _SetupFile);
      void writeSetupCmdInfo (File _SetupFile);

      // Internal Functions
      bool controlSolarVoltage(float _SetPoint);
      bool controlAccuVoltage(float _SetPoint);
      bool controlAccuCurrent(float _SetPoint);

      // Hardware
      PwmOutput *Output;
      uint8_t PinBoost;
      uint8_t PinBuck;
      ValueCallback getAccuVoltageCB;
      ValueCallback getAccuCurrentCB;
      ValueCallback getSolarVoltageCB;
      ValueCallback getSolarCurrentCB;
      float CurrentHyst;
      float VoltageHyst;
      float OutputStep;
      uint16_t UpdateInterval;
      float MaxMeanCurrent;
      float MaxPeakCurrent;

      // Konfig
      float AccuVoltageMax;
      float AccuCurrentMax;
      float AccuChargeEndCurrent;
      float AccuRechargeVoltage;
      float SolarVoltageOn;
      float SolarVoltageOff;
      float MpptVoltageMin;
      float MpptVoltageMax;
      float MpptStepVoltage;
      uint32_t MpptStepTime;
      uint16_t MpptCheckDelay;
      bool AutoStart;

      // Daten
      float AccuVoltage;
      float AccuCurrent;
      float AccuPower;
      float AccuEnergie15m;
      float AccuEnergie1h;
      float AccuEnergie1d;
      float SolarVoltage;
      float SolarCurrent;
      float SolarPower;
      float SolarEnergie15m;
      float SolarEnergie1h;
      float SolarEnergie1d;
      float DutyCycle;
      bool Charging;
      SolarCharger_State_T ChargeState;

      // Intern
      uint8_t Resolution;
      uint32_t Frequency;
      uint32_t StartDelay;
      uint32_t WaitFull;
      bool InitDone;
      float SumAccuEnergie15m;
      float SumAccuEnergie1h;
      float SumAccuEnergie1d;
      float SumSolarEnergie15m;
      float SumSolarEnergie1h;
      float SumSolarEnergie1d;
      // ... MPPT-Data
      float MpptMaxPower;
      float MpptMaxVoltage;
      float MpptSumPower;
      float MpptVoltage;
      bool MpptInRange;
      // ... Step Data
      SolarCharger_State_T LastState;
      uint32_t StepDelay;
      uint32_t MpptDelay;
      // ... Init
      int Last15m;
      int Last1h;
      int Last1d;
      uint32_t LastMillis;
      uint32_t UpdateMillis;

    public:
      SolarCharger (uint8_t _PinBoost, uint8_t _PinBuck, const char *_Name, PwmOutput *_Output, float _MaxMeanCurrent, float _MaxPeakCurrent, float _CurrentHyst = 0.01, float _VoltageHyst = 0.01, float _OutputStep = 0.0001, uint16_t _UpdateInterval = 100);
      bool init (ValueCallback _GetSolarVoltageCB, ValueCallback _GetSolarCurrentCB, ValueCallback _GetAccuVoltageCB, ValueCallback _GetAccuCurrentCB, struct tm &_Time);
      void update (struct tm &_Time);
    };
  }
}

#endif