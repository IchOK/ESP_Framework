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
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const float Charger::CurrentHyst = 0.01;
    const float Charger::VoltageHyst = 0.01;
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
    Charger::Charger (INA219 *_Sensor, uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name, PwmOutput *_Output)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagFloat ("AccuVoltageMax", "Maximale Akku Ladespannung", "", false, TagUsage_T::UseConfig, &AccuVoltageMax, "V"));
      Tags.push_back (new TagFloat ("AccuVoltageMin", "Grenzwert Akku Entladen", "", false, TagUsage_T::UseConfig, &AccuVoltageMin, "V"));
      Tags.push_back (new TagFloat ("AccuChargeCurrent", "Maximaler Akku Ladestrom", "", false, TagUsage_T::UseConfig, &AccuChargeCurrent, "A"));
      Tags.push_back (new TagFloat ("AccuDischargeVoltage", "Minimale Akku Entladespannung", "", false, TagUsage_T::UseConfig, &AccuDischargeVoltage, "V"));
      Tags.push_back (new TagFloat ("AccuDischargeCurrent", "Maximaler Akku Entladestrom", "", false, TagUsage_T::UseConfig, &AccuDischargeCurrent, "A"));
      Tags.push_back (new TagFloat ("ChargeEndCurrent", "", "Laden Beendet Strom", false, TagUsage_T::UseConfig, &ChargeEndCurrent, "A"));
      Tags.push_back (new TagFloat ("DischargeEndCurrent", "Genzwert Akku Entladen", "", false, TagUsage_T::UseConfig, &DischargeEndCurrent, "A"));
      Tags.push_back (new TagUInt16 ("WaitDischarge", "Wartezeit für Entlade", "", false, TagUsage_T::UseConfig, &WaitDischarge, "s"));
      Tags.push_back (new TagFloat ("RechargeVoltage", "Spannungs Nachladen", "", false, TagUsage_T::UseConfig, &RechargeVoltage, "V"));

      Tags.push_back (new TagBool ("DoCheck", "Akku Prüfung aktiv", "", false, TagUsage_T::UseData, &DoCheck, "EIN", "AUS"));
      Tags.push_back (new TagBool ("DoCharge", "Akku Laden aktiv", "", false, TagUsage_T::UseData, &DoCharge, "EIN", "AUS"));
      Tags.push_back (new TagBool ("Fault", "Fehler aktiv", "", true, TagUsage_T::UseData, &Fault, "EIN", "AUS"));
      Tags.push_back (new TagFloat ("AccuVoltage", "Akku Spannung", "", true, TagUsage_T::UseData, &AccuVoltage, "V"));
      Tags.push_back (new TagFloat ("Current", "Akku Strom", "", true, TagUsage_T::UseData, &Current, "A"));
      Tags.push_back (new TagFloat ("ChargedAH", "Geladener Strom", "", true, TagUsage_T::UseData, &ChargedAH, "AH"));
      Tags.push_back (new TagFloat ("ChargedWH", "Geladene Leistung", "", true, TagUsage_T::UseData, &ChargedWH, "WH"));
      Tags.push_back (new TagFloat ("DischargedAH", "Entladener Strom", "", true, TagUsage_T::UseData, &DischargedAH, "AH"));
      Tags.push_back (new TagFloat ("DischargedWH", "Entledene Leistung", "", true, TagUsage_T::UseData, &DischargedWH, "WH"));
      Tags.push_back (new TagFloat ("DischargeVoltage", "Entladespannung", "", true, TagUsage_T::UseData, &DischargeVoltage, "V"));
      Tags.push_back (new TagFloat ("ChargeSP", "PWM Laden", "", true, TagUsage_T::UseData, &ChargeSP, "%"));
      Tags.push_back (new TagFloat ("DischargeSP", "PWM Entladen", "", true, TagUsage_T::UseData, &DischargeSP, "%"));

      Tags.push_back (new TagListUInt8 ("ChargeState", "Status", "", true, TagUsage_T::UseData, (uint8_t *)&ChargeState));
      TagListUInt8 *ChargeStateElement = static_cast<TagListUInt8 *> (Tags[Tags.size () - 1]);
      ChargeStateElement->List.insert ({Charger_State_T::IDLE, "Inaktiv"});
      ChargeStateElement->List.insert ({ Charger_State_T::CHARGE_CURRENT, "Laden Strom" });
      ChargeStateElement->List.insert ({ Charger_State_T::CHARGE_VOLTAGE, "Laden Spannung" });
      ChargeStateElement->List.insert ({ Charger_State_T::WAIT_CHARGE, "Warten Laden" });
      ChargeStateElement->List.insert ({ Charger_State_T::WAIT_DISCHARCH, "Warten Entladen" });
      ChargeStateElement->List.insert ({ Charger_State_T::DISCHARGE, "Entlade" });
      ChargeStateElement->List.insert ({ Charger_State_T::FAULT, "Fehler" });

      // Tags only used for in-/output


      // Intern
      ChargeState = Charger_State_T::IDLE;
      Resolution = 10;
      DutyScale = (pow (2.0, float (Resolution)) - 1.0) / 100.0;
      Frequency = 3000;

      // Konfig
      AccuVoltageMax = 4.2;
      AccuVoltageMin = 3.1;
      AccuChargeCurrent = 1.0;
      AccuDischargeVoltage = 3.5;
      AccuDischargeCurrent = 1.0;
      ChargeEndCurrent = 0.1;
      DischargeEndCurrent = 0.2;
      WaitDischarge = 20;
      RechargeVoltage = 4.0;

      // Daten
      DoCharge = false;
      DoCheck = false;
      Fault = false;
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
     * @brief Init the Charger
     */
    bool Charger::init () {
      Output->setupPin (PinCharge, Frequency, Resolution);
      Output->setupPin (PinDischarge, Frequency, Resolution);
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
      float CurrentDiff;
      float VoltageDiff;

      if (UpdateMillis >= UpdateInterval) {
        // Read Sensor Date
        AccuVoltage = Sensor->getVoltagePlus ();
        Current = abs (Sensor->getCurrent ());
        float Power = abs (Sensor->getPowerPlus ());
        float UpdateHours = float (UpdateMillis) / 3600000.0;

        // Go to Idle fi Charging is deactivated
        if (!(DoCharge || DoCheck)) {
          ChargeState = Charger_State_T::IDLE;
        }

        // Go to fault state ist Sensordata not valid
        if ((ChargeState != IDLE) && (ChargeState != FAULT)) {
          
          if (AccuVoltage < 1.0) {
            FaultDelay += UpdateMillis;
          } else if ((Current < 0.001) && ((ChargeSP > 10.0) or (DischargeSP > 10.0))) {
            FaultDelay += UpdateMillis;
          } else {
            FaultDelay = 0;
          }
          if (FaultDelay >= 2000) {
            if(Debug.println (FLAG_ERROR, false, Name, __func__, "Charging-Data not valid")) {
              Debug.print (FLAG_ERROR, false, Name, __func__, " - AccuVoltage: ");
              Debug.println (FLAG_ERROR, false, Name, __func__, AccuVoltage);
              Debug.print (FLAG_ERROR, false, Name, __func__, " - Current: ");
              Debug.println (FLAG_ERROR, false, Name, __func__, Current);
              Debug.print (FLAG_ERROR, false, Name, __func__, " - ChargeSP: ");
              Debug.println (FLAG_ERROR, false, Name, __func__, ChargeSP);
              Debug.print (FLAG_ERROR, false, Name, __func__, " - DischargeSP: ");
              Debug.println (FLAG_ERROR, false, Name, __func__, DischargeSP);
            }
            FaultState = ChargeState;
            ChargeState = FAULT;
            Fault = true;
          }
        } else {
          FaultDelay = 0;
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
            CurrentDiff = Current - AccuChargeCurrent;
            if (CurrentDiff > CurrentHyst) {
              ChargeSP -= OutputStep;
            } else if (CurrentDiff < -CurrentHyst) {
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
            VoltageDiff = AccuVoltage - AccuVoltageMax;
            if (VoltageDiff > VoltageHyst) {
              ChargeSP -= OutputStep;
            } else if (VoltageDiff < -VoltageHyst) {
              ChargeSP += OutputStep;
            }
          }
          if (Current < ChargeEndCurrent) {
            StepDelay += UpdateMillis;
            // Wait for Calculation, accu Voltage have to be stable
            if (StepDelay >= 10000) {
              StepDelay = 0;
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

          // controll charging voltage
          VoltageDiff = AccuVoltage - AccuVoltageMax;
          CurrentDiff = Current - AccuDischargeCurrent;

          if (CurrentDiff > CurrentHyst) {
            DischargeSP -= OutputStep;
          } else if (VoltageDiff < -VoltageHyst) {
            DischargeSP -= OutputStep;
          } else if (VoltageDiff > VoltageHyst) {
            DischargeSP += OutputStep;
          }

          if (Current < ChargeEndCurrent) {
            StepDelay += UpdateMillis;
            // Wait for Calculation, accu Voltage have to be stable
            if (StepDelay >= uint32_t (WaitDischarge) * 1000) {
              StepDelay = 0;
              DischargeSP = 0.0;
              ChargeState = Charger_State_T::DISCHARGE;
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

          if ((AccuVoltage <= AccuVoltageMin) || ((AccuVoltage <= AccuVoltageMin) && (Current <= DischargeEndCurrent))) {
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
            CurrentDiff = Current - AccuDischargeCurrent;
            if (CurrentDiff > CurrentHyst) {
              DischargeSP -= OutputStep;
            } else if (CurrentDiff < -CurrentHyst) {
              DischargeSP += OutputStep;
            }
          }

          break;

        default:
          ChargeSP = 0.0;
          DischargeSP = 0.0;
          ChargeState = Charger_State_T::IDLE;
          break;

        case Charger_State_T::FAULT:
          //----------------------------------------
          // Fault State have to Reset
          //----------------------------------------
          ChargeSP = 0.0;
          DischargeSP = 0.0;
          if (!Fault) {
            ChargeState = FaultState;
          }

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
        Output->writePin (PinCharge, ChargeSP);
        Output->writePin (PinDischarge, DischargeSP);
        UpdateMillis = 0;
      }
    }

    bool Charger::create (JsonObject _Config, std::vector<FuncParent *> &_Functions) {
      Debug.println (FLAG_SETUP, false, "Create", __func__, "FNC::Charger");
//      Charger (INA219 * _Sensor, uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name, JCA::SYS::PwmOutput *_Output);
//      _Functions.push_back(new Charger(JCA_FNC_INA219_Sensor, ))
    }
  }
}
