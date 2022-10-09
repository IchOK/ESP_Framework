
#include <JCA_IOT_Protocol.h>

namespace JCA {
  namespace IOT {
    const char *Protocol::JsonTagConfig = "config";
    const char *Protocol::JsonTagData = "data";
    const char *Protocol::JsonTagValue = "value";
    const char *Protocol::JsonTagUnit = "unit";
    const char *Protocol::DebugSeparator = " - ";

    Protocol::Protocol () {
      ;
    }

    JsonObject Protocol::addConfig (JsonObject &_Element) {
      return _Element.createNestedObject (JsonTagConfig);
    }

    JsonObject Protocol::addData (JsonObject &_Element) {
      return _Element.createNestedObject (JsonTagData);
    }

    JsonVariant Protocol::findConfig (JsonObject &_Element) {
      if (_Element.containsKey (JsonTagConfig)) {
        return _Element[JsonTagConfig].as<JsonObject> ();
      } else {
        return JsonVariant ();
      }
    }

    JsonVariant Protocol::findData (JsonObject &_Element) {
      if (_Element.containsKey (JsonTagData)) {
        return _Element[JsonTagData].as<JsonObject> ();
      } else {
        return JsonVariant ();
      }
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, bool _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, float _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, int16_t _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, uint16_t _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, int32_t _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, uint32_t _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, long _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, String _Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }

    void Protocol::createTag (JsonObject &_Data, const char *_Name, const char *_Unit, const char *_Value) {
      JsonObject Tag;
      Tag = _Data.createNestedObject (_Name);
      Tag[JsonTagValue] = _Value;
      Tag[JsonTagUnit] = _Unit;
    }
  }
}
