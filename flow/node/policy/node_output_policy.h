//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_NODE_OUTPUT_POLICY_H_
#define _SNAKEOIL_FLOW_NODE_OUTPUT_POLICY_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"
#include "../../result.h"

#include "../../slot/ioutput_slot.h"
#include "handle.hpp"

#include <snakeoil/std/container/map.hpp>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/thread/semaphore.hpp>

namespace so_flow
{
    namespace so_policy
    {
        class SNAKEOIL_FLOW_API node_output_policy
        {
            so_this_typedefs( node_output_policy ) ;

        protected:

            struct store_data
            {
                size_t touched = 0 ;
                bool_t signaled = false ;
                so_flow::ioutput_slot_ptr_t sptr = nullptr ;
            };
            so_typedef( store_data ) ;

            typedef so_std::map< so_flow::key_t, store_data_t > __output_t ;
            so_typedefs( __output_t, outputs ) ;

        public:

            typedef so_flow::ioutput_slot_ptr_t slot_ptr_t ;
            so_typedefs( so_flow::so_policy::handle<this_t>, handle ) ;            
            friend class handle_t ;

            typedef std::function< void_t ( so_flow::key_cref_t, store_data_ref_t ) > for_each_funk_t ;

        private:

            so_thread::mutex_t _mtx ;
            outputs_t _outputs ;

        private:

            node_output_policy( this_cref_t ) {}

        protected:

            node_output_policy( void_t ) ;
            node_output_policy( this_rref_t ) ;

        public:

            ~node_output_policy( void_t ) ;

        protected:

            so_flow::result add_slot( so_flow::key_cref_t, so_flow::ioutput_slot_ptr_t ) ;
            so_flow::result add_slot_and_touch( so_flow::key_in_t,
                so_flow::ioutput_slot_ptr_t, this_t::handle_out_t ) ;

            so_flow::result destroy_output_slot( so_flow::key_cref_t ) ;

            bool_t find_and_touch( so_flow::key_cref_t, this_t::handle_ref_t hnd_out ) ;

            void_t for_each( for_each_funk_t ) ;

        private:

            bool_t untouch( so_flow::key_in_t ) ;
        };
    }

    
}

#endif
