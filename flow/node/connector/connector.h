//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_CONNECTOR_CONNECTOR_H_
#define _SNAKEOIL_FLOW_NODE_CONNECTOR_CONNECTOR_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"

namespace so_flow
{
    namespace so_node
    {
        struct input_connector ;
        so_typedef( input_connector ) ;
        struct output_connector ;
        so_typedef( output_connector ) ;
        
        struct SNAKEOIL_FLOW_API input_connector
        {
            so_flow::key_t key ;
            so_flow::inode_ptr_t nptr ;
        };

        struct SNAKEOIL_FLOW_API output_connector
        {
            so_flow::key_t key ;
            so_flow::inode_ptr_t nptr ;

            so_flow::inode_ptr_t connect( input_connector_cref_t ) ;
        };
    }
}

#endif
