//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "camera.h"

#include "../../visitor/ivisitor.h"

#include <snakeoil/core/macros/move.h>

using namespace so_scene ;
using namespace so_scene::so_node ;

//*************************************************************************************
camera::camera( void_t ) noexcept
{

}

//*************************************************************************************
camera::camera( so_gfx::icamera_ptr_t cptr ) noexcept
{
    _cam_ptr = cptr ;
}

//*************************************************************************************
camera::camera( this_rref_t rhv ) noexcept : leaf( std::move(rhv) )
{
    so_move_member_ptr( _cam_ptr, rhv ) ;
}

//*************************************************************************************
camera::~camera( void_t ) noexcept
{}

//*************************************************************************************
camera::this_ptr_t camera::create( so_memory::purpose_cref_t p ) 
{
    return so_scene::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
camera::this_ptr_t camera::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_scene::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t camera::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t camera::transform_by( so_math::so_3d::trafof_cref_t trafo_in ) 
{
    if( _cam_ptr == nullptr ) return ;

    _cam_ptr->transform_by( trafo_in ) ;
}

//*************************************************************************************
void_t camera::set_transformation( so_math::so_3d::trafof_cref_t trafo_in ) 
{
    if( so_core::is_nullptr(_cam_ptr) ) return ;
    
    _cam_ptr->set_transformaion( trafo_in ) ;
}

//*************************************************************************************
so_gfx::icamera_cstr_t camera::get_camera( void_t ) const noexcept 
{
    return _cam_ptr ;
}

//*************************************************************************************
so_scene::result camera::apply( so_scene::so_visitor::ivisitor_ptr_t vptr ) 
{
    return vptr->visit( this ) ;
}

//*************************************************************************************
void_t camera::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
