/**
 * @file JCA_FNC_Feeder.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element for a daily Charger. With variable Time and Amount
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_Charger.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Charger::AccuVoltageMax_Name = "AccuVoltageMax";
    const char *Charger::AccuVoltageMax_Text = "Maximale Akku Ladespannung";
    const char *Charger::AccuVoltageMax_Unit = "V";
    const char *Charger::AccuVoltageMax_Comment = nullptr;
    const char *Charger::AccuVoltageMin_Name = "AccuVoltageMin";
    const char *Charger::AccuVoltageMin_Text = "Grenzwert Akku Entladen";
    const char *Charger::AccuVoltageMin_Unit = "V";
    const char *Charger::AccuVoltageMin_Comment = nullptr;
    const char *Charger::AccuChargeCurrent_Name = "AccuChargeCurrent";
    const char *Charger::AccuChargeCurrent_Text = "Maximaler Akku Ladestrom";
    const char *Charger::AccuChargeCurrent_Unit = "A";
    const char *Charger::AccuChargeCurrent_Comment = nullptr;
    const char *Charger::AccuDischargeCurrent_Name = "AccuDischargeCurrent";
    const char *Charger::AccuDischargeCurrent_Text = "Maximaler Akku Entladestrom";
    const char *Charger::AccuDischargeCurrent_Unit = "A";
    const char *Charger::AccuDischargeCurrent_Comment = nullptr;
    const char *Charger::ChargeEndCurrent_Name = "ChargeEndCurrent";
    const char *Charger::ChargeEndCurrent_Text = "Laden Beendet Strom";
    const char *Charger::ChargeEndCurrent_Unit = "A";
    const char *Charger::ChargeEndCurrent_Comment = nullptr;
    const char *Charger::WaitDischarge_Name = "WaitDischarge";
    const char *Charger::WaitDischarge_Text = "Wartezeit für Entlade";
    const char *Charger::WaitDischarge_Unit = "s";
    const char *Charger::WaitDischarge_Comment = nullptr;
    const char *Charger::RechargeVoltage_Name = "RechargeVoltage";
    const char *Charger::RechargeVoltage_Text = "Spannungs Nachladen";
    const char *Charger::RechargeVoltage_Unit = "V";
    const char *Charger::RechargeVoltage_Comment = nullptr;
    const char *Charger::DoCheck_Name = "DoCheck";
    const char *Charger::DoCheck_Text = "Akku Prüfung aktiv";
    const char *Charger::DoCheck_Comment = nullptr;
    const char *Charger::DoCheck_TextOn = nullptr;
    const char *Charger::DoCheck_TextOff = nullptr;
    const char *Charger::DoCharge_Name = "DoCharge";
    const char *Charger::DoCharge_Text = "Akku Laden aktiv";
    const char *Charger::DoCharge_Comment = nullptr;
    const char *Charger::DoCharge_TextOn = nullptr;
    const char *Charger::DoCharge_TextOff = nullptr;
    const char *Charger::AccuVoltage_Name = "AccuVoltage";
    const char *Charger::AccuVoltage_Text = "Akku Spannung";
    const char *Charger::AccuVoltage_Unit = "V";
    const char *Charger::AccuVoltage_Comment = nullptr;
    const char *Charger::Current_Name = "Current";
    const char *Charger::Current_Text = "Akku Strom";
    const char *Charger::Current_Unit = "A";
    const char *Charger::Current_Comment = nullptr;
    const char *Charger::ChargedAH_Name = "ChargedAH";
    const char *Charger::ChargedAH_Text = "Geladener Strom";
    const char *Charger::ChargedAH_Unit = "AH";
    const char *Charger::ChargedAH_Comment = nullptr;
    const char *Charger::ChargedWH_Name = "ChargedWH";
    const char *Charger::ChargedWH_Text = "Geladene Leistung";
    const char *Charger::ChargedWH_Unit = "WH";
    const char *Charger::ChargedWH_Comment = nullptr;
    const char *Charger::DischargedAH_Name = "DischargedAH";
    const char *Charger::DischargedAH_Text = "Entladener Strom";
    const char *Charger::DischargedAH_Unit = "AH";
    const char *Charger::DischargedAH_Comment = nullptr;
    const char *Charger::DischargedWH_Name = "DischargedWH";
    const char *Charger::DischargedWH_Text = "Entledene Leistung";
    const char *Charger::DischargedWH_Unit = "WH";
    const char *Charger::DischargedWH_Comment = nullptr;
    const char *Charger::DischargeSP_Name = "DischargeSP";
    const char *Charger::DischargeSP_Text = "PWM Entladen";
    const char *Charger::DischargeSP_Unit = "%";
    const char *Charger::DischargeSP_Comment = nullptr;
    const char *Charger::ChargeSP_Name = "ChargeSP";
    const char *Charger::ChargeSP_Text = "PWM Laden";
    const char *Charger::ChargeSP_Unit = "%";
    const char *Charger::ChargeSP_Comment = nullptr;
    const char *Charger::ChargeState_Name = "ChargeState";
    const char *Charger::ChargeState_Text = "Status";
    const char *Charger::ChargeState_Comment = nullptr;
    const char *Charger::ChargeState_Case_Undef = "Undefiniert";
    const char *Charger::ChargeState_Case_Idle = "Inaktiv";
    const char *Charger::ChargeState_Case_ChargeCurrent = "Laden-Strom";
    const char *Charger::ChargeState_Case_ChargeVoltage = "Laden-Spannung";
    const char *Charger::ChargeState_Case_WaitCharge = "Warten-Laden";
    const char *Charger::ChargeState_Case_WaitDischarge = "Warten-Entladen";
    const char *Charger::ChargeState_Case_Discharge = "Entladen";
    const char *Charger::DischargeEndCurrent_Name = "DischargeEndCurrent";
    const char *Charger::DischargeEndCurrent_Text = "Genzwert Akku Entladen";
    const char *Charger::DischargeEndCurrent_Unit = "A";
    const char *Charger::DischargeEndCurrent_Comment = nullptr;
    const char *Charger::DischargeVoltage_Name = "DischargeVoltage";
    const char *Charger::DischargeVoltage_Text = "Entladespannung";
    const char *Charger::DischargeVoltage_Unit = "V";
    const char *Charger::DischargeVoltage_Comment = nullptr;

    const float Charger::CurrentHyst = 0.02;
    const float Charger::VoltageHyst = 0.02;
    const float Charger::OutputStep = 0.1;
    const uint16_t Charger::UpdateInterval = 100;

/**
 * @brief Construct a new Charger::Charger object
 *
 * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
 * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
 * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
 * @param _Name Element Name inside the Communication
 */
