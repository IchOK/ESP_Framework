
#ifndef _JCA_FNC_PROTOCOL_
#define _JCA_FNC_PROTOCOL_

#include <ArduinoJson.h>

#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace IOT {
    class Protocol {
    protected:
      // Json Tags
      static const char *JsonTagConfig;
      static const char *JsonTagData;
      static const char *JsonTagValue;
      static const char *JsonTagUnit;
      static const char *DebugSeparator;

      // Intern
      JsonObject addConfig (JsonObject &_Element);
      JsonObject addData (JsonObject &_Element);
      JsonVariant findConfig (JsonObject &_Element);
      JsonVariant findData (JsonObject &_Element);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, bool _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, float _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, int16_t _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, uint16_t _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, int32_t _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, uint32_t _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, long _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, String _Value);
      void createTag (JsonObject &_Data, const char *_Name, const char *_Unit, const char *_Value);

    public:
      Protocol ();
    };
  }
}

#endif