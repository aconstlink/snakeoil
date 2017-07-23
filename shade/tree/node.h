//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_NODE_NODE_H_
#define _SNAKEOIL_SHADE_NODE_NODE_H_

#include "../typedefs.h"
#include "../protos.h"
#include "../api.h"
#include "../result.h"

namespace so_shade
{
    namespace so_tree
    {
        class SNAKEOIL_SHADE_API node
        {
            so_this_typedefs( node ) ;

        public:

            virtual node_ptr_t clone( void_t ) const = 0 ;
            virtual void_t destroy( void_t ) = 0 ;

        };
        so_typedef( node ) ;
    }
}

#endif
