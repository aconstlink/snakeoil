//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_GROUP_H_
#define _SNAKEOIL_IMEX_NODE_GROUP_H_

#include "node.h"

#include <vector>

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API group : public node, public iparent
        {
            so_this_typedefs( group ) ;
            so_typedefs( std::vector<node_ptr_t>, children ) ;

        private:

            children_t _children ;

        public:

            group( void_t ) ;
            group( this_rref_t ) ;
            virtual ~group( void_t ) ;

        public:

            size_t get_num_children( void_t ) const ;
            node_ptr_t get_child( size_t i ) ;

        protected:

            bool_t contains( node_ptr_t ) const ;
            
            /// only returns true if the child has been added.
            bool_t add_child( node_ptr_t ) ;

            bool_t remove_child( node_ptr_t ) ;
            bool_t exchange_child( node_ptr_t old_ptr, node_ptr_t new_ptr ) ;
        };
        so_typedef( group ) ;
    }
}

#endif
