/**
 * @file JCA_FNC_Valve2DPosImp.cpp
 * @author JCA (https://github.com/ichok)
 * @brief
 * @version 0.1
 * @date 2025-04-13
 *
 * Copyright Jochen Cabrera 2025
 * Apache License
 *
 */
#include <JCA_FNC_Valve2DPosImp.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *Valve2DPosImp::ClassName = "Valve2DPosImp";
    const char *Valve2DPosImp::SetupTagType = "valve2DPosImp";
    const char *Valve2DPosImp::SetupTagOpenPin = "pinOpen";
    const char *Valve2DPosImp::SetupTagClosePin = "pinClose";

    /**
     * @brief Construct a new Valve2DPosImp::Valve2DPosImp object
     *
     * @param _PinOpen Digital Pin for Open
     * @param _PinClose Digital Pin for Close
     * @param _Name Element Name inside the Communication
     */
    Valve2DPosImp::Valve2DPosImp (uint8_t _PinOpen, uint8_t _PinClose, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("NoPulseTimeout", "Wartezeit Impules", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &NoPulseTimeout, "ms"));
      Tags.push_back (new TagUInt16 ("PositionPulseHyst", "Positions Hysterese", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &PositionPulseHyst, ""));
      Tags.push_back (new TagUInt16 ("StepTimeout", "Schritt Maxzeit", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &StepTimeout, "s"));
      Tags.push_back (new TagUInt16 ("WaitTime", "Wartezeit Umschaltung", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &WaitTime, "ms"));
      Tags.push_back (new TagString ("FaultCode", "Fehler Code", "", TagAccessType_T::Read, TagUsage_T::UseConfig, &FaultCode));

      Tags.push_back (new TagFloat ("Setpoint", "Sollposition", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &Setpoint, "%"));
      Tags.push_back (new TagFloat ("Position", "Istposition", "", TagAccessType_T::Read, TagUsage_T::UseData, &Position, "%"));
      Tags.push_back (new TagBool ("InitRequest", "Initialisierung anfordern", "", TagAccessType_T::ReadWrite, TagUsage_T::UseData, &InitRequest, "Run", "Init"));
      Tags.push_back (new TagString ("StateCode", "Status", "", TagAccessType_T::Read, TagUsage_T::UseData, &StateCode));
      Tags.push_back (new TagInt32 ("PositionPulse", "Impuls Position", "", TagAccessType_T::Read, TagUsage_T::UseData, &PositionPulse, ""));
      Tags.push_back (new TagUInt16 ("PulseCount", "Impuls Eingang", "", TagAccessType_T::ReadWrite, TagUsage_T::UseData, &PulseCount, ""));

      // Init Hardware
      PinOpen
          = _PinOpen;
      pinMode (PinOpen, OUTPUT);
      digitalWrite (PinOpen, LOW);
      PinClose = _PinClose;
      pinMode (PinClose, OUTPUT);
      digitalWrite (PinClose, LOW);
      // Initialisierung der Variablen
      NoPulseTimeout = 2000;
      PositionPulseHyst = 5;
      StepTimeout = 3;
      WaitTime = 5000;

      PulseCount = 0;
      Setpoint = 0.0;
      Position = 0.0;
      InitRequest = false;
      CurrentState = State_T::INIT_OPEN;
      FaultCode = "";

      NextState = State_T::INIT_OPEN;
      MaxPositionPulse = 0;
      PositionPulse = 0;
      LastPulseCount = 0;
      LastPulseTime = 0;
      StepTime = 0;
      LastMillis = millis ();
    }

    String Valve2DPosImp::getStateString (State_T _State) {
      switch (_State) {
      case State_T::INIT_OPEN:
        return "Init Öffnen";
      case State_T::INIT_CLOSE:
        return "Init Schließen";
      case State_T::INPOSITION:
        return "In Position";
      case State_T::OPENING:
        return "Öffnen";
      case State_T::CLOSING:
        return "Schließen";
      case State_T::WAITING:
        return "Warten";
      case State_T::FAULT:
        return "Fehler";
      default:
        return "Unbekannt";
      }
    }

    void Valve2DPosImp::update (struct tm &_Time) {
      uint32_t CurrentMillis = millis ();
      uint32_t DiffMillis = CurrentMillis - LastMillis;
      LastMillis = CurrentMillis;

      // Get pulse since last update and reset Timer if pulse detected
      uint16_t DiffPulse = PulseCount - LastPulseCount;
      if (DiffPulse > 0) {
        LastPulseTime = 0;
      } else {
        LastPulseTime += DiffMillis;
      }
      LastPulseCount = PulseCount;

      // Check if setpoint is in range
      if (Setpoint < 0.0) {
        Setpoint = 0.0;
      }
      else if (Setpoint > 100.0) {
        Setpoint = 100.0;
      }
      int32_t SetpointPulse = int32_t (Setpoint / 100.0 * (float)MaxPositionPulse);

      // Check if state change is requested
      if (InitRequest) {
        InitRequest = false;
        NextState = State_T::INIT_OPEN;
        FaultCode = "";
      }
      if (CurrentState != NextState) {
        if (NextState == State_T::FAULT) {
          StepTime = 0;
          CurrentState = State_T::FAULT;
        } else if (CurrentState != State_T::WAITING) {
          StepTime = 0;
          CurrentState = State_T::WAITING;
        }
      }

      //------------------------------------------------------------
      // State Machine
      //------------------------------------------------------------
      switch (CurrentState) {
      case State_T::WAITING:
        digitalWrite (PinClose, LOW);
        digitalWrite (PinOpen, LOW);
        if (StepTime >= WaitTime) {
          CurrentState = NextState;
          LastPulseTime = 0;
          StepTime = 0;
        }
        break;

      case State_T::INIT_OPEN:
        digitalWrite (PinOpen, HIGH);
        digitalWrite (PinClose, LOW);
        if (LastPulseTime > NoPulseTimeout) {
          // Close Position reached
          PositionPulse = 0;
          NextState = State_T::INIT_CLOSE;
        } else if (StepTime / 1000 > StepTimeout) {
          // Timeout reached
          NextState = State_T::FAULT;
          FaultCode = "InitOpen-Timeout";
          Debug.println (FLAG_ERROR, false, Name, __func__, "Timeout: Init Open takes too long");
        }
        break;

      case State_T::INIT_CLOSE:
        digitalWrite (PinClose, HIGH);
        digitalWrite (PinOpen, LOW);
        PositionPulse += DiffPulse;
        if (LastPulseTime > NoPulseTimeout) {
          // Close Position reached
          MaxPositionPulse = PositionPulse;
          PositionPulse = 0;
          if (MaxPositionPulse > 10) {
            NextState = State_T::INPOSITION;
          } else {
            NextState = State_T::FAULT;
            FaultCode = "InitClose-NoPulse";
            Debug.println (FLAG_ERROR, false, Name, __func__, "Init Close: Position too small");
          }
        } else if (StepTime / 1000 > StepTimeout) {
          // Timeout reached
          NextState = State_T::FAULT;
          FaultCode = "InitClose-Timeout";
          Debug.println (FLAG_ERROR, false, Name, __func__, "Timeout: Init Close takes too long");
        }
        break;

      case State_T::INPOSITION:
        digitalWrite (PinClose, LOW);
        digitalWrite (PinOpen, LOW);
        if (SetpointPulse > PositionPulse + PositionPulseHyst) {
          NextState = State_T::OPENING;
        } else if (SetpointPulse < PositionPulse - PositionPulseHyst) {
          NextState = State_T::CLOSING;
        }
        break;

      case State_T::OPENING:
        digitalWrite (PinOpen, HIGH);
        digitalWrite (PinClose, LOW);
        PositionPulse += DiffPulse;
        if (PositionPulse >= SetpointPulse) {
          NextState = State_T::INPOSITION;
        } else if (LastPulseTime > NoPulseTimeout) {
          if (Setpoint >= 95.0) {
            MaxPositionPulse = PositionPulse;
            NextState = State_T::INPOSITION;
          } else {
            NextState = State_T::FAULT;
            FaultCode = "Open-Timeout";
            Debug.println (FLAG_ERROR, false, Name, __func__, "Timeout: Open takes too long");
          }
        }
        break;

      case State_T::CLOSING:
        digitalWrite (PinClose, HIGH);
        digitalWrite (PinOpen, LOW);
        PositionPulse -= DiffPulse;
        if (PositionPulse <= SetpointPulse) {
          NextState = State_T::INPOSITION;
        } else if (LastPulseTime > NoPulseTimeout) {
          if (Setpoint <= 5.0) {
            PositionPulse = 0;
            NextState = State_T::INPOSITION;
          } else {
            NextState = State_T::FAULT;
            FaultCode = "Close-Timeout";
            Debug.println (FLAG_ERROR, false, Name, __func__, "Timeout: Close takes too long");
          }
        }
        break;

      case State_T::FAULT:
        digitalWrite (PinClose, LOW);
        digitalWrite (PinOpen, LOW);
        break;
      }

      if (CurrentState != NextState) {
        StateCode = getStateString (CurrentState) + " -> " + getStateString (NextState);
      } else {
        StateCode = getStateString (CurrentState);
      }
      StepTime += DiffMillis;
      Position = (float)PositionPulse / (float)MaxPositionPulse * 100.0;
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void Valve2DPosImp::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
    }

    /**
     * @brief Create a new Instanz of the Class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the Instanz to
     * @param _Hardware List of knowen Hardware-References
     * @return true
     * @return false
     */
    bool Valve2DPosImp::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t PinOpen = GetSetupValueUINT8 (SetupTagOpenPin, Done, _Setup, Log);
      uint8_t PinClose = GetSetupValueUINT8 (SetupTagClosePin, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new Valve2DPosImp (PinOpen, PinClose, Name));
        Log["done"] = Name + " (Valve2DPosImp - Open:" + String (PinOpen) + " - Close:" + String (PinClose) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}