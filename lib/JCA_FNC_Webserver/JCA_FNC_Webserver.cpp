/**
 * @file JCA_FNC_DigitalOut.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Framework Element get a DigitalOut by an analog Distancesensor. With Calibation an Alarmlevel
 * @version 1.0
 * @date 2022-11-06
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */

#include <JCA_FNC_Webserver.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    Webserver::Webserver (JCA::IOT::Webserver *_ServerRef, const char *_Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagString ("Hostname", "Hostname", "Hostname wirde erst nache dem Reboot aktiv", false, TagUsage_T::UseConfig, &Hostname));
      Tags.push_back (new TagUInt32 ("WsUpdateCycle", "Websocket Updatezyklus", "", false, TagUsage_T::UseConfig, &WsUpdateCycle, "ms"));
      Tags.push_back (new TagUInt32 ("TimeSync", "Websocket Updatezyklus", "", false, TagUsage_T::UseConfig, &TimeSync, "s", std::bind (&Webserver::setTimeCB, this)));

      Tags.push_back (new TagString ("Time", "Systemzeit", "", false, TagUsage_T::UseData, &ActTime));

      // Init Data
      ServerRef = _ServerRef;
      Hostname = ServerRef->Hostname;
      WsUpdateCycle = ServerRef->WsUpdateCycle;
      TimeSync = 0;
      ActTime = ServerRef->getTime ();
    }

    void Webserver::setTimeCB () {
      ServerRef->setTime (TimeSync);
      TimeSync = 0;
    }

    /**
     * @brief Handling DigitalOut
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void Webserver::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      ServerRef->Hostname = Hostname;
      ServerRef->WsUpdateCycle = WsUpdateCycle;
      ActTime = ServerRef->getTime ();
    }
  }
}
