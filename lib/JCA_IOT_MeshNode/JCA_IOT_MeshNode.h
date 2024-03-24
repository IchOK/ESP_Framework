/**
 * @file JCA_IOT_MeshNode.h
 * @author JCA (https://github.com/ichok)
 * @brief Handling Class for IOT-Mesh-Nodes
 * Handling incoming messages
 * - Sub-Type "Hello"
 *   - A Node send e Hello-Message every 10 Seconds
 *   - Name will stored together with the NodeID
 *   - {"id":1, "n": "NAME_OF_THT_NODE"}
 * - Sub-Type "Write"
 *   - Write-Messages passes data to a function-tag
 *   - {}"id":2, "fnc":"NAME_OF_THE_FUNCTION", "tag":"NAME_OF_THE_TAG", "val": BOOL/REAL}
 * - Sub-Type "Read"
 *   - Read-Messages gets data from a function-tag
 *   - Returns an Ack-Message containing the src-Index, if the data was found
 *   - {}"id":3, "fnc":"NAME_OF_THE_FUNCTION", "tag":"NAME_OF_THE_TAG", "src": INT}
 * - Sub-Type "Ack"
 *   - Ack-Messages passes the data to a function-tag 
 *   - src-Index maps to the function and tag
 *   - {}"id":13, "src": INT, "val": BOOL/REAL}
 * @version 1.0
 * @date 2024-03-24
 *
 * Copyright Jochen Cabrera 2024
 * Apache License
 *
 */

#ifndef _JCA_IOT_MESHNODE_
#define _JCA_IOT_MESHNODE_

#include <painlessMesh.h>
#include <vector>

#include <JCA_FNC_Parent.h>

// Konfiguration of the Mesh-Network
#define JCA_IOT_MESH_PREFIX "JCA_IOT_MESH"
#define JCA_IOT_MESH_PASSWORD "JCA_IOT_MESHPASSWORD"
#define JCA_IOT_MESH_PORT 5555
#define JCA_IOT_MESH_CONNMAX 10000

namespace JCA {
  namespace IOT {
    class MeshNode {
      private:
        const char *ObjectName = "IOT::MeshNode";
        

      public:
        bool init (TSTRING _Role, uint8_t _Channel, uint16_t _DebugType);
        bool handle ();
    };
  }
}


#endif