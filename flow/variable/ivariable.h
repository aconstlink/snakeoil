//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_IVARIABLE_H_
#define _SNAKEOIL_FLOW_IVARIABLE_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API ivariable
    {

    public:

        virtual iinput_slot_ptr_t create_input_slot( so_memory::purpose_cref_t ) = 0 ;
        virtual ioutput_slot_ptr_t create_output_slot( so_memory::purpose_cref_t ) = 0 ;

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
