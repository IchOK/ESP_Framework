
#ifndef _JCA_SYS_PWM_
#define _JCA_SYS_PWM_

#include <JCA_SYS_DebugOut.h>
#ifdef ESP32
  #include <JCA_SYS_PWM_C.h>
#endif

namespace JCA {
  namespace SYS {
#ifdef ESP32
    struct PwmChannel_T {
      uint8_t Pin;
      uint8_t Resolution;
    };
#endif
    class PwmOutput {
    private:
#ifdef ESP8266
      uint8_t Resolution;
#elif ESP32
      static const uint8_t ChannelCount;
      PwmChannel_T Channels[JCA_SYS_PWM_CHANNELS];
#endif

    public:
      PwmOutput ();
      bool setupPin (uint8_t _Pin, uint32_t _Freq, uint8_t _BitResolution);
      bool writePin (uint8_t _Pin, float _Value);
    };
  }
}

#endif