/**
 * @file JCA_IOT_UdpListener.cpp
 * @author JCA (https://github.com/ichok)
 * @brief UdpListener-Functions of the Server
 * @version 0.1
 * @date 2025-04-06
 *
 * Copyright Jochen Cabrera 2025
 * Apache License
 *
 */
#include <JCA_IOT_Server.h>
using namespace JCA::SYS;

namespace JCA {
  namespace IOT {
    void Server::udpPacketHandler (AsyncUDPPacket _Packet) {
      Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, "Packet received");
      JsonDocument JBuffer;
      JsonObject InData;

      DeserializationError Error = deserializeJson (JBuffer, _Packet.data ());
      if (Error) {
        if (Debug.print (FLAG_ERROR, true, ObjectName, __func__, "+ deserializeJson() failed: ")) {
          Debug.println (FLAG_ERROR, true, ObjectName, __func__, Error.c_str ());
          Debug.print (FLAG_ERROR, true, ObjectName, __func__, "+ Packet:");
          Debug.println (FLAG_ERROR, true, ObjectName, __func__, (char *)(_Packet.data ()));
        }
        JBuffer.clear ();
      }
      InData = JBuffer.as<JsonObject> ();

      // Systemdatenpunkte auslesen
      if (InData["TimeSync"].is<unsigned long> ()) {
        unsigned long Epoch = InData["TimeSync"].as<unsigned long> ();
        setTime (Epoch);
        if (Debug.print (FLAG_TRAFFIC, true, ObjectName, __func__, "TimeSync received: ")) {
          Debug.println (FLAG_TRAFFIC, true, ObjectName, __func__, Epoch);
        }
      }
    }
  }
}
