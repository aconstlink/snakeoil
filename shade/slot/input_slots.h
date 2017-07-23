//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GRAPH_INPUT_SLOT_NODE_H_
#define _SNAKEOIL_SHADE_GRAPH_INPUT_SLOT_NODE_H_

#include "../protos.h"
#include "../api.h"
#include "../result.h"
#include "../typedefs.h"

#include "../type.h"

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    
    class SNAKEOIL_SHADE_API input_slots
    {
        so_this_typedefs( input_slots ) ;

        so_typedefs( so_std::vector<iinput_slot_ptr_t>, slots ) ;
        so_typedefs( so_std::vector<iinput_slot_cptr_t>, const_slots ) ;

        so_typedefs( std::function< void_t( so_shade::iinput_slot_ptr_t ) >, input_slot_funk ) ;

    private:

        slots_t _inputs ;

    protected:

        input_slots( void_t ) ;
        input_slots( this_rref_t ) ;

    public:

        virtual ~input_slots( void_t ) ;

    public:

        /// will return a container with all input slots that do not
        /// connect to a output slot.
        const_slots_t find_all_free_input_slots( void_t ) const ;

        so_shade::iinput_slot_ptr_t find_input_slot_by_name( so_std::string_cref_t ) ;

        so_shade::result connect_by_name( so_std::string_cref_t, ioutput_slot_ptr_t ) ;
        so_shade::result connect_by_name( so_std::string_cref_t my, 
            so_std::string_cref_t other, output_slots_ptr_t ) ;

    protected:

        void_t for_each_input( input_slot_funk_t ) ;

        so_shade::result add_input_slot( iinput_slot_ptr_t ) ;
        so_shade::result remove_input_slot( iinput_slot_ptr_t ) ;

        static bool_t try_connect_to_input_by_name( so_std::string_cref_t name,
                so_shade::iinput_slot_ptr_t to_ptr, so_shade::ioutput_slot_ptr_t incoming_ptr ) ;

        so_shade::types_t get_input_slot_types( void_t ) const ;
            
        
    };
    so_typedef( input_slots ) ;
    
}

#endif
