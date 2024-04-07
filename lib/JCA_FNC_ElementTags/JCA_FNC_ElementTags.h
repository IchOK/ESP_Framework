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
      TagString = 7
    };

    class ElementTag {
      protected:
        String createSetupTagBase ();
        void getJsonObjectBase (JsonObject &_Data);

      public:
        // Default Informations
        ElementTagTypes_T Type;
        String Name;
        String Text;
        String Comment;
        bool ReadOnly;

        ElementTag (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagTypes_T _Type);
        virtual String createSetupTag () = 0;
        virtual void getJsonObject (JsonObject &_Data) = 0;
        virtual void addJsonTag (JsonObject &_Tags) = 0;
    };
  }
}

#endif