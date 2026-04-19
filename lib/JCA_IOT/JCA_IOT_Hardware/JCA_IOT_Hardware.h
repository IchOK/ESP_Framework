/**
 * @file JCA_IOT_Hardware.h
 * @author JCA (https://github.com/ichok)
 * @brief Collection of creation methods for Hardware-Interfaces, the creation methos has to be added to the HardwareList in Main.cpp
 * @version 1.0
 * @date 2025-07-23
 *
 * Copyright Jochen Cabrera 2025
 * Apache License
 *
 */

#ifndef _JCA_IOT_HARDWARE_
#define _JCA_IOT_HARDWARE_

#include <ArduinoJson.h>
#include <JCA_IOT_FuncHandler.h>
#include <JCA_SYS_DebugOut.h>
#include <JCA_SYS_PwmOutput.h>
#include <map>
#include <vector>

#include <OneWire.h>
#include <Wire.h>

namespace JCA {
  namespace IOT {
    // OneWire Interface
    void AddOneWire (FuncHandler &_Handler);
    bool CreateOneWire (JsonObject _Setup, JsonObject _Log, std::map<String, void *> &HardwareMapping);
    void GetOneWireSchema (JsonObject &_Schema);

    // SYS_PwmOutput Interface
    void AddPwmOutput (FuncHandler &_Handler);
    bool CreatePwmOutput (JsonObject _Setup, JsonObject _Log, std::map<String, void *> &HardwareMapping);
    void GetPwmOutputSchema (JsonObject &_Schema);

    // TwoWire (I2C) Interface
    void AddTwoWire (FuncHandler &_Handler);
    bool CreateTwoWire (JsonObject _Setup, JsonObject _Log, std::map<String, void *> &HardwareMapping);
    void GetTwoWireSchema (JsonObject &_Schema);
  }
} // namespace JCA

#endif