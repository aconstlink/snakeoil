//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_NODE_LEAF_ARG_NODE_H_
#define _SNAKEOIL_SHADE_NODE_LEAF_ARG_NODE_H_

#include "../leaf_node.h"

namespace so_shade
{
    namespace so_tree
    {
        class SNAKEOIL_SHADE_API arg_node : public node
        {
            so_this_typedefs( arg_node ) ;

        public:

            arg_node( void_t ) ;
            arg_node( this_rref_t ) ;
            arg_node( this_cref_t ) ;
            virtual ~arg_node( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            virtual node_ptr_t clone( void_t ) const ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( arg_node ) ;
    }
}

#endif
