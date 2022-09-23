
#include <JCA_FNC_Level.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    Level::Level (uint8_t _Pin, const char *_Name) {
      strncpy (ObjectName, _Name, sizeof (ObjectName));
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
        Value = (float)(RawValue - RawEmpty) / (float)(RawFull - RawEmpty) * 100.0;
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

    void Level::set (JsonObject _Collection) {
      if (_Collection.containsKey (ObjectName)) {
        JsonObject ObjectData = _Collection[ObjectName].as<JsonObject> ();
        if (ObjectData.containsKey ("config")) {
          setConfig (ObjectData["config"].as<JsonObject> ());
        }
        if (ObjectData.containsKey ("data")) {
          setData (ObjectData["data"].as<JsonObject> ());
        }
      }
    }

    void Level::getConfig (JsonObject &_Collection) {
      JsonObject ObjectData = _Collection.createNestedObject (ObjectName);
      JsonObject Config = ObjectData.createNestedObject ("config");
      createConfig (Config);
      if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "ObjectData:")) {
        String ObjectStr;
        serializeJson (ObjectData, ObjectStr);
        Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ObjectStr);
      }
    }

    void Level::getData (JsonObject &_Collection) {
      JsonObject ObjectData = _Collection.createNestedObject (ObjectName);
      JsonObject Data = ObjectData.createNestedObject ("data");
      createData (Data);
      if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "ObjectData:")) {
        String ObjectStr;
        serializeJson (ObjectData, ObjectStr);
        Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ObjectStr);
      }
    }

    void Level::getAll (JsonObject &_Collection) {
      JsonObject ObjectData = _Collection.createNestedObject (ObjectName);
      JsonObject Config = ObjectData.createNestedObject ("config");
      JsonObject Data = ObjectData.createNestedObject ("data");
      createConfig (Config);
      createData (Data);
    }

    float Level::getValue () {
      return Value;
    }

    bool Level::getAlarm () {
      return Alarm;
    }

    void Level::setConfig (JsonObject _Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Set");
      if (_Data.containsKey ("RawEmpty")) {
        RawEmpty = _Data["RawEmpty"].as<int> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "RawEmpty:")) {
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, RawEmpty);
        }
      }
      if (_Data.containsKey ("RawFull")) {
        RawFull = _Data["RawFull"].as<int> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "RawFull:")) {
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, RawFull);
        }
      }
      if (_Data.containsKey ("AlarmLevel")) {
        AlarmLevel = _Data["AlarmLevel"].as<float> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "AlarmLevel:")) {
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, AlarmLevel);
        }
      }
      if (_Data.containsKey ("ReadInterval")) {
        ReadInterval = _Data["ReadInterval"].as<float> ();
        if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "ReadInterval:")) {
          Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ReadInterval);
        }
      }
    }

    void Level::setData (JsonObject _Data) {
      ;
    }

    void Level::createConfig (JsonObject &_Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Get");
      _Data["RawEmpty"] = RawEmpty;
      _Data["RawFull"] = RawFull;
      _Data["AlarmLevel"] = AlarmLevel;
      _Data["ReadInterval"] = ReadInterval;
    }

    void Level::createData (JsonObject &_Data) {
      Debug.println (FLAG_CONFIG, false, ObjectName, __func__, "Get");
      _Data["Value"] = Value;
      _Data["Alarm"] = Alarm;
      _Data["RawValue"] = RawValue;
    }
  }
}
