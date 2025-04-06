/**
 * @file JCA_IOT_Webserver_Socket.cpp
 * @author JCA (https://github.com/ichok)
 * @brief WebSocket-Functions of the Server
 * @version 0.1
 * @date 2022-09-23
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Server.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    void Server::onWsData (JsonVariantCallback _CB) {
      wsDataCB = _CB;
    }
    void Server::onWsUpdate (JsonVariantCallback _CB) {
      wsUpdateCB = _CB;
    }
    void Server::setWsUpdateCycle (uint32_t _CycleTime) {
      WsUpdateCycle = _CycleTime;
    }
    bool Server::doWsUpdate () {
      return doWsUpdate (nullptr);
    }

    void Server::onWsEvent (AsyncWebSocket *_Server, AsyncWebSocketClient *_Client, AwsEventType _Type, void *_Arg, uint8_t *_Data, size_t _Len) {
      Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "Start");
      if (_Type == WS_EVT_CONNECT) {
        doWsUpdate (_Client);
      } else if (_Type == WS_EVT_DATA) {
        wsHandleData (_Client, _Arg, _Data, _Len);
      }
    }

    void Server::wsHandleData (AsyncWebSocketClient *_Client, void *_Arg, uint8_t *_Data, size_t _Len) {
      AwsFrameInfo *Info = (AwsFrameInfo *)_Arg;
      if (Info->opcode == WS_TEXT) {
        // Initialise Message-Buffer on first Frame
        if (Info->index == 0) {
          _Client->_tempObject = malloc (Info->len + 10);
          Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "+ MsgLen: ");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, Info->len);
        }
        // Append Data to Message-Buffer
        memcpy ((uint8_t *)(_Client->_tempObject) + Info->index, _Data, _Len);
        // Handle Message if last Frame ist received
        if (Info->final && Info->index + _Len == Info->len) {
          ((uint8_t *)(_Client->_tempObject))[Info->len] = 0;
          DynamicJsonDocument JsonInDoc (1000);
          DynamicJsonDocument JsonOutDoc (10000);
          JsonVariant InData;
          JsonVariant OutData = JsonOutDoc.as<JsonVariant> ();

          Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "+ Buffer: ");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, (char *)(_Client->_tempObject));

          DeserializationError Error = deserializeJson (JsonInDoc, (char *)(_Client->_tempObject));
          if (Error) {
            if (Debug.print (FLAG_ERROR, true, ObjectName, __func__, "+ deserializeJson() failed: ")) {
              Debug.println (FLAG_ERROR, true, ObjectName, __func__, Error.c_str ());
              Debug.print (FLAG_ERROR, true, ObjectName, __func__, "+ Body:");
              Debug.println (FLAG_ERROR, true, ObjectName, __func__, (char *)(_Client->_tempObject));
            }
            JsonInDoc.clear ();
          }

          InData = JsonInDoc.as<JsonVariant> ();

          // Call externak datahandling Functions
          if (wsDataCB) {
            wsDataCB (InData, OutData);
          } else if (restApiPostCB) {
            restApiPostCB (InData, OutData);
          }

          // Create Response
          String Response;
          serializeJson (OutData, Response);
          Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, Response);
          if (_Client->canSend ()) {
            _Client->text (Response);
          }
        }
      }
    }

    bool Server::doWsUpdate (AsyncWebSocketClient *_Client) {
      DynamicJsonDocument JsonDoc (10000);
      JsonVariant InData;
      JsonVariant OutData = JsonDoc.as<JsonVariant> ();

      if (_Client != nullptr) {
        // check if selected Client can send Data
        if (!_Client->canSend ()) {
          return false;
        }
      } else {
        // check if selected Client can send Data
        if (WebSocketObject.count () == 0) {
          return false;
        }
      }

      // Call externak datahandling Functions
      if (wsUpdateCB) {
        wsUpdateCB (InData, OutData);
      } else if (restApiGetCB) {
        restApiGetCB (InData, OutData);
      }

      // Create Response
      String Response;
      serializeJson (OutData, Response);
      Debug.println (FLAG_LOOP, true, ObjectName, __func__, Response);
      if (_Client != nullptr) {
        _Client->text (Response);
      } else {
        WebSocketObject.textAll (Response);
      }
      return true;
    }
  }
}
