
#include <JCA_SYS_TimerESP32.h>

namespace JCA {
  namespace SYS {
    uint64_t TimerESP32::getCounterValue (uint8_t _Index) {
      uint64_t Value = 0;
      timer_get_counter_value (Timers[_Index].Group, Timers[_Index].Index, &Value);
      return Value;
    }
    uint64_t TimerESP32::getAlarmValue (uint8_t _Index) {
      uint64_t Value = 0;
      timer_get_alarm_value (Timers[_Index].Group, Timers[_Index].Index, &Value);
      return Value;
    }
    double TimerESP32::getCounterTimeSec (uint8_t _Index) {
      double Value = 0.0;
      timer_get_counter_time_sec (Timers[_Index].Group, Timers[_Index].Index, &Value);
      return Value;
    }
    bool TimerESP32::setCounterValue (uint8_t _Index, uint64_t _Value){
      return EspErrorIsOK (timer_set_counter_value (Timers[_Index].Group, Timers[_Index].Index, _Value));
    }
    bool TimerESP32::start (uint8_t _Index) {
      return EspErrorIsOK (timer_start (Timers[_Index].Group, Timers[_Index].Index));
    }
    bool TimerESP32::pause (uint8_t _Index){
      return EspErrorIsOK (timer_pause (Timers[_Index].Group, Timers[_Index].Index));
    }
    bool TimerESP32::setCounterMode (uint8_t _Index, timer_count_dir_t _Dir) {
      return EspErrorIsOK (timer_set_counter_mode (Timers[_Index].Group, Timers[_Index].Index, _Dir));
    }
    bool TimerESP32::setAutoReload (uint8_t _Index, timer_autoreload_t _Reload) {
      return EspErrorIsOK (timer_set_auto_reload (Timers[_Index].Group, Timers[_Index].Index, _Reload));
    }
    bool TimerESP32::setDivider (uint8_t _Index, uint32_t _Divider) {
      return EspErrorIsOK (timer_set_divider (Timers[_Index].Group, Timers[_Index].Index, _Divider));
    }
    bool TimerESP32::setAlarmValue (uint8_t _Index, uint64_t _Value) {
      return EspErrorIsOK (timer_set_alarm_value (Timers[_Index].Group, Timers[_Index].Index, _Value));
    }
    bool TimerESP32::setAlarm (uint8_t _Index, timer_alarm_t _Enable) {
      return EspErrorIsOK (timer_set_alarm (Timers[_Index].Group, Timers[_Index].Index, _Enable));
    }
    bool TimerESP32::isrCallbackAdd (uint8_t _Index, const TimerESP32_Callback &_CB, void *_Args){
      return EspErrorIsOK(timer_isr_callback_add (Timers[_Index].Group, Timers[_Index].Index, _CB, _Args, 0));
    }
    bool TimerESP32::enableIntr (uint8_t _Index) {
      return EspErrorIsOK (timer_enable_intr (Timers[_Index].Group, Timers[_Index].Index));
    }
    bool TimerESP32::disableIntr (uint8_t _Index) {
      return EspErrorIsOK (timer_disable_intr (Timers[_Index].Group, Timers[_Index].Index));
    }

    int8_t TimerESP32::addTimer (bool _Start, bool _Alarm, bool _AutoReload) {
      int8_t AddIndex = -1;
      // Check if there is a free Timer
      for (uint8_t i = 0; i < JCA_SYS_TIMERESP32_MAX_TIMERS; i++) {
        if (!Timers[i].Init) {
          AddIndex = i;
          break;
        }
      }

      if (AddIndex < 0) {
        // no Timer found
        return -1;
      } else {
        // Setting up a new Timer
        timer_config_t Config;
        esp_err_t ErrorCode;
        if (_Alarm) {
          Config.alarm_en = TIMER_ALARM_EN;
        } else {
          Config.alarm_en = TIMER_ALARM_DIS;
        }
        if (_Start) {
          Config.counter_en = TIMER_START;
        } else {
          Config.counter_en = TIMER_PAUSE;
        }
        Config.intr_type = TIMER_INTR_MAX;
        Config.counter_dir = TIMER_COUNT_UP;
        if (_AutoReload) {
          Config.auto_reload = TIMER_AUTORELOAD_EN;
        } else {
          Config.auto_reload = TIMER_AUTORELOAD_DIS;
        }
        Config.divider = JCA_SYS_TIMERESP32_DIVIDER;

        // init Timer
        ErrorCode = timer_init (Timers[AddIndex].Group, Timers[AddIndex].Index, &Config);
        if (!EspErrorIsOK(ErrorCode)) {
          Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Init : ");
          EspErrorDebugOut(ErrorCode, FLAG_ERROR);
          return -1;
        }

        // Set Counter-Value to start by 0
        ErrorCode = timer_set_counter_value (Timers[AddIndex].Group, Timers[AddIndex].Index, 0);
        if (!EspErrorIsOK (ErrorCode)) {
          Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Zero : ");
          EspErrorDebugOut (ErrorCode, FLAG_ERROR);
          timer_deinit (Timers[AddIndex].Group, Timers[AddIndex].Index);
          return -1;
        }

        // Timer Setup Done
        Timers[AddIndex].Init = true;
        return AddIndex;
      }
    }

