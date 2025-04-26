/**
 * @file JCA_FNC_DigitalIn.h
 * @brief Framework Element to count changes on a digital input using a hardware interrupt.
 * @version 1.0
 * @date 2023-10-10
 *
 * Apache License
 */

#ifndef _JCA_FNC_DIGITALIN_
#define _JCA_FNC_DIGITALIN_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagUInt16.h>
#include <JCA_TAG_TagUInt32.h>
#include <JCA_TAG_TagBool.h>

namespace JCA {
  namespace FNC {
    class DigitalIn : public FuncParent {
    public:
      enum Mode_T : uint8_t {
        MODE_INPUT = 0,
        MODE_COUNT = 1
      };

    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagInputPin;
      static const char *SetupTagPullup;
      static const char *SetupTagMode;

      // Hardware
      uint8_t Pin;
      Mode_T Mode;

      // Konfig
      uint32_t DebounceTime;

      // Daten
      uint16_t ValueCount;
      bool ValueInput;

      // Interrupt Handler
      volatile bool LastStableState;
      volatile uint32_t LastDebounceTime;
      volatile uint16_t Counter;
      static void IRAM_ATTR handleInterrupt(void* arg);

    public:
      DigitalIn (uint8_t _Pin, String _Pullup, String _Mode, String _Name);
      ~DigitalIn() {;}
      void update(struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      static bool Create(JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif