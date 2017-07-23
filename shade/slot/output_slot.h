//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_OUTPUT_SLOT_H_
#define _SNAKEOIL_SHADE_SLOT_OUTPUT_SLOT_H_

#include "connector_registry.h"
#include "ioutput_slot.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    class SNAKEOIL_SHADE_API output_slot : public ioutput_slot, public connector_registry
    {
        so_this_typedefs( output_slot ) ;

        so_typedefs( so_std::vector<so_shade::iinput_slot_ptr_t>, slots ) ;

    private:

        so_shade::type _type ;
        so_std::string_t _name ;
        slots_t _inputs ;

    public:

        output_slot( so_std::string_cref_t ) ;
        output_slot( so_std::string_cref_t, so_shade::type_cref_t ) ;
        output_slot( this_rref_t ) ;
        virtual ~output_slot( void_t ) ;
       
    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;
        
        virtual so_std::string_cref_t get_name( void_t ) const ;
        virtual so_shade::type_cref_t get_type( void_t ) const ;

        virtual void_t set_type( so_shade::type_cref_t ) ;

        virtual so_shade::result connect( iinput_slot_ptr_t ) ;
        virtual so_shade::result disconnect( so_shade::iinput_slot_ptr_t ) ;
        virtual so_shade::result disconnect_input_slots( void_t ) ;

        virtual void_t destroy( void_t ) ;

    public:

        virtual so_shade::result register_connector( so_shade::iconnector_ptr_t );
        virtual so_shade::result unregister_connector( so_shade::iconnector_ptr_t );

    };
    so_typedef( output_slot ) ;
}

#endif

