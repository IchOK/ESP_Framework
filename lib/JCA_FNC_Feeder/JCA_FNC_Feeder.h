
#ifndef _JCA_FNC_FEEDER_
#define _JCA_FNC_FEEDER_

#include <AccelStepper.h>
#include <ArduinoJson.h>
#include <time.h>

#include <JCA_SYS_DebugOut.h>
#include <JCA_FNC_Parent.h>

namespace JCA {
  namespace FNC {
    class Feeder : public Protocol{
    private:
      // Protocol Datapoint description
      static const char *FeedingHour_Name;
      static const char *FeedingHour_Text;
      static const char *FeedingHour_Unit;
      static const char *FeedingHour_Comment;
      static const char *FeedingMinute_Name;
      static const char *FeedingMinute_Text;
      static const char *FeedingMinute_Unit;
      static const char *FeedingMinute_Comment;
      static const char *SteppsPerRotation_Name;
      static const char *SteppsPerRotation_Text;
      static const char *SteppsPerRotation_Unit;
      static const char *SteppsPerRotation_Comment;
      static const char *FeedingRotations_Name;
      static const char *FeedingRotations_Text;
      static const char *FeedingRotaions_Unit;
      static const char *FeedingRotaions_Comment;
      static const char *Acceleration_Name;
      static const char *Acceleration_Text;
      static const char *Acceleration_Unit;
      static const char *Acceleration_Comment;
      static const char *MaxSpeed_Name;
      static const char *MaxSpeed_Text;
      static const char *MaxSpeed_Unit;
      static const char *MaxSpeed_Comment;
      static const char *ConstSpeed_Name;
      static const char *ConstSpeed_Text;
      static const char *ConstSpeed_Unit;
      static const char *ConstSpeed_Comment;
      static const char *Feeding_Name;
      static const char *Feeding_Text;
      static const char *Feeding_Comment;
      static const char *Feeding_TextOn;
      static const char *Feeding_TextOff;
      static const char *DistanceToGo_Name;
      static const char *DistanceToGo_Text;
      static const char *DistanceToGo_Unit;
      static const char *DistanceToGo_Comment;
      static const char *RunConst_Name;
      static const char *RunConst_Text;
      static const char *RunConst_Comment;
      static const char *RunConst_TextOn;
      static const char *RunConst_TextOff;
      static const char *Speed_Name;
      static const char *Speed_Text;
      static const char *Speed_Unit;
      static const char *Speed_Comment;
      static const char *CmdDoFeed_Name;
      static const char *CmdDoFeed_Text;
      static const char *CmdDoFeed_Comment;
      static const char *CmdDoFeed_Type;
      static const char *CmdDoFeed_BtnText;
      
      // Protocol Functions
      void setConfig (JsonArray _Tags);
      void setData (JsonArray _Tags);
      void setCmd (JsonArray _Tags);
      void createConfigTags (JsonArray &_Tags);
      void createDataTags (JsonArray &_Tags);
      void createCmdInfoTags (JsonArray &_Tags);

      // Hardware
      AccelStepper Stepper;

      // Konfig
      int16_t FeedingHour;
      int16_t FeedingMinute;
      float SteppsPerRotation;
      float FeedingRotations;
      float Acceleration;
      float MaxSpeed;
      float ConstSpeed;

      // Daten
      bool RunConst;
      bool Feeding;

      // Intern
      bool DoFeed;
      bool AutoFeedDone;

    public:
      Feeder (uint8_t _PinEnable, uint8_t _PinStep, uint8_t _PinDir, const char *_Name);
      void update (struct tm &_Time);
    };
  }
}

#endif