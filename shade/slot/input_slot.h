//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_INPUT_SLOT_H_
#define _SNAKEOIL_SHADE_SLOT_INPUT_SLOT_H_

#include "connector_registry.h"
#include "iinput_slot.h"

namespace so_shade
{
    class SNAKEOIL_SHADE_API input_slot : public so_shade::iinput_slot, public connector_registry
    {
        so_this_typedefs( input_slot ) ;

    private:

        so_shade::type _type ;
        so_std::string_t _name ;
        so_shade::ioutput_slot_ptr_t _os_ptr = nullptr ;

    public:

        input_slot( so_std::string_cref_t ) ;
        input_slot( so_std::string_cref_t, so_shade::type_cref_t ) ;
        input_slot( this_rref_t ) ;
        virtual ~input_slot( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;
        
        virtual so_std::string_cref_t get_name( void_t ) const ;
        virtual so_shade::type_cref_t get_type( void_t ) const ;

        virtual void_t set_type( so_shade::type_cref_t ) ;

    public:

        virtual bool_t is_input_connected( void_t ) const ;
        virtual so_shade::result connect( so_shade::ioutput_slot_ptr_t ) ;
        virtual so_shade::result disconnect( so_shade::ioutput_slot_ptr_t ) ;
        virtual so_shade::result disconnect_output_slots( void_t ) ;

    public:

        virtual void_t destroy( void_t ) ;

    public:

        virtual so_shade::result register_connector( so_shade::iconnector_ptr_t );
        virtual so_shade::result unregister_connector( so_shade::iconnector_ptr_t );


    };
    so_typedef( input_slot ) ;
}

#endif
