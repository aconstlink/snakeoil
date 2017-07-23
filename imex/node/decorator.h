//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_DECORATOR_H_
#define _SNAKEOIL_IMEX_NODE_DECORATOR_H_

#include "node.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API decorator : public node, public iparent
        {
            so_this_typedefs( decorator ) ;

        private:

            node_ptr_t _decorated_ptr = nullptr ;

        public:

            decorator( void_t ) ;
            decorator( node_ptr_t ) ;
            decorator( this_rref_t ) ;
            virtual ~decorator( void_t ) ;

        public:

            node_ptr_t get_decorated( void_t ) ;
            node_cptr_t get_decorated( void_t ) const ;

            bool_t has_decorated( void_t ) const ;

        protected:

            void_t set_decorated( node_ptr_t ) ;
        };
        so_typedef( decorator ) ;
    }
}

#endif
