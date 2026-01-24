/**
 * @file JCA_LNK_LinkDirect.h
 * @author JCA (https://github.com/ichok)
 * @brief Direct link type definition
 * @version 1.0
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef _JCA_LNK_LINKDIRECT_
#define _JCA_LNK_LINKDIRECT_

#include <JCA_LNK_FuncLink.h>
#include <ArduinoJson.h>
#include <JCA_TAG_Parent.h>
#include <vector>

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
     * @brief Direct Link Type
     * 
     * Direct link: Value from one From-Tag is forwarded to all To-Tags
     */
    class LinkDirect {
    public:
      /**
       * @brief Add Direct link type to FuncHandler
       * 
       * @param _Handler Function Handler to register link type to
       */
      static void AddToHandler(JCA::IOT::FuncHandler &_Handler);
      
      /**
       * @brief Get setup schema for Direct link type
       * 
       * @param _Schema JSON object to fill with schema information
       */
      static void GetSetupSchema(JsonObject &_Schema);
      
      /**
       * @brief Update function for Direct link type
       * 
       * @param _Link Pointer to the link to update
       * @param _Functions Reference to the functions vector
       * @param _LinkDoc Reference to JsonDocument for temporary values
       */
      static void Update(JCA::LNK::FuncLink *_Link, std::vector<JCA::FNC::FuncParent *> &_Functions, JsonDocument &_LinkDoc);
    };
  }
}

#endif
