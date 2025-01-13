/**
 * @file JCA_FNC_ClockValues.h
 * @brief Framework Element to manage clock-based switching points with configurable values and behaviors.
 * @version 1.0
 * @date 2023-10-10
 *
 * Apache License
 */

#ifndef _JCA_FNC_CLOCKVALUES_
#define _JCA_FNC_CLOCKVALUES_

#include <ArduinoJson.h>
#include <time.h>
#include <vector>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagUInt32.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagBool.h>
#include <JCA_TAG_TagUInt8.h>

namespace JCA {
  namespace FNC {
    class ClockValues : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagCountClockPoints;

      // Konfig
      uint8_t CountClockPoints;

      // Daten
      struct ClockPoint {
        uint32_t Time;
        float Value;
        bool DoRamp;
      };
      std::vector<ClockPoint> ClockPoints;

      // Aktueller Schaltwert
      float Value;

      // Intern
      void updateTags();

    public:
      ClockValues(uint8_t _Count, String _Name);
      ~ClockValues() {;}
      void update(struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      static bool Create(JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
    };
  }
}

#endif