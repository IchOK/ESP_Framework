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

#include <ArduinoJson.h>
#include <FS.h>
#include <map>
#include <vector>

#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_Parent.h>

using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    class FuncParent {
    protected:
      // Element Strings for Parent and Debug-Output
      static const char *ClassName;
      static const char *DebugSeparator;

      // Intern
      String Name;
      String Comment;

      // Dataconfig
      std::vector<TagParent*> Tags;

      // Create Parent-Structure
      // Functions Get/Set Data from/to Tag-Vector
      bool writeFunctionTags (File _FuncFile, TagUsage_T _FilterUsage);

      // Creation Sub-Functions
      static uint8_t GetSetupValueUINT8 (const char *_TagName, bool &_Done, JsonObject _Setup, JsonObject _Log);
      static uint16_t GetSetupValueUINT16 (const char *_TagName, bool &_Done, JsonObject _Setup, JsonObject _Log);
      static uint8_t GetSetupValueUINT8Arr (const char *_TagName, uint8_t *&_Values, bool &_Done, JsonObject _Setup, JsonObject _Log);
      static String GetSetupValueString (const char *_TagName, bool &_Done, JsonObject _Setup, JsonObject _Log);
      static void *GetSetupHardwareRef (const char *_TagName, String &_HwName, bool &_Done, JsonObject _Setup, JsonObject _Log, std::map<String, void *> _Hardware);

    public :
      // Json Tags
      static const char *JsonTagElements;
      static const char *JsonTagConfig;
      static const char *JsonTagData;
      static const char *JsonTagAll;
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
      FuncParent (String _Name, String _Comment);
      FuncParent (String _Name);
      String getName ();
      void writeFunction (File _FuncFile, bool &_Init);
      void setValues (JsonObject &_Function);
      void addValues (JsonObject &_Function);
      int16_t getTagIndex (String _Name);
      bool setTagValueByIndex (int16_t _Index, JsonVariant _Value);
      bool getTagValueByIndex (int16_t _Index, JsonVariant _Value);
      virtual void update (struct tm &_Time) { ; };
    };
  }
}

#endif