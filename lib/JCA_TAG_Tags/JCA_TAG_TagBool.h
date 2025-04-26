/**
 * @file JCA_TAG_TAGBool.h
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

        TagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText, SetCallback _CB, TagTypes_T _Type = TypeBool);
        TagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText, TagTypes_T _Type = TypeBool);
        ~TagBool () { ; };
        String writeTag ();
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        void addValue (JsonObject &_Values);
    };
  }
}

#endif