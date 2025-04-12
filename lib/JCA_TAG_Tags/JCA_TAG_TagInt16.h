/**
 * @file JCA_TAG_TAGInt16.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.1
 * @date 2024-04-07
 * @changelog
 * - [1.1] 2025-04-12: Add type to constructor, to override with custom web styles
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_TAG_TAGINT16_
#define _JCA_TAG_TAGINT16_

#include <JCA_TAG_Parent.h>

namespace JCA {
  namespace TAG {
    class TagInt16 : public TagParent {
      public:
        // Type Informations
        String Unit;

        TagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, int16_t *_Value, String _Unit, SetCallback _CB, TagTypes_T _Type = TypeInt16);
        TagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, int16_t *_Value, String _Unit, TagTypes_T _Type = TypeInt16);
        ~TagInt16 () { ; };
        String writeTag ();
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        void addValue (JsonObject &_Values);
    };
  }
}

#endif