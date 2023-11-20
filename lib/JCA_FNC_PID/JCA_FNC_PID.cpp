/**
 * @file JCA_FNC_Feeder.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element for a daily PID. With variable Time and Amount
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_PID.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    /* #region(collapsed) Datapoint description */
    const char *PID::Gain_Name = "Gain";
    const char *PID::Gain_Text = "Verstärkungsfaktor";
    const char *PID::Gain_Unit = "";
    const char *PID::Gain_Comment = nullptr;
    const char *PID::IntergalTime_Name = "IntergalTime";
    const char *PID::IntergalTime_Text = "Intergal Zeit";
    const char *PID::IntergalTime_Unit = "s";
    const char *PID::IntergalTime_Comment = nullptr;
    const char *PID::DerivativeTime_Name = "DerivativeTime";
    const char *PID::DerivativeTime_Text = "Differenzial Zeit";
    const char *PID::DerivativeTime_Unit = "2";
    const char *PID::DerivativeTime_Comment = nullptr;
    const char *PID::UpdateInterval_Name = "UpdateInterval";
    const char *PID::UpdateInterval_Text = "Aktuallisierungs Interval";
    const char *PID::UpdateInterval_Unit = "ms";
    const char *PID::UpdateInterval_Comment = nullptr;
    const char *PID::ValueMin_Name = "ValueMin";
    const char *PID::ValueMin_Text = "Istwert Min";
    const char *PID::ValueMin_Unit = "°C";
    const char *PID::ValueMin_Comment = nullptr;
    const char *PID::ValueMax_Name = "ValueMax";
    const char *PID::ValueMax_Text = "Istwert Max";
    const char *PID::ValueMax_Unit = "°C";
    const char *PID::ValueMax_Comment = nullptr;
    const char *PID::SetpointAuto_Name = "SetpointAuto";
    const char *PID::SetpointAuto_Text = "Sollwert Automatik";
    const char *PID::SetpointAuto_Unit = "°C";
    const char *PID::SetpointAuto_Comment = nullptr;
    const char *PID::SetpointManual_Name = "SetpointManual";
    const char *PID::SetpointManual_Text = "Sollwert Hand";
    const char *PID::SetpointManual_Unit = "°C";
    const char *PID::SetpointManual_Comment = nullptr;
    const char *PID::OutputAuto_Name = "OutputAuto";
    const char *PID::OutputAuto_Text = "Stellwert Automatik";
    const char *PID::OutputAuto_Unit = "%";
    const char *PID::OutputAuto_Comment = nullptr;
    const char *PID::OutputManual_Name = "OutputManual";
    const char *PID::OutputManual_Text = "Stellwert Hand";
    const char *PID::OutputManual_Unit = "%";
    const char *PID::OutputManual_Comment = nullptr;
    const char *PID::ManualSetpoint_Name = "ManualSetpoint";
    const char *PID::ManualSetpoint_Text = "Hand-Sollwert aktiv";
    const char *PID::ManualSetpoint_Comment = nullptr;
    const char *PID::ManualSetpoint_TextOn = nullptr;
    const char *PID::ManualSetpoint_TextOff = nullptr;
    const char *PID::ManualOutput_Name = "ManualOutput";
    const char *PID::ManualOutput_Text = "Hand-Stellwert aktiv";
    const char *PID::ManualOutput_Comment = nullptr;
    const char *PID::ManualOutput_TextOn = nullptr;
    const char *PID::ManualOutput_TextOff = nullptr;
    const char *PID::CurrentValue_Name = "CurrentValue";
    const char *PID::CurrentValue_Text = "Istwert";
    const char *PID::CurrentValue_Unit = "°C";
    const char *PID::CurrentValue_Comment = nullptr;
    /* #endregion */

    /**
     * @brief Construct a new PID::PID object
     *
     * @param _PinOutput Output Pin that ist Connected to the Discharging-Driver
     * @param _Name Element Name inside the Communication
     * @param _Output Pointer to the PWM-Handling-Object
     */
    PID::PID (uint8_t _PinOutput, const char *_Name, PwmOutput *_Output)
        : Parent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      // Intern
      Resolution = 10;
      DutyScale = (pow (2.0, float (Resolution)) - 1.0) / 100.0;
      Frequency = 3000;
      InitDone = false;
      OutputP = 0.0;
      OutputI = 0.0;
      OutputD = 0.0;
      OldValue = 0.0;

      // Konfig
      Gain = 0.8;
      IntergalTime = 5.0;
      DerivativeTime = 0.0;
      UpdateInterval = 1000;
      ValueMin = 0.0;
      ValueMax = 150.0;

      // Daten
      SetpointAuto = 0.0;
      SetpointManual = 0.0;
      OutputAuto = 0.0;
      OutputManual = 0.0;
      ManualSetpoint = false;
      ManualOutput = false;
      CurrentValue = 0.0;

      // Hardware
      Output = _Output;
      PinOutput = _PinOutput;
    }

    /**
     * @brief Add Config-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Config-Tags ("config": {})
     */
    void PID::createConfigValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[Gain_Name] = Gain;
      _Values[IntergalTime_Name] = IntergalTime;
      _Values[DerivativeTime_Name] = DerivativeTime;
      _Values[UpdateInterval_Name] = UpdateInterval;
      _Values[ValueMin_Name] = ValueMin;
      _Values[ValueMax_Name] = ValueMax;
    }

    /**
     * @brief Add Data-Tags to a JSON-Object, containing the current Value
     *
     * @param _Values Object to add the Data-Tags ("data": {})
     */
    void PID::createDataValues (JsonObject &_Values) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Get");
      _Values[SetpointAuto_Name] = SetpointAuto;
      _Values[SetpointManual_Name] = SetpointManual;
      _Values[OutputAuto_Name] = OutputAuto;
      _Values[OutputManual_Name] = OutputManual;
      _Values[ManualSetpoint_Name] = ManualSetpoint;
      _Values[ManualOutput_Name] = ManualOutput;
      _Values[CurrentValue_Name] = CurrentValue;
    }

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void PID::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == Gain_Name) {
          Gain = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, Gain_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, Gain);
          }
        }
        if (Tag[JsonTagName] == IntergalTime_Name) {
          IntergalTime = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, IntergalTime_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, IntergalTime);
          }
        }
        if (Tag[JsonTagName] == DerivativeTime_Name) {
          DerivativeTime = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, DerivativeTime_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, DerivativeTime);
          }
        }
        if (Tag[JsonTagName] == UpdateInterval_Name) {
          UpdateInterval = Tag[JsonTagValue].as<uint16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, UpdateInterval_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, UpdateInterval);
          }
        }
        if (Tag[JsonTagName] == ValueMin_Name) {
          ValueMin = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ValueMin_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ValueMin);
          }
        }
        if (Tag[JsonTagName] == ValueMax_Name) {
          ValueMax = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ValueMax_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ValueMax);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void PID::setData (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == SetpointManual_Name) {
          SetpointManual = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, SetpointManual_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, SetpointManual);
          }
        }
        if (Tag[JsonTagName] == OutputManual_Name) {
          OutputManual = Tag[JsonTagValue].as<float> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, OutputManual_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, OutputManual);
          }
        }
        if (Tag[JsonTagName] == ManualSetpoint_Name) {
          ManualSetpoint = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, ManualSetpoint_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, ManualSetpoint);
          }
        }
        if (Tag[JsonTagName] == ManualOutput_Name) {
          ManualOutput = Tag[JsonTagValue].as<bool> ();
          if (Debug.print (FLAG_LOOP, false, Name, __func__, ManualOutput_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_LOOP, false, Name, __func__, ManualOutput);
          }
        }
      }
    }

    /**
     * @brief Execute the Commands
     *
     * @param _Tags Array of Commands ("cmd": [])
     */
    void PID::setCmd (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
    }

    /**
     * @brief Write the Config-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void PID::writeSetupConfig (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagConfig) + "\":[");
      _SetupFile.println ("{" + createSetupTag (Gain_Name, Gain_Text, Gain_Comment, false, Gain_Unit, Gain) + "}");
      _SetupFile.println (",{" + createSetupTag (IntergalTime_Name, IntergalTime_Text, IntergalTime_Comment, false, IntergalTime_Unit, IntergalTime) + "}");
      _SetupFile.println (",{" + createSetupTag (DerivativeTime_Name, DerivativeTime_Text, DerivativeTime_Comment, false, DerivativeTime_Unit, DerivativeTime) + "}");
      _SetupFile.println (",{" + createSetupTag (UpdateInterval_Name, UpdateInterval_Text, UpdateInterval_Comment, false, UpdateInterval_Unit, UpdateInterval) + "}");
      _SetupFile.println (",{" + createSetupTag (ValueMin_Name, ValueMin_Text, ValueMin_Comment, true, ValueMin_Unit, ValueMin) + "}");
      _SetupFile.println (",{" + createSetupTag (ValueMax_Name, ValueMax_Text, ValueMax_Comment, true, ValueMax_Unit, ValueMax) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Data-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void PID::writeSetupData (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
      _SetupFile.println (",\"" + String (JsonTagData) + "\":[");
      _SetupFile.println ("{" + createSetupTag (SetpointAuto_Name, SetpointAuto_Text, SetpointAuto_Comment, false, SetpointAuto_Unit, SetpointAuto) + "}");
      _SetupFile.println (",{" + createSetupTag (SetpointManual_Name, SetpointManual_Text, SetpointManual_Comment, false, SetpointManual_Unit, SetpointManual) + "}");
      _SetupFile.println (",{" + createSetupTag (OutputAuto_Name, OutputAuto_Text, OutputAuto_Comment, false, OutputAuto_Unit, OutputAuto) + "}");
      _SetupFile.println (",{" + createSetupTag (OutputManual_Name, OutputManual_Text, OutputManual_Comment, false, OutputManual_Unit, OutputManual) + "}");
      _SetupFile.println (",{" + createSetupTag (ManualSetpoint_Name, ManualSetpoint_Text, ManualSetpoint_Comment, false, ManualSetpoint_TextOn, ManualSetpoint_TextOff, false) + "}");
      _SetupFile.println (",{" + createSetupTag (ManualOutput_Name, ManualOutput_Text, ManualOutput_Comment, false, ManualOutput_TextOn, ManualOutput_TextOff, false) + "}");
      _SetupFile.println (",{" + createSetupTag (CurrentValue_Name, CurrentValue_Text, CurrentValue_Comment, true, CurrentValue_Unit, CurrentValue) + "}");
      _SetupFile.println ("]");
    }

    /**
     * @brief Write the Command-Tags to Setup-File
     *
     * @param _SetupFile File to write
     */
    void PID::writeSetupCmdInfo (File _SetupFile) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Write");
    }

    /**
     * @brief Init the PID
     */
    bool PID::init (ValueCallback _GetCurrentValueCB) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Setup");
      InitDone = Output->setupPin (PinOutput, Frequency, Resolution);
      if (_GetCurrentValueCB) {
        getCurrentValueCB = _GetCurrentValueCB;
      } else {
        InitDone = false;
      }
      LastMillis = millis ();
      UpdateMillis = 0;
      return true;
    }

    /**
     * @brief Handling the PID
     * Handling the Charge-Discharge-Function and check if DoCheck is requested
     * @param _Time Current Time
     */
    void PID::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      // Get Update Intervall
      uint32_t ActMillis = millis ();
      UpdateMillis += (ActMillis - LastMillis);
      LastMillis = ActMillis;
      float CurrentDiff;
      float VoltageDiff;

      if (UpdateMillis >= UpdateInterval) {
        if (InitDone) {
          float ActSetpoint;
          float ActOutput;

          // Select Setpoint
          if (ManualSetpoint) {
            ActSetpoint = SetpointManual;
          } else {
            ActSetpoint = SetpointAuto;
            SetpointManual = SetpointAuto;
          }

          // Calculation PID
          // TODO

          // Reverse calc PID
          // TODO

          // Output
          Output->writePin (PinOutput, ActOutput);
        }
        UpdateMillis = 0;
      }
    }
  }
}
