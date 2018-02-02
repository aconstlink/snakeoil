//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "log.h"

#include <snakeoil/log/global.h>

#include <sstream>
#include <iomanip>

using namespace so_ui ;
using namespace so_ui::so_visitor ;

log::this_ptr_t log::create( void_t )
{
    return so_ui::memory::alloc( this_t(), 
        "[so_visitor::log::create]" ) ;
}

void_t log::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

void_t log::destroy( void_t )
{
    this_t::destroy( this ) ;
}

so_ui::result log::visit( so_ui::so_node::node_ptr_t )
{
    so_log::global::status( "-> node" ) ;

    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::node_ptr_t )
{
    so_log::global::status( "<- node" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::group_ptr_t )
{
    so_log::global::status( "-> group" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::group_ptr_t )
{
    so_log::global::status( "<- group" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::decorator_ptr_t )
{
    so_log::global::status( "-> decorator" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::decorator_ptr_t )
{
    so_log::global::status( "<- decorator" ) ;
    return so_ui::ok ;
}

so_ui::result log::visit( so_ui::so_node::leaf_ptr_t )
{
    so_log::global::status( "-> leaf" ) ;
    return so_ui::ok ;
}

so_ui::result log::post_visit( so_ui::so_node::leaf_ptr_t )
{
    so_log::global::status( "<- leaf" ) ;
    return so_ui::ok ;
}