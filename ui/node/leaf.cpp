//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "leaf.h"
#include "../visitor/ivisitor.h"

#include <snakeoil/log/global.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

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
so_ui::result leaf::apply( so_ui::so_visitor::ivisitor_ptr_t vptr ) 
{
    auto res = vptr->visit( this ) ;
    so_log::global::error( so_ui::no_success(res), 
        "[so_ui::leaf::apply] : visit" ) ;
    return vptr->post_visit( this ) ;
}