//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_INOUT_SLOT_H_
#define _SNAKEOIL_SHADE_SLOT_INOUT_SLOT_H_

#include "iinput_slot.h"
#include "ioutput_slot.h"
#include "connector_registry.h"

#include <snakeoil/std/string/string.hpp>

namespace so_shade
{
    class SNAKEOIL_SHADE_API inout_slot : public iinput_slot, public ioutput_slot, public connector_registry
    {
        so_this_typedefs( inout_slot ) ;

    private:

        so_shade::type _type ;
        so_std::string_t _name ;

        so_shade::input_slot_ptr_t _input_ptr = nullptr ;
        so_shade::output_slot_ptr_t _output_ptr = nullptr ;  

    public:

        inout_slot( so_std::string_cref_t ) ;
        inout_slot( this_rref_t ) ;
        virtual ~inout_slot( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_std::string_cref_t get_name( void_t ) const ;
        virtual so_shade::type_cref_t get_type( void_t ) const ;

        virtual void_t set_type( so_shade::type_cref_t ) ;

        virtual bool_t is_input_connected( void_t ) const ;
        virtual so_shade::result connect( so_shade::ioutput_slot_ptr_t ) ;
        virtual so_shade::result connect( so_shade::iinput_slot_ptr_t ) ;
        virtual void_t destroy( void_t ) ;

    public: // iinput_slot interface

        virtual so_shade::result disconnect( so_shade::ioutput_slot_ptr_t ) ;
        virtual so_shade::result disconnect_output_slots( void_t ) ;

    public: // ioutput_slot interface

        virtual so_shade::result disconnect( so_shade::iinput_slot_ptr_t ) ;
        virtual so_shade::result disconnect_input_slots( void_t ) ;

    public:

        virtual so_shade::result register_connector( so_shade::iconnector_ptr_t );
        virtual so_shade::result unregister_connector( so_shade::iconnector_ptr_t );
    };
}

#endif
