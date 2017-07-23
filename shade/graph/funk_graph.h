//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GRAPH_GRAPH_H_
#define _SNAKEOIL_SHADE_GRAPH_GRAPH_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../so_api.h"
#include "../result.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_graph
    {
        class SNAKEOIL_SHADE_API funk_graph
        {
            so_this_typedefs( funk_graph ) ;

            struct funk_item
            {
                so_this_typedefs( funk_item ) ;

                so_std::string_t name ;
                so_shade::so_graph::funk_node_ptr_t funk_nptr = nullptr ;

                funk_item( void_t ) {}
                funk_item( this_rref_t rhv )
                {
                    so_move_member_ptr( funk_nptr, rhv ) ;
                    name = std::move( rhv.name ) ;
                }
                funk_item( this_cref_t rhv )
                {
                    funk_nptr = rhv.funk_nptr ;
                    name = rhv.name ;
                }
                
                ~funk_item( void_t ){}

            private:
                
                
            };
            so_typedef( funk_item ) ;
            so_typedefs( so_std::vector<funk_item_t>, funk_items ) ;

            typedef std::function< void_t ( funk_item_cref_t ) > iter_funk_t ;

        private:

            funk_items_t _nodes ;

            /// for name generation
            size_t _counter = 0 ;

        public:

            funk_graph( void_t ) ;
            funk_graph( this_rref_t ) ;
            ~funk_graph( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            so_shade::result add_node( so_shade::so_graph::funk_node_ptr_t ) ;
            
            so_shade::so_graph::funk_node_ptr_t find_node( so_std::string_cref_t ) ;
            so_shade::so_graph::funk_node_cptr_t find_node( so_std::string_cref_t ) const ;

            void_t for_each( iter_funk_t ) ;
            
        };
    }
}

#endif
