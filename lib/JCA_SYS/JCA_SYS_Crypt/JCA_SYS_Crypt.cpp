#include "JCA_SYS_Crypt.h"

namespace JCA {
  namespace SYS {
    // Feste, zufällige Base64-Zeichentabelle
    const char *Crypt::base64Chars = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM1234567890+/";

    String Crypt::encode (const String &input) {
      String encoded = "";
      int val = 0;
      int valb = -6;

      for (uint8_t c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
          encoded += base64Chars[(val >> valb) & 0x3F];
          valb -= 6;
        }
      }

      if (valb > -6) {
        encoded += base64Chars[((val << 8) >> (valb + 8)) & 0x3F];
      }

      while (encoded.length() % 4) {
        encoded += '=';
      }

      return encoded;
    }

    String Crypt::decode (const String &input) {
      int decodeTable[256] = {0};
      for (int i = 0; i < 64; i++) {
        decodeTable[(uint8_t)base64Chars[i]] = i;
      }

      String decoded = "";
      int val = 0;
      int valb = -8;

      for (uint8_t c : input) {
        if (c == '=') break;
        if (decodeTable[c] == 0 && c != base64Chars[0]) continue;
        val = (val << 6) + decodeTable[c];
        valb += 6;
        if (valb >= 0) {
          decoded += char((val >> valb) & 0xFF);
          valb -= 8;
        }
      }

      return decoded;
    }
  }
}