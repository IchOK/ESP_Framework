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

#include <JCA_FNC_WebserverLink.h>
using namespace JCA::SYS;
using namespace JCA::TAG;

namespace JCA {
  namespace FNC {
    const char *WebserverLink::ClassName = "WebserverLink";
    const char *WebserverLink::SetupTagType = "webserver";
    const char *WebserverLink::SetupTagRefName = "WebServer";

    /**
     * @brief Construct a new DigitalOut::DigitalOut object
     *
     * @param _Pin Analog Pin conected to the DigitalOut-Sensor
     * @param _Name Element Name inside the Communication
     */
    WebserverLink::WebserverLink (JCA::IOT::Webserver *_ServerRef, String _Name)
        : FuncParent (_Name) {
      Debug.println (FLAG_SETUP, false, Name, __func__, "Create");
      // Create Tag-List
      Tags.push_back (new TagString ("Hostname", "Hostname", "Hostname wirde erst nache dem Reboot aktiv", false, TagUsage_T::UseConfig, &Hostname));
      Tags.push_back (new TagUInt32 ("WsUpdateCycle", "Websocket Updatezyklus", "", false, TagUsage_T::UseConfig, &WsUpdateCycle, "ms"));
      Tags.push_back (new TagUInt32 ("TimeSync", "Websocket Updatezyklus", "", false, TagUsage_T::UseConfig, &TimeSync, "s", std::bind (&WebserverLink::setTimeCB, this)));

      Tags.push_back (new TagString ("Time", "Systemzeit", "", false, TagUsage_T::UseData, &ActTime));

      // Init Data
      ServerRef = _ServerRef;
      Hostname = ServerRef->Hostname;
      WsUpdateCycle = ServerRef->WsUpdateCycle;
      TimeSync = 0;
      ActTime = ServerRef->getTime ();
    }

    void WebserverLink::setTimeCB () {
      ServerRef->setTime (TimeSync);
      TimeSync = 0;
    }

    /**
     * @brief Handling DigitalOut
     * Write the digital Output-Pin and check the AutoOff Delay
     * @param time Current Time to check the Samplerate
     */
    void WebserverLink::update (struct tm &time) {
      Debug.println (FLAG_LOOP, false, Name, __func__, "Run");
      ServerRef->Hostname = Hostname;
      ServerRef->WsUpdateCycle = WsUpdateCycle;
      ActTime = ServerRef->getTime ();
    }

    /**
     * @brief Adds the creation method to the Function-Handler
     *
     * @param _Handler Function Handler
     */
    void WebserverLink::AddToHandler (JCA::IOT::FuncHandler &_Handler) {
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
    bool WebserverLink::Create (JsonObject _Setup, JsonObject _Log, std::vector<FuncParent *> &_Functions, std::map<String, void *> _Hardware) {
      Debug.println (FLAG_SETUP, true, ClassName, __func__, "Start");
      bool Done = true;
      JsonObject Log = _Log.createNestedObject (SetupTagType);

      String Name = GetSetupValueString (JCA_IOT_FUNCHANDLER_SETUP_NAME, Done, _Setup, _Log);
      String WebserverName;
      JCA::IOT::Webserver *ServerRef = static_cast<JCA::IOT::Webserver *> (GetSetupHardwareRef (SetupTagRefName, WebserverName, Done, _Setup, _Log, _Hardware));

      if (Done) {
        _Functions.push_back (new WebserverLink(ServerRef, Name));
        Log["done"] = Name + "(Webserver: " + WebserverName + ")";
        Debug.println (FLAG_SETUP, true, ClassName, __func__, "Done");
      }
      return Done;
    }
  }
}
