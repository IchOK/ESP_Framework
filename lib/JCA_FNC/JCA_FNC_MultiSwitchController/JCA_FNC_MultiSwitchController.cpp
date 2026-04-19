/**
 * @file JCA_FNC_MultiSwitchController.cpp
 * @brief Multi-Stage On/Off Controller with an arbitrary number of staged digital outputs.
 *
 * @version 0.2
 * @date 2026-04-19
 *
 * Apache License
 */

#include <JCA_FNC_MultiSwitchController.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *MultiSwitchController::ClassName = "MultiSwitchController";
    const char *MultiSwitchController::SetupTagType = "multiSwitchController";
    const char *MultiSwitchController::SetupTagOutputPins = "pinsOutput";
    const char *MultiSwitchController::SetupTagProcessUnit = "inUnit";

    /**
     * @brief Construct a new MultiSwitchController object
     *
     * @param _Pins Array of digital output pins (length = _CountOutputs)
     * @param _CountOutputs Number of outputs to use (>= 1)
     * @param _ProcessUnit Unit of the process variable / setpoint (e.g. "°C")
     * @param _Name Element name inside the FuncHandler
     */
    MultiSwitchController::MultiSwitchController (uint8_t *_Pins, uint8_t _CountOutputs, String _ProcessUnit, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      OutputCount = _CountOutputs;

      // Allocate per-output dynamic state
      Pins = new uint8_t[OutputCount];
      OnHyst = new float[OutputCount];
      Active = new bool[OutputCount];

      // Common configuration
      Tags.push_back (new TagFloat ("SetpointMin", "Minimaler Sollwert", "", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &SetpointMin, _ProcessUnit));
      Tags.push_back (new TagFloat ("SetpointMax", "Maximaler Sollwert", "", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &SetpointMax, _ProcessUnit));
      Tags.push_back (new TagFloat ("OffHyst", "Aus-Hysterese (gemeinsam)", "Normal: Aus bei Istwert > Soll + OffHyst. Invertiert: Aus bei Istwert < Soll - OffHyst", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &OffHyst, _ProcessUnit));
      Tags.push_back (new TagBool ("InvertDirection", "Wirkrichtung invertieren", "Aus: Ein wenn Ist zu niedrig (Heizen). Ein: Ein wenn Ist zu hoch (Kühlen)", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &InvertDirection, "NORMAL", "INVERT"));
      Tags.push_back (new TagUInt16 ("SwitchDeadTime", "Umschalt-Totzeit", "Pause in ms zwischen zwei Stufen (nur Stufe zu Stufe); 0 = sofort", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &SwitchDeadTimeMs, "ms"));

      // Per-output configuration tags
      for (uint8_t i = 0; i < OutputCount; i++) {
        String NumStr = String (i + 1);
        Tags.push_back (new TagFloat ("OnHyst" + NumStr, "Ein-Hysterese " + NumStr, "0 = Stufe deaktiviert. Sonst: Ein gemäss PvEff/SpEff (siehe Doku).", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &OnHyst[i], _ProcessUnit));
      }

      // Process data
      Tags.push_back (new TagFloat ("ProcessVar", "Istwert", "", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &ProcessVar, _ProcessUnit));
      Tags.push_back (new TagFloat ("Setpoint", "Sollwert", "", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &Setpoint, _ProcessUnit));

      // Status of each individual output
      for (uint8_t i = 0; i < OutputCount; i++) {
        String NumStr = String (i + 1);
        Tags.push_back (new TagBool ("Active" + NumStr, "Ausgang " + NumStr + " aktiv", "", TagAccessType_T::Read, TagUsage_T::UseData, &Active[i], "EIN", "AUS"));
      }
      Tags.push_back (new TagInt16 ("ActiveOutput", "Aktiver Ausgang", "Index des aktuell aktiven Ausgangs (1..n) oder 0 wenn keiner aktiv ist", TagAccessType_T::Read, TagUsage_T::UseData, &ActiveOutput, ""));

      // Init configuration with neutral defaults
      SetpointMin = 0.0f;
      SetpointMax = 100.0f;
      OffHyst = 1.0f;
      InvertDirection = false;
      SwitchDeadTimeMs = 200;
      for (uint8_t i = 0; i < OutputCount; i++) {
        OnHyst[i] = 1.0f + static_cast<float> (i);
        Active[i] = false;
        Pins[i] = 0;
      }

      // Init data
      Setpoint = 0.0f;
      ProcessVar = 0.0f;
      ActiveOutput = 0;
      LatchedStage = 0;
      HwActiveOutput = 0;
      InDeadTime = false;
      PendingOutput = 0;
      DeadTimeStartedAt = 0;

      // Init hardware
      for (uint8_t i = 0; i < OutputCount; i++) {
        Pins[i] = _Pins[i];
        pinMode (Pins[i], OUTPUT);
        digitalWrite (Pins[i], LOW);
      }
    }

    MultiSwitchController::~MultiSwitchController () {
      if (Pins != nullptr) {
        delete[] Pins;
        Pins = nullptr;
      }
      if (OnHyst != nullptr) {
        delete[] OnHyst;
        OnHyst = nullptr;
      }
      if (Active != nullptr) {
        delete[] Active;
        Active = nullptr;
      }
    }

    void MultiSwitchController::pinsApply (int16_t activeOneBased) {
      for (uint8_t i = 0; i < OutputCount; i++) {
        bool On = (activeOneBased == static_cast<int16_t> (i + 1));
        digitalWrite (Pins[i], On ? HIGH : LOW);
      }
    }

    /**
     * @brief Cyclic update: staging with latch at Soll - EinHyst + AusHyst, optional switch dead time.
     *
     * InvertDirection maps to PvEff = -ProcessVar, SpEff = -Setpoint so one code path suffices.
     */
    void MultiSwitchController::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");

      const unsigned long now = millis ();
      const float PvEff = InvertDirection ? -ProcessVar : ProcessVar;
      const float SpEff = InvertDirection ? -Setpoint : Setpoint;

      int16_t Desired = 0;

      if (PvEff > SpEff + OffHyst) {
        LatchedStage = 0;
        Desired = 0;
      } else {
        int16_t Best = -1;
        float BestHyst = -1.0f;
        for (uint8_t i = 0; i < OutputCount; i++) {
          if (!stageEnabled (i)) {
            continue;
          }
          if (PvEff < SpEff - OnHyst[i]) {
            if (Best < 0 || OnHyst[i] > BestHyst) {
              Best = static_cast<int16_t> (i);
              BestHyst = OnHyst[i];
            }
          }
        }

        if (LatchedStage > 0) {
          const uint8_t idx = static_cast<uint8_t> (LatchedStage - 1);
          if (idx >= OutputCount || !stageEnabled (idx)) {
            LatchedStage = (Best >= 0) ? static_cast<int16_t> (Best + 1) : 0;
            Desired = LatchedStage;
          } else if (PvEff > SpEff - OnHyst[idx] + OffHyst) {
            LatchedStage = (Best >= 0) ? static_cast<int16_t> (Best + 1) : 0;
            Desired = LatchedStage;
          } else {
            if (Best >= 0 && OnHyst[static_cast<uint8_t> (Best)] > OnHyst[idx]) {
              LatchedStage = static_cast<int16_t> (Best + 1);
            }
            Desired = LatchedStage;
          }
        } else {
          Desired = (Best >= 0) ? static_cast<int16_t> (Best + 1) : 0;
          LatchedStage = Desired;
        }
      }

      if (InDeadTime) {
        if (Desired == 0) {
          InDeadTime = false;
          PendingOutput = 0;
          pinsApply (0);
          HwActiveOutput = 0;
        } else if (Desired != PendingOutput) {
          PendingOutput = Desired;
          DeadTimeStartedAt = now;
        } else if ((now - DeadTimeStartedAt) >= static_cast<unsigned long> (SwitchDeadTimeMs)) {
          InDeadTime = false;
          if (PendingOutput > 0) {
            pinsApply (PendingOutput);
            HwActiveOutput = PendingOutput;
          }
          PendingOutput = 0;
        }
      }

      if (!InDeadTime) {
        if (Desired != HwActiveOutput) {
          if (HwActiveOutput > 0 && Desired > 0 && Desired != HwActiveOutput) {
            pinsApply (0);
            HwActiveOutput = 0;
            if (SwitchDeadTimeMs == 0) {
              pinsApply (Desired);
              HwActiveOutput = Desired;
            } else {
              PendingOutput = Desired;
              DeadTimeStartedAt = now;
              InDeadTime = true;
            }
          } else if (Desired == 0) {
            pinsApply (0);
            HwActiveOutput = 0;
          } else if (HwActiveOutput == 0 && Desired > 0) {
            pinsApply (Desired);
            HwActiveOutput = Desired;
          }
        }
      }

      for (uint8_t i = 0; i < OutputCount; i++) {
        Active[i] = (HwActiveOutput == static_cast<int16_t> (i + 1));
      }
      ActiveOutput = HwActiveOutput;
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     */
    void MultiSwitchController::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
      _Handler.FunctionSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> (SetupTagType, GetSetupSchema));
    }

    /**
     * @brief Create a new instance of the class using the JSON-Configdata
     */
    bool MultiSwitchController::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      String ProcessUnit = GetSetupValueString (SetupTagProcessUnit, Done, _Setup, Log);
      uint8_t *PinsOutput = nullptr;
      uint8_t CountOutputs = GetSetupValueUINT8Arr (SetupTagOutputPins, PinsOutput, Done, _Setup, Log);

      if (Done) {
        if (CountOutputs == 0) {
          Log["error"] = String ("At least one output pin required");
          Done = false;
        }
      }

      if (Done) {
        _Functions.push_back (new MultiSwitchController (PinsOutput, CountOutputs, ProcessUnit, Name));
        Log["done"] = Name + " (Process:" + ProcessUnit + ", Outputs:" + String (CountOutputs) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      if (PinsOutput != nullptr) {
        delete[] PinsOutput;
      }
      return Done;
    }

    void MultiSwitchController::GetSetupSchema (JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray> ();

      JsonObject NameParam = Parameters.add<JsonObject> ();
      NameParam["name"] = JCA_IOT_FUNCHANDLER_SETUP_NAME;
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Funktion für die Kommunikation";

      JsonObject ProcessUnitParam = Parameters.add<JsonObject> ();
      ProcessUnitParam["name"] = SetupTagProcessUnit;
      ProcessUnitParam["type"] = "string";
      ProcessUnitParam["comment"] = "Einheit des Prozesswerts und der Hysteresen";

      JsonObject PinsOutputParam = Parameters.add<JsonObject> ();
      PinsOutputParam["name"] = SetupTagOutputPins;
      PinsOutputParam["type"] = "uint8[]";
      PinsOutputParam["comment"] = "Array von digitalen Ausgangs-Pins (1..n Stufen, dynamisch)";
    }
  }
}
