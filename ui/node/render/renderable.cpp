//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "renderable.h"
#include "../../visitor/ivisitor.h"

#include <snakeoil/gpu/variable/variable_set.h>
#include <snakeoil/gpu/program/config.h>

#include <snakeoil/flow/node/variable/variable_node.h>
#include <snakeoil/flow/variable/variable_set.h>

#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

using namespace so_ui ;
using namespace so_ui::so_node ;

//*************************************************************************************
renderable::renderable( void_t ) 
{
}

//*************************************************************************************
renderable::renderable( this_rref_t rhv ) : base_t(std::move(rhv))
{
    
}

//*************************************************************************************
renderable::renderable( iparent_ptr_t pptr ) : base_t(pptr)
{}

//*************************************************************************************
renderable::~renderable( void_t )
{
}

//*************************************************************************************
renderable::this_ptr_t renderable::create( so_memory::purpose_cref_t purp ) 
{
    return so_ui::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
renderable::this_ptr_t renderable::create( this_rref_t rhv, so_memory::purpose_cref_t purp) 
{
    return so_ui::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************************
void_t renderable::destroy( this_ptr_t ptr ) 
{
    so_ui::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_ui::result renderable::apply( so_ui::so_visitor::ivisitor_ptr_t ptr ) 
{
    auto const res = ptr->visit( this ) ;
    if( res == so_ui::call_base )
    {
        return base_t::apply( ptr ) ;
    }
    return res ;
}

//*************************************************************************************
void_t renderable::destroy( void_t ) 
{
    this_t::destroy( this )  ;
}

//*************************************************************************************
void_t renderable::on_draw( so_ui::so_visitor::rect_render_2d_ptr_t )
{

}

//*************************************************************************************
void_t renderable::on_draw( so_ui::so_visitor::line_render_2d_ptr_t )
{

}

//*************************************************************************************
void_t renderable::on_draw( so_ui::so_visitor::text_render_2d_ptr_t )
{

}

//*************************************************************************************
void_t renderable::on_draw( so_ui::so_visitor::image_render_2d_ptr_t )
{

}
