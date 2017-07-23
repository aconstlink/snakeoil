//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "image.h"

#include <snakeoil/core/macros/move.h>

using namespace so_imex ;

//*************************************************************************************
image::image( void_t ) 
{}

//*************************************************************************************
image::image( so_imex::pixel_format pf, so_core::type t, width_t w, height_t h ) 
{
    this_t::create_image_data( pf, t, w, h ) ;
}

//*************************************************************************************
image::image( this_rref_t rhv ) 
{
    so_move_member_ptr( data_ptr, rhv ) ;
    
    data_type = rhv.data_type ;    
    pix_format = rhv.pix_format ;
    sib = rhv.sib ;
    width = rhv.width ;
    height = rhv.height ;
    image_fmt = rhv.image_fmt ;
}

//*************************************************************************************
image::~image( void_t ) 
{
    if( data_ptr != nullptr )
        so_imex::memory::dealloc( data_ptr ) ;
}

//*************************************************************************************
image::this_ptr_t image::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
image::this_ptr_t image::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t image::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t image::create_image_data( so_imex::pixel_format pf, so_core::type t, width_t w, height_t h ) 
{
    pix_format = pf ;
    data_type = t ;
    width = w ;
    height = h ;
    sib = w * h * so_imex::to_component_count(pf) * so_core::size_of(t) ;
    data_ptr = so_imex::memory::alloc( sib, "[image::create_image_data] : image_data" ) ;
}

//*************************************************************************************
void_t image::destroy( void_t )
{
    this_t::destroy( this ) ;
}
