
#include <JCA_FNC_Level.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Level::NameRawEmpty = "RawEmpty";
    const char *Level::NameRawFull = "RawFull";
    const char *Level::NameAlarmLevel = "AlarmLevel";
    const char *Level::NameReadInterval = "ReadInterval";
    const char *Level::UnitRawEmpty = "#";
    const char *Level::UnitRawFull = "#";
    const char *Level::UnitAlarmLevel = "%";
    const char *Level::UnitReadInterval = "s";
    const char *Level::NameLevel = "Level";
    const char *Level::NameAlarm = "Alarm";
    const char *Level::NameRawValue = "RawValue";
    const char *Level::UnitLevel = "%";
    const char *Level::UnitAlarm = "";
    const char *Level::UnitRawValue = "#";

    Level::Level (uint8_t _Pin, const char *_Name)
        : Parent (_Name) {
      RawEmpty = 0;
      RawFull = 1024;
      Pin = _Pin;
      ReadInterval = 1;
      Value = 50.0;
      AlarmLevel = 0.0;
      Alarm = false;
      LastSeconds = 0;
      IntervalCount = 0;
      RawValue = 0;
    }

    void Level::update (struct tm &time) {

      // Get Seconds of Day
      if (LastSeconds != time.tm_sec) {
        IntervalCount++;
        LastSeconds = time.tm_sec;
      }

      if (IntervalCount >= ReadInterval) {
        IntervalCount = 0;
        RawValue = analogRead (Pin);
        Value = Value * 0.9 + (float)(RawValue - RawEmpty) / (float)(RawFull - RawEmpty) * 10.0;
        if (Alarm) {
          if (Value > AlarmLevel + 5.0) {
            Alarm = false;
          }
        } else {
          if (Value < AlarmLevel) {
            Alarm = true;
          }
        }
      }
    }

    float Level::getValue () {
      return Value;
    }

    bool Level::getAlarm () {
      return Alarm;
    }

    void Level::setConfig (JsonObject _Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Set");
      if (_Data.containsKey (NameRawEmpty)) {
        RawEmpty = _Data[NameRawEmpty].as<int16_t> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameRawEmpty)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, RawEmpty);
        }
      }
      if (_Data.containsKey (NameRawFull)) {
        RawFull = _Data[NameRawFull].as<int16_t> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameRawFull)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, RawFull);
        }
      }
      if (_Data.containsKey (NameAlarmLevel)) {
        AlarmLevel = _Data[NameAlarmLevel].as<int16_t> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameAlarmLevel)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, AlarmLevel);
        }
      }
      if (_Data.containsKey (NameReadInterval)) {
        ReadInterval = _Data[NameReadInterval].as<int16_t> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, NameReadInterval)) {
          Debug.print (FLAG_CONFIG, false, ObjectName, __func__, DebugSeparator);
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ReadInterval);
        }
      }
    }

    void Level::setData (JsonObject _Data) {
      ;
    }

    void Level::createConfig (JsonObject &_Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Get");
      createTag (_Data, NameRawEmpty, UnitRawEmpty, RawEmpty);
      createTag (_Data, NameRawFull, UnitRawFull, RawFull);
      createTag (_Data, NameAlarmLevel, UnitAlarmLevel, AlarmLevel);
      createTag (_Data, NameReadInterval, UnitReadInterval, ReadInterval);
    }

    void Level::createData (JsonObject &_Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Get");
      createTag (_Data, NameLevel, UnitLevel, Value);
      createTag (_Data, NameAlarm, UnitAlarm, Alarm);
      createTag (_Data, NameRawValue, UnitRawValue, RawValue);
    }
  }
}
