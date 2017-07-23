//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GRAPH_OUTPUT_SLOT_NODE_H_
#define _SNAKEOIL_SHADE_GRAPH_OUTPUT_SLOT_NODE_H_

#include "../protos.h"
#include "../api.h"
#include "../result.h"
#include "../typedefs.h"

#include "../type.h"

#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    
    class SNAKEOIL_SHADE_API output_slots
    {
        so_this_typedefs( output_slots ) ;

        so_typedefs( so_std::vector<ioutput_slot_ptr_t>, slots ) ;
        so_typedefs( so_std::vector<ioutput_slot_cptr_t>, const_slots ) ;

        so_typedefs( std::function< void_t( so_shade::ioutput_slot_ptr_t ) >, output_slot_funk ) ;

    private:

        slots_t _outputs ;

    public:

        output_slots( void_t ) ;
        output_slots( this_rref_t ) ;
        virtual ~output_slots( void_t ) ;

        so_shade::ioutput_slot_ptr_t find_output_slot_by_name( so_std::string_cref_t ) ;
        so_shade::ioutput_slot_ptr_t get_output_slot( size_t i ) ;

        so_shade::result connect_by_name( so_std::string_cref_t name, iinput_slot_ptr_t ) ;
        so_shade::result connect_by_name( so_std::string_cref_t my,
            so_std::string_cref_t other, input_slots_ptr_t ) ;

    protected:
        
        void_t for_each_output( output_slot_funk_t ) ;
        so_shade::result add_output_slot( ioutput_slot_ptr_t ) ;

        so_shade::types_t get_output_types( void_t ) const ;
            

    };
    so_typedef( output_slots ) ;
    
}

#endif
