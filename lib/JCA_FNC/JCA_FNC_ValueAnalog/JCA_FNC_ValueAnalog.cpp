/**
 * @file JCA_FNC_ValueAnalog.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a ValueAnalog by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_ValueAnalog.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *ValueAnalog::ClassName = "ValueAnalog";
    const char *ValueAnalog::SetupTagType = "valueAnalog";
    const char *ValueAnalog::SetupTagUnit = "unit";
    /**
     * @brief Construct a new ValueAnalog::ValueAnalog object
     *
     * @param _Name Element Name inside the Communication
     */
    ValueAnalog::ValueAnalog (String _Name, String _Unit)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagFloat ("Value", "Wert", "", false, TagUsage_T::UseData, &Value, _Unit));
      // Init Data
      Value = false;
    }

    /**
     * @brief Handling ValueAnalog
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void ValueAnalog::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     * 
     * @param _Handler Function Handler
     */
    void ValueAnalog::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
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
    bool ValueAnalog::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      String Unit = GetSetupValueString (SetupTagUnit, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new ValueAnalog(Name, Unit));
        Log["done"] = Name + "(Unit: " + Unit + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
