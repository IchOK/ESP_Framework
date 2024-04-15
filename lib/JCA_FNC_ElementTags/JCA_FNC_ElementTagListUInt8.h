/**
 * @file JCA_FNC_ElementTagListUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-07
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ELEMENTTAGLISTUINT8_
#define _JCA_FNC_ELEMENTTAGLISTUINT8_

#include <map>
#include <JCA_FNC_ElementTags.h>

namespace JCA {
  namespace FNC {
    class ElementTagListUInt8 : public ElementTag {
      public:
        // Type Informations
        std::map<uint8_t, String> List;

        ElementTagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value, SetCallback _CB);
        ElementTagListUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value);
        bool getValue (JsonVariant _Value);
        bool setValue (JsonVariant _Value);
        String createSetupTag ();
        void getTagObject (JsonObject &_Tag);
        void addTagValue (JsonObject &_Values);
    };
  }
}

#endif