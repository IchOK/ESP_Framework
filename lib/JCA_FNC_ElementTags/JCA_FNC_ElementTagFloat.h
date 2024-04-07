/**
 * @file JCA_FNC_ElementTagFloat.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ELEMENTTAGFLOAT_
#define _JCA_FNC_ELEMENTTAGFLOAT_

#include <JCA_FNC_ElementTags.h>

namespace JCA {
  namespace FNC {
    class ElementTagFloat : public ElementTag {
      public:
        // Type Informations
        float *Value;
        String Unit;

        ElementTagFloat (String _Name, String _Text, String _Comment, bool _ReadOnly, float *_Value, String _Unit);
        String createSetupTag ();
        void getJsonObject (JsonObject &_Data);
        void addJsonTag (JsonObject &_Tags);
    };
  }
}

#endif