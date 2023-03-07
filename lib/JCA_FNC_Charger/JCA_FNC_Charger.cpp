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
    const char *Charger::AccuVoltageMin_Text = "Minimale Akku Entladespannung";
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
    const char *Charger::AccuVoltage_Text = "Verbleibende Schritte";
    const char *Charger::AccuVoltage_Unit = "st";
    const char *Charger::AccuVoltage_Comment = nullptr;
    const char *Charger::Current_Name = "Current";
    const char *Charger::Current_Text = "aktuelle Geschwindigkeit";
    const char *Charger::Current_Unit = "st/s";
    const char *Charger::Current_Comment = nullptr;
    const char *Charger::ChargedAH_Name = "ChagedAH";
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

    /**
     * @brief Construct a new Charger::Charger object
     *
     * @param _PinEnable Pin that is connected to the Enable in on the Stepper-Driver
     * @param _PinStep Pin that is connected to the Step in on the Stepper-Driver
     * @param _PinDir Pin that is connected to the Direction in on the Stepper-Driver
     * @param _Name Element Name inside the Communication
     */
    Charger::Charger (INA219 *_Sensor, uint8_t _PinCharge, uint8_t _PinDischarge, const char *_Name)
        : Parent (_Name) {

      // Intern
      ChargeState = Charger_State::IDLE;

      // Konfig
      AccuVoltageMax = 4.2;
      AccuVoltageMin = 3.0;
      AccuChargeCurrent = 1.0;
      AccuDischargeCurrent = 1.0;

      // Daten
      DoCharge = true;
      DoCheck = false;
      AccuVoltage = 0.0;
      Current = 0.0;
      ChargedAH = 0.0;
      ChargedWH = 0.0;
      DischargedAH = 0.0;
      DischargedWH = 0.0;

      // Hardware
      PinCharge = _PinCharge;
      PinDischarge = _PinDischarge;
      Sensor = _Sensor;
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
          AccuVoltageMax = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AccuVoltageMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AccuVoltageMax);
          }
        }
        if (Tag[JsonTagName] == AccuVoltageMin_Name) {
          AccuVoltageMin = Tag[JsonTagValue].as<int16_t> ();
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
        if (Tag[JsonTagName] == WaitDischarge_Name) {
          WaitDischarge = Tag[JsonTagValue].as<float> ();
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
    }

    /**
     * @brief Create a list of Config-Tags containing the current Value
     *
     * @param _Tags Array the Tags have to add
     */
    void Charger::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      _SetupFile.println (",\"" + String(JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (AccuVoltageMax_Name, AccuVoltageMax_Text, AccuVoltageMax_Comment, false, AccuVoltageMax_Unit, AccuVoltageMax) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuVoltageMin_Name, AccuVoltageMin_Text, AccuVoltageMin_Comment, false, AccuVoltageMin_Unit, AccuVoltageMin) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuChargeCurrent_Name, AccuChargeCurrent_Text, AccuChargeCurrent_Comment, false, AccuChargeCurrent_Unit, AccuChargeCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuDischargeCurrent_Name, AccuDischargeCurrent_Text, AccuDischargeCurrent_Comment, false, AccuDischargeCurrent_Unit, AccuDischargeCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargeEndCurrent_Name, ChargeEndCurrent_Text, ChargeEndCurrent_Comment, false, ChargeEndCurrent_Unit, ChargeEndCurrent) + "}");
      _SetupFile.println (",{" + createSetupTag (WaitDischarge_Name, WaitDischarge_Text, WaitDischarge_Comment, false, WaitDischarge_Unit, WaitDischarge) + "}");
      _SetupFile.println (",{" + createSetupTag (RechargeVoltage_Name, RechargeVoltage_Text, RechargeVoltage_Comment, false, RechargeVoltage_Unit, RechargeVoltage) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Create a list of Data-Tags containing the current Value
     *
     * @param _Tags Array the Tags have to add
     */
    void Charger::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      _SetupFile.println (",\"" + String(JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (DoCheck_Name, DoCheck_Text, DoCheck_Comment, false, DoCheck_TextOn, DoCheck_TextOff, DoCheck) + "}");
      _SetupFile.println (",{" + createSetupTag (DoCharge_Name, DoCharge_Text, DoCharge_Comment, false, DoCharge_TextOn, DoCharge_TextOff, DoCharge) + "}");
      _SetupFile.println (",{" + createSetupTag (AccuVoltage_Name, AccuVoltage_Text, AccuVoltage_Comment, true, AccuVoltage_Unit, AccuVoltage) + "}");
      _SetupFile.println (",{" + createSetupTag (Current_Name, Current_Text, Current_Comment, true, Current_Unit, Current) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargedAH_Name, ChargedAH_Text, ChargedAH_Comment, true, ChargedAH_Unit, ChargedAH) + "}");
      _SetupFile.println (",{" + createSetupTag (ChargedWH_Name, ChargedWH_Text, ChargedWH_Comment, true, ChargedWH_Unit, ChargedWH) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargedAH_Name, DischargedAH_Text, DischargedAH_Comment, true, DischargedAH_Unit, DischargedAH) + "}");
      _SetupFile.println (",{" + createSetupTag (DischargedWH_Name, DischargedWH_Text, DischargedWH_Comment, true, DischargedWH_Unit, DischargedWH) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Create a list of Command-Informations
     *
     * @param _Tags Array the Command-Infos have to add
     */
    void Charger::writeSetupCmdInfo (File _SetupFile) {
    }

    void Charger::createConfigValues (JsonObject &_Values) {
      _Values[AccuVoltageMax_Name] = AccuVoltageMax;
      _Values[AccuVoltageMin_Name] = AccuVoltageMin;
      _Values[AccuChargeCurrent_Name] = AccuChargeCurrent;
      _Values[AccuDischargeCurrent_Name] = AccuDischargeCurrent;
      _Values[ChargeEndCurrent_Name] = ChargeEndCurrent;
      _Values[WaitDischarge_Name] = WaitDischarge;
      _Values[RechargeVoltage_Name] = RechargeVoltage;
    }

    void Charger::createDataValues (JsonObject &_Values) {
      _Values[DoCheck_Name] = DoCheck;
      _Values[DoCharge_Name] = DoCharge;
      _Values[Current_Name] = Current;
      _Values[ChargedAH_Name] = ChargedAH;
      _Values[ChargedWH_Name] = ChargedWH;
      _Values[DischargedAH_Name] = DischargedAH;
      _Values[DischargedWH_Name] = DischargedWH;
    }

    void writeSetupConfig (File _SetupFile) {

    }

    void writeSetupData (File _SetupFile) {

    }

    void writeSetupCmdInfo (File _SetupFile) {

    }

    /**
     * @brief Handling the Charger
     * Handling the Stepper-Functions and check if DoCheck is requested
     * @param _Time Current Time to check automated feeding
     */
    void Charger::update (struct tm &_Time) {
      bool AutoFeed = AccuVoltageMax == _Time.tm_hour && AccuVoltageMin == _Time.tm_min && _Time.tm_year > 100;

    }
  }
}
