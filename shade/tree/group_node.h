//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_NODE_GROUP_NODE_H_
#define _SNAKEOIL_SHADE_NODE_GROUP_NODE_H_

#include "node.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_tree
    {
        class SNAKEOIL_SHADE_API group : public node
        {
            so_this_typedefs( group ) ;

            so_typedefs( so_std::vector<node_ptr_t>, nodes ) ;

        private:

            nodes_t _nodes ;

        private:

            group( this_cref_t ) {}

        public:

            group( void_t ) ;
            group( this_rref_t ) ;            
            virtual ~group( void_t ) ;

        protected:

            bool_t find( node_ptr_t ) const ;
            so_shade::result add_node( node_ptr_t ) ;            
            so_shade::result exchange( node_ptr_t which_ptr, node_ptr_t with_ptr ) ;

        public:

            virtual node_ptr_t clone( void_t ) const = 0 ;
            virtual void_t destroy( void_t ) = 0 ;

        };
        so_typedef( group ) ;
    }
}

#endif
