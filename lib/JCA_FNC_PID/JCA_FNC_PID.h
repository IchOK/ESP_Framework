/**
 * @file JCA_FNC_PID.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element to charge and discharge an Accu with Capacity calculation
 * @version 0.1
 * @date 2023-03-07
 *
 * Copyright Jochen Cabrera 2023
 * Apache License
 *
 */

#ifndef _JCA_FNC_PID_
#define _JCA_FNC_PID_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_Functions.h>
#include <JCA_SYS_PwmOutput.h>

namespace JCA {
  namespace FNC {
    class PID : public Parent {
    private:
      /* #region(collapsed) Datapoint description */
      static const char *Gain_Name;
      static const char *Gain_Text;
      static const char *Gain_Unit;
      static const char *Gain_Comment;
      static const char *IntergalTime_Name;
      static const char *IntergalTime_Text;
      static const char *IntergalTime_Unit;
      static const char *IntergalTime_Comment;
      static const char *DerivativeTime_Name;
      static const char *DerivativeTime_Text;
      static const char *DerivativeTime_Unit;
      static const char *DerivativeTime_Comment;
      static const char *UpdateInterval_Name;
      static const char *UpdateInterval_Text;
      static const char *UpdateInterval_Unit;
      static const char *UpdateInterval_Comment;
      static const char *ValueMin_Name;
      static const char *ValueMin_Text;
      static const char *ValueMin_Unit;
      static const char *ValueMin_Comment;
      static const char *ValueMax_Name;
      static const char *ValueMax_Text;
      static const char *ValueMax_Unit;
      static const char *ValueMax_Comment;
      static const char *SetpointAuto_Name;
      static const char *SetpointAuto_Text;
      static const char *SetpointAuto_Unit;
      static const char *SetpointAuto_Comment;
      static const char *SetpointManual_Name;
      static const char *SetpointManual_Text;
      static const char *SetpointManual_Unit;
      static const char *SetpointManual_Comment;
      static const char *OutputAuto_Name;
      static const char *OutputAuto_Text;
      static const char *OutputAuto_Unit;
      static const char *OutputAuto_Comment;
      static const char *OutputManual_Name;
      static const char *OutputManual_Text;
      static const char *OutputManual_Unit;
      static const char *OutputManual_Comment;
      static const char *ManualSetpoint_Name;
      static const char *ManualSetpoint_Text;
      static const char *ManualSetpoint_Comment;
      static const char *ManualSetpoint_TextOn;
      static const char *ManualSetpoint_TextOff;
      static const char *ManualOutput_Name;
      static const char *ManualOutput_Text;
      static const char *ManualOutput_Comment;
      static const char *ManualOutput_TextOn;
      static const char *ManualOutput_TextOff;
      static const char *CurrentValue_Name;
      static const char *CurrentValue_Text;
      static const char *CurrentValue_Unit;
      static const char *CurrentValue_Comment;
      /* #endregion */

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
      PwmOutput *Output;
      uint8_t PinOutput;
      ValueCallback getCurrentValueCB;

      // Konfig
      float Gain;
      float IntergalTime;
      float DerivativeTime;
      uint16_t UpdateInterval;
      float ValueMin;
      float ValueMax;

      // Daten
      float SetpointAuto;
      float SetpointManual;
      float OutputAuto;
      float OutputManual;
      bool ManualSetpoint;
      bool ManualOutput;
      float CurrentValue;

      // Intern
      uint8_t Resolution;
      uint32_t Frequency;
      float DutyScale;
      bool InitDone;
      uint32_t LastMillis;
      uint32_t UpdateMillis;
      float OutputP;
      float OutputI;
      float OutputD;
      float OldValue;

    public:
      PID (uint8_t _PinOutput, const char *_Name, JCA::SYS::PwmOutput *_Output);
      bool init (ValueCallback _GetCurrentValueCB);
      void update (struct tm &_Time);
    };
  }
}

#endif