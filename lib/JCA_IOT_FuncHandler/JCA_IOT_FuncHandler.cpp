#include <JCA_IOT_FuncHandler.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace IOT {
    FuncHandler::FuncHandler (String _SetupFilePath) {
      SetupFilePath = _SetupFilePath;
    }

    void FuncHandler::update (struct tm &_Time) {

    }
  }
}