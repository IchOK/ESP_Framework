/**
 * @file JCA_LNK_LinkAdd.h
 * @author JCA (https://github.com/ichok)
 * @brief Add link type definition
 * @version 1.0
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef _JCA_LNK_LINKADD_
#define _JCA_LNK_LINKADD_

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
     * @brief Add Link Type
     * 
     * Add link: The value of the Input is added to the Output
     */
    class LinkAdd {
    public:
      /**
       * @brief Add Add link type to FuncHandler
       * 
       * @param _Handler Function Handler to register link type to
       */
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      
      /**
       * @brief Get setup schema for Add link type
       * 
       * @param _Schema JSON object to fill with schema information
       */
      static void GetSetupSchema(JsonObject &_Schema);
      
      /**
       * @brief Update function for Add link type
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
