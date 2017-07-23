//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_FLOW_FLOW_NODE_MANAGER_H_
#define _SNAKEOIL_MANAGER_FLOW_FLOW_NODE_MANAGER_H_

#include "../api.h"
#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"

#include <snakeoil/thread/typedefs.h>

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_manager
{
    class SNAKEOIL_MANAGER_API flow_node_manager
    {
        so_this_typedefs( flow_node_manager ) ;

        private:

            struct store_item
            {
                // nodes
            };
            so_typedef( store_item ) ;

            so_typedefs( so_resource::resource_manager<store_item>, flow_node_mgr ) ;
            flow_node_mgr_t _flow_node_mgr ;

        private: // more resource managers here

        public:

            so_typedefs( flow_node_mgr_t::handle_t, handle ) ;
            so_typedefs( flow_node_mgr_t::handles_t, handles ) ;

            struct manage_params
            {
                // linear_spline
            };
            so_typedef( manage_params ) ;

        private:

            so_thread::mutex_t _mtx_counter ;
            size_t _counter = 0 ;

        public:

            flow_node_manager( void_t ) ;
            flow_node_manager( this_rref_t ) ;
            flow_node_manager( this_cref_t ) = delete ;

            virtual ~flow_node_manager( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            bool_t acquire( so_manager::key_cref_t key_in,
                so_resource::purpose_cref_t p, handle_out_t hnd_out ) ;

            bool_t release( handle_rref_t hnd ) ;

        public: // insert/reserve/exchange

            /// does not acquire the resource. It only inserts it.
            so_manager::result insert( so_manager::key_cref_t key_in, manage_params_cref_t mp ) ;
    };
}

#endif