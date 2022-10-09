
#include <JCA_FNC_Parent.h>
using namespace JCA::SYS;

namespace JCA {
  namespace FNC {
    Parent::Parent (const char *_Name) {
      strncpy (ObjectName, _Name, sizeof (ObjectName));
    }

    void Parent::set (JsonObject _Collection) {
      if (_Collection.containsKey (ObjectName)) {
        JsonObject Element = _Collection[ObjectName].as<JsonObject> ();
        JsonVariant Data;
        Data = findConfig(Element);
        if (Data.is<JsonObject>()) {
          setConfig (Data.as<JsonObject> ());
        }
        Data = findData (Element);
        if (Data.is<JsonObject> ()) {
          setData (Data.as<JsonObject> ());
        }
      }
    }

    void Parent::getConfig (JsonObject &_Collection) {
      JsonObject Element = _Collection.createNestedObject (ObjectName);
      JsonObject Config = addConfig (Element);
      createConfig (Config);
      if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "Element:")) {
        String ObjectStr;
        serializeJson (Element, ObjectStr);
        Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ObjectStr);
      }
    }

    void Parent::getData (JsonObject &_Collection) {
      JsonObject Element = _Collection.createNestedObject (ObjectName);
      JsonObject Data = addData (Element);
      createData (Data);
      if (Debug.print (FLAG_CONFIG, false, ObjectName, __func__, "ObjectData:")) {
        String ObjectStr;
        serializeJson (Element, ObjectStr);
        Debug.println (FLAG_CONFIG, false, ObjectName, __func__, ObjectStr);
      }
    }

    void Parent::getAll (JsonObject &_Collection) {
      JsonObject Element = _Collection.createNestedObject (ObjectName);
      JsonObject Config = addConfig (Element);
      JsonObject Data = addData (Element);
      createConfig (Config);
      createData (Data);
    }
  }
}
