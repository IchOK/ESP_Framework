
#include <JCA_FNC_Level.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    const char *Level::RawEmpty_Name = "RawEmpty";
    const char *Level::RawEmpty_Text = "Rohwert Leer";
    const char *Level::RawEmpty_Unit = "#";
    const char *Level::RawEmpty_Comment = nullptr;
    const char *Level::RawFull_Name = "RawFull";
    const char *Level::RawFull_Text = "Rohwert Voll";
    const char *Level::RawFull_Unit = "#";
    const char *Level::RawFull_Comment = nullptr;
    const char *Level::AlarmLevel_Name = "AlarmLevel";
    const char *Level::AlarmLevel_Text = "Alarm Grenzwert";
    const char *Level::AlarmLevel_Unit = "%";
    const char *Level::AlarmLevel_Comment = nullptr;
    const char *Level::ReadInterval_Name = "ReadInterval";
    const char *Level::ReadInterval_Text = "Leseintervall";
    const char *Level::ReadInterval_Unit = "s";
    const char *Level::ReadInterval_Comment = nullptr;
    const char *Level::Level_Name = "Level";
    const char *Level::Level_Text = "Niveau";
    const char *Level::Level_Unit = "%";
    const char *Level::Level_Comment = nullptr;
    const char *Level::Alarm_Name = "Alarm";
    const char *Level::Alarm_Text = "Alarm";
    const char *Level::Alarm_Comment = nullptr;
    const char *Level::Alarm_TextOn = nullptr;
    const char *Level::Alarm_TextOff = nullptr;
    const char *Level::RawValue_Name = "RawValue";
    const char *Level::RawValue_Text = "Rohwert";
    const char *Level::RawValue_Unit = "#";
    const char *Level::RawValue_Comment = nullptr;

    /**
     * @brief Construct a new Level::Level object
     *
     * @param _Pin Analog Pin conected to the Level-Sensor
     * @param _Name Element Name inside the Communication
     */
    Level::Level (uint8_t _Pin, const char *_Name)
        : Protocol (_Name) {
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

    /**
     * @brief Set the Element Config
     * Only existing Tags will be updated
     * @param _Tags Array of Config-Tags ("config": [])
     */
    void Level::setConfig (JsonArray _Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Set");
      for (JsonObject Tag : _Tags) {
        if (Tag[JsonTagName] == RawEmpty_Name) {
          RawEmpty = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, RawEmpty_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, RawEmpty);
          }
        }
        if (Tag[JsonTagName] == RawFull_Name) {
          RawFull = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, RawFull_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, RawFull);
          }
        }
        if (Tag[JsonTagName] == AlarmLevel_Name) {
          AlarmLevel = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, AlarmLevel_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, AlarmLevel);
          }
        }
        if (Tag[JsonTagName] == ReadInterval_Name) {
          ReadInterval = Tag[JsonTagValue].as<int16_t> ();
          if (Debug.print (FLAG_CONFIG, false, Name, __func__, ReadInterval_Name)) {
            Debug.print (FLAG_CONFIG, false, Name, __func__, DebugSeparator);
            Debug.println (FLAG_CONFIG, false, Name, __func__, ReadInterval);
          }
        }
      }
    }

    /**
     * @brief Set the Element Data
     * currently not used
     * @param _Tags Array of Data-Tags ("data": [])
     */
    void Level::setData (JsonArray _Tags) {
    }

    /**
     * @brief Execute the Commands
     * currently not used
     * @param _Tags Array of Commands ("cmd": [])
     */
    void Level::setCmd (JsonArray _Tags) {
    }

    /**
     * @brief Create a list of Config-Tags containing the current Value
     *
     * @param _Tags Array the Tags have to add
     */
    void Level::createConfigTags (JsonArray &_Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      createTag (_Tags, RawEmpty_Name, RawEmpty_Text, RawEmpty_Comment, RawEmpty_Unit, RawEmpty);
      createTag (_Tags, RawFull_Name, RawFull_Text, RawFull_Comment, RawFull_Unit, RawFull);
      createTag (_Tags, AlarmLevel_Name, AlarmLevel_Text, Alarm_Comment, AlarmLevel_Unit, AlarmLevel);
      createTag (_Tags, ReadInterval_Name, ReadInterval_Text, ReadInterval_Comment, ReadInterval_Unit, ReadInterval);
    }

    /**
     * @brief Create a list of Data-Tags containing the current Value
     *
     * @param _Tags Array the Tags have to add
     */
    void Level::createDataTags (JsonArray &_Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
      createTag (_Tags, Level_Name, Level_Text, Level_Comment, Level_Unit, Value);
      createTag (_Tags, Alarm_Name, Alarm_Text, Alarm_Comment, Alarm_TextOn, Alarm_TextOff, Alarm);
      createTag (_Tags, RawValue_Name, RawValue_Text, RawValue_Comment, RawValue_Unit, RawValue);
    }

    /**
     * @brief Create a list of Command-Informations
     *
     * @param _Tags Array the Command-Infos have to add
     */
    void Level::createCmdInfoTags (JsonArray &_Tags) {
      Debug.println (FLAG_CONFIG, false, Name, __func__, "Get");
    }

    /**
     * @brief Handling Level-Sensor
     * Read and scale the Level and check if the Alarm value is reached
     * @param time Current Time to check the Samplerate
     */
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

    /**
     * @brief Get the scaled Level Value
     * just return the last Value, don't read the current Hardware-Value
     * @return float Current Level (0-100%)
     */
    float Level::getValue () {
      return Value;
    }

    /**
     * @brief Level Alarm of the Feeder
     *
     * @return true Level is less Limit
     * @return false Level is good
     */
    bool Level::getAlarm () {
      return Alarm;
    }
  }
}
