//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "direct_gpx_render.h"
#include "../../node/transform/transform_3d.h"
#include "../../node/render/renderable.h"
#include "../../node/render/render_state.h"
#include "../../node/camera/camera.h"

#include <snakeoil/gpx/driver/driver_async.h>
#include <snakeoil/gpx/system/iuser_system.h>

#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/core/macros/move.h>

using namespace so_scene ;
using namespace so_scene::so_visitor ;

//*************************************************************************************
direct_gpx_render::this_ptr_t direct_gpx_render::create( so_memory::purpose_cref_t p ) 
{
    return so_scene::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
direct_gpx_render::this_ptr_t direct_gpx_render::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_scene::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t direct_gpx_render::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*************************************************************************************
direct_gpx_render::direct_gpx_render( void_t ) 
{
    _trafo_stack.push(
        so_math::mat4f_t( so_math::so_matrix::with_identity() )
        ) ;
}

//*************************************************************************************
direct_gpx_render::direct_gpx_render( so_gpx::iuser_system_ptr_t sys_ptr ) :
direct_gpx_render()
{
    _gpx_system_ptr = sys_ptr ;
}

//*************************************************************************************
direct_gpx_render::direct_gpx_render( this_rref_t rhv ) : base_t(std::move(rhv) )
{
    _trafo_stack = std::move( rhv._trafo_stack ) ;
    so_move_member_ptr( _gpx_system_ptr, rhv ) ;
    so_move_member_ptr( _lens_ptr, rhv ) ;

    _view = rhv._view ;
    _proj = rhv._proj ;
    _proj_params = rhv._proj_params ;
}

//*************************************************************************************
direct_gpx_render::~direct_gpx_render( void_t ) 
{}

//*************************************************************************************
void_t direct_gpx_render::set_view_matrix( so_math::mat4f_cref_t m ) 
{
    _view = m ;
}

//*************************************************************************************
void_t direct_gpx_render::set_proj_matrix( so_math::mat4f_cref_t m ) 
{
    _proj = m ;
}

//*************************************************************************************
void_t direct_gpx_render::set_proj_params( so_math::vec4f_cref_t params ) 
{
    _proj_params = params ;
}

//*************************************************************************************
void_t direct_gpx_render::set_lens( so_gfx::ilens_ptr_t ptr ) 
{
    if( so_core::is_nullptr(ptr) ) return ;

    _lens_ptr = ptr ;
    _lens_ptr->get_view_matrix( _view ) ;
    _lens_ptr->get_proj_matrix( _proj ) ;
}

//*************************************************************************************
void_t direct_gpx_render::use_lens( void_t ) 
{
    so_assert( _lens_ptr != nullptr ) ;

    _lens_ptr->get_view_matrix( _view ) ;
    _lens_ptr->get_proj_matrix( _proj ) ;
}

//*************************************************************************************
void_t direct_gpx_render::use_varset( size_t const vs ) 
{
    _varset = vs ;
}

//*************************************************************************************
so_scene::result direct_gpx_render::visit( so_scene::so_node::transform_3d_ptr_t trafo_ptr ) 
{
    so_math::so_3d::trafof_cref_t trafo_in = trafo_ptr->compute_trafo() ;
    _trafo_stack.push( _trafo_stack.top() * trafo_in ) ;

    return so_scene::ok ;
}

//*************************************************************************************
so_scene::result direct_gpx_render::post_visit( so_scene::so_node::transform_3d_ptr_t trafo_ptr ) 
{
    _trafo_stack.pop() ;
    return so_scene::ok ;
}

//*************************************************************************************
so_scene::result direct_gpx_render::visit( so_scene::so_node::camera_ptr_t cptr ) 
{
    cptr->set_transformation( _trafo_stack.top() ) ;

    return so_scene::ok ;
}

//*************************************************************************************
so_scene::result direct_gpx_render::visit( so_scene::so_node::renderable_ptr_t nptr )  
{
    if( so_core::is_nullptr(nptr) )
        return so_scene::invalid_argument ;

    if( so_core::is_not_nullptr( _lens_ptr ) )
    {
        _lens_ptr->get_view_matrix( _view ) ;
        _lens_ptr->get_proj_matrix( _proj ) ;
    }

    //nptr->set_object_matrix( so_math::mat4f_t( so_math::so_matrix::with_identity() ) ) ;
    //nptr->set_world_matrix( so_math::mat4f_t( so_math::so_matrix::with_identity() ) ) ;
    //nptr->set_view_matrix( so_math::mat4f_t( so_math::so_matrix::with_identity() ) ) ;
    //nptr->set_proj_matrix( so_math::mat4f_t( so_math::so_matrix::with_identity() ) ) ;

    nptr->set_world_matrix( _trafo_stack.top().get_transformation() ) ;
    nptr->set_view_matrix( _view ) ;
    nptr->set_proj_matrix( _proj ) ;
    nptr->set_proj_param0( _proj_params ) ;

    nptr->compute_mvp_matrix() ;

    size_t const varset = _varset ;
    _gpx_system_ptr->execute( [this,nptr, varset]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        so_gpu::variable_set_ptr_t vs_ptr ;
        if( nptr->get_varset(varset, vs_ptr ) )
        {
            drv_ptr->load_variable( vs_ptr ) ;
            drv_ptr->execute( so_gpu::so_driver::render_config_info( nptr->get_config(), varset ) ) ;
        }
    } ) ; 
    return so_scene::ok ;
}

//*************************************************************************************
so_scene::result direct_gpx_render::visit( so_scene::so_node::render_state_ptr_t nptr )  
{
    _gpx_system_ptr->execute( [this, nptr]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->push_state( nptr->get_attributes() ) ;
        drv_ptr->change_states( nptr->get_attributes(), nptr->get_states() ) ;
    } ) ;

    return so_scene::ok ;
}

//*************************************************************************************
so_scene::result direct_gpx_render::post_visit( so_scene::so_node::render_state_ptr_t nptr )  
{
    _gpx_system_ptr->execute( [this, nptr]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->pop_state() ;
    } ) ;

    return so_scene::ok ;
}

//*************************************************************************************
void_t direct_gpx_render::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
