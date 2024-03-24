/**
 * @file JCA_IOT_MeshGateway.h
 * @author JCA (https://github.com/ichok)
 * @brief Handling Class for IOT-Mesh-Gateway
 * Handling incoming Mesh-Messages
 * - Sub-Type Hello
 *   - A Node send e Hello-Message every 10 Seconds
 *   - Name will stored Name-Map together with the NodeID
 *   - Passed to WebSocket
 *   - {"id":1, "n": "NAME_OF_THE_NODE"}
 * - Sub-Type Write
 *   - Write-Messages passes data to WebSocket
 *   - {"id":2, "fnc":"NAME_OF_THE_FUNCTION", "tag":"NAME_OF_THE_TAG", "val": BOOL/REAL}
 * - Sub-Type Read
 *   - Read-Messages passes data to WebSocket
 *   - {"id":3, "fnc":"NAME_OF_THE_FUNCTION", "tag":"NAME_OF_THE_TAG", "src": INT}
 * - Sub-Type Ack
 *   - Ack-Messages passes data to WebSocket
 *   - {"id":13, "src": INT, "val": BOOL/REAL}
 * Handling incoming WebSocket-Messages
 * - Sub-Type Hello
 *   - Same structur as mesh
 *   - Will broadcasted to the Mesh
 * - Sub-Type Write/Read/Ack
 *   - Same structur as mesh with name "n": "NAME_OF_DEST_NODE"
 *   - if name is empty send broadcast
 *   - else translate name to dest-id by Name-Map and send a single Message
 * - Sub-Type Tree
 *   - Requests the Node-Tree
 *   - Send back the Routing Information
 *   - {"id": 4} -> {"id": 14, "data" : JSON_STRING_ROUTES}
 * - Sub-Type Names
 *   - Requests the Name-Map
 *   - Send back the Name-Map
 *   - {"id": 5} -> {"id": 15, "data" : {"NAME_OF_NODE_x": ID_NODE_X, ...}}
 * @version 1.0
 * @date 2024-03-24
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_IOT_MESHGATEWAY_
#define _JCA_IOT_MESHGATEWAY_

#include <painlessMesh.h>
#include <vector>

// Konfiguration of the Mesh-Network
#define JCA_IOT_MESH_PREFIX "JCA_IOT_MESH"
#define JCA_IOT_MESH_PASSWORD "JCA_IOT_MESHPASSWORD"
#define JCA_IOT_MESH_PORT 5555

#define JCA_IOT_MESH_ROLE "Gateway"
#define JCA_IOT_MESH_HOST "MeshGateway"
#define JCA_IOT_MESH_CONNMAX 10000

namespace JCA {
  namespace IOT {
    class MeshGateway {
      private:
        const char *ObjectName = "IOT::MeshGateway";
        

      public:
        bool init (TSTRING _WifiSSID, TSTRING _WifiPassword, uint8_t _Channel, uint16_t _DebugType);
        bool handle ();
    };
  }
}


#endif