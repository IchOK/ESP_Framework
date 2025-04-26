/**
 * @file JCA_TAG_TAGUInt32.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.2
 * @date 2024-04-07
 * @changelog
 * - [1.1] 2025-04-12: Add type to constructor, to override with custom web styles
 * - [1.2] 2025-04-26: Change ReadOnly to AccessType
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

        TagUInt32 (String _Name, String _Text, String _Comment, TagAccessType_T _Access, TagUsage_T _Usage, uint32_t *_Value, String _Unit, SetCallback _CB, TagTypes_T _Type = TypeUInt32);
        TagUInt32 (String _Name, String _Text, String _Comment, TagAccessType_T _Access, TagUsage_T _Usage, uint32_t *_Value, String _Unit, TagTypes_T _Type = TypeUInt32);
        ~TagUInt32() {;};
        String writeTag ();
        bool getValue (JsonVariant _Value, TagAccessType_T _Access);
        bool setValue (JsonVariant _Value, TagAccessType_T _Access);
        void addValue (JsonObject &_Values, TagAccessType_T _Access);
    };
  }
}
#endif
