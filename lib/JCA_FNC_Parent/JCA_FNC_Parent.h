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
#include <vector>

#include <JCA_SYS_DebugOut.h>
#include <JCA_TAG_Parent.h>

using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    class FuncParent {
    protected:
      // Element Strings for Parent and Debug-Output
      static const char *DebugSeparator;

      // Intern
      String Name;
      String Comment;

      // Dataconfig
      std::vector<TagParent*> Tags;

      // Create Parent-Structure
      // Functions Get/Set Data from/to Tag-Vector
      void writeFunctionTags (File _FuncFile, TagUsage_T _FilterUsage);

    public:
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