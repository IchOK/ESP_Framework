/**
 * @file JCA_IOT_Webserver_Web.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Website-Functions of the Webserver
 * @version 0.1
 * @date 2022-09-07
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Webserver.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {

    void Webserver::onWebHomeReplace (AwsTemplateProcessor _CB) {
      replaceHomeWildcardsCB = _CB;
    }

    void Webserver::onWebConfigReplace (AwsTemplateProcessor _CB) {
      replaceConfigWildcardsCB = _CB;
    }

    /**
     * @brief Handle the POST-Request on the Connection-Site
     *
     * @param _Request Request data from Web-Client
     */
    void Webserver::onWebConnectPost (AsyncWebServerRequest *_Request) {
      DynamicJsonDocument JsonDoc (1000);
      JsonObject Config;
      JsonObject WiFiConfig;

      File ConfigFile = LittleFS.open (JCA_IOT_WEBSERVER_CONFIGPATH, "r");
      if (ConfigFile) {
        Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config File Found");
        DeserializationError Error = deserializeJson (JsonDoc, ConfigFile);
        if (!Error) {
          Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Deserialize Done");
          Config = JsonDoc.as<JsonObject> ();
          if (Config.containsKey (JCA_IOT_WEBSERVER_CONFKEY_WIFI)) {
            Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config Node Found");
            WiFiConfig = Config[JCA_IOT_WEBSERVER_CONFKEY_WIFI].as<JsonObject> ();
          } else {
            Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "Config Node Created");
            WiFiConfig = JsonDoc.createNestedObject (JCA_IOT_WEBSERVER_CONFKEY_WIFI);
          }
        } else {
          Debug.print (FLAG_ERROR, true, ObjectName, __func__, "deserializeJson() failed: ");
          Debug.println (FLAG_ERROR, true, ObjectName, __func__, Error.c_str ());
          Debug.println (FLAG_ERROR, true, ObjectName, __func__, "Create new Konfig");
          JsonDoc.clear ();
          WiFiConfig = Config.createNestedObject (JCA_IOT_WEBSERVER_CONFKEY_WIFI);
        }
        ConfigFile.close ();
      } else {
        Debug.println (FLAG_ERROR, true, ObjectName, __func__, "Config File NOT found");
        Debug.println (FLAG_ERROR, true, ObjectName, __func__, "Create new Konfig");
        JsonDoc.clear ();
        WiFiConfig = JsonDoc.createNestedObject (JCA_IOT_WEBSERVER_CONFKEY_WIFI);
      }

      // Write Data to Config-Object
      int params = _Request->params ();
      for (int i = 0; i < params; i++) {
        AsyncWebParameter *p = _Request->getParam (i);
        if (p->isPost ()) {
          if (p->name () == JCA_IOT_WEBSERVER_CONFKEY_WIFI_DHCP) {
            if (p->value () == "on") {
              WiFiConfig[p->name ()] = true;
            } else {
              WiFiConfig[p->name ()] = false;
            }
          } else {
            WiFiConfig[p->name ()] = p->value ().c_str ();
          }
        }
      }

      // Save Config Object
      ConfigFile = LittleFS.open (JCA_IOT_WEBSERVER_CONFIGPATH, "w");
      size_t WrittenBytes = serializeJson (JsonDoc, ConfigFile);
      ConfigFile.close ();
      Debug.print (FLAG_CONFIG, true, ObjectName, __func__, "Write Config File [");
      Debug.print (FLAG_CONFIG, true, ObjectName, __func__, WrittenBytes);
      Debug.println (FLAG_CONFIG, true, ObjectName, __func__, "]");

      // Read back Config-File
      readConfig ();

      // Connect and Answer to the Client
      if (Connector.doConnect ()) {
        _Request->send (200, "text/plain", "Connect to Network");
      } else {
        _Request->send (200, "text/plain", "Network Config invalid");
      }
    }

    /**
     * @brief Handle the GET-Request from the Connection-Site
     *
     * @param _Request Request data from Web-Client
     */
    void Webserver::onWebConnectGet (AsyncWebServerRequest *_Request) {
      if (!_Request->authenticate (ConfUser, ConfPassword)) {
        return _Request->requestAuthentication ();
      }
      _Request->send_P (200, "text/html", PageFrame, [this] (const String &_Var) -> String { return this->replaceConnectWildcards (_Var); });
    }

    /**
     * @brief Handle the GET-Request for the System-Site
     *
     * @param _Request Request data from Web-Client
     */
    void Webserver::onWebSystemGet (AsyncWebServerRequest *_Request) {
      if (!_Request->authenticate (ConfUser, ConfPassword)) {
        return _Request->requestAuthentication ();
      }
      _Request->send_P (200, "text/html", PageFrame, [this] (const String &_Var) -> String { return this->replaceSystemWildcards (_Var); });
    }

    /**
     * @brief Handle the File-Upload on the System-Site
     *
     * @param _Request Request data from Web-Client
     * @param _Filename Name of uploaded File
     * @param _Index Offset of the Data from the Datablock
     * @param _Data File data
     * @param _Len Length of the Datablock
     * @param _Final Last Datablock of the uploaded File
     */
    void Webserver::onWebSystemUploadData (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) {
      if (!_Request->authenticate (ConfUser, ConfPassword)) {
        return _Request->requestAuthentication ();
      }
      Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, String ("Client:" + _Request->client ()->remoteIP ().toString () + " " + _Request->url ()));
      if (!_Index) {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, String ("Upload Start: " + String (_Filename)));
        // open the file on first call and store the file handle in the request object
        _Request->_tempFile = LittleFS.open ("/" + _Filename, "w");
      }
      if (_Len) {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, String ("Writing file: " + String (_Filename) + " index=" + String (_Index) + " len=" + String (_Len)));
        // stream the incoming chunk to the opened file
        _Request->_tempFile.write (_Data, _Len);
      }
      if (_Final) {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, String ("Upload Complete: " + String (_Filename) + ",size: " + String (_Index + _Len)));
        // close the file handle as the upload is now done
        _Request->_tempFile.close ();
      }
    }

    /**
     * @brief Handle the Response to the Clinte after Firmware Update
     *
     * @param _Request Request data from Web-Client
     */
    void Webserver::onWebSystemUpdate (AsyncWebServerRequest *_Request) {
      if (!Update.hasError ()) {
        AsyncWebServerResponse *Response = _Request->beginResponse (301);
        Response->addHeader ("Location", JCA_IOT_WEBSERVER_PATH_SYS);
        Response->addHeader ("Retry-After", "60");
        _Request->send (Response);
        delay (100);
        onSystemResetCB ();
      }
    }

    /**
     * @brief Handle the Firmare-Update on the System-Site
     *
     * @param _Request Request data from Web-Client
     * @param _Filename Name of Firmware File
     * @param _Index Offset of the Data from the Datablock
     * @param _Data File data
     * @param _Len Length of the Datablock
     * @param _Final Last Datablock of the Firmware File
     */
    void Webserver::onWebSystemUpdateData (AsyncWebServerRequest *_Request, String _Filename, size_t _Index, uint8_t *_Data, size_t _Len, bool _Final) {
      if (!_Request->authenticate (ConfUser, ConfPassword)) {
        return _Request->requestAuthentication ();
      }
      if (!_Index) {
        Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "Update Start: ");
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, _Filename.c_str ());
        #ifdef ESP8266
        Update.runAsync (true);
        if (!Update.begin ((ESP.getFreeSketchSpace () - 0x1000) & 0xFFFFF000)) {
        #elif ESP32
        if (!Update.begin ()) {
        #endif
          if (Debug.print (FLAG_ERROR, true, ObjectName, __func__, "")) {
            Update.printError (Serial);
          }
        }
      }
      if (!Update.hasError ()) {
        if (Update.write (_Data, _Len) != _Len) {
          if (Debug.print (FLAG_ERROR, true, ObjectName, __func__, "")) {
            Update.printError (Serial);
          }
        }
      }
      if (_Final) {
        if (Update.end (true)) {
          Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "Update Success: ");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, _Index + _Len);
        } else {
          if (Debug.print (FLAG_ERROR, true, ObjectName, __func__, "")) {
            Update.printError (Serial);
          }
        }
      }
    }

    /**
     * @brief Initialize the Controller Reset
     *
     * @param _Request Request data from Web-Client
     */
    void Webserver::onWebSystemReset (AsyncWebServerRequest *_Request) {
      if (!_Request->authenticate (ConfUser, ConfPassword)) {
        return _Request->requestAuthentication ();
      }
      AsyncWebServerResponse *Response = _Request->beginResponse (301);
      Response->addHeader ("Location", JCA_IOT_WEBSERVER_PATH_SYS);
      Response->addHeader ("Retry-After", "60");
      _Request->send (Response);
      if (onSystemResetCB) {
        delay (100);
        onSystemResetCB ();
      }
    }

    /**
     * @brief
     *
     * @param _Request
     */
    void Webserver::onWebHomeGet (AsyncWebServerRequest *_Request) {
      if (LittleFS.exists (JCA_IOT_WEBSERVER_PATH_HOME)) {
        _Request->send (LittleFS, JCA_IOT_WEBSERVER_PATH_HOME, String (), false, [this] (const String &_Var) -> String { return this->replaceHomeWildcards (_Var); });
      } else {
        _Request->redirect (JCA_IOT_WEBSERVER_PATH_SYS);
      }
    }

    void Webserver::onWebConfigGet (AsyncWebServerRequest *_Request) {
      if (LittleFS.exists (JCA_IOT_WEBSERVER_PATH_CONFIG)) {
        _Request->send (LittleFS, JCA_IOT_WEBSERVER_PATH_CONFIG, String (), false, [this] (const String &_Var) -> String { return this->replaceConfigWildcards (_Var); });
      } else {
        _Request->redirect (JCA_IOT_WEBSERVER_PATH_SYS);
      }
    }

    /**
     * @brief Replace Default Wildcards in Websites
     *
     * @param var Wildcard
     * @return String Replace String
     */
    String Webserver::replaceDefaultWildcards (const String &var) {
      if (var == "TITLE") {
        return String (Hostname);
      }
      if (var == "SVG_LOGO") {
        return String (SvgLogo);
      }
      if (var == "SVG_HOME") {
        return String (SvgHome);
      }
      if (var == "SVG_CONFIG") {
        return String (SvgConfig);
      }
      if (var == "SVG_WIFI") {
        return String (SvgWiFi);
      }
      if (var == "SVG_SYSTEM") {
        return String (SvgSystem);
      }
      if (var == "CONF_FILE") {
        return WebConfigFile;
      }
      return String ();
    }

    /**
     * @brief Replace Wildcards of Home Site using the external Replace Callback Function
     *
     * @param var Wildcard
     * @return String Replace String
     */
    String Webserver::replaceHomeWildcards (const String &var) {
      String RetVal;
      if (replaceConfigWildcardsCB) {
        RetVal = replaceConfigWildcardsCB (var);
        if (!RetVal.isEmpty ()) {
          return RetVal;
        }
      }
      RetVal = replaceDefaultWildcards (var);
      if (!RetVal.isEmpty ()) {
        return RetVal;
      }
      return String ();
    }

    /**
     * @brief Replace Wildcards of Config Site using the external Replace Callback Function
     *
     * @param var Wildcard
     * @return String Replace String
     */
    String Webserver::replaceConfigWildcards (const String &var) {
      String RetVal;
      if (replaceConfigWildcardsCB) {
        RetVal = replaceConfigWildcardsCB (var);
        if (!RetVal.isEmpty ()) {
          return RetVal;
        }
      }
      RetVal = replaceDefaultWildcards (var);
      if (!RetVal.isEmpty ()) {
        return RetVal;
      }
      return String ();
    }

    /**
     * @brief Replace Wildcards of System Site
     *
     * @param var Wildcard
     * @return String Replace String
     */
    String Webserver::replaceSystemWildcards (const String &var) {
      String RetVal;
      RetVal = replaceDefaultWildcards (var);
      if (!RetVal.isEmpty ()) {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "Replace from Default Function");
        return RetVal;
      }
      if (var == "NAME") {
        return F ("System");
      }
      if (var == "STYLE") {
        return F (":root{--ColorSystem:var(--contrast)}");
      }
      if (var == "SECTION") {
        return String (SectionSys);
      }
      if (var == "FW_VERSION") {
        return String (AUTO_VERSION);
      }
      if (var == "BOARD_NAME") {
        return String (ARDUINO_BOARD);
      }
      if (var == "BOARD_VERSION") {
        return String ("");
      }
      if (var == "BOARD_VARIANT") {
        return String (BOARD_VARIANT);
      }
      if (var == "BOARD_MCU") {
        return String (BOARD_MCU);
      }
      if (var == "CONFIGFILE") {
        return String (JCA_IOT_WEBSERVER_CONFIGPATH);
      }
      return String ();
    }

    /**
     * @brief Replace Wildcards of Connect Site
     *
     * @param var Wildcard
     * @return String Replace String
     */
    String Webserver::replaceConnectWildcards (const String &var) {
      String RetVal;
      RetVal = Connector.replaceWildcards (var);
      if (!RetVal.isEmpty ()) {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "Replace from Connector Function");
        return RetVal;
      }
      RetVal = replaceDefaultWildcards (var);
      if (!RetVal.isEmpty ()) {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "Replace from Default Function");
        return RetVal;
      }
      if (var == "SECTION") {
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, var);
        return String (SectionConnect);
      }
      return String ();
    }
  }
}