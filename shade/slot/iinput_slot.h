//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_IINPUT_SLOT_H_
#define _SNAKEOIL_SHADE_SLOT_IINPUT_SLOT_H_

#include "islot.h"

namespace so_shade
{
    class SNAKEOIL_SHADE_API iinput_slot : public so_shade::islot
    {
        
    public:

        virtual bool_t is_input_connected( void_t ) const = 0 ;
        virtual so_shade::result connect( so_shade::ioutput_slot_ptr_t ) = 0 ;
        virtual so_shade::result disconnect( so_shade::ioutput_slot_ptr_t ) = 0 ;
        virtual so_shade::result disconnect_output_slots( void_t ) = 0 ;

        virtual so_shade::result register_connector( so_shade::iconnector_ptr_t ) = 0;
        virtual so_shade::result unregister_connector( so_shade::iconnector_ptr_t ) = 0;

        virtual void_t set_type( so_shade::type_cref_t ) = 0 ;

    };
}

#endif
