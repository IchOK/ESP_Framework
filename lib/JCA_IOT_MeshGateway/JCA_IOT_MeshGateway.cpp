/**
 * @file JCA_IOT_MeshGateway.cpp
 * @author JCA (https://github.com/ichok)
 * @brief Handling Class for IOT-Mesh-Gateway
 * @version 1.0
 * @date 2024.03.24
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#include <JCA_IOT_MeshGateway.h>

using namespace JCA::SYS;

// Handle IOT Tasks (User-Code and Mesh)
Scheduler UserScheduler;
// Mesh Backend
painlessMesh Mesh;

namespace JCA {
  namespace IOT {
    /**
     * @brief Initialise the Mesh Node as Gateway
     * 
     * @param _WifiSSID SSID of the Wifi-AP, the Gateway should connect to
     * @param _WifiPassword Password of the Wifi-AP
     * @param _Channel Channel of the Wifi-AP
     * @param _DebugType Debuglevel for Mesh backend
     * @return true Gateway is connected to Wifi
     * @return false connection to Wifi failed
     */
    bool MeshGateway::init (TSTRING _WifiSSID, TSTRING _WifiPassword, uint8_t _Channel, uint16_t _DebugType) {
      uint32_t LastMillis;
      uint32_t ConnMillis = 0;
      bool IsConnected = false;

      Debug.println (FLAG_SETUP, true, ObjectName, __func__, "Init");
      // define Debug Message-Type if set
      if (_DebugType != 0) {
        Mesh.setDebugMsgTypes (_DebugType);
      }

      // Init Mesh Backend
      Mesh.init (JCA_IOT_MESH_PREFIX, JCA_IOT_MESH_PASSWORD, &UserScheduler, JCA_IOT_MESH_PORT, WIFI_MODE_APSTA, _Channel);

      // Setup WiFi Connection
      Mesh.stationManual(_WifiSSID, _WifiPassword);
      Mesh.setHostname(JCA_IOT_MESH_GW_HOST);

      // Define Node as Rott
      Mesh.setRoot(true);
      Mesh.setContainsRoot(true);

      // Check if Gateway ist connected to the Wifi-Network
      Debug.print (FLAG_SETUP, true, ObjectName, __func__, "Connect ");
      LastMillis = millis ();
      while (!IsConnected && ConnMillis < JCA_IOT_MESH_GW_CONNMAX) {
        if (WiFi.status () == WL_CONNECTED) {
          IsConnected = true;
        } else {
          Debug.print (FLAG_SETUP, true, ObjectName, __func__, "0");
          uint32_t ActMillis = millis();
          ConnMillis += ActMillis - LastMillis;
          ActMillis = LastMillis;
          yield();
        }
      }

      // Initialisation done
      if (IsConnected) {
        Debug.print (FLAG_SETUP, true, ObjectName, __func__, " DONE : ");
        Debug.println (FLAG_SETUP, true, ObjectName, __func__, Mesh.getAPIP ().toString ());
        return true;
      } else {
        Debug.println (FLAG_SETUP, true, ObjectName, __func__, " FAILED");
        return false;
      }
    }

    bool MeshGateway::handle() {
      ;
    }
  }
}