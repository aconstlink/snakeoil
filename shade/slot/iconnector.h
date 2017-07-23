//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_SLOT_ICONNECTOR_H_
#define _SNAKEOIL_SHADE_SLOT_ICONNECTOR_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

namespace so_shade
{
    enum class connect_direction
    {
        input_to_output,
        output_to_input
    };

    class SNAKEOIL_SHADE_API iconnector
    {
    public:

        virtual void_t on_disconnect( so_shade::iinput_slot_cptr_t ) = 0;
        virtual void_t on_disconnect( so_shade::ioutput_slot_cptr_t ) = 0;

    public:

        virtual bool_t on_connect( so_shade::iinput_slot_ptr_t, 
            so_shade::ioutput_slot_ptr_t, connect_direction ) = 0;
                
    public:

        virtual void_t destroy( void_t ) = 0 ; 
    };
}

#endif
