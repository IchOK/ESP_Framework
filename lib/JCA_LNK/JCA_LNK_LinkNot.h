/**
 * @file JCA_LNK_LinkNot.h
 * @author JCA (https://github.com/ichok)
 * @brief Not link type definition
 * @version 1.0
 * @date 2026-05-16
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef _JCA_LNK_LINKNOT_
#define _JCA_LNK_LINKNOT_

#include "ArduinoJson/Variant/JsonVariant.hpp"
#include "JCA_IOT_FuncHandler.h"
#include <ArduinoJson.h>
#include "JCA_TAG_Parent.h"

namespace JCA {
  namespace LNK {
    /**
     * @brief Not Link Type
     *
     * Logical NOT of one bool input; result is written to all bool outputs.
     */
    class LinkNot {
    public:
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      static void GetSetupSchema(JsonObject &_Schema);
      static bool Update(JsonArray _Inputs, JsonVariant &_Output);
    };
  }
}

#endif
