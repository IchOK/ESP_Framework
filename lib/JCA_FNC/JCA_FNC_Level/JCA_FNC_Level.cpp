/**
 * @file JCA_FNC_Level.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a Level by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_Level.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *Level::ClassName = "Level";
    const char *Level::SetupTagType = "level";
    const char *Level::SetupTagInputPin = "pinInput";

    /**
     * @brief Construct a new Level::Level object
     *
     * @param _Pin Analog Pin conected to the Level-Sensor
     * @param _Name Element Name inside the Communication
     */
    Level::Level (uint8_t _Pin, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagInt16 ("RawEmpty", "Rohwert Leer", "", false, TagUsage_T::UseConfig, &RawEmpty, "#"));
      Tags.push_back (new TagInt16 ("RawFull", "Rohwert Voll", "", false, TagUsage_T::UseConfig, &RawFull, "#"));
      Tags.push_back (new TagFloat ("AlarmLevel", "Alarm Grenzwer", "", false, TagUsage_T::UseConfig, &AlarmLevel, "%"));
      Tags.push_back (new TagUInt16 ("ReadInterval", "Leseintervall", "", false, TagUsage_T::UseConfig, &ReadInterval, "s"));

      Tags.push_back (new TagFloat ("Value", "Niveau", "", true, TagUsage_T::UseData, &Value, "%"));
      Tags.push_back (new TagBool ("Alarm", "Alarm", "", true, TagUsage_T::UseData, &Alarm, "EIN", "AUS"));
      Tags.push_back (new TagInt16 ("RawValue", "Rohwert", "", true, TagUsage_T::UseData, &RawValue, "#"));

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
     * @brief Handling Level-Sensor
     * Read and scale the Level and check if the Alarm value is reached
     * @param time Current Time to check the Samplerate
     */
    void Level::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
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
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void Level::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
    }

    /**
     * @brief Create a new Instanz of the Class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the Instanz to
     * @param _Hardware List of knowen Hardware-References
     * @return true
     * @return false
     */
    bool Level::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t InputPin = GetSetupValueUINT8 (SetupTagInputPin, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new Level(InputPin, Name));
        Log["done"] = Name + "(Pin:" + String (InputPin) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
