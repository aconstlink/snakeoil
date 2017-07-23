//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_LEAF_H_
#define _SNAKEOIL_IMEX_NODE_LEAF_H_

#include "node.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API leaf : public node
        {
            so_this_typedefs( leaf ) ;

        public:

            leaf( void_t ) ;
            leaf( this_rref_t ) ;
            virtual ~leaf( void_t ) ;

        };
    }
}

#endif
