
#ifndef _JCA_FNC_DS18B20_
#define _JCA_FNC_DS18B20_

#include <ArduinoJson.h>
#include <OneWire.h>
#include <time.h>

#include <JCA_FNC_Parent.h>
#include <JCA_SYS_DebugOut.h>

#define JCA_FNC_DS18B20_TYPE_S 0x10
#define JCA_FNC_DS18B20_TYPE_B 0x28
#define JCA_FNC_DS18B20_TYPE_22 0x22

#define JCA_FNC_DS18B20_CMD_CONV 0x44
#define JCA_FNC_DS18B20_CMD_READ 0xBE
#define JCA_FNC_DS18B20_CMD_WRITE 0x4E
#define JCA_FNC_DS18B20_CMD_COPY 0x48
#define JCA_FNC_DS18B20_CMD_RECALL 0xB8
#define JCA_FNC_DS18B20_CMD_POWER 0xB4

namespace JCA {
  namespace FNC {
    class DS18B20 : public Protocol{
    private:
      // Datapoint description
      static const char *Filter_Name;
      static const char *Filter_Text;
      static const char *Filter_Unit;
      static const char *Filter_Comment;
      static const char *Addr_Name;
      static const char *Addr_Text;
      static const char *Addr_Unit;
      static const char *Addr_Comment;
      static const char *ReadInterval_Name;
      static const char *ReadInterval_Text;
      static const char *ReadInterval_Unit;
      static const char *ReadInterval_Comment;
      static const char *Temp_Name;
      static const char *Temp_Text;
      static const char *Temp_Unit;
      static const char *Temp_Comment;

      // Protocol Functions
      void createConfigValues (JsonObject &_Values);
      void createDataValues (JsonObject &_Values);
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);

      void writeSetupConfig (File _SetupFile);
      void writeSetupData (File _SetupFile);
      void writeSetupCmdInfo (File _SetupFile);

      // Hardware
      OneWire* Wire;

      // Konfig
      float Filter;
      uint8_t Addr[8];
      uint16_t ReadInterval;

      // Daten
      float Value;

      // Intern
      uint8_t Raw[12];
      int32_t Resend;
      bool ReadData;
      uint32_t LastMillis;

      void HexStringToByteArray (String _HexString, uint8_t *_ByteArray, uint8_t _Length);
      uint8_t HexCharToInt(char _HexChar);
      String ByteArrayToHexString (uint8_t *_ByteArray, uint8_t _Length);

    public:
      DS18B20 (OneWire *_Wire, const char *_Name);
      void update (struct tm &_Time);
      float getValue();
    };
  }
}

#endif