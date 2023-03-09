/**
 * @file JCA_FNC_Parent.h
 * @author JCA (https://github.com/ichok)
 * @brief Parent Class of all Framework Elements.
 * @version 1.0
 * @date 2022-12-10
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#ifndef _JCA_FNC_PARENT_
#define _JCA_FNC_PARENT_

#include "FS.h"
#include <ArduinoJson.h>

#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Parent {
    protected:
      // Element Strings for Parent and Debug-Output
      static const char *BtnOnDefault;
      static const char *BtnOffDefault;
      static const char *DebugSeparator;

      // Intern
      String Name;
      String Comment;

      // Prototypes for Child Elements
      virtual void createConfigValues (JsonObject &_Values) = 0;
      virtual void createDataValues (JsonObject &_Values) = 0;
      virtual void setConfig (JsonArray _Tags) = 0;
      virtual void setData (JsonArray _Tags) = 0;
      virtual void setCmd (JsonArray _Tags) = 0;

      virtual void writeSetupConfig (File _SetupFile) = 0;
      virtual void writeSetupData (File _SetupFile) = 0;
      virtual void writeSetupCmdInfo (File _SetupFile) = 0;

      // Get Element-Data by Name
      JsonVariant findConfig (JsonArray &_Elements);
      JsonVariant findData (JsonArray &_Elements);
      JsonVariant findCmd (JsonArray &_Elements);

      // Create Parent-Structure
      String createDefaultTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_BtnOnText, const char *_BtnOffText, bool _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, float _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int16_t _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint16_t _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int32_t _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint32_t _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, long _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, String _Value);
      String createSetupTag (const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Value);
      String createSetupCmdInfo (const char *_Name, const char *_Text, const char *_Comment, const char *_Type);
      String createSetupCmdInfo (const char *_Name, const char *_Text, const char *_Comment, const char *_Type, const char *_BtnText);

      // Conversion Functions
      void HexStringToByteArray (String _HexString, uint8_t *_ByteArray, uint8_t _Length);
      uint8_t HexCharToInt (char _HexChar);
      String ByteArrayToHexString (uint8_t *_ByteArray, uint8_t _Length);

    public:
      // Json Tags
      static const char *JsonTagElements;
      static const char *JsonTagConfig;
      static const char *JsonTagData;
      static const char *JsonTagCmdInfo;
      static const char *JsonTagCmd;
      static const char *JsonTagName;
      static const char *JsonTagText;
      static const char *JsonTagComment;
      static const char *JsonTagValue;
      static const char *JsonTagUnit;
      static const char *JsonTagOn;
      static const char *JsonTagOff;
      static const char *JsonTagType;
      static const char *JsonTagReadOnly;

      // external Functions
      Parent (String _Name, String _Comment);
      Parent (String _Name);
      virtual void update (struct tm &_Time) = 0;
      void set (JsonArray &_Elements);

      void getValues (JsonObject &_Elements);
      void writeSetup (File _SetupFile, bool &_ElementInit);
    };
  }
}

#endif