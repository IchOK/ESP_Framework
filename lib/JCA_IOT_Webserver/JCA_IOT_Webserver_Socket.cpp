/**
 * @file JCA_IOT_Webserver_Socket.cpp
 * @author JCA (https://github.com/ichok)
 * @brief WebSocket-Functions of the Webserver
 * @version 0.1
 * @date 2022-09-23
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Webserver.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    void Webserver::onWsData (JsonVariantCallback _CB) {
      wsDataCB = _CB;
    }
    void Webserver::onWsUpdate (JsonVariantCallback _CB) {
      wsUpdateCB = _CB;
    }
    void Webserver::setWsUpdateCycle (uint32_t _CycleTime) {
      WsUpdateCycle = _CycleTime;
    }
    bool Webserver::doWsUpdate () {
      return doWsUpdate (nullptr);
    }

    void Webserver::onWsEvent (AsyncWebSocket *_Server, AsyncWebSocketClient *_Client, AwsEventType _Type, void *_Arg, uint8_t *_Data, size_t _Len) {
      if (_Type == WS_EVT_CONNECT) {
        doWsUpdate (_Client);
      } else if (_Type == WS_EVT_DATA) {
        wsHandleData (_Client, _Arg, _Data, _Len);
      }
    }

    void Webserver::wsHandleData (AsyncWebSocketClient *_Client, void *_Arg, uint8_t *_Data, size_t _Len) {
      AwsFrameInfo *Info = (AwsFrameInfo *)_Arg;
      if (Info->opcode == WS_TEXT) {
        // Initialise Message-Buffer on first Frame
        if (Info->index == 0) {
          _Client->_tempObject = malloc (Info->len);
        }
        // Append Data to Message-Buffer
        memcpy ((uint8_t *)(_Client->_tempObject) + Info->index, _Data, _Len);
        // Handle Message if last Frame ist received
        if (Info->final && Info->index + _Len == Info->len) {

          DynamicJsonDocument JBuffer (1000);
          JsonVariant InData;
          JsonVariant OutData;

          DeserializationError Error = deserializeJson (JBuffer, (char *)(_Client->_tempObject));
          if (Error) {
            if (Debug.print (FLAG_ERROR, true, ObjectName, __func__, "+ deserializeJson() failed: ")) {
              Debug.println (FLAG_ERROR, true, ObjectName, __func__, Error.c_str ());
              Debug.print (FLAG_ERROR, true, ObjectName, __func__, "+ Body:");
              Debug.println (FLAG_ERROR, true, ObjectName, __func__, (char *)(_Client->_tempObject));
            }
            JBuffer.clear ();
          }

          InData = JBuffer.as<JsonVariant> ();

          // Handle System-Data
          recvSystemMsg (InData);

          if (wsDataCB) {
            OutData = wsDataCB (InData);
          } else if (restApiPostCB) {
            OutData = restApiPostCB (InData);
          }
          if (!OutData.is<JsonObject> ()) {
            JsonDoc.clear ();
            OutData = JsonDoc.to<JsonVariant> ();
            Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "No Answer defined");
          }

          // Add System Informations
          createSystemMsg (OutData);
          JsonDoc = OutData.as<JsonObject> ();

          String Response;
          serializeJson (JsonDoc, Response);
          Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, Response);
          if (_Client->canSend ()) {
            _Client->text (Response);
          }
        }
      }
    }

    bool Webserver::doWsUpdate (AsyncWebSocketClient *_Client) {
      JsonVariant OutData;
      JsonVariant InData;

      if (_Client != nullptr) {
        // check if selected Client can send Data
        if (!_Client->canSend ()) {
          return false;
        }
      } else {
        // check if selected Client can send Data
        if (Websocket.count() == 0){
          return false;
        }
      }

      if (wsUpdateCB) {
        OutData = wsUpdateCB (InData);
      } else if (restApiGetCB) {
        OutData = restApiGetCB (InData);
      }
      if (!OutData.is<JsonObject> ()) {
        JsonDoc.clear ();
        OutData = JsonDoc.to<JsonVariant> ();
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "No Answer defined");
      }

      // Add System Informations
      createSystemMsg (OutData);
      JsonDoc = OutData.as<JsonObject> ();

      String Response;
      serializeJson (JsonDoc, Response);
      Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, Response);
      if (_Client != nullptr) {
        _Client->text (Response);
      } else {
        Websocket.textAll (Response);
      }
      return true;
    }
  }
}
