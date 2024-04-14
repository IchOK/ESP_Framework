/**
 * @file JCA_SYS_Conversion.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of Conversion Functions
 * @version 1.0
 * @date 2024-04-14
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_SYS_CONVERSION_
#define _JCA_SYS_CONVERSION_

#include <Arduino.h>

namespace JCA {
  namespace SYS {
    bool HexStringToByteArray (String _HexString, uint8_t *_ByteArray, uint8_t _Length);
    uint8_t HexCharToInt (char _HexChar);
    String ByteArrayToHexString (uint8_t *_ByteArray, uint8_t _Length);
  }
}

#endif