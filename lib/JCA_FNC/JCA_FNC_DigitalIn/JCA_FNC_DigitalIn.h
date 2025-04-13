/**
 * @file JCA_FNC_DigitalCounter.h
 * @brief Framework Element to count changes on a digital input using a hardware interrupt.
 * @version 1.0
 * @date 2023-10-10
 *
 * Apache License
 */

#ifndef _JCA_FNC_DIGITALCOUNTER_
#define _JCA_FNC_DIGITALCOUNTER_

#include <ArduinoJson.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    class DigitalCounter : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagInputPin;

      // Hardware
      uint8_t Pin;

      // Daten
      volatile uint16_t Counter;

      // Interrupt Handler
      static void IRAM_ATTR handleInterrupt(void* arg);

    public:
      DigitalCounter(uint8_t _Pin, String _Name);
      ~DigitalCounter() {;}
      void update(struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      static bool Create(JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif