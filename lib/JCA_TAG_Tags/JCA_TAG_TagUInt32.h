/**
 * @file JCA_TAG_TAGUInt32.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_TAG_TAGUINT32_
#define _JCA_TAG_TAGUINT32_

#include <JCA_TAG_Parent.h>

namespace JCA {
  namespace TAG {
    class TagUInt32 : public TagParent {
      public:
        // Type Informations
        String Unit;

        TagUInt32 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint32_t *_Value, String _Unit, SetCallback _CB);
        TagUInt32 (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, uint32_t *_Value, String _Unit);
        String writeTag ();
        bool getValue (JsonVariant _Value);
        bool setValue (JsonVariant _Value);
        void addValue (JsonObject &_Values);
    };
  }
}

#endif