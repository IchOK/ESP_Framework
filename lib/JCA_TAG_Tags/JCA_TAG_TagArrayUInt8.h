/**
 * @file JCA_TAG_TAGArrayUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-14
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_TAG_TAGARRAYUINT8_
#define _JCA_TAG_TAGARRAYUINT8_

#include <JCA_TAG_Parent.h>

namespace JCA {
  namespace TAG {
    class TagArrayUInt8 : public TagParent {
      public:
        // Type Informations
        uint8_t Length;
        
        TagArrayUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint8_t *_Value, uint8_t _Length, SetCallback _CB);
        TagArrayUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint8_t *_Value, uint8_t _Length);
        ~TagArrayUInt8() {;};
        String writeTag ();
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        void addValue (JsonObject &_Values);
    };
  }
}

#endif