#ifdef ESP8266
    Charger::Charger (INA219 *_Sensor, uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name)
        : Parent (_Name) {
#elif ESP32
    Charger::Charger (INA219 *_Sensor, uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name, Pwm *_Output)
        : Parent (_Name) {
#endif
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      // Intern
      ChargeState = Charger_State_T::IDLE;
      Resolution = 10;
      DutyScale = (pow (2.0, float (Resolution)) - 1.0) / 100.0;
      Frequency = 3000;

      // Konfig
      AccuVoltageMax = 4.2;
      AccuVoltageMin = 3.5;
      AccuChargeCurrent = 1.0;
      AccuDischargeCurrent = 1.0;
      ChargeEndCurrent = 0.1;
      DischargeEndCurrent = 0.2;
      WaitDischarge = 20;
      RechargeVoltage = 4.0;

      // Daten
      DoCharge = false;
      DoCheck = false;
      AccuVoltage = 0.0;
      Current = 0.0;
      ChargedAH = 0.0;
      ChargedWH = 0.0;
      DischargedAH = 0.0;
      DischargedWH = 0.0;
      DischargeVoltage = 0.0;
      ChargeSP = 0.0;
      DischargeSP = 0.0;

      // Hardware
      Output = _Output;
      PinCharge = _PinCharge;
      PinDischarge = _PinDischarge;
      Sensor = _Sensor;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void Charger::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[AccuVoltageMax_Name] = AccuVoltageMax;
      _Values[AccuVoltageMin_Name] = AccuVoltageMin;
      _Values[AccuChargeCurrent_Name] = AccuChargeCurrent;
      _Values[AccuDischargeCurrent_Name] = AccuDischargeCurrent;
      _Values[ChargeEndCurrent_Name] = ChargeEndCurrent;
      _Values[DischargeEndCurrent_Name] = DischargeEndCurrent;
      _Values[WaitDischarge_Name] = WaitDischarge;
      _Values[RechargeVoltage_Name] = RechargeVoltage;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void Charger::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[DoCheck_Name] = DoCheck;
      _Values[DoCharge_Name] = DoCharge;
      _Values[AccuVoltage_Name] = AccuVoltage;
      _Values[Current_Name] = Current;
      _Values[ChargedAH_Name] = ChargedAH;
      _Values[ChargedWH_Name] = ChargedWH;
      _Values[DischargedAH_Name] = DischargedAH;
      _Values[DischargedWH_Name] = DischargedWH;
      _Values[DischargeVoltage_Name] = DischargeVoltage;
      _Values[ChargeSP_Name] = ChargeSP;
      _Values[DischargeSP_Name] = DischargeSP;
      switch (ChargeState) {
      case Charger_State_T::IDLE:
        _Values[ChargeState_Name] = ChargeState_Case_Idle;
        break;
      case Charger_State_T::CHARGE_CURRENT:
        _Values[ChargeState_Name] = ChargeState_Case_ChargeCurrent;
        break;
      case Charger_State_T::CHARGE_VOLTAGE:
        _Values[ChargeState_Name] = ChargeState_Case_ChargeVoltage;
        break;
      case Charger_State_T::WAIT_CHARGE:
        _Values[ChargeState_Name] = ChargeState_Case_WaitCharge;
        break;
      case Charger_State_T::WAIT_DISCHARCH:
        _Values[ChargeState_Name] = ChargeState_Case_WaitDischarge;
        break;
      case Charger_State_T::DISCHARGE:
        _Values[ChargeState_Name] = ChargeState_Case_Discharge;
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
    void Charger::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == AccuVoltageMax_Name) {
          AccuVoltageMax = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuVoltageMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuVoltageMax);
          }
        }
        if (Tag[JsonTagName] == AccuVoltageMin_Name) {
          AccuVoltageMin = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuVoltageMin_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuVoltageMin);
          }
        }
        if (Tag[JsonTagName] == AccuChargeCurrent_Name) {
          AccuChargeCurrent = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuChargeCurrent_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuChargeCurrent);
          }
        }
        if (Tag[JsonTagName] == AccuDischargeCurrent_Name) {
          AccuDischargeCurrent = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuDischargeCurrent_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuDischargeCurrent);
          }
        }
        if (Tag[JsonTagName] == ChargeEndCurrent_Name) {
          ChargeEndCurrent = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ChargeEndCurrent_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ChargeEndCurrent);
          }
        }
        if (Tag[JsonTagName] == DischargeEndCurrent_Name) {
          DischargeEndCurrent = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, DischargeEndCurrent_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, DischargeEndCurrent);
          }
        }
        if (Tag[JsonTagName] == WaitDischarge_Name) {
          WaitDischarge = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, WaitDischarge_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, WaitDischarge);
          }
        }
        if (Tag[JsonTagName] == RechargeVoltage_Name) {
          RechargeVoltage = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, RechargeVoltage_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, RechargeVoltage);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void Charger::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == DoCharge_Name) {
          DoCharge = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, DoCharge_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, DoCharge);
          }
          if (DoCharge) {
            DoCheck = false;
          }
        }

        if (Tag[JsonTagName] == DoCheck_Name) {
          DoCheck = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, DoCheck_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, DoCheck);
          }
          if (DoCheck) {
            DoCharge = false;
          }
        }
      }
    }

    /**
     * @brief Execute the Commands
     *
     * @param _Tags Array of Commands ("cmd": [])
     */
    void Charger::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void Charger::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (AccuVoltageMax_Name, AccuVoltageMax_Text, AccuVoltageMax_Comment, false, AccuVoltageMax_Unit, AccuVoltageMax) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuVoltageMin_Name, AccuVoltageMin_Text, AccuVoltageMin_Comment, false, AccuVoltageMin_Unit, AccuVoltageMin) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuChargeCurrent_Name, AccuChargeCurrent_Text, AccuChargeCurrent_Comment, false, AccuChargeCurrent_Unit, AccuChargeCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuDischargeCurrent_Name, AccuDischargeCurrent_Text, AccuDischargeCurrent_Comment, false, AccuDischargeCurrent_Unit, AccuDischargeCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargeEndCurrent_Name, ChargeEndCurrent_Text, ChargeEndCurrent_Comment, false, ChargeEndCurrent_Unit, ChargeEndCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargeEndCurrent_Name, DischargeEndCurrent_Text, DischargeEndCurrent_Comment, false, DischargeEndCurrent_Unit, DischargeEndCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (WaitDischarge_Name, WaitDischarge_Text, WaitDischarge_Comment, false, WaitDischarge_Unit, WaitDischarge) + "}");
      _SetupFile.println (",{" + createSetupTag (RechargeVoltage_Name, RechargeVoltage_Text, RechargeVoltage_Comment, false, RechargeVoltage_Unit, RechargeVoltage) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void Charger::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (DoCheck_Name, DoCheck_Text, DoCheck_Comment, false, DoCheck_TextOn, DoCheck_TextOff, DoCheck) + "}");
      _SetupFile.println (",{" + createSetupTag (DoCharge_Name, DoCharge_Text, DoCharge_Comment, false, DoCharge_TextOn, DoCharge_TextOff, DoCharge) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargeState_Name, ChargeState_Text, ChargeState_Comment, true, "Idle") + "}");
      _SetupFile.println (",{" + createSetupTag (AccuVoltage_Name, AccuVoltage_Text, AccuVoltage_Comment, true, AccuVoltage_Unit, AccuVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (Current_Name, Current_Text, Current_Comment, true, Current_Unit, Current) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargedAH_Name, ChargedAH_Text, ChargedAH_Comment, true, ChargedAH_Unit, ChargedAH) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargedWH_Name, ChargedWH_Text, ChargedWH_Comment, true, ChargedWH_Unit, ChargedWH) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargedAH_Name, DischargedAH_Text, DischargedAH_Comment, true, DischargedAH_Unit, DischargedAH) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargedWH_Name, DischargedWH_Text, DischargedWH_Comment, true, DischargedWH_Unit, DischargedWH) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargeVoltage_Name, DischargeVoltage_Text, DischargeVoltage_Comment, true, DischargeVoltage_Unit, DischargeVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargeSP_Name, ChargeSP_Text, ChargeSP_Comment, true, ChargeSP_Unit, ChargeSP) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargeSP_Name, DischargeSP_Text, DischargeSP_Comment, true, DischargeSP_Unit, DischargeSP) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void Charger::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Init the Charger
     */
    bool Charger::init () {
      pinMode (PinDischarge, OUTPUT);
      pinMode (PinCharge, OUTPUT);
      digitalWrite (PinDischarge, false);
      digitalWrite (PinCharge, false);
#ifdef ESP8266
      analogWriteResolution (Resolution);
      analogWriteFreq (Frequency);
      analogWrite (PinCharge, 0);
      analogWrite (PinDischarge, 0);
#elif ESP32
      Output->setResolution (PinCharge, Resolution);
      Output->setResolution (PinDischarge, Resolution);
      Output->setFrequency (PinCharge, Frequency);
      Output->setFrequency (PinDischarge, Frequency);
      Output->write (PinCharge, 0);
      Output->write (PinDischarge, 0);
#endif
      Sensor->setInterval (UpdateInterval);
      LastMillis = millis ();
      UpdateMillis = 0;
      return true;
    }

    /**
     * @brief Handling the Charger
     * Handling the Stepper-Functions and check if DoCheck is requested
     * @param _Time Current Time to check automated feeding
     */
    void Charger::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      // Get Update Intervall
      uint32_t ActMillis = millis ();
      UpdateMillis += (ActMillis - LastMillis);
      LastMillis = ActMillis;

      if (UpdateMillis >= UpdateInterval) {
        // Read Sensor Date
        AccuVoltage = Sensor->getVoltagePlus ();
        Current = abs (Sensor->getCurrent ());
        float Power = abs (Sensor->getPowerPlus ());
        float UpdateHours = float (UpdateMillis) / 3600000.0;

        if (!(DoCharge || DoCheck)) {
          ChargeState = Charger_State_T::IDLE;
        }
        switch (ChargeState) {
        case Charger_State_T::IDLE:
          //----------------------------------------
          // Idle, waiting for Requests
          //----------------------------------------
          ChargeSP = 0.0;
          DischargeSP = 0.0;

          if (DoCharge || DoCheck) {
            CurrentStep = 0.0;
            PowerStep = 0.0,
            ChargedAH = 0.0;
            ChargedWH = 0.0;
            DischargedAH = 0.0;
            DischargedWH = 0.0;
            DischargeVoltage = 0.0;
            DischargeHold = false;
            ChargeState = Charger_State_T::CHARGE_CURRENT;
          }
          break;

        case Charger_State_T::CHARGE_CURRENT:
          //----------------------------------------
          // Charging the accu by constant current
          //----------------------------------------
          DischargeSP = 0.0;

          // Calculate charged Power
          if (DoCharge) {
            CurrentStep += (Current * UpdateHours);
            if (CurrentStep >= CURRENT_AH_STEPS) {
              ChargedAH += CURRENT_AH_STEPS;
              CurrentStep -= CURRENT_AH_STEPS;
            }
            PowerStep += (Power * UpdateHours);
            if (PowerStep >= POWER_AH_STEPS) {
              ChargedWH += POWER_AH_STEPS;
              PowerStep -= POWER_AH_STEPS;
            }
          }

          if (AccuVoltage >= AccuVoltageMax) {
            // Check Voltage to change to constant Voltage Mode
            ChargeSP -= OutputStep;
            ChargeState = Charger_State_T::CHARGE_VOLTAGE;
            StepDelay = 0;
          } else {
            // controll charging current
            float CurrentDiff = Current - AccuChargeCurrent;
            if (CurrentDiff > CurrentHyst) {
              ChargeSP -= OutputStep;
            } else if (CurrentDiff < CurrentHyst) {
              ChargeSP += OutputStep;
            }
          }
          break;

        case Charger_State_T::CHARGE_VOLTAGE:
          //----------------------------------------
          // Charging the accu by constant voltage
          //----------------------------------------
          DischargeSP = 0.0;

          // Calculate charged Power
          if (DoCharge) {
            CurrentStep += (Current * UpdateHours);
            if (CurrentStep >= CURRENT_AH_STEPS) {
              ChargedAH += CURRENT_AH_STEPS;
              CurrentStep -= CURRENT_AH_STEPS;
            }
            PowerStep += (Power * UpdateHours);
            if (PowerStep >= POWER_AH_STEPS) {
              ChargedWH += POWER_AH_STEPS;
              PowerStep -= POWER_AH_STEPS;
            }
          }

          if (Current >= AccuChargeCurrent + CurrentHyst) {
            // Check Voltage to change to constant Voltage Mode
            ChargeSP -= OutputStep;
            ChargeState = Charger_State_T::CHARGE_CURRENT;
          } else {
            // controll charging voltage
            float VoltageDiff = AccuVoltage - AccuVoltageMax;
            if (VoltageDiff > VoltageHyst) {
              ChargeSP -= OutputStep;
            } else if (VoltageDiff < VoltageHyst) {
              ChargeSP += OutputStep;
            }
          }
          if (Current < ChargeEndCurrent) {
            StepDelay += UpdateMillis;
            // Wait for Calculation, accu Voltage have to be stable
            if (StepDelay >= uint32_t (WaitDischarge) * 1000) {
              StepDelay = false;
              ChargeSP = 0.0;

              if (DoCharge) {
                // Finalize charged calculation
                ChargedAH += CurrentStep;
                CurrentStep = 0.0;
                ChargedWH += PowerStep;
                PowerStep = 0.0;
                ChargeState = Charger_State_T::WAIT_CHARGE;
              } else if (DoCheck) {
                StepDelay = 0;
                ChargeState = Charger_State_T::WAIT_DISCHARCH;
              } else {
                ChargeState = Charger_State_T::IDLE;
              }
            }
          }
          break;

        case Charger_State_T::WAIT_CHARGE:
          //----------------------------------------
          // Waiting for recharge
          //----------------------------------------
          ChargeSP = 0.0;
          DischargeSP = 0.0;
          if (AccuVoltage <= RechargeVoltage) {
            ChargeState = Charger_State_T::CHARGE_CURRENT;
          }
          break;

        case Charger_State_T::WAIT_DISCHARCH:
          //----------------------------------------
          // waiting until discharging
          //----------------------------------------
          ChargeSP = 0.0;

          if ((AccuVoltage <= (AccuVoltageMax - VoltageHyst)) || ((AccuVoltage <= AccuVoltageMax) && (DischargeHold || (Current < ChargeEndCurrent)))) {
            if (!DischargeHold) {
              DischargeHold = true;
              DischargeSave = DischargeSP;
              StepDelay = 0;
            }
            DischargeSP = 0.0;
            // Wait for Calculation, accu Voltage have to be stable
            StepDelay += UpdateMillis;
            if (StepDelay >= uint32_t (WaitDischarge) * 1000) {
              DischargeHold = false;
              ChargeState = Charger_State_T::DISCHARGE;
            }

          } else {
            if (DischargeHold) {
              DischargeHold = false;
              DischargeSP = DischargeSave;
              StepDelay = 0;
            }
            StepDelay += UpdateMillis;
            // controll discharging current
            float CurrentDiff = Current - AccuDischargeCurrent;
            if (CurrentDiff > CurrentHyst) {
              DischargeSP -= OutputStep;
            } else if ((CurrentDiff < CurrentHyst) && (StepDelay >= 2000)) {
              DischargeSP += OutputStep;
            }
          }

          break;

        case Charger_State_T::DISCHARGE:
          //----------------------------------------
          // Discharge until Low-Level
          //----------------------------------------
          ChargeSP = 0.0;

          // Calculate discharged Power
          CurrentStep += (Current * UpdateHours);
          if (CurrentStep >= CURRENT_AH_STEPS) {
            DischargedAH += CURRENT_AH_STEPS;
            CurrentStep -= CURRENT_AH_STEPS;
          }
          PowerStep += (Power * UpdateHours);
          if (PowerStep >= POWER_AH_STEPS) {
            DischargedWH += POWER_AH_STEPS;
            PowerStep -= POWER_AH_STEPS;
          }

          if ((AccuVoltage <= AccuVoltageMin) && (Current <= DischargeEndCurrent)) {
            DischargeHold = true;
          }
          if (DischargeHold) {
            StepDelay += UpdateMillis;
            // Wait for Calculation, accu Voltage have to be stable
            if (StepDelay >= uint32_t (WaitDischarge) * 1000) {
              DoCheck = false;
              DoCharge = true;
              DischargeVoltage = AccuVoltage;
              DischargedAH += CurrentStep;
              CurrentStep = 0.0;
              DischargedWH += PowerStep;
              PowerStep = 0.0;
              DischargeHold = false;
              ChargeState = Charger_State_T::CHARGE_CURRENT;
            }
          } else {
            StepDelay = 0;
            // controll discharging current
            float CurrentDiff = Current - AccuDischargeCurrent;
            if (CurrentDiff > CurrentHyst) {
              DischargeSP -= OutputStep;
            } else if (CurrentDiff < CurrentHyst) {
              DischargeSP += OutputStep;
            }
          }

          break;

        default:
          ChargeSP = 0.0;
          DischargeSP = 0.0;
          ChargeState = Charger_State_T::IDLE;
          break;
        }
        if (Debug.print (FLAG_DATA, false, Name, __func__, "State:")) {
          Debug.println (FLAG_DATA, false, Name, __func__, ChargeState);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Voltage   [V]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, AccuVoltage);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Current   [A]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, Current);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Charge    [%]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, ChargeSP);
          Debug.print (FLAG_DATA, false, Name, __func__, " - Discharge [%]: ");
          Debug.println (FLAG_DATA, false, Name, __func__, DischargeSP);
        }
        // Write Output
        if (ChargeSP < 0.0) {
          ChargeSP = 0.0;
        } else if (ChargeSP > 100.0) {
          ChargeSP = 100.0;
        }
        if (DischargeSP < 0.0) {
          DischargeSP = 0.0;
        } else if (DischargeSP > 100.0) {
          DischargeSP = 100.0;
        }
#ifdef ESP8266
        analogWrite (PinCharge, uint32_t (ChargeSP * DutyScale));
        analogWrite (PinDischarge, uint32_t (DischargeSP * DutyScale));
#elif ESP32
        Output->write (PinCharge, uint32_t (ChargeSP * DutyScale));
        Output->write (PinDischarge, uint32_t (DischargeSP * DutyScale));
#endif
        UpdateMillis = 0;
      }
    }
  }
}
