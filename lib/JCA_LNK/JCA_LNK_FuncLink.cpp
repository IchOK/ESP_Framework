#include <JCA_LNK_FuncLink.h>

namespace JCA {
  namespace LNK {
    FuncLink::FuncLink(FuncLinkType_T _Type) {
        Input = std::vector<FuncLinkPair_T>();
        Output = std::vector<FuncLinkPair_T>();
        Type = _Type;
      }
  
      FuncLink::~FuncLink() {
        Input.clear();
        Output.clear();
      }
  
      void FuncLink::addInput(FuncLinkPair_T _Input) {
        Input.push_back(_Input);
      }
  
      void FuncLink::addOutput(FuncLinkPair_T _Output) {
        Output.push_back(_Output);
      }
  
      FuncLinkPair_T FuncLink::getInput(uint8_t _Index) {
        return Input[_Index];
      }
  
      FuncLinkPair_T FuncLink::getOutput(uint8_t _Index) {
        return Output[_Index];
      }
  
    }
}