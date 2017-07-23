//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_GRAPH_GROUP_H_
#define _SNAKEOIL_SHADE_GRAPH_GROUP_H_

#include "node.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_shade
{
    namespace so_graph
    {
        class SNAKEOIL_SHADE_API group : public node
        {
            so_this_typedefs( group ) ;

            so_typedefs( so_std::vector<so_shade::so_graph::node_ptr_t>, nodes ) ;
            
        private:

            nodes_t _children ;

        public:

            group( void_t ) ;
            group( this_rref_t ) ;
            virtual ~group( void_t ) ;

        public:

            so_shade::result rebuild_slots( void_t ) ;

        protected:

            so_shade::result add_child( node_ptr_t ) ;
        };
    }
}

#endif
