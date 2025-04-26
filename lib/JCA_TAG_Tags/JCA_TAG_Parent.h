/**
 * @file JCA_TAG_TAGs.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.2
 * @date 2024-04-07
 * - [1.2] 2025-04-26: Change ReadOnly to AccessType
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_TAG_TAGS_
#define _JCA_TAG_TAGS_

#include "FS.h"
#include <ArduinoJson.h>

#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_Conversion.h>

#define JCA_TAG_TAGS_JsonName "name"
#define JCA_TAG_TAGS_JsonText "text"
#define JCA_TAG_TAGS_JsonComment "comment"
#define JCA_TAG_TAGS_JsonValue "value"
#define JCA_TAG_TAGS_JsonUnit "unit"
#define JCA_TAG_TAGS_JsonOn "on"
#define JCA_TAG_TAGS_JsonOff "off"
#define JCA_TAG_TAGS_JsonType "type"
#define JCA_TAG_TAGS_JsonReadOnly "readOnly"
#define JCA_TAG_TAGS_JsonList "list"
#define JCA_TAG_TAGS_JsonListIndex "i"
#define JCA_TAG_TAGS_JsonListValue "v"

namespace JCA {
  namespace TAG {
    enum TagTypes_T : uint8_t {
      TypeNone = 0,
      TypeBool = 1,
      TypeFloat = 2,
      TypeInt8 = 3,
      TypeUInt8 = 4,
      TypeInt16 = 5,
      TypeUInt16 = 6,
      TypeInt32 = 7,
      TypeUInt32 = 8,
      TypeString = 9,
      TypeTime = 51,
      TypeDateTime = 52,
      TypeColor = 53,
      TypeBoolCmd = 54,
      TypeDaySelect = 55,
      TypeListUInt8 = 101,
      TypeArrayUInt8 = 102
    };

    enum TagUsage_T : uint8_t {
      UseInOut = 0x00,     ///< Tag only used for function-function iteraction
      UseData = 0x01,      ///< Tag storing runtime data
      UseConfig = 0x02,    ///< Tag storing configuration
      UseCmd = 0x04,       ///< Only execute a command
      GetWebData = 0x05,   ///< filter tags for the data-tab on the website
      GetWebConfig = 0x02, ///< filter tags for the config-tab on the website
      GetAll = 0xff        ///< get all tags
    };

    enum TagAccessType_T : uint8_t {
      Read = 0x01,
      Write = 0x02,
      ReadWrite = 0x03,
      Save = 0x04
    };

    typedef std::function<void (void)> SetCallback;
    class TagParent {
      protected:
        SetCallback afterSetCB;
        String writeTagBase ();

      public:
        // Default Informations
        TagTypes_T Type;
        TagUsage_T Usage;
        String Name;
        String Text;
        String Comment;
        TagAccessType_T Access;
        void* Value;

        TagParent (String _Name, String _Text, String _Comment, TagAccessType_T _Access, void *_Value, TagTypes_T _Type, TagUsage_T _Usage, SetCallback _CB);
        TagParent (String _Name, String _Text, String _Comment, TagAccessType_T _Access, void *_Value, TagTypes_T _Type, TagUsage_T _Usage);
        virtual ~TagParent() {;};
        virtual String writeTag () { return ""; };
        virtual bool getValue (JsonVariant _Value, TagAccessType_T _Access) { return false; };
        virtual bool setValue(JsonVariant _Value, TagAccessType_T _Access) {return false; };
        virtual void addValue (JsonObject &_Values, TagAccessType_T _Access) { ; };
    };
  }
}

#endif