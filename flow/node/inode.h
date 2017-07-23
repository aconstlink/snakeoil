//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_INODE_H_
#define _SNAKEOIL_FLOW_NODE_INODE_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include "connector/connector.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_flow
{
    class SNAKEOIL_FLOW_API inode
    {

    public:

        so_typedefs( so_std::vector<inode_ptr_t>, nodes ) ;

    public:

        so_flow::so_node::input_connector_t input( so_flow::key_t key )
        {
            return so_flow::so_node::input_connector_t{ key, this } ;
        }

        so_flow::so_node::output_connector_t output( so_flow::key_t key ) 
        {
            return so_flow::so_node::output_connector_t{ key, this } ;
        }

    public:

        /// connect me on another node with the passed key for the input slot
        /// @param k1 [in] output slot key of this
        /// @param k2 [in] input slot key of that
        /// @param that [in] connect to input slot with k2
        virtual so_flow::result connect_this_output_to_that_input( so_flow::key_cref_t this_key, 
            so_flow::key_cref_t that_key, so_flow::inode_ptr_t that ) = 0 ;

        /// connect some output slot sptr to this input slot with key k1
        /// @param key [in] the input slot key of this
        /// @param slot [in] the output slot to be connected
        virtual so_flow::result connect( so_flow::key_cref_t, so_flow::ioutput_slot_ptr_t ) = 0 ;

        /// connect some input slot sptr to this output slot with key k1
        /// @param key [in] the output slot key of this
        /// @param slot [in] the input slot to be connected
        virtual so_flow::result connect( so_flow::key_cref_t, so_flow::iinput_slot_ptr_t ) = 0 ;
        

    public:

        virtual so_flow::result on_trigger( so_flow::inode::nodes_ref_t ) = 0 ;
        virtual so_flow::result on_update( so_flow::inode::nodes_ref_t ) = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
