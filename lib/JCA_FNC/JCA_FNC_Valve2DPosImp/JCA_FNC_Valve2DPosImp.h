/**
 * @file JCA_FNC_Valve2DPosImp.h
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element position Valve, with open/close an external pulse counter
 * @version 0.1
 * @date 2025-04-13
 *
 * Copyright Jochen Cabrera 2025
 * Apache License
 *
 */
#ifndef _JCA_FNC_VALVE2DPOSIMP_
#define _JCA_FNC_VALVE2DPOSIMP_

#include <Arduino.h>
#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagUInt16.h>
#include <JCA_TAG_TagString.h>
#include <JCA_TAG_TagInt32.h>

namespace JCA {
  namespace FNC {
    class Valve2DPosImp : public FuncParent {
    public:
      enum State_T : uint8_t {
        INIT_OPEN = 0,
        INIT_CLOSE = 1,
        INPOSITION = 2,
        OPENING = 3,
        CLOSING = 4,
        WAITING = 5,
        FAULT = 9
      };
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagOpenPin;
      static const char *SetupTagClosePin;

      // Hardware
      uint8_t PinOpen;
      uint8_t PinClose;

      // Konfig
      uint16_t NoPulseTimeout;
      uint16_t PositionPulseHyst;
      uint16_t StepTimeout;
      uint16_t WaitTime;

      // Daten
      uint16_t PulseCount;
      float Setpoint;
      float Position;
      bool InitRequest;
      State_T CurrentState;
      String FaultCode;
      String StateCode;

      // Intern
      State_T NextState;
      int32_t MaxPositionPulse;
      int32_t PositionPulse;
      uint16_t LastPulseCount;
      uint16_t LastPulseTime;
      uint32_t StepTime;
      uint32_t LastMillis;

      String getStateString (State_T _State);

    public:
      Valve2DPosImp (uint8_t _PinOpen, uint8_t _PinClose, String _Name);
      ~Valve2DPosImp () {}

      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif