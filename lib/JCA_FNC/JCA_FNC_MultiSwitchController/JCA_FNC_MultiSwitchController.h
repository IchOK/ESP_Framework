/**
 * @file JCA_FNC_MultiSwitchController.h
 * @brief Framework Element for a Multi-Stage On/Off Controller with an arbitrary number of staged outputs.
 *
 * Each output owns its own switch-on hysteresis (OnHyst[i]). One single common
 * switch-off hysteresis (OffHyst) is shared across all outputs.
 *
 * OnHyst[i] == 0: stage i is disabled (never selected, pin stays off).
 *
 * Switching rules use effective PV/SP (sign flip when InvertDirection is true).
 * Normal effective direction:
 *  - Strict ON for stage i: PvEff < SpEff - OnHyst[i]
 *  - Global OFF: PvEff > SpEff + OffHyst
 *  - Staging hysteresis: while stage k is active it stays on until
 *    PvEff > SpEff - OnHyst[k] + OffHyst; then the best strict candidate
 *    (largest OnHyst among enabled stages) is chosen again.
 *  - While latched, if a strictly "stronger" stage qualifies (larger OnHyst),
 *    switch up immediately.
 *  - Only one output is physically on at a time. When changing from one stage
 *    to another non-zero stage, outputs go all-off first, then after
 *    SwitchDeadTimeMs the new stage turns on (no overlap).
 *
 * The number of outputs is determined dynamically by the size of the
 * "pinsOutput" array in the setup JSON; there is no compiled-in maximum.
 *
 * @version 0.2
 * @date 2026-04-19
 *
 * Apache License
 */

#ifndef _JCA_FNC_MULTISWITCHCONTROLLER_
#define _JCA_FNC_MULTISWITCHCONTROLLER_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagInt16.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    class MultiSwitchController : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagOutputPins;
      static const char *SetupTagProcessUnit;

      // Hardware (heap-allocated, length = OutputCount)
      uint8_t *Pins;
      uint8_t OutputCount;

      // Konfig
      float SetpointMin;
      float SetpointMax;
      float OffHyst;
      bool InvertDirection;
      uint16_t SwitchDeadTimeMs;
      float *OnHyst;        // length = OutputCount

      // Daten
      float Setpoint;
      float ProcessVar;
      bool *Active;          // length = OutputCount
      int16_t ActiveOutput;  // 1..OutputCount hardware-on, 0 = none

      // Intern: Stufenlogik & Umschalt-Totzeit
      int16_t LatchedStage;     // 0 = none, else 1..OutputCount (logical stage)
      int16_t HwActiveOutput;   // 0 = alle Pins aus, sonst 1..OutputCount physisch ein
      bool InDeadTime;
      int16_t PendingOutput;    // 1..OutputCount nach Totzeit einschalten
      unsigned long DeadTimeStartedAt;

      bool stageEnabled (uint8_t i) const { return OnHyst[i] > 0.0f; }
      void pinsApply (int16_t activeOneBased);

    public:
      MultiSwitchController (uint8_t *_Pins, uint8_t _CountOutputs, String _ProcessUnit, String _Name);
      ~MultiSwitchController ();

      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
      static void GetSetupSchema (JsonObject &_Schema);
    };
  }
}

#endif
