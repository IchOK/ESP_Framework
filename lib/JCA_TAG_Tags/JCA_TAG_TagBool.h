/**
 * @file JCA_TAG_TAGBool.h
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

#ifndef _JCA_TAG_TAGBOOL_
#define _JCA_TAG_TAGBOOL_

#include <JCA_TAG_Parent.h>

namespace JCA {
  namespace TAG {
    class TagBool : public TagParent {
      public:
        // Type Informations
        String BtnOnText;
        String BtnOffText;

        TagBool (String _Name, String _Text, String _Comment, TagAccessType_T _Access, TagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText, SetCallback _CB, TagTypes_T _Type = TypeBool);
        TagBool (String _Name, String _Text, String _Comment, TagAccessType_T _Access, TagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText, TagTypes_T _Type = TypeBool);
        ~TagBool () { ; };
        String writeTag ();
        bool getValue (JsonVariant _Value, TagAccessType_T _Access);
        bool setValue (JsonVariant _Value, TagAccessType_T _Access);
        void addValue (JsonObject &_Values, TagAccessType_T _Access);
    };
  }
}

#endif