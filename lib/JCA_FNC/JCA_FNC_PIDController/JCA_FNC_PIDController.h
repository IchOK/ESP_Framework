/**
 * @file JCA_FNC_PIDController.h
 * @brief Framework Element for a PID Controller with separate Hand-Auto modes for setpoint and output.
 * @version 0.3
 * @date 2025-04-13
 *
 * Apache License
 */

#ifndef _JCA_FNC_PIDCONTROLLER_
#define _JCA_FNC_PIDCONTROLLER_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagBool.h>

namespace JCA {
  namespace FNC {
    class PIDController : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagProcessUnit;
      static const char *SetupTagOutputUnit;

      // Konfig
      float P;  // Proportional gain
      float Ti; // Integral time
      float Td; // Derivative time
      float TdLag; // Lag time for derivative
      float SetpointMin; // Minimum setpoint value
      float SetpointMax; // Maximum setpoint value
      float OutputMin;    // Minimum output value
      float OutputMax;    // Maximum output value

      // Daten
      float Setpoint;       // Sollwert
      float ManualSetpoint; // Manuell eingestellter Sollwert
      float ProcessVar;     // Istwert
      float Value;         // Stellwert
      bool ManualSetpointMode; // Hand-Auto-Umschaltung für Sollwert
      bool ManualOutputMode; // Hand-Auto-Umschaltung für Stellwert

      // Intern
      float Proportional;
      float Integral;
      float Derivative;
      float LastError;
      float LastDerivative;
      float LastProcessVar;
      unsigned long LastUpdateMillis;

    public:
      PIDController(String _Name, String _ProcessUnit, String _OutputUnit);
      ~PIDController() {;}

      void update(struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      static bool Create(JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif