//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "leaf.h"
#include "../visitor/ivisitor.h"

#include <snakeoil/log/log.h>

using namespace so_scene ;
using namespace so_scene::so_node ;

//*******************************************************************
leaf::leaf( void_t ) 
{
}

//*******************************************************************
leaf::leaf( iparent_ptr_t ptr ) : base_t(ptr)
{
}

//*******************************************************************
leaf::leaf( this_rref_t rhv ) : base_t(std::move(rhv))
{
}

//*******************************************************************
leaf::~leaf( void_t )  
{
}

//*******************************************************************
so_scene::result leaf::apply( so_scene::so_visitor::ivisitor_ptr_t vptr ) 
{
    auto res = vptr->visit( this ) ;
    so_log::log::error( so_scene::no_success(res), 
        "[so_scene::leaf::apply] : visit" ) ;
    return vptr->post_visit( this ) ;
}
