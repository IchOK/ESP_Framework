/**
 * @file JCA_FNC_OLED128x64.h
 * @brief Framework Element to drive a 128x64 SSD1306 OLED display via I2C.
 *
 * Up to 3 text/value lines can be shown. Each line consists of a free text
 * (label) and a numeric value with an optional format string.
 *
 * Format string syntax (per line):
 *  - "999.9"  -> 3 integer digits + 1 decimal, space-padded     ("  1.5")
 *  - "009.9"  -> 3 integer digits + 1 decimal, zero-padded      ("001.5")
 *  - "999"    -> integer only, space-padded                     ("  1")
 *  - "009"    -> integer only, zero-padded                      ("001")
 *  - empty    -> default String(value) without padding/precision
 *
 * The character font size is derived from the configured number of lines
 * and the maximum length of "text + space + formatted value" across all
 * visible lines, so that text always fits horizontally and vertically.
 *
 * @version 0.1
 * @date 2026-04-19
 *
 * Apache License
 */

#ifndef _JCA_FNC_OLED128X64_
#define _JCA_FNC_OLED128X64_

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_TagFloat.h>
#include <JCA_TAG_TagString.h>
#include <JCA_TAG_TagUInt16.h>

namespace JCA {
  namespace FNC {
    class OLED128x64 : public FuncParent {
    private:
      static const char *ClassName;

      // Function-Handler JSON-Tags
      static const char *SetupTagType;
      static const char *SetupTagAddr;
      static const char *SetupTagRefName;
      static const char *SetupTagLineCount;

      // Display dimensions and limits
      static const uint8_t ScreenWidth = 128;
      static const uint8_t ScreenHeight = 64;
      static const uint8_t MaxLines = 3;
      static const uint8_t CharBaseWidth = 6;  // GFX 5x7 font + 1 spacing
      static const uint8_t CharBaseHeight = 8; // GFX 5x7 font baseline grid

      // Hardware
      Adafruit_SSD1306 *Display;
      uint8_t Addr;

      // Konfig
      uint8_t LineCount;
      String LineText[MaxLines];
      String LineFormat[MaxLines];
      uint16_t UpdateIntervalMs;

      // Daten
      float LineValue[MaxLines];

      // Intern
      bool InitDone;
      bool LayoutDirty;
      uint8_t FontSize;
      String LastShown[MaxLines];
      String LastTextShown[MaxLines];
      unsigned long LastRenderMillis;

      static String formatValue (float _Value, const String &_Format);
      void markDirtyCB ();
      void recalcLayout ();
      void render ();

    public:
      OLED128x64 (TwoWire *_Wire, uint8_t _Addr, uint8_t _LineCount, String _Name);
      ~OLED128x64 ();
      void update (struct tm &_Time);

      // Function Handler Statics
      static void AddToHandler (JCA::IOT::FuncHandler &_Handler);
      static bool Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware);
      static void GetSetupSchema (JsonObject &_Schema);
    };
  }
}

#endif
