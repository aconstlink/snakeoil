//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_ISLOT_H_
#define _SNAKEOIL_FLOW_ISLOT_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

namespace so_flow
{
    class SNAKEOIL_FLOW_API islot
    {

    public:

        virtual void_t set_owner( so_flow::inode_ptr_t ) = 0 ;
        virtual so_flow::inode_ptr_t get_owner( void_t ) = 0 ;        
        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif
