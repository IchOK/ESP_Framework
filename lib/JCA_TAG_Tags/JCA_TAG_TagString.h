/**
 * @file JCA_TAG_TAGString.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_TAG_TAGSTRING_
#define _JCA_TAG_TAGSTRING_

#include <JCA_TAG_Parent.h>

namespace JCA {
  namespace TAG {
    class TagString : public TagParent {
      public:
        // Type Informations

        TagString (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, String *_Value, SetCallback _CB);
        TagString (String _Name, String _Text, String _Comment, bool _ReadOnly, TagUsage_T _Usage, String *_Value);
        String writeTag ();
        bool getValue (JsonVariant _Value);
        bool setValue (JsonVariant _Value);
        void addValue (JsonObject &_Values);
    };
  }
}

#endif