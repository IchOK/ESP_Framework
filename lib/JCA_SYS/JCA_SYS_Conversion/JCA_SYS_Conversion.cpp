/**
 * @file JCA_FNC_Parent.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Parent Class of all Framework Elements.
 * @version 1.0
 * @date 2022-12-10
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_SYS_Conversion.h>

namespace JCA {
  namespace SYS {

    /**
     * @brief Convert Hex-String to Byte-Array
     *
     * @param _HexString String with HEX-Values without Format-Prefix
     * @param _ByteArray Pointer to the Byte-Array
     * @param _Length Length of the Byte Array
     * @return true String length matchs Array-Length
     * @return false String length didn't match
     */
    bool HexStringToByteArray (String _HexString, uint8_t *_ByteArray, uint8_t _Length) {
      if (_HexString.length () == _Length * 2) {
        for (size_t i = 0; i < _Length; i++) {
          _ByteArray[i] = HexCharToInt (_HexString.charAt (i * 2 + 1)) + HexCharToInt (_HexString.charAt (i * 2)) * 16;
        }
        return true;
      } else {
        return false;
      }
    }

    /**
     * @brief Convert a single HEX-Char to Integer
     *
     * @param _HexChar HEX-Value as Char
     * @return uint8_t Integer-Value
     */
    uint8_t HexCharToInt (char _HexChar) {
      unsigned Result = -1;
      if (('0' <= _HexChar) && (_HexChar <= '9')) {
        Result = _HexChar - '0';
      } else if (('A' <= _HexChar) && (_HexChar <= 'F')) {
        Result = 10 + _HexChar - 'A';
      } else if (('a' <= _HexChar) && (_HexChar <= 'f')) {
        Result = 10 + _HexChar - 'a';
      }
      return Result;
    }

    /**
     * @brief Convert Byte-Array to Hex-String
     *
     * @param _ByteArray Pointer to the Byte-Array
     * @param _Length Length of the Byte Array
     * @return String HEX-Decodet
     */
    String ByteArrayToHexString (uint8_t *_ByteArray, uint8_t _Length) {
      String Result = "";
      for (size_t i = 0; i < _Length; i++) {
        if (_ByteArray[i] < 16) {
          Result += "0";
        }
        Result += String (_ByteArray[i], HEX);
      }
      return Result;
    }
  }
}

