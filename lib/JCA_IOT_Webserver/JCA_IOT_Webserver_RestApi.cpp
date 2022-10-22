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
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    void Webserver::onRestApiRequest (AsyncWebServerRequest *_Request, JsonVariant &_Json) {
      JsonVariant OutData;

      if (Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, _Request->methodToString ())) {
        Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "+ Body:");
        String JsonBody;
        serializeJson (_Json, JsonBody);
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, JsonBody);
      }

      // Handle System-Data
      if (_Json.containsKey (Protocol::JsonTagElements)) {
        JsonArray Elements = (_Json.as<JsonObject> ())[Protocol::JsonTagElements].as<JsonArray> ();
        set (Elements);
      }

      // Call externak datahandling Functions
      switch (_Request->method ()) {
      case HTTP_GET:
        if (restApiGetCB) {
          OutData = restApiGetCB (_Json);
        }
        break;

      case HTTP_POST:
        if (restApiPostCB) {
          OutData = restApiPostCB (_Json);
        }
        break;

      case HTTP_PUT:
        if (restApiPutCB) {
          OutData = restApiPutCB (_Json);
        }
        break;

      case HTTP_PATCH:
        if (restApiPatchCB) {
          OutData = restApiPatchCB (_Json);
        }
        break;

      case HTTP_DELETE:
        if (restApiDeleteCB) {
          OutData = restApiDeleteCB (_Json);
        }
        break;

      default:
        break;
      }

      // Add System Informations
      JsonArray Elements;
      if (OutData.containsKey(Protocol::JsonTagElements)) {
        Elements = (OutData.as<JsonObject> ())[Protocol::JsonTagElements].as<JsonArray> ();
      } else {
        JsonDoc.clear ();
        Elements = JsonDoc.createNestedArray (Protocol::JsonTagElements);
        Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "No Answer defined");
      }
      getAll(Elements);

      // Create Response
      JsonDoc = OutData.as<JsonObject> ();
      String response;
      serializeJson (JsonDoc, response);
      Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "+ Response:");
      Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, response);
      _Request->send (200, "application/json", response);
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