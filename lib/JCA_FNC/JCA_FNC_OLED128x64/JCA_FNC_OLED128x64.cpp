/**
 * @file JCA_FNC_OLED128x64.cpp
 * @brief 128x64 SSD1306 OLED display with up to 3 text/value lines.
 *
 * @version 0.1
 * @date 2026-04-19
 *
 * Apache License
 */

#include "JCA_FNC_OLED128x64.h"
#include "JCA_SYS_DebugOut.h"
#include "JCA_TAG_TagFloat.h"
#include "JCA_TAG_TagString.h"
#include "JCA_TAG_TagUInt16.h"
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *OLED128x64::ClassName = "OLED128x64";
    const char *OLED128x64::SetupTagType = "oled128x64";
    const char *OLED128x64::SetupTagAddr = "addr";
    const char *OLED128x64::SetupTagRefName = "refName";
    const char *OLED128x64::SetupTagLineCount = "lines";

    /**
     * @brief Construct a new OLED128x64 object
     *
     * @param _Wire Pointer to the TwoWire interface (use &Wire for default I2C)
     * @param _Addr I2C address of the SSD1306 (typical: 0x3C)
     * @param _LineCount Number of visible lines (1..3)
     * @param _Name Element name inside the FuncHandler
     */
    OLED128x64::OLED128x64 (TwoWire *_Wire, uint8_t _Addr, uint8_t _LineCount, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");

      Addr = _Addr;
      if (_LineCount < 1) {
        _LineCount = 1;
      }
      if (_LineCount > MaxLines) {
        _LineCount = MaxLines;
      }
      LineCount = _LineCount;

      Display = new Adafruit_SSD1306 (ScreenWidth, ScreenHeight, _Wire == nullptr ? &Wire : _Wire, -1);

      Tags.push_back (new TagUInt16 ("UpdateInterval", "Bildschirm Aktualisierung", "Intervall in ms zwischen zwei Display-Updates (0 = jeden Loop)", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &UpdateIntervalMs, "ms"));

      for (uint8_t i = 0; i < LineCount; i++) {
        String NumStr = String (i + 1);
        Tags.push_back (new TagString ("Text" + NumStr, "Text Zeile " + NumStr, "Beschriftung links neben dem Wert", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &LineText[i], std::bind (&OLED128x64::markDirtyCB, this)));
        Tags.push_back (new TagString ("Format" + NumStr, "Format Zeile " + NumStr, "Format wie 999.9 oder 009.9 (führende Nullen)", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseConfig, &LineFormat[i], std::bind (&OLED128x64::markDirtyCB, this)));
      }
      for (uint8_t i = 0; i < LineCount; i++) {
        String NumStr = String (i + 1);
        Tags.push_back (new TagFloat ("Value" + NumStr, "Wert Zeile " + NumStr, "", static_cast<TagAccessType_T> (TagAccessType_T::ReadWrite | TagAccessType_T::Save), TagUsage_T::UseData, &LineValue[i], ""));
      }

      UpdateIntervalMs = 250;
      for (uint8_t i = 0; i < MaxLines; i++) {
        LineText[i] = "";
        LineFormat[i] = "999.9";
        LineValue[i] = 0.0f;
        LastShown[i] = "";
        LastTextShown[i] = "";
      }
      InitDone = false;
      LayoutDirty = true;
      FontSize = 1;
      LastRenderMillis = 0;
    }

    OLED128x64::~OLED128x64 () {
      if (Display != nullptr) {
        delete Display;
        Display = nullptr;
      }
    }

    void OLED128x64::markDirtyCB () {
      LayoutDirty = true;
    }

    /**
     * @brief Convert a value to a String according to the user format string.
     *
     * Format examples:
     *   "999.9" -> "%5.1f"  ("  1.5")
     *   "009.9" -> "%05.1f" ("001.5")
     *   "999"   -> "%3.0f"  ("  1")
     *   "009"   -> "%03.0f" ("001")
     *   ""      -> default String(value)
     */
    String OLED128x64::formatValue (float _Value, const String &_Format) {
      if (_Format.length () == 0) {
        return String (_Value);
      }

      bool LeadingZeros = (_Format.charAt (0) == '0');
      int DotIdx = _Format.indexOf ('.');

      uint8_t IntDigits;
      uint8_t DecDigits;
      uint8_t TotalWidth;
      if (DotIdx < 0) {
        IntDigits = static_cast<uint8_t> (_Format.length ());
        DecDigits = 0;
        TotalWidth = IntDigits;
      } else {
        IntDigits = static_cast<uint8_t> (DotIdx);
        DecDigits = static_cast<uint8_t> (_Format.length () - DotIdx - 1);
        TotalWidth = IntDigits + 1 + DecDigits; // includes the '.'
      }
      if (TotalWidth == 0) {
        return String (_Value);
      }

      char Buf[32];
      if (LeadingZeros) {
        snprintf (Buf, sizeof (Buf), "%0*.*f", static_cast<int> (TotalWidth), static_cast<int> (DecDigits), _Value);
      } else {
        snprintf (Buf, sizeof (Buf), "%*.*f", static_cast<int> (TotalWidth), static_cast<int> (DecDigits), _Value);
      }
      return String (Buf);
    }

    /**
     * @brief Calculate uniform font size that fits all visible lines.
     *
     * Width per char  = FontSize * CharBaseWidth (6 px).
     * Height per char = FontSize * CharBaseHeight (8 px).
     * Per-row height available = ScreenHeight / LineCount.
     */
    void OLED128x64::recalcLayout () {
      // Maximum length of "text + space + formatted value" across all lines
      uint8_t MaxChars = 1;
      for (uint8_t i = 0; i < LineCount; i++) {
        String Sample = formatValue (0.0f, LineFormat[i]);
        uint8_t Len = static_cast<uint8_t> (LineText[i].length () + 1 + Sample.length ());
        if (Len > MaxChars) {
          MaxChars = Len;
        }
      }

      uint8_t RowHeight = ScreenHeight / LineCount;

      uint8_t MaxByHeight = RowHeight / CharBaseHeight;
      uint8_t MaxByWidth = (ScreenWidth / CharBaseWidth) / MaxChars;
      if (MaxByHeight < 1) {
        MaxByHeight = 1;
      }
      if (MaxByWidth < 1) {
        MaxByWidth = 1;
      }
      FontSize = (MaxByHeight < MaxByWidth) ? MaxByHeight : MaxByWidth;
      // Clamp to a sane upper bound (Adafruit GFX scales arbitrarily; cap at 4)
      if (FontSize > 4) {
        FontSize = 4;
      }
    }

    /**
     * @brief Push current state to the display. Skips redraw if nothing changed.
     */
    void OLED128x64::render () {
      if (!InitDone || Display == nullptr) {
        return;
      }

      bool AnyChange = LayoutDirty;
      String FormattedNow[MaxLines];
      for (uint8_t i = 0; i < LineCount; i++) {
        FormattedNow[i] = formatValue (LineValue[i], LineFormat[i]);
        if (FormattedNow[i] != LastShown[i] || LineText[i] != LastTextShown[i]) {
          AnyChange = true;
        }
      }
      if (!AnyChange) {
        return;
      }

      if (LayoutDirty) {
        recalcLayout ();
        LayoutDirty = false;
      }

      Display->clearDisplay ();
      Display->setTextColor (SSD1306_WHITE);
      Display->setTextSize (FontSize);

      const uint8_t CharW = FontSize * CharBaseWidth;
      const uint8_t CharH = FontSize * CharBaseHeight;
      const uint8_t RowHeight = ScreenHeight / LineCount;
      const uint8_t YOffset = (RowHeight - CharH) / 2;

      for (uint8_t i = 0; i < LineCount; i++) {
        uint16_t Y = i * RowHeight + YOffset;

        Display->setCursor (0, Y);
        Display->print (LineText[i]);

        // Right-align the formatted value
        uint16_t ValuePxWidth = static_cast<uint16_t> (FormattedNow[i].length ()) * CharW;
        int16_t ValueX = static_cast<int16_t> (ScreenWidth) - static_cast<int16_t> (ValuePxWidth);
        if (ValueX < 0) {
          ValueX = 0;
        }
        Display->setCursor (ValueX, Y);
        Display->print (FormattedNow[i]);

        LastShown[i] = FormattedNow[i];
        LastTextShown[i] = LineText[i];
      }

      Display->display ();
    }

    /**
     * @brief Cyclic update: lazy init of the display, throttled rendering.
     */
    void OLED128x64::update (struct tm &_Time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");

      if (!InitDone) {
        if (Display != nullptr && Display->begin (SSD1306_SWITCHCAPVCC, Addr)) {
          Display->clearDisplay ();
          Display->display ();
          InitDone = true;
          LayoutDirty = true;
          Debug.println (FLAG_SETUP, false, Name, __func__, "OLED init OK");
        } else {
          Debug.println (FLAG_ERROR, false, Name, __func__, "OLED init failed");
          return;
        }
      }

      unsigned long Now = millis ();
      if (UpdateIntervalMs == 0 || (Now - LastRenderMillis) >= UpdateIntervalMs) {
        render ();
        LastRenderMillis = Now;
      }
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     */
    void OLED128x64::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
      _Handler.FunctionSchemaList.insert (std::pair<String, std::function<void (JsonObject &)>> (SetupTagType, GetSetupSchema));
    }

    bool OLED128x64::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject> ();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, Log);

      // refName is optional: if missing use default Wire
      TwoWire *TwoWireRef = nullptr;
      String TwoWireName = "Wire";
      if (_Setup[SetupTagRefName].is<JsonVariant> ()) {
        bool RefDone = true;
        TwoWireRef = static_cast<TwoWire *> (GetSetupHardwareRef (SetupTagRefName, TwoWireName, RefDone, _Setup, Log, _Hardware));
        if (!RefDone) {
          Done = false;
        }
      } else {
        TwoWireRef = &Wire;
      }

      uint8_t Addr = 0x3C;
      if (_Setup[SetupTagAddr].is<int> ()) {
        Addr = _Setup[SetupTagAddr].as<uint8_t> ();
      }

      uint8_t LineCount = 3;
      if (_Setup[SetupTagLineCount].is<int> ()) {
        LineCount = _Setup[SetupTagLineCount].as<uint8_t> ();
      }
      if (LineCount < 1 || LineCount > 3) {
        Log["error"] = String ("lines must be between 1 and 3 (got ") + String (LineCount) + ")";
        Done = false;
      }

      if (Done) {
        _Functions.push_back (new OLED128x64 (TwoWireRef, Addr, LineCount, Name));
        Log["done"] = Name + " (Addr:0x" + String (Addr, HEX) + ", TwoWire:" + TwoWireName + ", Lines:" + String (LineCount) + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }

    void OLED128x64::GetSetupSchema (JsonObject &_Schema) {
      JsonArray Parameters = _Schema["parameters"].to<JsonArray> ();

      JsonObject NameParam = Parameters.add<JsonObject> ();
      NameParam["name"] = JCA_IOT_FUNCHANDLER_SETUP_NAME;
      NameParam["type"] = "string";
      NameParam["comment"] = "Name der Funktion für die Kommunikation";

      JsonObject RefNameParam = Parameters.add<JsonObject> ();
      RefNameParam["name"] = SetupTagRefName;
      RefNameParam["type"] = "string";
      RefNameParam["comment"] = "Name der TwoWire/I2C-Hardware-Instanz (optional, sonst Standard-Wire)";

      JsonObject AddrParam = Parameters.add<JsonObject> ();
      AddrParam["name"] = SetupTagAddr;
      AddrParam["type"] = "uint8";
      AddrParam["comment"] = "I2C-Adresse des SSD1306 (z.B. 60 = 0x3C)";

      JsonObject LinesParam = Parameters.add<JsonObject> ();
      LinesParam["name"] = SetupTagLineCount;
      LinesParam["type"] = "uint8";
      LinesParam["comment"] = "Anzahl Zeilen (1..3)";
    }
  }
}
