//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"
#include "../api.h"
#include "../protos.h"
#include "../result.h"

namespace so_ui
{
    namespace so_node
    {
        /// allows to specify node that can act as 
        /// parent nodes. Nodes without any child nodes MUST NOT
        /// be parent nodes!
        class SNAKEOIL_UI_API iparent
        {
        //protected:

          //  virtual so_ui::result replace( node_ptr_t which_ptr, node_ptr_t with_ptr ) = 0 ;
            //virtual so_ui::result detach( node_ptr_t which_ptr ) = 0 ;

        };
    }
}

