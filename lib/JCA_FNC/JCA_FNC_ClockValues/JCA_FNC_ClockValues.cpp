/**
 * @file JCA_FNC_ClockValues.cpp
 * @brief Framework Element to manage clock-based switching points with configurable values and behaviors.
 * @version 1.0
 * @date 2023-10-10
 *
 * Apache License
 */

#include <JCA_FNC_ClockValues.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *ClockValues::ClassName = "ClockValues";
    const char *ClockValues::SetupTagType = "clockValues";
    const char *ClockValues::SetupTagCountClockPoints = "countClockPoints";

    /**
     * @brief Construct a new ClockValues::ClockValues object
     *
     * @param _Name Element Name inside the Communication
     */
    ClockValues::ClockValues(uint8_t Count, String _Name)
        : FuncParent(_Name), CountClockPoints(0), Value(0.0) {
      Debug.println(FLAG_SETUP, false, Name, __func__, "Create");
      CountClockPoints = Count;

      // Create Tag-List
      Tags.push_back(new TagUInt8("CountClockPoints", "Anzahl Schaltpunkte", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &CountClockPoints, "#", [this]() { updateTags(); }));
      Tags.push_back(new TagFloat("Value", "Aktueller Schaltwert", "", TagAccessType_T::Read, TagUsage_T::UseData, &Value, ""));

      updateTags();
    }

    /**
     * @brief Update the Tags based on the current number of clock points
     */
    void ClockValues::updateTags() {
      // Remove existing clock point tags
      Tags.erase(Tags.begin() + 2, Tags.end());

      // Adjust the size of the ClockPoints vector
      if (ClockPoints.size() < CountClockPoints) {
        // Add new ClockPoints if needed
        for (uint8_t i = ClockPoints.size(); i < CountClockPoints; i++) {
          ClockPoints.push_back({0, 0.0, false});
        }
      } else if (ClockPoints.size() > CountClockPoints) {
        // Remove excess ClockPoints if needed
        ClockPoints.resize(CountClockPoints);
      }

      // Add new clock point tags
      for (uint8_t i = 0; i < CountClockPoints; i++) {
        String indexStr = String(i + 1);
        Tags.push_back(new TagUInt32("Time" + indexStr, "Schaltpunkt " + indexStr, "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &ClockPoints[i].Time, "s", TagTypes_T::TypeTime));
        Tags.push_back(new TagFloat("Value" + indexStr, "Wert " + indexStr, "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &ClockPoints[i].Value, ""));
        Tags.push_back(new TagBool("DoRamp" + indexStr, "Rampen " + indexStr, "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &ClockPoints[i].DoRamp, "EIN", "AUS"));
      }
    }

    /**
     * @brief Handling ClockValues
     * Update the values based on the current time
     * @param time Current Time to check the Samplerate
     */
    void ClockValues::update(struct tm &time) {
      Debug.println(FLAG_LOOP, false, Name, __func__, "Run");

      // Nur bearbeiten wenn ein Datenpunkt vorhanden ist
      if (CountClockPoints == 0) {
        return;
      }

      // Berechne die aktuelle Zeit in Sekunden seit Tagesbeginn
      uint32_t currentTimeInSeconds = time.tm_hour * 3600 + time.tm_min * 60 + time.tm_sec;

      // Setze den aktuellen Wert auf den ersten Schaltpunkt
      Value = ClockPoints[0].Value;

      // Finde den aktuellen Schaltpunkt
      for (size_t i = 0; i < ClockPoints.size(); i++) {
        if (currentTimeInSeconds > ClockPoints[i].Time) {
          if (i == ClockPoints.size() - 1) {
            // Wenn der aktuelle Schaltpunkt der letzte ist, dann setze den Wert und breche ab
            Value = ClockPoints[i].Value;
            break;
          } else if (currentTimeInSeconds <= ClockPoints[i + 1].Time) {
            if (ClockPoints[i].DoRamp) {
              // Berechne den interpolierten Wert
              uint32_t nextTime = ClockPoints[i + 1].Time;
              float nextValue = ClockPoints[i + 1].Value;
              float timeDiff = nextTime - ClockPoints[i].Time;
              float valueDiff = nextValue - ClockPoints[i].Value;
              float timeElapsed = currentTimeInSeconds - ClockPoints[i].Time;
              Value = ClockPoints[i].Value + (valueDiff * (timeElapsed / timeDiff));
            } else {
              // Setze den aktuellen Wert
              Value = ClockPoints[i].Value;
            }
            break;
          }
        }
      }
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void ClockValues::AddToHandler(JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert(std::pair<String, std::function<bool(JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>>(SetupTagType, Create));
    }

    /**
     * @brief Create a new instance of the class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the instance to
     * @param _Hardware List of known Hardware-References
     * @return true if creation was successful
     * @return false if creation failed
     */
    bool ClockValues::Create(JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println(FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString(JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t CountClockPoints = GetSetupValueUINT8(SetupTagCountClockPoints, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new ClockValues (CountClockPoints, Name));
        Log["done"] = Name + " (CountClockPoints:" + String(CountClockPoints) + ")";
        Debug.println(FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}