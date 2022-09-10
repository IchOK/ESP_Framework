/**
 * @file JCA_IOT_Webserver_RestApi.cpp
 * @author JCA (https://github.com/ichok)
 * @brief RestAPI-Functions of the Webserver
 * @version 0.1
 * @date 2022-09-07
 *
 * Copyright Jochen Cabrera 2022
 * Apache License
 *
 */
#include <JCA_IOT_Webserver.h>

namespace JCA {
  namespace IOT {
    void Webserver::onRestApiRequest (AsyncWebServerRequest *_Request, JsonVariant &_Json) {
      const char *FunctionName = "onRestApiRequest";
      JsonVariant OutData;
      if (Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, _Request->methodToString ())) {
        Debug.print (FLAG_TRAFFIC, true, ObjectName, FunctionName, "+ Body:");
        String JsonBody;
        serializeJson (_Json, JsonBody);
        Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, JsonBody);
      }
      switch (_Request->method ()) {
      case HTTP_GET:
        if (restApiGetCB) {
          OutData = restApiGetCB (_Json);
          if (OutData.is<JsonArray> ()) {
            JsonDoc = OutData.as<JsonArray> ();
          } else if (OutData.is<JsonObject> ()) {
            JsonDoc = OutData.as<JsonObject> ();
          }
          String response;
          serializeJson (JsonDoc, response);
          Debug.print (FLAG_TRAFFIC, true, ObjectName, FunctionName, "+ Response:");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, response);
          _Request->send (200, "application/json", response);
          return;
        }
        break;

      case HTTP_POST:
        if (restApiPostCB) {
          OutData = restApiPostCB (_Json);
          if (OutData.is<JsonArray> ()) {
            JsonDoc = OutData.as<JsonArray> ();
          } else if (OutData.is<JsonObject> ()) {
            JsonDoc = OutData.as<JsonObject> ();
          }
          String response;
          serializeJson (JsonDoc, response);
          Debug.print (FLAG_TRAFFIC, true, ObjectName, FunctionName, "+ Response:");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, response);
          _Request->send (200, "application/json", response);
          return;
        }
        break;

      case HTTP_PUT:
        if (restApiPutCB) {
          OutData = restApiPutCB (_Json);
          if (OutData.is<JsonArray> ()) {
            JsonDoc = OutData.as<JsonArray> ();
          } else if (OutData.is<JsonObject> ()) {
            JsonDoc = OutData.as<JsonObject> ();
          }
          String response;
          serializeJson (JsonDoc, response);
          Debug.print (FLAG_TRAFFIC, true, ObjectName, FunctionName, "+ Response:");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, response);
          _Request->send (200, "application/json", response);
          return;
        }
        break;

      case HTTP_PATCH:
        if (restApiPatchCB) {
          OutData = restApiPatchCB (_Json);
          if (OutData.is<JsonArray> ()) {
            JsonDoc = OutData.as<JsonArray> ();
          } else if (OutData.is<JsonObject> ()) {
            JsonDoc = OutData.as<JsonObject> ();
          }
          String response;
          serializeJson (JsonDoc, response);
          Debug.print (FLAG_TRAFFIC, true, ObjectName, FunctionName, "+ Response:");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, response);
          _Request->send (200, "application/json", response);
          return;
        }
        break;

      case HTTP_DELETE:
        if (restApiDeleteCB) {
          OutData = restApiDeleteCB (_Json);
          if (OutData.is<JsonArray> ()) {
            JsonDoc = OutData.as<JsonArray> ();
          } else if (OutData.is<JsonObject> ()) {
            JsonDoc = OutData.as<JsonObject> ();
          }
          String response;
          serializeJson (JsonDoc, response);
          Debug.print (FLAG_TRAFFIC, true, ObjectName, FunctionName, "+ Response:");
          Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, response);
          _Request->send (200, "application/json", response);
          return;
        }
        break;

      default:
        break;
      }
      Debug.println (FLAG_TRAFFIC, true, ObjectName, FunctionName, "No Answer defined");
      _Request->send (405, "text/plain", _Request->methodToString ());
    }

    void Webserver::onRestApiGet (JsonVariantCallback _CB) {
      restApiGetCB = _CB;
    }

    void Webserver::onRestApiPost (JsonVariantCallback _CB) {
      restApiPostCB = _CB;
    }

    void Webserver::onRestApiPut (JsonVariantCallback _CB) {
      restApiPutCB = _CB;
    }

    void Webserver::onRestApiPatch (JsonVariantCallback _CB) {
      restApiPatchCB = _CB;
    }

    void Webserver::onRestApiDelete (JsonVariantCallback _CB) {
      restApiDeleteCB = _CB;
    }
  }
}