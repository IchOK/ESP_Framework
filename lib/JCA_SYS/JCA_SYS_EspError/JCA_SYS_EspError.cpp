/**
 * @file JCA_SYS_EspError.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Check and DebugPrint the ESP-Error-Type
 * @version 1.0
 * @date 2024-05-26
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_SYS_EspError.h>

namespace JCA {
  namespace SYS {
    
    bool EspErrorIsOK (esp_err_t _Code) {
      #ifdef ESP32
        return _Code == ESP_OK;
      #else
        return _Code == 0;
      #endif

    }
    void EspErrorDebugOut (esp_err_t _Code, JCA::SYS::DEBUGOUT_FLAGS _Flag) {
      #ifdef ESP32
        Debug.println (_Flag, true, "EspError", __func__, esp_err_to_name (_Code));
      #else
        Debug.println (_Flag, true, "EspError", __func__, "Error Code: " + String (_Code));
      #endif
    }
  }
}
