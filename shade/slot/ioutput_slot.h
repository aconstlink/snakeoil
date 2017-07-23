//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_IOUTPUT_SLOT_H_
#define _SNAKEOIL_SHADE_SLOT_IOUTPUT_SLOT_H_

#include "islot.h"

namespace so_shade
{
    class SNAKEOIL_SHADE_API ioutput_slot : public so_shade::islot
    {
    public:

        virtual so_shade::result connect( so_shade::iinput_slot_ptr_t ) = 0 ;
        virtual so_shade::result disconnect( so_shade::iinput_slot_ptr_t ) = 0 ;
        virtual so_shade::result disconnect_input_slots( void_t ) = 0 ;

        virtual so_shade::result register_connector( so_shade::iconnector_ptr_t ) = 0 ;
        virtual so_shade::result unregister_connector( so_shade::iconnector_ptr_t ) = 0 ;

        virtual void_t set_type( so_shade::type_cref_t ) = 0 ;
    };
}

#endif
