//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_NODE_H_
#define _SNAKEOIL_IMEX_MODULE_DAE_ELEMENTS_DAE_NODE_H_

#include "dae_protos.h"
#include "dae_visitor.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_imex
{
    namespace so_dae
    {
        class node
        {
            so_this_typedefs( node ) ;
            so_typedefs( so_std::vector<this_ptr_t>, children ) ;

        private:

            so_std::string_t _tag_name ;
            children_t _children ;
            
        public:

            node( so_std::string_cref_t tag_name ) : _tag_name( tag_name )
            {}

            node( this_cref_t rhv ) = delete ;

            node( this_rref_t rhv ) : _tag_name( std::move(rhv._tag_name) ), 
                _children( std::move(rhv._children) )
            {}
            
            ~node( void_t ) 
            {
                for( auto * child_ptr : _children )
                {
                    so_imex::memory::dealloc( child_ptr ) ;
                }
            }

        public:

            void_t add_node( this_ptr_t ptr )
            {
                _children.push_back( ptr ) ;
            }

        public:

            virtual void_t apply( so_dae::visitor_ptr_t v )
            {
                return this_t::traverse_children( v ) ;
            }

        private:

            void_t traverse_children( so_dae::visitor_ptr_t vptr )
            {
                if( _children.size() == 0 ) return ;

                vptr->descent( this ) ;

                for( auto * child_ptr : _children )
                {
                    child_ptr->apply( vptr ) ;
                }

                vptr->ascent( this ) ;
            }
        };
    }
}

#endif