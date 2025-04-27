#ifndef _JCA_SYS_CRYPT_
#define _JCA_SYS_CRYPT_

#include <Arduino.h>

namespace JCA {
  namespace SYS {
    class Crypt {
    private:
      // Feste, zufällige Base64-Zeichentabelle
      static const char *base64Chars;
    public:
      static String encode(const String &input);
      static String decode(const String &input);
    };
  }
}

#endif