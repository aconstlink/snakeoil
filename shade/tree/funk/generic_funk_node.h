//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_TREE_FUNK_GENERIC_FUNK_NODE_H_
#define _SNAKEOIL_SHADE_TREE_FUNK_GENERIC_FUNK_NODE_H_

#include "../funk_node.h"

namespace so_shade
{
    namespace so_tree
    {
        /// main purpose is to give access to the funk_node
        class SNAKEOIL_SHADE_API generic_funk_node : public funk_node
        {
            typedef funk_node base_t ;
            so_this_typedefs( generic_funk_node ) ;

        public:

            generic_funk_node( so_std::string_cref_t ) ;
            generic_funk_node( this_rref_t ) ;
            virtual ~generic_funk_node( void_t ) ;

        public:

            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public: // interface

            virtual node_ptr_t clone( void_t ) const ;
            virtual void_t destroy( void_t ) ;
        };
        so_typedef( generic_funk_node ) ;
    }
}

#endif
