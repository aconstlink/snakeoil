//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../protos.h"
#include "../typedefs.h"

namespace so_ui
{
    class inode_listener
    {
    public:

        virtual void_t on_move( so_ui::so_node::node_ptr_t ) = 0 ;
    };
    so_typedef( inode_listener ) ;
}