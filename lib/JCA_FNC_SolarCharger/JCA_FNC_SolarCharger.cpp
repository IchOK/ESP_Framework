/**
 * @file JCA_FNC_SolarCharger.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to charge accu with Solarpower using MPPT Tacking
 * @version 0.1
 * @date 2023-07-28
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#include <JCA_FNC_SolarCharger.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {

    /* #region(collapsed) Datapoint description */
    const char *SolarCharger::AccuVoltageMax_Name = "AccuVoltageMax";
    const char *SolarCharger::AccuVoltageMax_Text = "Maximale Akku Ladespannung";
    const char *SolarCharger::AccuVoltageMax_Unit = "V";
    const char *SolarCharger::AccuVoltageMax_Comment = nullptr;
    const char *SolarCharger::AccuCurrentMax_Name = "AccuCurrentMax";
    const char *SolarCharger::AccuCurrentMax_Text = "Maximaler Akku Ladestrom";
    const char *SolarCharger::AccuCurrentMax_Unit = "A";
    const char *SolarCharger::AccuCurrentMax_Comment = nullptr;
    const char *SolarCharger::AccuChargeEndCurrent_Name = "AccuChargeEndCurrent";
    const char *SolarCharger::AccuChargeEndCurrent_Text = "Laden Beendet Strom";
    const char *SolarCharger::AccuChargeEndCurrent_Unit = "A";
    const char *SolarCharger::AccuChargeEndCurrent_Comment = nullptr;
    const char *SolarCharger::AccuRechargeVoltage_Name = "AccuRechargeVoltage";
    const char *SolarCharger::AccuRechargeVoltage_Text = "Spannungs Nachladen";
    const char *SolarCharger::AccuRechargeVoltage_Unit = "V";
    const char *SolarCharger::AccuRechargeVoltage_Comment = nullptr;
    const char *SolarCharger::AccuVoltage_Name = "AccuVoltage";
    const char *SolarCharger::AccuVoltage_Text = "Akku Spannung";
    const char *SolarCharger::AccuVoltage_Unit = "V";
    const char *SolarCharger::AccuVoltage_Comment = nullptr;
    const char *SolarCharger::AccuCurrent_Name = "AccuCurrent";
    const char *SolarCharger::AccuCurrent_Text = "Akku Strom";
    const char *SolarCharger::AccuCurrent_Unit = "A";
    const char *SolarCharger::AccuCurrent_Comment = nullptr;
    const char *SolarCharger::AccuPower_Name = "AccuPower";
    const char *SolarCharger::AccuPower_Text = "Akku Leistung";
    const char *SolarCharger::AccuPower_Unit = "W";
    const char *SolarCharger::AccuPower_Comment = nullptr;
    const char *SolarCharger::AccuEnergie15m_Name = "AccuEnergie15m";
    const char *SolarCharger::AccuEnergie15m_Text = "geladene Energie 15 Min";
    const char *SolarCharger::AccuEnergie15m_Unit = "Wh";
    const char *SolarCharger::AccuEnergie15m_Comment = nullptr;
    const char *SolarCharger::AccuEnergie1h_Name = "AccuEnergie1h";
    const char *SolarCharger::AccuEnergie1h_Text = "geladene Energie 1 H";
    const char *SolarCharger::AccuEnergie1h_Unit = "Wh";
    const char *SolarCharger::AccuEnergie1h_Comment = nullptr;
    const char *SolarCharger::AccuEnergie1d_Name = "AccuEnergie1d";
    const char *SolarCharger::AccuEnergie1d_Text = "geladene Energie 1 Tag";
    const char *SolarCharger::AccuEnergie1d_Unit = "Wh";
    const char *SolarCharger::AccuEnergie1d_Comment = nullptr;
    const char *SolarCharger::SolarVoltageOn_Name = "SolarVoltageOn";
    const char *SolarCharger::SolarVoltageOn_Text = "Solar einschalt Spannung";
    const char *SolarCharger::SolarVoltageOn_Unit = "V";
    const char *SolarCharger::SolarVoltageOn_Comment = nullptr;
    const char *SolarCharger::SolarVoltageOff_Name = "SolarVoltageOff";
    const char *SolarCharger::SolarVoltageOff_Text = "Solar ausschalt Spannung";
    const char *SolarCharger::SolarVoltageOff_Unit = "V";
    const char *SolarCharger::SolarVoltageOff_Comment = nullptr;
    const char *SolarCharger::SolarVoltage_Name = "SolarVoltage";
    const char *SolarCharger::SolarVoltage_Text = "Solar Spannung";
    const char *SolarCharger::SolarVoltage_Unit = "V";
    const char *SolarCharger::SolarVoltage_Comment = nullptr;
    const char *SolarCharger::SolarCurrent_Name = "SolarCurrent";
    const char *SolarCharger::SolarCurrent_Text = "Solar Strom";
    const char *SolarCharger::SolarCurrent_Unit = "A";
    const char *SolarCharger::SolarCurrent_Comment = nullptr;
    const char *SolarCharger::SolarPower_Name = "SolarPower";
    const char *SolarCharger::SolarPower_Text = "Solar Leistung";
    const char *SolarCharger::SolarPower_Unit = "W";
    const char *SolarCharger::SolarPower_Comment = nullptr;
    const char *SolarCharger::SolarEnergie15m_Name = "SolarEnergie15m";
    const char *SolarCharger::SolarEnergie15m_Text = "bezogene Energie 15 Min";
    const char *SolarCharger::SolarEnergie15m_Unit = "Wh";
    const char *SolarCharger::SolarEnergie15m_Comment = nullptr;
    const char *SolarCharger::SolarEnergie1h_Name = "SolarEnergie1h";
    const char *SolarCharger::SolarEnergie1h_Text = "bezogene Energie 1 H";
    const char *SolarCharger::SolarEnergie1h_Unit = "Wh";
    const char *SolarCharger::SolarEnergie1h_Comment = nullptr;
    const char *SolarCharger::SolarEnergie1d_Name = "SolarEnergie1d";
    const char *SolarCharger::SolarEnergie1d_Text = "bezogene Energie 1 Tag";
    const char *SolarCharger::SolarEnergie1d_Unit = "Wh";
    const char *SolarCharger::SolarEnergie1d_Comment = nullptr;
    const char *SolarCharger::AutoStart_Name = "AutoStart";
    const char *SolarCharger::AutoStart_Text = "Laden automatisch beginnen";
    const char *SolarCharger::AutoStart_Comment = nullptr;
    const char *SolarCharger::AutoStart_TextOn = nullptr;
    const char *SolarCharger::AutoStart_TextOff = nullptr;
    const char *SolarCharger::Charging_Name = "Charging";
    const char *SolarCharger::Charging_Text = "Laden aktiv";
    const char *SolarCharger::Charging_Comment = nullptr;
    const char *SolarCharger::Charging_TextOn = nullptr;
    const char *SolarCharger::Charging_TextOff = nullptr;
    const char *SolarCharger::MpptVoltage_Name = "MpptVoltage";
    const char *SolarCharger::MpptVoltage_Text = "MPPT Spannung";
    const char *SolarCharger::MpptVoltage_Unit = "V";
    const char *SolarCharger::MpptVoltage_Comment = nullptr;
    const char *SolarCharger::MpptVoltageMin_Name = "MpptVoltageMin";
    const char *SolarCharger::MpptVoltageMin_Text = "MPPT Spannung Start";
    const char *SolarCharger::MpptVoltageMin_Unit = "V";
    const char *SolarCharger::MpptVoltageMin_Comment = nullptr;
    const char *SolarCharger::MpptVoltageMax_Name = "MpptVoltageMax";
    const char *SolarCharger::MpptVoltageMax_Text = "MPPT Spannung Ende";
    const char *SolarCharger::MpptVoltageMax_Unit = "V";
    const char *SolarCharger::MpptVoltageMax_Comment = nullptr;
    const char *SolarCharger::MpptStepVoltage_Name = "MpptStepVoltage";
    const char *SolarCharger::MpptStepVoltage_Text = "MPPT Schritt-Spannung";
    const char *SolarCharger::MpptStepVoltage_Unit = "V";
    const char *SolarCharger::MpptStepVoltage_Comment = nullptr;
    const char *SolarCharger::MpptStepTime_Name = "MpptStepTime";
    const char *SolarCharger::MpptStepTime_Text = "MPPT Schritt-Zeit";
    const char *SolarCharger::MpptStepTime_Unit = "ms";
    const char *SolarCharger::MpptStepTime_Comment = nullptr;
    const char *SolarCharger::MpptCheckDelay_Name = "MpptCheckDelay";
    const char *SolarCharger::MpptCheckDelay_Text = "MPPT Prüfintervall";
    const char *SolarCharger::MpptCheckDelay_Unit = "s";
    const char *SolarCharger::MpptCheckDelay_Comment = nullptr;
    const char *SolarCharger::DutyCycle_Name = "DutyCycle";
    const char *SolarCharger::DutyCycle_Text = "Verstärungsfaktor";
    const char *SolarCharger::DutyCycle_Unit = "";
    const char *SolarCharger::DutyCycle_Comment = nullptr;
    const char *SolarCharger::ChargeState_Name = "ChargeState";
    const char *SolarCharger::ChargeState_Text = "Status";
    const char *SolarCharger::ChargeState_Comment = nullptr;
    const char *SolarCharger::ChargeState_Case_Undef = "Undefiniert";
    const char *SolarCharger::ChargeState_Case_Idle = "Inaktiv";
    const char *SolarCharger::ChargeState_Case_ChargeMppt = "Laden-MPPT";
    const char *SolarCharger::ChargeState_Case_ChargeCurrent = "Laden-Strom";
    const char *SolarCharger::ChargeState_Case_ChargeVoltage = "Laden-Spannung";
    const char *SolarCharger::ChargeState_Case_TrackMppt = "Check-MPPT";
    const char *SolarCharger::ChargeState_Case_Fault = "Fehler";
    /* #endregion */

    const float SolarCharger::MaxOutputDutyCycle = 95.0;
    const float SolarCharger::MinDutyCycle = 0.1;
    const float SolarCharger::MaxDutyCycle = 1.8;
    const uint32_t SolarCharger::WaitStart = 5000;

    /**
     * @brief Construct a new SolarCharger::SolarCharger object
     *
     * @param _PinBoost Pin that is connected to the Enable in on the Stepper-Driver
     * @param _PinBuck Pin that is connected to the Step in on the Stepper-Driver
     * @param _Name Element Name inside the Communication
     * @param _Output Pointer to the PWM-Handling-Object
     * @param _MaxMeanCurrent Mean Current, depend on the used Components
     * @param _MaxPeakCurrent Peak Current, depend on the used Components
     * @param _CurrentHyst Current Hysteresis to change Duty Cycle by OutputStep, in Current-Controll-Mode
     * @param _VoltageHyst Voltage Hysteresis to change Duty Cycle by OutputStep, in Voltage-Controll-Mode
     * @param _OutputStep Duty Cylce Step up/down, if Value - Setpoint </> Hyst
     * @param _UpdateInterval Update intervall to check the state and change output
     */
    SolarCharger::SolarCharger (uint8_t _PinBoost, uint8_t _PinBuck, const char *_Name, PwmOutput *_Output, float _MaxMeanCurrent, float _MaxPeakCurrent, float _CurrentHyst, float _VoltageHyst, float _OutputStep, uint16_t _UpdateInterval)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      // Hardware
      Output = _Output;
      PinBoost = _PinBoost;
      PinBuck = _PinBuck;
      CurrentHyst = _CurrentHyst;
      VoltageHyst = _VoltageHyst;
      OutputStep = _OutputStep;
      UpdateInterval = _UpdateInterval;
      MaxMeanCurrent = _MaxMeanCurrent;
      MaxPeakCurrent = _MaxPeakCurrent;

      // Intern
      Resolution = 10;
      Frequency = 78000;
      StartDelay = 0;
      MpptDelay = 0;
      StartDelay = 0;
      WaitFull = 10000;
      InitDone = false;
      SumAccuEnergie15m = 0;
      SumAccuEnergie1h = 0;
      SumAccuEnergie1d = 0;
      SumSolarEnergie15m = 0;
      SumSolarEnergie1h = 0;
      SumSolarEnergie1d = 0;
      MpptMaxPower = 0;
      MpptSumPower = 0.0;
      MpptVoltage = 0.0;

      // Konfig
      AccuVoltageMax = 14.4;
      AccuCurrentMax = 10.0;
      AccuChargeEndCurrent = 0.4;
      AccuRechargeVoltage = 13.5;
      SolarVoltageOn = 12.0;
      SolarVoltageOff = 10.0;
      MpptVoltageMin = 12.0;
      MpptVoltageMax = 18.0;
      MpptStepVoltage = 0.1;
      MpptStepTime = 200;
      MpptCheckDelay = 600;
      AutoStart = false;

      // Daten
      AccuVoltage = 0.0;
      AccuCurrent = 0.0;
      AccuPower = 0.0;
      AccuEnergie15m = 0.0;
      AccuEnergie1h = 0.0;
      AccuEnergie1d = 0.0;
      SolarVoltage = 0.0;
      SolarCurrent = 0.0;
      SolarPower = 0.0;
      SolarEnergie15m = 0.0;
      SolarEnergie1h = 0.0;
      SolarEnergie1d = 0.0;
      DutyCycle = 0.0;
      Charging = false;
      ChargeState = SolarCharger_State_T::IDLE;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void SolarCharger::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[AccuVoltageMax_Name] = AccuVoltageMax;
      _Values[AccuCurrentMax_Name] = AccuCurrentMax;
      _Values[AccuChargeEndCurrent_Name] = AccuChargeEndCurrent;
      _Values[AccuRechargeVoltage_Name] = AccuRechargeVoltage;
      _Values[SolarVoltageOn_Name] = SolarVoltageOn;
      _Values[SolarVoltageOff_Name] = SolarVoltageOff;
      _Values[MpptVoltageMin_Name] = MpptVoltageMin;
      _Values[MpptVoltageMax_Name] = MpptVoltageMax;
      _Values[MpptStepVoltage_Name] = MpptStepVoltage;
      _Values[MpptStepTime_Name] = MpptStepTime;
      _Values[MpptCheckDelay_Name] = MpptCheckDelay;
      _Values[AutoStart_Name] = AutoStart;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void SolarCharger::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[AccuVoltage_Name] = AccuVoltage;
      _Values[AccuCurrent_Name] = AccuCurrent;
      _Values[AccuPower_Name] = AccuPower;
      _Values[AccuEnergie15m_Name] = AccuEnergie15m;
      _Values[AccuEnergie1h_Name] = AccuEnergie1h;
      _Values[AccuEnergie1d_Name] = AccuEnergie1d;
      _Values[SolarVoltage_Name] = SolarVoltage;
      _Values[SolarCurrent_Name] = SolarCurrent;
      _Values[SolarPower_Name] = SolarPower;
      _Values[SolarEnergie15m_Name] = SolarEnergie15m;
      _Values[SolarEnergie1h_Name] = SolarEnergie1h;
      _Values[SolarEnergie1d_Name] = SolarEnergie1d;
      _Values[MpptVoltage_Name] = MpptVoltage;
      _Values[DutyCycle_Name] = DutyCycle;
      _Values[Charging_Name] = Charging;
      switch (ChargeState) {
      case SolarCharger_State_T::IDLE:
        _Values[ChargeState_Name] = ChargeState_Case_Idle;
        break;
      case SolarCharger_State_T::CHARGE_MPPT:
        _Values[ChargeState_Name] = ChargeState_Case_ChargeMppt;
        break;
      case SolarCharger_State_T::CHARGE_CURRENT:
        _Values[ChargeState_Name] = ChargeState_Case_ChargeCurrent;
        break;
      case SolarCharger_State_T::CHARGE_VOLTAGE:
        _Values[ChargeState_Name] = ChargeState_Case_ChargeVoltage;
        break;
      case SolarCharger_State_T::TRACK_MPPT:
        _Values[ChargeState_Name] = ChargeState_Case_TrackMppt;
        break;
      case SolarCharger_State_T::FULL:
        _Values[ChargeState_Name] = ChargeState_Case_Fault;
        break;
      default:
        _Values[ChargeState_Name] = ChargeState_Case_Undef;
        break;
      }
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void SolarCharger::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == AccuVoltageMax_Name) {
          AccuVoltageMax = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuVoltageMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuVoltageMax);
          }
        }
        if (Tag[JsonTagName] == AccuCurrentMax_Name) {
          AccuCurrentMax = Tag[JsonTagValue].as<float> ();
          if (AccuCurrentMax > MaxMeanCurrent) {
            AccuCurrentMax = MaxMeanCurrent;
          }
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuCurrentMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuCurrentMax);
          }
        }
        if (Tag[JsonTagName] == AccuChargeEndCurrent_Name) {
          AccuChargeEndCurrent = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuChargeEndCurrent_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuChargeEndCurrent);
          }
        }
        if (Tag[JsonTagName] == AccuRechargeVoltage_Name) {
          AccuRechargeVoltage = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuRechargeVoltage_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuRechargeVoltage);
          }
        }
        if (Tag[JsonTagName] == SolarVoltageOn_Name) {
          SolarVoltageOn = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, SolarVoltageOn_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, SolarVoltageOn);
          }
        }
        if (Tag[JsonTagName] == SolarVoltageOff_Name) {
          SolarVoltageOff = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, SolarVoltageOff_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, SolarVoltageOff);
          }
        }
        if (Tag[JsonTagName] == MpptVoltageMin_Name) {
          MpptVoltageMin = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptVoltageMin_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MpptVoltageMin);
          }
        }
        if (Tag[JsonTagName] == MpptVoltageMax_Name) {
          MpptVoltageMax = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptVoltageMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MpptVoltageMax);
          }
        }
        if (Tag[JsonTagName] == MpptStepVoltage_Name) {
          MpptStepVoltage = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptStepVoltage_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MpptStepVoltage);
          }
        }
        if (Tag[JsonTagName] == MpptStepTime_Name) {
          MpptStepTime = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptStepTime_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MpptStepTime);
          }
        }
        if (Tag[JsonTagName] == MpptCheckDelay_Name) {
          MpptCheckDelay = Tag[JsonTagValue].as<uint32_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptCheckDelay_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MpptCheckDelay);
          }
        }
        if (Tag[JsonTagName] == AutoStart_Name) {
          AutoStart = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, AutoStart_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, AutoStart);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void SolarCharger::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == Charging_Name) {
          Charging = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, Charging_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, Charging);
          }
        }
        if (Tag[JsonTagName] == MpptVoltage_Name) {
          if (ChargeState == SolarCharger_State_T::CHARGE_MPPT) {
            float SetVoltage = Tag[JsonTagValue].as<float> ();
            if (SetVoltage > MpptVoltageMax) {
              SetVoltage = MpptVoltageMax;
            }
            if (SetVoltage > MpptVoltageMin) {
              SetVoltage = MpptVoltageMin;
            }
            MpptVoltage = SetVoltage;
            if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptVoltage_Name)) {
              Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
              Debug.println (FLAG_CONFIG, false, Name, __func__, MpptVoltage);
            }
          }
        }
      }
    }

    /**
     * @brief Execute the Commands
     *
     * @param _Tags Array of Commands ("cmd": [])
     */
    void SolarCharger::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void SolarCharger::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (AccuVoltageMax_Name, AccuVoltageMax_Text, AccuVoltageMax_Comment, false, AccuVoltageMax_Unit, AccuVoltageMax) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuCurrentMax_Name, AccuCurrentMax_Text, AccuCurrentMax_Comment, false, AccuCurrentMax_Unit, AccuCurrentMax) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuChargeEndCurrent_Name, AccuChargeEndCurrent_Text, AccuChargeEndCurrent_Comment, false, AccuChargeEndCurrent_Unit, AccuChargeEndCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuRechargeVoltage_Name, AccuRechargeVoltage_Text, AccuRechargeVoltage_Comment, false, AccuRechargeVoltage_Unit, AccuRechargeVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarVoltageOn_Name, SolarVoltageOn_Text, SolarVoltageOn_Comment, false, SolarVoltageOn_Unit, SolarVoltageOn) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarVoltageOff_Name, SolarVoltageOff_Text, SolarVoltageOff_Comment, false, SolarVoltageOff_Unit, SolarVoltageOff) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptVoltageMin_Name, MpptVoltageMin_Text, MpptVoltageMin_Comment, false, MpptVoltageMin_Unit, MpptVoltageMin) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptVoltageMax_Name, MpptVoltageMax_Text, MpptVoltageMax_Comment, false, MpptVoltageMax_Unit, MpptVoltageMax) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptStepVoltage_Name, MpptStepVoltage_Text, MpptStepVoltage_Comment, false, MpptStepVoltage_Unit, MpptStepVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptStepTime_Name, MpptStepTime_Text, MpptStepTime_Comment, false, MpptStepTime_Unit, MpptStepTime) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptCheckDelay_Name, MpptCheckDelay_Text, MpptCheckDelay_Comment, false, MpptCheckDelay_Unit, MpptCheckDelay) + "}");
      _SetupFile.println (",{" + createSetupTag (AutoStart_Name, AutoStart_Text, AutoStart_Comment, false, AutoStart_TextOn, AutoStart_TextOff, false) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void SolarCharger::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (AccuVoltage_Name, AccuVoltage_Text, AccuVoltage_Comment, true, AccuVoltage_Unit, AccuVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuCurrent_Name, AccuCurrent_Text, AccuCurrent_Comment, true, AccuCurrent_Unit, AccuCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuPower_Name, AccuPower_Text, AccuPower_Comment, true, AccuPower_Unit, AccuPower) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuEnergie15m_Name, AccuEnergie15m_Text, AccuEnergie15m_Comment, true, AccuEnergie15m_Unit, AccuEnergie15m) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuEnergie1h_Name, AccuEnergie1h_Text, AccuEnergie1h_Comment, true, AccuEnergie1h_Unit, AccuEnergie1h) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuEnergie1d_Name, AccuEnergie1d_Text, AccuEnergie1d_Comment, true, AccuEnergie1d_Unit, AccuEnergie1d) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarVoltage_Name, SolarVoltage_Text, SolarVoltage_Comment, true, SolarVoltage_Unit, SolarVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarCurrent_Name, SolarCurrent_Text, SolarCurrent_Comment, true, SolarCurrent_Unit, SolarCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarPower_Name, SolarPower_Text, SolarPower_Comment, true, SolarPower_Unit, SolarPower) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarEnergie15m_Name, SolarEnergie15m_Text, SolarEnergie15m_Comment, true, SolarEnergie15m_Unit, SolarEnergie15m) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarEnergie1h_Name, SolarEnergie1h_Text, SolarEnergie1h_Comment, true, SolarEnergie1h_Unit, SolarEnergie1h) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarEnergie1d_Name, SolarEnergie1d_Text, SolarEnergie1d_Comment, true, SolarEnergie1d_Unit, SolarEnergie1d) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptVoltage_Name, MpptVoltage_Text, MpptVoltage_Comment, false, MpptVoltage_Unit, MpptVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (DutyCycle_Name, DutyCycle_Text, DutyCycle_Comment, true, DutyCycle_Unit, DutyCycle) + "}");
      _SetupFile.println (",{" + createSetupTag (Charging_Name, Charging_Text, Charging_Comment, false, Charging_TextOn, Charging_TextOff, false) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargeState_Name, ChargeState_Text, ChargeState_Comment, true, "Idle") + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void SolarCharger::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Control SolarVoltage on DutyCycle
     * 
     * @param _SetPoint Setpoint for solar input Voltage
     * @return true DutyCycle changed
     * @return false DutyCycle not changed
     */
    bool SolarCharger::controlSolarVoltage (float _SetPoint) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Control");
      float VoltageDiff = SolarVoltage - _SetPoint;
      if (VoltageDiff >= VoltageHyst) {
        DutyCycle -= OutputStep;
        return true;
      } else if (VoltageDiff <= -VoltageHyst) {
        DutyCycle += OutputStep;
        return true;
      }
      return false;
    }

    /**
     * @brief Control AccuVoltage on DutyCycle
     *
     * @param _SetPoint Setpoint for accu output Voltage
     * @return true DutyCycle changed
     * @return false DutyCycle not changed
     */
    bool SolarCharger::controlAccuVoltage (float _SetPoint) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Control");
      float VoltageDiff = AccuVoltage - _SetPoint;
      if (VoltageDiff >= VoltageHyst) {
        DutyCycle -= OutputStep;
        return true;
      } else if (VoltageDiff <= -VoltageHyst) {
        DutyCycle += OutputStep;
        return true;
      }
      return false;
    }

    /**
     * @brief Control SolarVoltage on DutyCycle
     *
     * @param _SetPoint Setpoint for accu output current
     * @return true DutyCycle changed
     * @return false DutyCycle not changed
     */
    bool SolarCharger::controlAccuCurrent (float _SetPoint) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Control");
      float CurrentDiff = AccuCurrent - _SetPoint;
      if (CurrentDiff >= CurrentHyst) {
        DutyCycle -= OutputStep;
        return true;
      } else if (CurrentDiff <= -CurrentHyst) {
        DutyCycle += OutputStep;
        return true;
      }
      return false;
    }

    /**
     * @brief Init the SolarCharger
     */
    bool SolarCharger::init (ValueCallback _GetSolarVoltageCB, ValueCallback _GetSolarCurrentCB, ValueCallback _GetAccuVoltageCB, ValueCallback _GetAccuCurrentCB, struct tm &_Time) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Setup");
      InitDone = Output->setupPin (PinBoost, Frequency, Resolution);
      if (!Output->setupPin (PinBuck, Frequency, Resolution)) {
        InitDone = false;
      }
      if (_GetSolarVoltageCB) {
        getSolarVoltageCB = _GetSolarVoltageCB;
      } else {
        InitDone = false;
      }
      if (_GetSolarCurrentCB) {
        getSolarCurrentCB = _GetSolarCurrentCB;
      } else {
        InitDone = false;
      }
      if (_GetAccuVoltageCB) {
        getAccuVoltageCB = _GetAccuVoltageCB;
      } else {
        InitDone = false;
      }
      if (_GetAccuCurrentCB) {
        getAccuCurrentCB = _GetAccuCurrentCB;
      } else {
        InitDone = false;
      }
      Last15m = _Time.tm_min % 15;
      Last1h = _Time.tm_hour;
      Last1d = _Time.tm_yday;
      LastMillis = millis ();
      UpdateMillis = 0;
      return InitDone;
    }

    /**
     * @brief Handling the SolarCharger
     * Handling the Stepper-Functions and check if DoCheck is requested
     * @param _Time Current Time to check automated feeding
     */
    void SolarCharger::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      // Get Update Intervall
      uint32_t ActMillis = millis ();
      UpdateMillis += (ActMillis - LastMillis);
      LastMillis = ActMillis;

      if (UpdateMillis >= UpdateInterval && InitDone) {
        // Read Sensor Date
        AccuVoltage = getAccuVoltageCB ();
        AccuCurrent = getAccuCurrentCB ();
        AccuPower = AccuVoltage * AccuCurrent;
        SolarVoltage = getSolarVoltageCB ();
        SolarCurrent = getSolarCurrentCB ();
        SolarPower = SolarVoltage * SolarCurrent;

        // Calc energie Data
        float AccuEnergie = AccuPower * UpdateMillis / 3600000.0;
        float SolarEnergie = SolarPower * UpdateMillis / 3600000.0;
        SumAccuEnergie15m += AccuEnergie;
        SumAccuEnergie1h += AccuEnergie;
        SumAccuEnergie1d += AccuEnergie;
        SumSolarEnergie15m += SolarEnergie;
        SumSolarEnergie1h += SolarEnergie;
        SumSolarEnergie1d += SolarEnergie;

        // Check Time Interval
        if ((_Time.tm_min % 15) != Last15m) {
          AccuEnergie15m = SumAccuEnergie15m;
          SolarEnergie15m = SumSolarEnergie15m;
          SumAccuEnergie15m = 0;
          SumSolarEnergie15m = 0;
          Last15m = _Time.tm_min % 15;
        }
        if (_Time.tm_hour != Last1h) {
          AccuEnergie1h = SumAccuEnergie1h;
          SolarEnergie1h = SumSolarEnergie1h;
          SumAccuEnergie1h = 0;
          SumSolarEnergie1h = 0;
          Last1h = _Time.tm_hour;
        }
        if (_Time.tm_yday != Last1d) {
          AccuEnergie1d = SumAccuEnergie1d;
          SolarEnergie1d = SumSolarEnergie1d;
          SumAccuEnergie1d = 0;
          SumSolarEnergie1d = 0;
          Last1d = _Time.tm_yday;
        }

        if (ChargeState != SolarCharger_State_T::IDLE) {
          if (SolarVoltage < SolarVoltageOff) {
            ChargeState = SolarCharger_State_T::IDLE;
          }
        }

        if (StartDelay < WaitStart) {
          StartDelay += UpdateMillis;
          if (StartDelay >= WaitStart) {
            if (AutoStart) {
              Charging = true;
            }
          }
        }

        if (Charging) {
          // New State, reset Step-Data
          bool StepInit = ChargeState != LastState;
          if (StepInit) {
            StepDelay = 0;
            LastState = ChargeState;
          }
          switch (ChargeState) {
          case SolarCharger_State_T::IDLE:
            //----------------------------------------
            // Idle, waiting for Requests
            //----------------------------------------
            if (SolarVoltage > SolarVoltageOn) {
              // Select DC-DC Mode and calculat initial DutyCycle
              DutyCycle = AccuVoltageMax / SolarVoltage;
              MpptVoltage = SolarVoltage;
              ChargeState = SolarCharger_State_T::CHARGE_MPPT;
            }
            break;

          case SolarCharger_State_T::CHARGE_MPPT:
            //----------------------------------------
            // Charging the accu by MPPT
            //----------------------------------------
            MpptDelay += UpdateMillis;
            if (AccuCurrent > AccuCurrentMax) {
              // Change to Const-Current charging if maximum Current is reached
              DutyCycle -= OutputStep;
              ChargeState = SolarCharger_State_T::CHARGE_CURRENT;
            } else if (AccuVoltage > AccuVoltageMax) {
              // Change to Const-Voltage charging if maximum Voltage is reached
              DutyCycle -= OutputStep;
              ChargeState = SolarCharger_State_T::CHARGE_VOLTAGE;
            } else if (MpptDelay >= MpptCheckDelay) {
              ChargeState = SolarCharger_State_T::TRACK_MPPT;
            } else {
              // controll Solar Voltage
              controlSolarVoltage(MpptVoltage);
            }
            break;

          case SolarCharger_State_T::CHARGE_CURRENT:
            //----------------------------------------
            // Charging the accu by constant current
            //----------------------------------------
            if (AccuVoltage >= AccuVoltageMax + VoltageHyst) {
              // Check Voltage to change to constant Voltage Mode
              DutyCycle -= OutputStep;
              ChargeState = SolarCharger_State_T::CHARGE_VOLTAGE;
            } else {
              // controll charging current
              controlAccuCurrent (AccuCurrentMax);
            }
            break;

          case SolarCharger_State_T::CHARGE_VOLTAGE:
            //----------------------------------------
            // Charging the accu by constant voltage
            //----------------------------------------
            MpptDelay += UpdateMillis;
            if (AccuCurrent >= AccuCurrentMax + CurrentHyst) {
              // Check Current to change to constant Current Mode
              DutyCycle -= OutputStep;
              ChargeState = SolarCharger_State_T::CHARGE_CURRENT;
            }
            else if (MpptDelay >= MpptCheckDelay) {
              ChargeState = SolarCharger_State_T::TRACK_MPPT;
            }
            else {
              // controll charging voltage
              controlAccuVoltage (AccuVoltageMax);
            }
            if (AccuCurrent < AccuChargeEndCurrent) {
              StepDelay += UpdateMillis;
              // Wait for Accu Voltage have to be stable
              if (StepDelay >= WaitFull) {
                ChargeState = SolarCharger_State_T::FULL;
              }
            } else {
              StepDelay = 0;
            }
            break;

          case SolarCharger_State_T::TRACK_MPPT:
            //----------------------------------------
            // Check MPPT
            // not used, working with continuous tracking
            //----------------------------------------
            // Start with minimum MpptVoltage
            if (StepInit) {
              MpptVoltage = MpptVoltageMin;
              MpptInRange = false;
              MpptSumPower = 0.0;
              MpptMaxPower = 0.0;
            }

            // Check im Solar-Voltage is in range
            if (MpptInRange) {
              StepDelay += UpdateMillis;
              MpptSumPower += AccuPower * (float)UpdateMillis;
              if (StepDelay >= MpptStepTime) {
                float ActPower = MpptSumPower / (float)StepDelay;
                if (ActPower > MpptMaxPower) {
                  MpptMaxPower = ActPower;
                  MpptMaxVoltage = MpptVoltage;
                }
                MpptVoltage += MpptStepVoltage;
                if (MpptVoltage > MpptVoltageMax) {
                  ChargeState = SolarCharger_State_T::CHARGE_MPPT;
                  MpptVoltage = MpptMaxVoltage;
                } else {
                  MpptInRange = false;
                  MpptSumPower = 0.0;
                  MpptMaxPower = 0.0;
                }
              }
            } else if (abs(SolarVoltage - MpptVoltage) < VoltageHyst) {
              MpptInRange = true;
            }

            if (AccuCurrent > AccuCurrentMax) {
              // Change to Const-Current charging if maximum Current is reached
              DutyCycle -= OutputStep;
              ChargeState = SolarCharger_State_T::CHARGE_CURRENT;
            } else if (AccuVoltage > AccuVoltageMax) {
              // Change to Const-Voltage charging if maximum Voltage is reached
              DutyCycle -= OutputStep;
              ChargeState = SolarCharger_State_T::CHARGE_VOLTAGE;
            } else {
              // controll Solar Voltage
              controlSolarVoltage(MpptVoltage);
            }

            MpptDelay = 0;
            break;

          case SolarCharger_State_T::FULL:
            //----------------------------------------
            // Accu ist Full
            //----------------------------------------
            if (AccuVoltage < AccuRechargeVoltage) {
              StepDelay += UpdateMillis;
              // calculat initial DutyCycle

              if (StepDelay >= WaitFull) {
                // Select DC-DC Mode and calculat initial DutyCycle
                DutyCycle = AccuVoltageMax / SolarVoltage;
                MpptVoltage = SolarVoltage;
                ChargeState = SolarCharger_State_T::CHARGE_MPPT;
              }
            } else {
              StepDelay = 0;
            }
            break;
          }
        } else {
          ChargeState = SolarCharger_State_T::IDLE;
        }
        if (Debug.print (FLAG_DATA, false, Name, __func__, "State:")) {
          Debug.println (FLAG_DATA, false, Name, __func__, ChargeState);
          Debug.print (FLAG_DATA, false, Name, __func__, " - AccuVoltage   [V]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, AccuVoltage);
          Debug.print (FLAG_DATA, false, Name, __func__, " - AccuCurrent   [A]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, AccuCurrent);
          Debug.print (FLAG_DATA, false, Name, __func__, " - AccuEnergie   [Wh]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, AccuEnergie);
          Debug.print (FLAG_DATA, false, Name, __func__, " - SolarVoltage  [%]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, SolarVoltage);
          Debug.print (FLAG_DATA, false, Name, __func__, " - SolarCurrent  [%]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, SolarCurrent);
          Debug.print (FLAG_DATA, false, Name, __func__, " - SolarEnergie   [Wh]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, SolarEnergie);
        }

        // Write Output
        float BoostDuty;
        float BuckDuty;
        bool EnableOutput = ChargeState == SolarCharger_State_T::CHARGE_MPPT
                            || ChargeState == SolarCharger_State_T::CHARGE_CURRENT
                            || ChargeState == SolarCharger_State_T::CHARGE_VOLTAGE;

        if (EnableOutput) {
          if (DutyCycle < MinDutyCycle) {
            DutyCycle = MinDutyCycle;
          } else if (DutyCycle > MaxDutyCycle) {
            DutyCycle = MaxDutyCycle;
          }
          if (DutyCycle < 1.0) {
            BoostDuty = MaxOutputDutyCycle;
            BuckDuty = DutyCycle * MaxOutputDutyCycle;
          } else if (DutyCycle > 1.0) {
            BoostDuty = MaxOutputDutyCycle / DutyCycle;
            BuckDuty = MaxOutputDutyCycle;
          } else {
            BoostDuty = MaxOutputDutyCycle;
            BuckDuty = MaxOutputDutyCycle;
          }
        } else {
          BoostDuty = 0.0;
          BuckDuty = 0.0;
        }

        Output->writePin (PinBoost, BoostDuty);
        Output->writePin (PinBuck, BuckDuty);
        UpdateMillis = 0;
      }
    }
  }
}
