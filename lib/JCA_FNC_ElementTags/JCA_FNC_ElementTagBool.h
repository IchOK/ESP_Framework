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
        bool *Value;
        String BtnOnText;
        String BtnOffText;

        ElementTagBool (String _Name, String _Text, String _Comment, bool _ReadOnly, bool *_Value, String _BtnOnText, String _BtnOffText);
        String createSetupTag ();
        void getJsonObject (JsonObject &_Data);
        void addJsonTag (JsonObject &_Tags);
    };
  }
}

#endif