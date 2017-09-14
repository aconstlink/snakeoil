//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_NODE_H_
#define _SNAKEOIL_FLOW_NODE_NODE_H_

#include "inode.h"

#include "policy/node_input_policy.h"
#include "policy/node_output_policy.h"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/thread/semaphore.hpp>

namespace so_flow
{
    /// provides the most important functionality for data flow nodes.
    class SNAKEOIL_FLOW_API node : public inode, 
        private so_policy::node_input_policy, private so_policy::node_output_policy
    {
        so_this_typedefs( node ) ;

        so_typedefs( so_policy::node_input_policy, inputs ) ;
        so_typedefs( so_policy::node_output_policy, outputs ) ;

        /// + per on_trigger
        /// - per on_update
        so_thread::semaphore_t _triggered ;

    private:

        node( this_cref_t ) {}

    public:

        node( void_t ) ;
        node( this_rref_t ) ;
        virtual ~node( void_t ) ;
    
    public:

        this_ref_t operator = ( this_rref_t ) ;

    public: 

        virtual so_flow::result connect( so_flow::key_cref_t, so_flow::ioutput_slot_ptr_t ) ;
        virtual so_flow::result connect( so_flow::key_cref_t, so_flow::iinput_slot_ptr_t ) ;
        
        virtual so_flow::result connect_this_output_to_that_input( so_flow::key_cref_t, 
            so_flow::key_cref_t, so_flow::inode_ptr_t ) final ;

        virtual so_flow::result on_trigger( so_flow::inode::nodes_ref_t ) final ;
        virtual so_flow::result on_update( so_flow::inode::nodes_ref_t ) final ;
        
    public:

        so_flow::result create_input_slot( so_std::string_cref_t name, so_flow::ivariable_ptr_t ) ;
        so_flow::result create_output_slot( so_std::string_cref_t name, so_flow::ivariable_ptr_t ) ;

#if 0
        template< typename T >
        so_flow::result create_input_slot( so_std::string_cref_t name, T * var_ptr )
        {
            if( so_core::is_nullptr( var_ptr ) )
                return so_flow::invalid_pointer ;

            auto * is_ptr = so_flow::generic_input_slot<T>::create(
                so_flow::generic_input_slot<T>( var_ptr ), "[so_flow::node::create_input_slot]" ) ;

            auto const res = inputs_t::add_slot( name, is_ptr ) ;
            if( so_flow::no_success(res) )
            {
                so_log::global::error( "[so_flow::node::create_input_slot] : can not create slot." ) ;
                is_ptr->destroy() ;
                return res ;
            }

            is_ptr->set_owner( this ) ;

            return so_flow::ok ;
        }

        template< typename T >
        so_flow::result create_output_slot( so_std::string_cref_t name, T * var_ptr )
        {
            if( so_core::is_nullptr( var_ptr ) )
                return so_flow::invalid_pointer ;

            auto * os_ptr = so_flow::generic_output_slot<T>::create(
                so_flow::generic_output_slot<T>( var_ptr ), 
                "[so_flow::node::create_output_slot] : " + name ) ;
            
            auto const res = outputs_t::add_slot( name, os_ptr ) ;
            if(so_flow::no_success( res ))
            {
                so_log::global::error( "[so_flow::node::create_output_slot] : can not create slot." ) ;
                os_ptr->destroy() ;
                return res ;
            }

            os_ptr->set_owner( this ) ;

            return so_flow::ok ;
        }
#endif 

    protected: // interface
        
        /// allows the user to decide is the trigger 
        /// should be done. By default, this function returns true.
        /// By default, this node will trigger.
        /// this allows to avoid executing a whole subgraph is
        /// certain condition isn't met.
        virtual bool_t on_trigger( void_t ) ;

        /// called on the real update of the logic
        virtual bool_t on_update( void_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
