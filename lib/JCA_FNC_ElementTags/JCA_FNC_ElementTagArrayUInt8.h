/**
 * @file JCA_FNC_ElementTagArrayUInt8.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Tag-Classes to create an Element
 * @version 1.0
 * @date 2024-04-14
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_FNC_ELEMENTTAGARRAYUINT8_
#define _JCA_FNC_ELEMENTTAGARRAYUINT8_

#include <JCA_FNC_ElementTags.h>

namespace JCA {
  namespace FNC {
    class ElementTagArrayUInt8 : public ElementTag {
      public:
        // Type Informations
        uint8_t Length;
        
        ElementTagArrayUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value, uint8_t _Length, SetCallback _CB);
        ElementTagArrayUInt8 (String _Name, String _Text, String _Comment, bool _ReadOnly, ElementTagUsage_T _Usage, uint8_t *_Value, uint8_t _Length);
        bool getValue (JsonVariant _Value);
        bool setValue(JsonVariant _Value);
        String createSetupTag ();
        void getTagObject (JsonObject &_Tag);
        void addTagValue (JsonObject &_Values);
    };
  }
}

#endif