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
    const char *SolarCharger::AccuVoltageMax_Name = "AccuVoltageMax";
    const char *SolarCharger::AccuVoltageMax_Text = "Maximale Akku Ladespannung";
    const char *SolarCharger::AccuVoltageMax_Unit = "V";
    const char *SolarCharger::AccuVoltageMax_Comment = nullptr;
    const char *SolarCharger::AccuCurrentMax_Name = "AccuCurrentMax";
    const char *SolarCharger::AccuCurrentMax_Text = "Maximaler Akku Ladestrom";
    const char *SolarCharger::AccuCurrentMax_Unit = "A";
    const char *SolarCharger::AccuCurrentMax_Comment = nullptr;
    const char *SolarCharger::AccuChargeEndCurrent_Name = "ChargeEndCurrent";
    const char *SolarCharger::AccuChargeEndCurrent_Text = "Laden Beendet Strom";
    const char *SolarCharger::AccuChargeEndCurrent_Unit = "A";
    const char *SolarCharger::AccuChargeEndCurrent_Comment = nullptr;
    const char *SolarCharger::AccuRechargeVoltage_Name = "RechargeVoltage";
    const char *SolarCharger::AccuRechargeVoltage_Text = "Spannungs Nachladen";
    const char *SolarCharger::AccuRechargeVoltage_Unit = "V";
    const char *SolarCharger::AccuRechargeVoltage_Comment = nullptr;
    const char *SolarCharger::AccuVoltage_Name = "AccuVoltage";
    const char *SolarCharger::AccuVoltage_Text = "Akku Spannung";
    const char *SolarCharger::AccuVoltage_Unit = "V";
    const char *SolarCharger::AccuVoltage_Comment = nullptr;
    const char *SolarCharger::AccuCurrent_Name = "Current";
    const char *SolarCharger::AccuCurrent_Text = "Akku Strom";
    const char *SolarCharger::AccuCurrent_Unit = "A";
    const char *SolarCharger::AccuCurrent_Comment = nullptr;
    const char *SolarCharger::AccuPower_Name = "AccuPower";
    const char *SolarCharger::AccuPower_Text = "Entladespannung";
    const char *SolarCharger::AccuPower_Unit = "V";
    const char *SolarCharger::AccuPower_Comment = nullptr;
    const char *SolarCharger::SolarVoltageMin_Name = "SolarVoltageMin";
    const char *SolarCharger::SolarVoltageMin_Text = "Geladene Leistung";
    const char *SolarCharger::SolarVoltageMin_Unit = "WH";
    const char *SolarCharger::SolarVoltageMin_Comment = nullptr;
    const char *SolarCharger::MpptInterval_Name = "MpptInterval";
    const char *SolarCharger::MpptInterval_Text = "Entladener Strom";
    const char *SolarCharger::MpptInterval_Unit = "AH";
    const char *SolarCharger::MpptInterval_Comment = nullptr;
    const char *SolarCharger::SolarVoltage_Name = "SolarVoltage";
    const char *SolarCharger::SolarVoltage_Text = "Entledene Leistung";
    const char *SolarCharger::SolarVoltage_Unit = "WH";
    const char *SolarCharger::SolarVoltage_Comment = nullptr;
    const char *SolarCharger::SolarCurrent_Name = "SolarCurrent";
    const char *SolarCharger::SolarCurrent_Text = "PWM Laden";
    const char *SolarCharger::SolarCurrent_Unit = "%";
    const char *SolarCharger::SolarCurrent_Comment = nullptr;
    const char *SolarCharger::SolarPower_Name = "SolarPower";
    const char *SolarCharger::SolarPower_Text = "Genzwert Akku Entladen";
    const char *SolarCharger::SolarPower_Unit = "A";
    const char *SolarCharger::SolarPower_Comment = nullptr;
    const char *SolarCharger::Fault_Name = "Fault";
    const char *SolarCharger::Fault_Text = "Fehler aktiv";
    const char *SolarCharger::Fault_Comment = nullptr;
    const char *SolarCharger::Fault_TextOn = "Quittieren";
    const char *SolarCharger::Fault_TextOff = "";
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

    const float SolarCharger::CurrentHyst = 0.01;
    const float SolarCharger::VoltageHyst = 0.01;
    const float SolarCharger::OutputStep = 0.1;
    const uint16_t SolarCharger::UpdateInterval = 100;

    /**
     * @brief Construct a new SolarCharger::SolarCharger object
     *
     * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
     * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
     * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
     * @param _Name Element Name inside the Communication
     */
    SolarCharger::SolarCharger (uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name, PwmOutput *_Output)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      // Intern
      ChargeState = SolarCharger_State_T::IDLE;
      Resolution = 10;
      DutyScale = (pow (2.0, float (Resolution)) - 1.0) / 100.0;
      Frequency = 78000;

      // Konfig
      AccuVoltageMax = 14.4;
      AccuCurrentMax = 10.0;
      AccuChargeEndCurrent = 0.4;
      AccuRechargeVoltage = 13.5;
      SolarVoltageMin = 10.0;
      MpptInterval = 600000;

      // Daten
      AccuVoltage = 0.0;
      AccuCurrent = 0.0;
      AccuPower = 0.0;
      SolarVoltage = 0.0;
      SolarCurrent = 0.0;
      SolarPower = 0.0;
      MpptDelay = 0;
      FaultDelay = 0;

      // Hardware
      Output = _Output;
      PinBoost = _PinCharge;
      PinBuck = _PinDischarge;
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
      _Values[SolarVoltageMin_Name] = SolarVoltageMin;
      _Values[MpptInterval_Name] = MpptInterval;
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
      _Values[SolarVoltage_Name] = SolarVoltage;
      _Values[SolarCurrent_Name] = SolarCurrent;
      _Values[SolarPower_Name] = SolarPower;
      _Values[Fault_Name] = (ChargeState == FAULT);
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
      case SolarCharger_State_T::FAULT:
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
        if (Tag[JsonTagName] == SolarVoltageMin_Name) {
          SolarVoltageMin = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, SolarVoltageMin_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, SolarVoltageMin);
          }
        }
        if (Tag[JsonTagName] == MpptInterval_Name) {
          MpptInterval = Tag[JsonTagValue].as<uint32_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, MpptInterval_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, MpptInterval);
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
        if (Tag[JsonTagName] == Fault_Name) {
          if (ChargeState == FAULT) {
            ChargeState = FaultState;
          }
          if (Debug.print (FLAG_LOOP, false, Name, __func__, Fault_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, "Ack");
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
      _SetupFile.println (",{" + createSetupTag (SolarVoltageMin_Name, SolarVoltageMin_Text, SolarVoltageMin_Comment, true, SolarVoltageMin_Unit, SolarVoltageMin) + "}");
      _SetupFile.println (",{" + createSetupTag (MpptInterval_Name, MpptInterval_Text, MpptInterval_Comment, true, MpptInterval_Unit, MpptInterval) + "}");
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
      _SetupFile.println (",{" + createSetupTag (SolarVoltage_Name, SolarVoltage_Text, SolarVoltage_Comment, true, SolarVoltage_Unit, SolarVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarCurrent_Name, SolarCurrent_Text, SolarCurrent_Comment, true, SolarCurrent_Unit, SolarCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (SolarPower_Name, SolarPower_Text, SolarPower_Comment, false, SolarPower_Unit, SolarPower) + "}");
      _SetupFile.println (",{" + createSetupTag (Fault_Name, Fault_Text, Fault_Comment, false, Fault_TextOn, Fault_TextOff, false) + "}");
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
     * @brief Init the SolarCharger
     */
    bool SolarCharger::init () {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Setup");
      Output->setupPin (PinBoost, Frequency, Resolution);
      Output->setupPin (PinBuck, Frequency, Resolution);
      LastMillis = millis ();
      UpdateMillis = 0;
      return true;
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
      float CurrentDiff;
      float VoltageDiff;

      if (UpdateMillis >= UpdateInterval) {
        // Read Sensor Date
        AccuVoltage = getAccuVoltageCB();
        AccuCurrent = getAccuCurrentCB();
        AccuPower = AccuVoltage * AccuCurrent;
        SolarVoltage = getSolarVoltageCB ();
        SolarCurrent = getSolarCurrentCB ();
        SolarPower = SolarVoltage * SolarCurrent;

        switch (ChargeState) {
        case SolarCharger_State_T::IDLE:
          //----------------------------------------
          // Idle, waiting for Requests
          //----------------------------------------

          if (SolarVoltage > SolarVoltageMin) {
            // Select DC-DC Mode and calculat initial DutyCycle
            ChargeState = SolarCharger_State_T::TRACK_MPPT;
          }
          break;

        case SolarCharger_State_T::CHARGE_MPPT:
          //----------------------------------------
          // Charging the accu by MPPT Voltage
          //----------------------------------------
          break;

        case SolarCharger_State_T::CHARGE_CURRENT:
          //----------------------------------------
          // Charging the accu by constant current
          //----------------------------------------
          break;

        case SolarCharger_State_T::CHARGE_VOLTAGE:
          //----------------------------------------
          // Charging the accu by constant voltage
          //----------------------------------------
          break;

        case SolarCharger_State_T::TRACK_MPPT:
          //----------------------------------------
          // Check MPPT
          //----------------------------------------
          break;

        case SolarCharger_State_T::FAULT:
          //----------------------------------------
          // Fault State have to Reset
          //----------------------------------------
          break;
        }
        if (Debug.print (FLAG_DATA, false, Name, __func__, "State:")) {
          Debug.println (FLAG_DATA, false, Name, __func__, ChargeState);
          Debug.print (FLAG_DATA, false, Name, __func__, " - AccuVoltage   [V]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, AccuVoltage);
          Debug.print (FLAG_DATA, false, Name, __func__, " - AccuCurrent   [A]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, AccuCurrent);
          Debug.print (FLAG_DATA, false, Name, __func__, " - SolarVoltage  [%]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, SolarVoltage);
          Debug.print (FLAG_DATA, false, Name, __func__, " - SolarCurrent  [%]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, SolarCurrent);
        }
        // Write Output
        if (BoostDuty < 0.0) {
          BoostDuty = 0.0;
        } else if (BoostDuty > 99.0) {
          BoostDuty = 99.0;
        }
        if (BuckDuty < 0.0) {
          BuckDuty = 0.0;
        } else if (BuckDuty > 99.0) {
          BuckDuty = 99.0;
        }
        Output->writePin (PinBoost, BoostDuty);
        Output->writePin (PinBuck, BuckDuty);
        UpdateMillis = 0;
      }
    }
  }
}
