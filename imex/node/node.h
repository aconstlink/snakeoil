//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_NODE_H_
#define _SNAKEOIL_IMEX_NODE_NODE_H_

#include "iparent.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API node
        {
            so_this_typedefs( node ) ;

        private: 

            iparent_ptr_t _parent_ptr = nullptr ;

        private:

            // non-copyable
            node( this_cref_t ) {}

        public:

            node( void_t ) ;
            node( this_rref_t ) ;
            virtual ~node( void_t ) ;

        public:

            void_t set_parent( iparent_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) = 0 ;

        public:

            template< typename T >
            static bool_t is_of( so_imex::so_node::node_ptr_t ptr ) 
            {
                return dynamic_cast<T*>( ptr ) != nullptr ;
            }

            template< typename T >
            static T * safe_cast( so_imex::so_node::node_ptr_t ptr )
            {
                return dynamic_cast< T* >( ptr ) ;
            }

            template< typename T >
            static T * just_cast( so_imex::so_node::node_ptr_t ptr )
            {
                return static_cast< T* >( ptr ) ;
            }

        };
        so_typedef( node ) ;
    }
}

#endif
