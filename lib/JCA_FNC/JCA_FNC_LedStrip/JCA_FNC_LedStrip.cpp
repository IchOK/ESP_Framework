/**
 * @file JCA_FNC_LedStrip.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a LedStrip by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_LedStrip.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *LedStrip::ClassName = "LedStrip";
    const char *LedStrip::SetupTagType = "ledStrip";
    const char *LedStrip::SetupTagOutputPin = "pinOutput";
    const char *LedStrip::SetupTagNumLeds = "numLeds";
    const char *LedStrip::SetupTagPixelType = "pixelType";
    const char *LedStrip::SetupTagPixelSpeed = "pixelSpeed";

    /**
     * @brief Construct a new LedStrip::LedStrip object
     *
     * @param _Pin Analog Pin conected to the LedStrip-Sensor
     * @param _Name Element Name inside the Communication
     */
    LedStrip::LedStrip (uint8_t _Pin, uint8_t _NumLeds, neoPixelType _Type, String _Name)
        : FuncParent (_Name), Strip (_NumLeds, _Pin, _Type) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagUInt16 ("DelayAutoOff", "Verzögerung Auto-OFF", "Wird der Wert auf 0 gesetzt ist die Finktion inaktiv", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &DelayAutoOff, "Min"));

      Tags.push_back (new TagBool ("OnOff", "Einschalten", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &OnOff, "EIN", "AUS", std::bind (&LedStrip::updateColorCB, this)));
      Tags.push_back (new TagUInt32 ("Value", "Farbwert", "", static_cast<TagAccessType_T>(TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &Value, "RGB", std::bind (&LedStrip::updateColorCB, this), TagTypes_T::TypeColor));
      Tags.push_back (new TagUInt16 ("DelayCounter", "Verzögerung bis AUS", "", TagAccessType_T::ReadWrite, TagUsage_T::UseData, &DelayCounter, "Min"));

      DelayAutoOff = 0;
      OnOff = true;
      Value = 4108721;
      DelayCounter = 0;
      LastMillis = millis ();
      DelayMillis = 0;
      UpdateMillis = 0;
    }

    void  LedStrip::updateColorCB() {
      if (OnOff) {
        Strip.fill (Value, 0, 0);
      } else {
        Strip.clear ();
      }
      Strip.show ();
    }

    /**
     * @brief Init the LED-Strip
     */
    bool LedStrip::init () {
      Strip.begin ();
      Strip.show();
      return true;
    }

    /**
     * @brief Handling LedStrip
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void LedStrip::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");

      // Get Millis for AutoOff
      unsigned long ActMillis = millis ();
      unsigned long DiffMillis = ActMillis - LastMillis;
      LastMillis = ActMillis;

      if (DelayAutoOff > 0 && OnOff) {
        DelayMillis += DiffMillis;
        if (DelayMillis >= 60000) {
          DelayCounter++;
          DelayMillis -= 60000;
          if (DelayCounter >= DelayAutoOff) {
            OnOff = 0;
          }
        }
      } else {
        DelayCounter = 0;
        DelayMillis = 0;
      }

      UpdateMillis += DiffMillis;
      if (UpdateMillis >= 1000) {
        UpdateMillis -= 1000;
        updateColorCB();
      }
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void LedStrip::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
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
    bool LedStrip::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);
      uint8_t Pin = GetSetupValueUINT8 (SetupTagOutputPin, Done, _Setup, Log);
      uint8_t NumLeds = GetSetupValueUINT8 (SetupTagNumLeds, Done, _Setup, Log);
      String LedTypeName = GetSetupValueString (SetupTagPixelType, Done, _Setup, Log);
      uint16_t LedSpeed = GetSetupValueUINT16 (SetupTagPixelSpeed, Done, _Setup, Log);
      neoPixelType Type = 0;
      switch (LedSpeed)
      {
      case 400:
        Type = NEO_KHZ400;
        break;

      case 800:
        Type = NEO_KHZ800;
        break;

      default:
        _Log[SetupTagPixelSpeed] = "wrong value (400/800) " + LedTypeName;
        Done = false;
        break;
      }
      uint8_t TypeLen = LedTypeName.length ();
      if (TypeLen <= 4 && TypeLen >= 3) {
        for (uint8_t i = 0; i < TypeLen; i++ ) {
          switch (LedTypeName[i])
          {
          case 'R':
          case 'r':
            Type |= (i << 4);
            if (TypeLen == 3) {
              Type |= (i << 6);
            }
            break;

          case 'G':
          case 'g':
            Type |= (i << 2);
            break;

          case 'B':
          case 'b':
            Type |= i;
            break;

          case 'W':
          case 'w':
            if (TypeLen == 4) {
              Type |= (i << 6);
            } else {
              _Log[SetupTagPixelType] = "wrong format " + LedTypeName;
              Done = false;
            }
            break;

          default:
            _Log[SetupTagPixelType] = "wrong format " + LedTypeName;
            Done = false;
            break;
          }
        }
      }
      else {
        _Log[SetupTagPixelType] = "wrong format " + LedTypeName;
        Done = false;
      }

      if (Done) {
        _Functions.push_back (new LedStrip(Pin, NumLeds, Type, Name));
        Log["done"] = Name + "(Pin:" + String (Pin) + ", NumLeds: " + String (NumLeds) + ", Type: " + LedTypeName + ", Speed: " + String (LedSpeed) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
