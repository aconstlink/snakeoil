//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_NODE_LEAF_IMPORTED_FROM_H_
#define _SNAKEOIL_IMEX_NODE_LEAF_IMPORTED_FROM_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"
#include "../../result.h"

#include "../decorator.h"

namespace so_imex
{
    namespace so_node
    {
        class SNAKEOIL_IMEX_API imported_from : public so_imex::so_node::decorator
        {
            typedef decorator base_t ;
            so_this_typedefs( imported_from ) ;
            
        private:

            so_imex::string_t _format ;

        public:

            imported_from( void_t ) ;
            imported_from( so_imex::string_cref_t ) ;
            imported_from( so_imex::string_cref_t, so_imex::so_node::node_ptr_t ) ;
            imported_from( this_rref_t ) ;
            virtual ~imported_from( void_t ) ;

        public:

            static this_ptr_t create( so_memory::purpose_cref_t ) ;
            static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
            static void_t destroy( this_ptr_t ) ;

        public:

            void_t set_decorated( so_imex::so_node::node_ptr_t ) ;

        public:

            virtual void_t destroy( void_t ) ;
        };
        so_typedef( imported_from ) ;
    }
}
    
#endif
