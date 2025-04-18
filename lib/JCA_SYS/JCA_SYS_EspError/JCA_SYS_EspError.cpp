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
    #ifdef ESP32
      bool EspErrorIsOK (esp_err_t _Code) {
        return _Code == ESP_OK;
      }

      void EspErrorDebugOut (esp_err_t _Code, JCA::SYS::DEBUGOUT_FLAGS _Flag) {
        Debug.println (_Flag, true, "EspError", __func__, esp_err_to_name (_Code));
      }
    #else
    
      bool EspErrorIsOK (int _Code) {
        return _Code == 0;
      }
      void EspErrorDebugOut (int _Code, JCA::SYS::DEBUGOUT_FLAGS _Flag) {
        Debug.println (_Flag, true, "EspError", __func__, "Error Code: " + String (_Code));
      }
    #endif
  }
}
