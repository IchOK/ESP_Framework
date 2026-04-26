/**
 * @file JCA_LNK_LinkMove.h
 * @author JCA (https://github.com/ichok)
 * @brief Move link type definition
 * @version 1.0
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef _JCA_LNK_LINKMOVE_
#define _JCA_LNK_LINKMOVE_

#include "ArduinoJson/Variant/JsonVariant.hpp"
#include "JCA_IOT_FuncHandler.h"
#include <ArduinoJson.h>
#include "JCA_TAG_Parent.h"

// Forward declaration
namespace JCA {
  namespace IOT {
    class FuncHandler;
  }
  namespace FNC {
    class FuncParent;
  }
}

namespace JCA {
  namespace LNK {
    /**
     * @brief Move Link Type
     * 
     * Move link: Based on the first From-Tag, the value of the second From-Tag is forwarded to To-Tags
     */
    class LinkMove {
    public:
      /**
       * @brief Add Move link type to FuncHandler
       * 
       * @param _Handler Function Handler to register link type to
       */
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      
      /**
       * @brief Get setup schema for Move link type
       * 
       * @param _Schema JSON object to fill with schema information
       */
      static void GetSetupSchema(JsonObject &_Schema);
      
      /**
       * @brief Update function for Move link type
       * 
       * @param _Link Pointer to the link to update
       * @param _Functions Reference to the functions vector
       * @param _LinkDoc Reference to JsonDocument for temporary values
       */
      static bool Update(JsonArray _Inputs, JsonVariant &_Output);
    };
  }
}

#endif
