/**
 * @file JCA_TAG_TAGListUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_TAG_TAGLISTUINT8_
#define _JCA_TAG_TAGLISTUINT8_

#include <map>
#include <JCA_TAG_Parent.h>

namespace JCA {
  namespace TAG {
    class TagListUInt8 : public TagParent {
      public:
        // Type Informations
        std::map<uint8_t, String> List;

        TagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint8_t *_Value, SetCallback _CB);
        TagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint8_t *_Value);
        ~TagListUInt8 ();
        String writeTag ();
        bool getValue (JsonVariant _Value);
        bool setValue (JsonVariant _Value);
        void addValue (JsonObject &_Values);
    };
  }
}

#endif