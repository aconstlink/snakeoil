//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GRAPH_NODE_H_
#define _SNAKEOIL_SHADE_GRAPH_NODE_H_

#include "../result.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"

#include "../slot/input_slots.h"
#include "../slot/output_slots.h"
#include "../slot/iconnector.h"

namespace so_shade
{
    namespace so_graph
    {
        class SNAKEOIL_SHADE_API node : public input_slots, public output_slots
        {
            so_this_typedefs( node ) ;

        private:

            so_std::string_t _name ;

        public:

            node( void_t ) ;
            node( so_std::string_cref_t ) ;
            node( this_rref_t ) ;
            virtual ~node( void_t ) ;


        public:

            so_std::string_t get_name( void_t ) const ;

        public:

            virtual so_shade::result connect_by_name(
                so_std::string_cref_t, so_shade::ioutput_slot_ptr_t ) = 0 ;

        public:

            virtual void_t destroy( void_t ) = 0 ;

        };
        so_typedef( node ) ;
    }
}

#endif
