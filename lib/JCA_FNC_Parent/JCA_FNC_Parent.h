
#ifndef _JCA_FNC_PARENT_
#define _JCA_FNC_PARENT_

#include <ArduinoJson.h>

#include <JCA_SYS_DebugOut.h>
#include <JCA_IOT_Protocol.h>

namespace JCA {
  namespace FNC {
    class Parent : public JCA::IOT::Protocol{
    protected:
      char ObjectName[80];
      // Intern
      virtual void setConfig (JsonObject _Data) = 0;
      virtual void setData (JsonObject _Data) = 0;
      virtual void createConfig (JsonObject &_Data) = 0;
      virtual void createData (JsonObject &_Data) = 0;

    public:
      Parent (const char *_Name);
      virtual void update (struct tm &_Time) = 0;
      void set (JsonObject _Collection);
      void getConfig (JsonObject &_Collection);
      void getData (JsonObject &_Collection);
      void getAll (JsonObject &_Collection);
    };
  }
}

#endif