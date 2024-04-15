/**
 * @file JCA_FNC_ElementTagBool.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ELEMENTTAGBOOL_
#define _JCA_FNC_ELEMENTTAGBOOL_

#include <JCA_FNC_ElementTags.h>

namespace JCA {
  namespace FNC {
    class ElementTagBool : public ElementTag {
      public:
        // Type Informations
        String BtnOnText;
        String BtnOffText;

        ElementTagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText, SetCallback _CB);
        ElementTagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, bool *_Value, String _BtnOnText, String _BtnOffText);
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        String createSetupTag ();
        void getTagObject (JsonObject &_Tag);
        void addTagValue (JsonObject &_Values);
    };
  }
}

#endif