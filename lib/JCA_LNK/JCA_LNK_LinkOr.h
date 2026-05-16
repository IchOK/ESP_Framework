/**
 * @file JCA_LNK_LinkOr.h
 * @author JCA (https://github.com/ichok)
 * @brief Or link type definition
 * @version 1.0
 * @date 2026-05-16
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef _JCA_LNK_LINKOR_
#define _JCA_LNK_LINKOR_

#include "ArduinoJson/Variant/JsonVariant.hpp"
#include "JCA_IOT_FuncHandler.h"
#include <ArduinoJson.h>
#include "JCA_TAG_Parent.h"

namespace JCA {
  namespace LNK {
    /**
     * @brief Or Link Type
     *
     * Logical OR of all bool inputs; result is written to all bool outputs.
     */
    class LinkOr {
    public:
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      static void GetSetupSchema(JsonObject &_Schema);
      static bool Update(JsonArray _Inputs, JsonVariant &_Output);
    };
  }
}

#endif
