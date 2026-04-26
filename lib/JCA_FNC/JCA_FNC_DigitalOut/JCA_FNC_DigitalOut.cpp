/**
 * @file JCA_FNC_DigitalOut.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a DigitalOut by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include "JCA_FNC_DigitalOut.h"
#include "JCA_SYS_DebugOut.h"
#include "JCA_TAG_TagBool.h"
#include "JCA_TAG_TagListUInt8.h"
#include "JCA_TAG_TagUInt16.h"
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *DigitalOut::ClassName = "DigitalOut";
    const char *DigitalOut::SetupTagType = "digitalOut";
    const char *DigitalOut::SetupTagOutputPin = "pinOutput";
    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    DigitalOut::DigitalOut (uint8_t _Pin, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("DelayAutoOff", "Verzögerung Auto-OFF", "Wird der Wert auf 0 gesetzt ist die Finktion inaktiv", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &DelayAutoOff, "", nullptr));
      Tags.push_back (new TagListUInt8 ("DelayAutoOffUnit", "Einheit Auto-OFF", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, (uint8_t *)&DelayAutoOffUnit));
      TagListUInt8 *DelayAutoOffUnitElement = static_cast<TagListUInt8 *> (Tags[Tags.size () - 1]);
      DelayAutoOffUnitElement->List.insert ({ DelayAutoOffUnit_T::DELAY_UNIT_H, "h" });
      DelayAutoOffUnitElement->List.insert ({ DelayAutoOffUnit_T::DELAY_UNIT_M, "m" });
      DelayAutoOffUnitElement->List.insert ({ DelayAutoOffUnit_T::DELAY_UNIT_S, "s" });
      DelayAutoOffUnitElement->List.insert ({ DelayAutoOffUnit_T::DELAY_UNIT_MS, "ms" });

      Tags.push_back (new TagBool ("Value", "Eingeschaltet", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &Value, "EIN", "AUS"));
      Tags.push_back(new TagUInt16("DelayCounter", "Verzögerung Zähler", "", TagAccessType_T::Read, TagUsage_T::UseData, &DelayCounter, ""));
      // Init Data
      DelayAutoOff = 0;
      DelayAutoOffUnit = DelayAutoOffUnit_T::DELAY_UNIT_M;
      Pin = _Pin;
      pinMode(Pin, OUTPUT);
      Value = false;
      DelayCounter = 0;
      LastMillis = millis ();
      DelayMillis = 0;
    }

    /**
     * @brief Handling DigitalOut
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void DigitalOut::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");

      // Get Millis for AutoOff
      if (DelayAutoOff > 0 && Value) {
        unsigned long ActMillis = millis ();
        unsigned long DiffMillis = ActMillis - LastMillis;
        LastMillis = ActMillis;
        DelayMillis += DiffMillis;
        if (DelayMillis >= getDelayAutoOffUnitMillis()) {
          DelayCounter++;
          DelayMillis -= getDelayAutoOffUnitMillis();
          if (DelayCounter >= DelayAutoOff) {
            Value = false;
          }
        }
      } else {
        LastMillis = millis();
        DelayCounter = 0;
        DelayMillis = 0;
      }

      digitalWrite(Pin, Value);
    }

    uint32_t DigitalOut::getDelayAutoOffUnitMillis () const {
      switch (DelayAutoOffUnit) {
        case DelayAutoOffUnit_T::DELAY_UNIT_H:
          return 3600000;
        case DelayAutoOffUnit_T::DELAY_UNIT_S:
          return 1000;
        case DelayAutoOffUnit_T::DELAY_UNIT_MS:
          return 1;
        case DelayAutoOffUnit_T::DELAY_UNIT_M:
        default:
          return 60000;
      }
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     * 
     * @param _Handler Function Handler
     */
    void DigitalOut::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
      _Handler.FunctionSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> (SetupTagType, GetSetupSchema));
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
    bool DigitalOut::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t PinOutput = GetSetupValueUINT8(SetupTagOutputPin, Done, _Setup, Log);

      if (Done) {
        _Functions.push_back (new DigitalOut(PinOutput, Name));
        Log["done"] = Name + " (OutputPin:" + String (PinOutput) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }

    void DigitalOut::GetSetupSchema(JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray>();
      
      // name parameter
      JsonObject NameParam = Parameters.add<JsonObject>();
      NameParam["name"] = JCA_IOT_FUNCHANDLER_SETUP_NAME;
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Funktion für die Kommunikation";

      // outputPin parameter
      JsonObject OutputPinParam = Parameters.add<JsonObject>();
      OutputPinParam["name"] = SetupTagOutputPin;
      OutputPinParam["type"] = "uint8";
      OutputPinParam["comment"] = "Pin für den Ausgang";
    }
  }
}
