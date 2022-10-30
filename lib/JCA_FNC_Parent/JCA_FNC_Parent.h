
#ifndef _JCA_FNC_PARENT_
#define _JCA_FNC_PARENT_

#include <ArduinoJson.h>

#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace FNC {
    class Protocol {
    protected:
      // Element Strings for Protocol and Debug-Output
      static const char *BtnOnDefault;
      static const char *BtnOffDefault;
      static const char *DebugSeparator;

      // Intern
      String Name;
      String Comment;

      // Prototypes for Child Elements
      virtual void createConfigTags (JsonArray &_Tags) = 0;
      virtual void createDataTags (JsonArray &_Tags) = 0;
      virtual void createCmdInfoTags (JsonArray &_Tags) = 0;
      virtual void setConfig (JsonArray _Tags) = 0;
      virtual void setData (JsonArray _Tags) = 0;
      virtual void setCmd (JsonArray _Tags) = 0;

      // Get Element-Data by Name
      JsonVariant findConfig (JsonArray &_Elements);
      JsonVariant findData (JsonArray &_Elements);
      JsonVariant findCmd (JsonArray &_Elements);
      
      // Create Protocol-Structure
      JsonObject createEmptyElement (JsonArray &_Elements);
      JsonObject createEmptyTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_BtnOnText, const char *_BntOffText, bool _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, float _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int16_t _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint16_t _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, int32_t _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, uint32_t _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Unit, long _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, String _Value);
      void createTag (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, bool _ReadOnly, const char *_Value);
      void createCmdInfo (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, const char *_Type);
      void createCmdInfo (JsonArray &_Tags, const char *_Name, const char *_Text, const char *_Comment, const char *_Type, const char *_BtnText);

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
      Protocol (String _Name, String _Comment);
      Protocol (String _Name);
      virtual void update (struct tm &_Time) = 0;
      void getConfig (JsonArray &_Elements);
      void getData (JsonArray &_Elements);
      void getCmdInfo (JsonArray &_Elements);
      void getAll (JsonArray &_Elements);
      void set (JsonArray &_Elements);
    };
  }
}

#endif