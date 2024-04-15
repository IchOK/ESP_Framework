/**
 * @file JCA_FNC_ElementTagInt16.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ELEMENTTAGINT16_
#define _JCA_FNC_ELEMENTTAGINT16_

#include <JCA_FNC_ElementTags.h>

namespace JCA {
  namespace FNC {
    class ElementTagInt16 : public ElementTag {
      public:
        // Type Informations
        String Unit;

        ElementTagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, int16_t *_Value, String _Unit, SetCallback _CB);
        ElementTagInt16 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, int16_t *_Value, String _Unit);
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        String createSetupTag ();
        void getTagObject (JsonObject &_Tag);
        void addTagValue (JsonObject &_Values);
    };
  }
}

#endif