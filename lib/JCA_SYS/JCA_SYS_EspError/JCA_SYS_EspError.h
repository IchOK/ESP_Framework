/**
 * @file JCA_SYS_EspError.h
 * @author JCA (https://github.com/ichok)
 * @brief Check and DebugPrint the ESP-Error-Type
 * @version 1.0
 * @date 2024-05-26
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_SYS_ESPERROR_
#define _JCA_SYS_ESPERROR_

#ifdef ESP32
  #include <esp_err.h>
#endif
#include <JCA_SYS_DebugOut.h>

namespace JCA {
  namespace SYS {
    bool EspErrorIsOK (int _Code);
    void EspErrorDebugOut (int _Code, JCA::SYS::DEBUGOUT_FLAGS _Flag);
  }
}

#endif