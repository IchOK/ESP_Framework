
#ifndef _JCA_SYS_TIMERESP32_
#define _JCA_SYS_TIMERESP32_

#ifdef ESP32
  #include <driver/timer.h>
#else
  #error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_EspError.h>

#define JCA_SYS_TIMERESP32_MAX_TIMERS 2
#define JCA_SYS_TIMERESP32_DIVIDER 80

namespace JCA {
  namespace SYS {
    typedef bool (*TimerESP32_Callback) (void *);
    struct TimerESP32_Info_T {
      timer_group_t Group;
      timer_idx_t Index;
      TimerESP32_Callback InterruptCB;
      bool HasCB;
      bool Init;
    };

    static class TimerESP32 {
    private:
      TimerESP32_Info_T Timers[4] = {
        { (timer_group_t)0, (timer_idx_t)0, nullptr, false, false },
        { (timer_group_t)1, (timer_idx_t)0, nullptr, false, false },
        { (timer_group_t)0, (timer_idx_t)1, nullptr, false, false },
        { (timer_group_t)1, (timer_idx_t)1, nullptr, false, false }
      };

    public:
      // legacy Functions
      uint64_t getCounterValue (uint8_t _Index);
      double getCounterTimeSec (uint8_t _Index);
      bool setCounterValue (uint8_t _Index, uint64_t _Value);
      bool start (uint8_t _Index);
      bool pause (uint8_t _Index);
      bool setCounterMode (uint8_t _Index, timer_count_dir_t _Dir);
      bool setAutoReload (uint8_t _Index, timer_autoreload_t _Reload);
      bool setDivider (uint8_t _Index, uint32_t _Divider);
      bool setAlarmValue (uint8_t _Index, uint64_t _Value);
      uint64_t getAlarmValue (uint8_t _Index);
      bool setAlarm (uint8_t _Index, timer_alarm_t _Enable);
      bool isrCallbackAdd (uint8_t _Index, const TimerESP32_Callback &_CB, void *_Args);
      bool enableIntr (uint8_t _Index);
      bool disableIntr (uint8_t _Index);

      int8_t addTimer (bool _Start = false, bool _Alarm = true, bool _AutoReload = false);
      bool addCallback (uint8_t _Index, const uint64_t _Micros, const TimerESP32_Callback &_CB, void *_Args);
      bool removeTimer (uint8_t _Index);
      bool setMicros (uint8_t _Index, const uint64_t _Micros, bool _Start = false);
      bool startTimer (uint8_t _Index);
      bool startTimer (uint8_t _Index, const uint64_t _Micros);
      bool restartTimer (uint8_t _Index);
      bool restartTimer (uint8_t _Index, const uint64_t _Micros);
      bool pauseTimer (uint8_t _Index);
      bool autoReload (uint8_t _Index, bool _Enable);
      bool isRunning (uint8_t _Index);
      uint64_t getCounter (uint8_t _Index);

    } TimerESP32_Handler;
  }
}

#endif