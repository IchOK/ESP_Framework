/**
 * @file JCA_IOT_Webserver_RestApi.cpp
 * @author JCA (https://github.com/ichok)
 * @brief RestAPI-Functions of the Server
 * @version 0.1
 * @date 2022-09-07
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Server.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    void Server::onRestApiRequest (AsyncWebServerRequest *_Request, JsonVariant &_Json) {
      JsonDocument JsonDoc;
      JsonVariant OutData = JsonDoc.as<JsonVariant> ();

      if (Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, _Request->methodToString ())) {
        Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "+ Body:");
        String JsonBody;
        serializeJson (_Json, JsonBody);
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, JsonBody);
      }

      // Call externak datahandling Functions
      switch (_Request->method ()) {
      case HTTP_GET:
        if (restApiGetCB) {
          restApiGetCB (_Json, OutData);
        }
        break;

      case HTTP_POST:
        if (restApiPostCB) {
          restApiPostCB (_Json, OutData);
        }
        break;

      case HTTP_PUT:
        if (restApiPutCB) {
          restApiPutCB (_Json, OutData);
        }
        break;

      case HTTP_PATCH:
        if (restApiPatchCB) {
          restApiPatchCB (_Json, OutData);
        }
        break;

      case HTTP_DELETE:
        if (restApiDeleteCB) {
          restApiDeleteCB (_Json, OutData);
        }
        break;

      default:
        break;
      }

      // Add System Informations
      OutData["used"] = JsonDoc.size ();

      // Create Response
      String response;
      serializeJson (OutData, response);
      Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "+ Response:");
      Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, response);
      _Request->send (200, "application/json", response);
    }

    void Server::onRestApiGet (JsonVariantCallback _CB) {
      restApiGetCB = _CB;
    }

    void Server::onRestApiPost (JsonVariantCallback _CB) {
      restApiPostCB = _CB;
    }

    void Server::onRestApiPut (JsonVariantCallback _CB) {
      restApiPutCB = _CB;
    }

    void Server::onRestApiPatch (JsonVariantCallback _CB) {
      restApiPatchCB = _CB;
    }

    void Server::onRestApiDelete (JsonVariantCallback _CB) {
      restApiDeleteCB = _CB;
    }
  }
}