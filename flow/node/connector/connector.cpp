//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "connector.h"

#include "../inode.h"

#include <snakeoil/log/global.h>

using namespace so_flow ;
using namespace so_flow::so_node ;

so_flow::inode_ptr_t output_connector::connect( input_connector_cref_t icon ) 
{
    auto res = nptr->connect_this_output_to_that_input( key, icon.key, icon.nptr ) ;
    so_log::global::error( so_flow::no_success(res), "[output_connector::then] : connection failed" ) ;
    return icon.nptr ;
}
