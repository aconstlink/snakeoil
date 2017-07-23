//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_NODE_LEAF_NODE_H_
#define _SNAKEOIL_SHADE_NODE_LEAF_NODE_H_

#include "node.h"

namespace so_shade
{
    namespace so_tree
    {
        class SNAKEOIL_SHADE_API leaf : public node
        {
            so_this_typedefs( leaf ) ;

        public:

            virtual this_ptr_t clone( void_t ) const = 0 ;
            virtual void_t destroy( void_t ) = 0 ;
        };
        so_typedef( leaf ) ;
    }
}

#endif
