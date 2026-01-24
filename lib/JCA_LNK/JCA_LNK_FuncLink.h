/**
 * @file JCA_LNK_FuncLink.h
 * @author JCA (https://github.com/ichok)
 * @brief Handling class to create an handle links.
 * @version 1.1
 * @date 2026-01-24
 *
 * Copyright Jochen Cabrera 2026
 * Apache License
 *
 */

#ifndef _JCA_LNK_FUNCLINK_
#define _JCA_LNK_FUNCLINK_
 
 #include <ArduinoJson.h>
 #include <vector>
 
 #include <JCA_SYS_DebugOut.h>
 
 
namespace JCA {
  namespace LNK {
     struct FuncLinkPair_T{
       int16_t Func;
       int16_t Tag;
     };
     enum FuncLinkType_T : uint8_t {
       LinkNone = 0,
       LinkDirect = 1,
       LinkMove = 2
     };
     
     class FuncLink {
     private:
       std::vector<FuncLinkPair_T> Input;
       std::vector<FuncLinkPair_T> Output;
 
     public:
       FuncLinkType_T Type;
 
       FuncLink(FuncLinkType_T _Type);
       ~FuncLink();
       void addInput(FuncLinkPair_T _Input);
       void addOutput(FuncLinkPair_T _Output);
       FuncLinkPair_T getInput(uint8_t _Index);
       FuncLinkPair_T getOutput(uint8_t _Index);
       uint8_t getInputCount() { return Input.size(); };
       uint8_t getOutputCount() { return Output.size(); };
     };
 
   }
 }
 
 #endif