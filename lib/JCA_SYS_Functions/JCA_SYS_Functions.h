/**
 * @file JCA_SYS_Functions.h
 * @author JCA (https://github.com/ichok)
 * @brief Contains all standard functiontypes of the Framework
 * @version 0.1
 * @date 2023-07-31
 * 
 * Copyright Jochen Cabrera 2023
 * Apache License
 * 
 */

#ifndef _JCA_SYS_FUNCTIONS_
#define _JCA_SYS_FUNCTIONS_

#include <Arduino.h>
#include <ArduinoJson.h>

namespace JCA {
  namespace SYS {
    typedef std::function<float (void)> ValueCallback;
    typedef std::function<bool (void)> StateCallback;
    typedef std::function<void (float)> SetpointCallback;
    typedef std::function<void (bool)> CommandCallback;
    typedef std::function<void (JsonVariant &_In, JsonVariant &_Out)> JsonVariantCallback;
    typedef std::function<void (void)> SimpleCallback;
  }
}

#endif