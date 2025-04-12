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

#include <JCA_FNC_ServerLink.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *ServerLink::ClassName = "ServerLink";
    const char *ServerLink::SetupTagType = "server";
    const char *ServerLink::SetupTagRefName = "server";

    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    ServerLink::ServerLink (JCA::IOT::Server *_ServerRef, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagString ("Hostname", "Hostname", "Reboot erforderlich", false, TagUsage_T::UseConfig, &Hostname, std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagUInt32 ("WsUpdateCycle", "Websocket Updatezyklus", "", false, TagUsage_T::UseConfig, &WsUpdateCycle, "ms", std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagUInt16 ("WebServerPort", "Webserver Port", "Reboot erforderlich", false, TagUsage_T::UseConfig, &WebServerPort, "", std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagUInt16 ("UdpListenerPort", "UDP Port", "Reboot erforderlich", false, TagUsage_T::UseConfig, &UdpListenerPort, "", std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagUInt32 ("LocalTimeZone", "Zeitzone", "", false, TagUsage_T::UseConfig, &LocalTimeZone, "s", std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagBool ("DaylightSavingTime", "Sommerzeit", "", false, TagUsage_T::UseConfig, &DaylightSavingTime, "EIN", "AUS", std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagUInt16 ("RebootCounter", "Reboot-Counter", "", false, TagUsage_T::UseConfig, &RebootCounter, "", std::bind (&ServerLink::setServerDataCB, this)));
      Tags.push_back (new TagBool ("SaveConfig", "Konfiguration speichern", "", false, TagUsage_T::UseConfig, &SaveConfig, "Saving", "Save", std::bind (&ServerLink::writeServerConfigCB, this)));
      Tags.push_back (new TagUInt32 ("TimeSync", "Zeit-Sync (01.01.1970)", "", false, TagUsage_T::UseConfig, &TimeSync, "",std::bind (&ServerLink::setTimeCB, this), TagTypes_T::TypeDateTime));
      Tags.push_back (new TagString ("UtcTime", "UTC-Zeit", "", true, TagUsage_T::UseConfig, &SystemTime));

      Tags.push_back (new TagString ("LocalTime", "Lolak-Zeit", "", true, TagUsage_T::UseData, &LocalTime));

      // Init Data
      ServerRef = _ServerRef;
      getServerDataCB ();
      SaveConfig = false;
      TimeSync = 0;
    }

    void ServerLink::setTimeCB () {
      ServerRef->setTime (TimeSync);
      TimeSync = 0;
    }

    void ServerLink::writeServerConfigCB () {
      ServerRef->writeSystemConfig ();
      SaveConfig = false;
    }

    void ServerLink::setServerDataCB () {
      ServerRef->Hostname = Hostname;
      ServerRef->WsUpdateCycle = WsUpdateCycle;
      ServerRef->WebServerPort = WebServerPort;
      ServerRef->UdpListenerPort = UdpListenerPort;
      ServerRef->LocalTimeZone = LocalTimeZone;
      ServerRef->DaylightSavingTime = DaylightSavingTime;
      ServerRef->RebootCounter = RebootCounter;
    }

    void ServerLink::getServerDataCB () {
      Hostname = ServerRef->Hostname;
      WsUpdateCycle = ServerRef->WsUpdateCycle;
      WebServerPort = ServerRef->WebServerPort;
      UdpListenerPort = ServerRef->UdpListenerPort;
      LocalTimeZone = ServerRef->LocalTimeZone;
      DaylightSavingTime = ServerRef->DaylightSavingTime;
      RebootCounter = ServerRef->RebootCounter;
      SystemTime = ServerRef->getTimeFormated ("", false);
      LocalTime = ServerRef->getTimeFormated ("", true);
    }

    /**
     * @brief Handling DigitalOut
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void ServerLink::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      getServerDataCB ();
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void ServerLink::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
      _Handler.FunctionList.insert (std::pair<String, std::function<bool (JsonObject, JsonObject, std::vector<JCA::FNC::FuncParent *> &, std::map<String, void *>)>> (SetupTagType, Create));
    }

    /**
     * @brief Create a new Instanz of the Class using the JSON-Configdata and add it to the Functions-List
     *
     * @param _Setup Object contains the creation data
     * @param _Log Logging-Object for Debug after creation
     * @param _Functions List of Function to add the Instanz to
     * @param _Hardware List of knowen Hardware-References
     * @return true
     * @return false
     */
    bool ServerLink::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log[SetupTagType].to<JsonObject>();

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, _Log);
      String ServerName;
      JCA::IOT::Server *ServerRef = static_cast<JCA::IOT::Server *> (GetSetupHardwareRef (SetupTagRefName, ServerName, Done, _Setup, _Log, _Hardware));

      if (Done) {
        _Functions.push_back (new ServerLink(ServerRef, Name));
        Log["done"] = Name + "(Server: " + ServerName + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