    bool TimerESP32::addCallback (uint8_t _Index, const uint64_t _Micros, const TimerESP32_Callback &_CB, void *_Args) {
      bool RetValue = Timers[_Index].Init;
      esp_err_t ErrorCode;

      // Set Interrupt value in Microseconds
      ErrorCode = timer_set_alarm_value (Timers[_Index].Group, Timers[_Index].Index, _Micros);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-SetAlarm : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      // Enable Interrupt
      ErrorCode = timer_enable_intr (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-EnableInterrupt : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      // Register the ISR handler
      Timers[_Index].InterruptCB = _CB;
      ErrorCode = timer_isr_callback_add (Timers[_Index].Group, Timers[_Index].Index, Timers[_Index].InterruptCB, _Args, 0);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-AddCallback : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      Timers[_Index].HasCB = RetValue;
      return RetValue;
    }

    bool TimerESP32::removeTimer (uint8_t _Index) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      Timers[_Index].Init = false;

      ErrorCode = timer_deinit (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-DeInit : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::setMicros (uint8_t _Index, const uint64_t _Micros, bool _Start) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      // Set Interrupt value in Microseconds
      ErrorCode = timer_set_alarm_value (Timers[_Index].Group, Timers[_Index].Index, _Micros);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-SetAlarm : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      if (_Start) {
        ErrorCode = timer_start (Timers[_Index].Group, Timers[_Index].Index);
        if (!EspErrorIsOK (ErrorCode)) {
          Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Start : ");
          EspErrorDebugOut (ErrorCode, FLAG_ERROR);
          RetValue = false;
        }
      }

      return RetValue;
    }

    bool TimerESP32::startTimer (uint8_t _Index) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      ErrorCode = timer_start (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Start : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::startTimer (uint8_t _Index, const uint64_t _Micros) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      // Set Interrupt value in Microseconds
      ErrorCode = timer_set_alarm_value (Timers[_Index].Group, Timers[_Index].Index, _Micros);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-SetAlarm : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      ErrorCode = timer_start (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Start : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::restartTimer (uint8_t _Index) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      // Set Counter-Value to start by 0
      ErrorCode = timer_set_counter_value (Timers[_Index].Group, Timers[_Index].Index, 0);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Zero : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      ErrorCode = timer_start (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Start : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::restartTimer (uint8_t _Index, const uint64_t _Micros) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      // Set Counter-Value to start by 0
      ErrorCode = timer_set_counter_value (Timers[_Index].Group, Timers[_Index].Index, 0);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Zero : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      // Set Interrupt value in Microseconds
      ErrorCode = timer_set_alarm_value (Timers[_Index].Group, Timers[_Index].Index, _Micros);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-SetAlarm : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      ErrorCode = timer_start (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Start : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::pauseTimer (uint8_t _Index) {
      esp_err_t ErrorCode;
      bool RetValue = Timers[_Index].Init;

      ErrorCode = timer_pause (Timers[_Index].Group, Timers[_Index].Index);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-Pause : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::autoReload (uint8_t _Index, bool _Enable) {
      esp_err_t ErrorCode;
      timer_autoreload_t Type;
      bool RetValue = Timers[_Index].Init;

      if (_Enable) {
        Type = TIMER_AUTORELOAD_EN;
      } else {
        Type = TIMER_AUTORELOAD_DIS;
      }

      ErrorCode = timer_set_auto_reload (Timers[_Index].Group, Timers[_Index].Index, Type);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-AutoReload : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        RetValue = false;
      }

      return RetValue;
    }

    bool TimerESP32::isRunning (uint8_t _Index) {
      esp_err_t ErrorCode;
      timer_config_t ActConfig;
      ErrorCode = timer_get_config (Timers[_Index].Group, Timers[_Index].Index, &ActConfig);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-GetConfig : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        return false;
      }
      return ActConfig.counter_en == TIMER_START;
    }

    uint64_t TimerESP32::getCounter (uint8_t _Index) {
      esp_err_t ErrorCode;
      uint64_t ActValue;
      bool RetValue = Timers[_Index].Init;

      ErrorCode = timer_get_counter_value (Timers[_Index].Group, Timers[_Index].Index, &ActValue);
      if (!EspErrorIsOK (ErrorCode)) {
        Debug.print (FLAG_ERROR, false, "TimerESP32", __func__, "Timer-GetConfig : ");
        EspErrorDebugOut (ErrorCode, FLAG_ERROR);
        return 0;
      }
      return ActValue;
    }
  }
}