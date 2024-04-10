/**
 * @file JCA_FNC_ElementTags.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ELEMENTTAGS_
#define _JCA_FNC_ELEMENTTAGS_

#include "FS.h"
#include <ArduinoJson.h>

#include <JCA_SYS_DebugOut.h>

#define JCA_FNC_ELEMENTTAGS_JsonName "name"
#define JCA_FNC_ELEMENTTAGS_JsonText "text"
#define JCA_FNC_ELEMENTTAGS_JsonComment "comment"
#define JCA_FNC_ELEMENTTAGS_JsonValue "value"
#define JCA_FNC_ELEMENTTAGS_JsonUnit "unit"
#define JCA_FNC_ELEMENTTAGS_JsonOn "on"
#define JCA_FNC_ELEMENTTAGS_JsonOff "off"
#define JCA_FNC_ELEMENTTAGS_JsonType "type"
#define JCA_FNC_ELEMENTTAGS_JsonReadOnly "readOnly"
#define JCA_FNC_ELEMENTTAGS_JsonList "list"
#define JCA_FNC_ELEMENTTAGS_JsonListIndex "i"
#define JCA_FNC_ELEMENTTAGS_JsonListValue "v"

namespace JCA {
  namespace FNC {
    enum ElementTagTypes_T {
      TagNone = 0,
      TagBool = 1,
      TagFloat = 2,
      TagInt16 = 3,
      TagUInt16 = 4,
      TagInt32 = 5,
      TagUInt32 = 6,
      TagString = 7,
      TagListUInt8 = 8
    };

    enum ElementTagUsage_T {
      UseNone = 0x00,
      UseData = 0x01,
      UseConfig = 0x02,
      UseCmd = 0x04,
      UseIgnor = 0xff
    };
    class ElementTag {
      protected:
        String createSetupTagBase ();
        void getBaseObject (JsonObject &_Tag);

      public:
        // Default Informations
        ElementTagTypes_T Type;
        ElementTagUsage_T Usage;
        String Name;
        String Text;
        String Comment;
        bool ReadOnly;
        void* Value;

        ElementTag (String _Name, String _Text, String _Comment, bool _ReadOnly, void* _Value, ElementTagTypes_T _Type, ElementTagUsage_T _Usage);
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        virtual String createSetupTag () = 0;
        virtual void getTagObject (JsonObject &_Tag) = 0;
        virtual void addTagValue (JsonObject &_Values) = 0;
    };
  }
}

#endif