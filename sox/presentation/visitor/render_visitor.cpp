//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "render_visitor.h"

#include <snakeoil/scene/node/transform/transform_3d.h>
#include <snakeoil/scene/node/render/renderable.h>
#include <snakeoil/scene/node/render/render_state.h>
#include <snakeoil/scene/node/camera/camera.h>

#include <snakeoil/gpx/system/render_system.h>
#include <snakeoil/gpx/technique/itechnique.h>

#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/core/macros/move.h>

using namespace sox_presentation ;
using namespace sox_presentation::so_visitor ;

//*************************************************************************************
render_visitor::this_ptr_t render_visitor::create( so_memory::purpose_cref_t p ) 
{
    return sox_presentation::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
render_visitor::this_ptr_t render_visitor::create( 
    this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return sox_presentation::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t render_visitor::destroy( this_ptr_t ptr ) 
{
    sox_presentation::memory::dealloc( ptr ) ;
}

//*************************************************************************************
render_visitor::render_visitor( void_t ) 
{
    _trafo_stack.push(
        so_math::mat4f_t( so_math::so_matrix::with_identity() )
        ) ;
}

//*************************************************************************************
render_visitor::render_visitor( so_gpx::render_system_ptr_t sys_ptr ) :
render_visitor()
{
    _rsys = sys_ptr ;
}

//*************************************************************************************
render_visitor::render_visitor( this_rref_t rhv ) : base_t(std::move(rhv) )
{
    _trafo_stack = std::move( rhv._trafo_stack ) ;
    so_move_member_ptr( _rsys, rhv ) ;
    so_move_member_ptr( _lens_ptr, rhv ) ;

    _view = rhv._view ;
    _proj = rhv._proj ;
    _proj_params = rhv._proj_params ;
}

//*************************************************************************************
render_visitor::~render_visitor( void_t ) 
{}

//*************************************************************************************
void_t render_visitor::set_view_matrix( so_math::mat4f_cref_t m ) 
{
    _view = m ;
}

//*************************************************************************************
void_t render_visitor::set_proj_matrix( so_math::mat4f_cref_t m ) 
{
    _proj = m ;
}

//*************************************************************************************
void_t render_visitor::set_proj_params( so_math::vec4f_cref_t params ) 
{
    _proj_params = params ;
}

//*************************************************************************************
void_t render_visitor::set_lens( so_gfx::ilens_ptr_t ptr ) 
{
    if( so_core::is_nullptr(ptr) ) return ;

    _lens_ptr = ptr ;
    _lens_ptr->get_view_matrix( _view ) ;
    _lens_ptr->get_proj_matrix( _proj ) ;
}

//*************************************************************************************
void_t render_visitor::use_lens( void_t ) 
{
    so_assert( _lens_ptr != nullptr ) ;

    _lens_ptr->get_view_matrix( _view ) ;
    _lens_ptr->get_proj_matrix( _proj ) ;
}

//*************************************************************************************
void_t render_visitor::use_varset( size_t const vs ) 
{
    _varset = vs ;
}

//*************************************************************************************
so_scene::result render_visitor::visit( so_scene::so_node::transform_3d_ptr_t trafo_ptr ) 
{
    so_math::so_3d::trafof_cref_t trafo_in = trafo_ptr->compute_trafo() ;
    _trafo_stack.push( _trafo_stack.top() * trafo_in ) ;

    return so_scene::result::ok ;
}

//*************************************************************************************
so_scene::result render_visitor::post_visit( so_scene::so_node::transform_3d_ptr_t trafo_ptr ) 
{
    _trafo_stack.pop() ;
    return so_scene::result::ok ;
}

//*************************************************************************************
so_scene::result render_visitor::visit( so_scene::so_node::camera_ptr_t cptr ) 
{
    cptr->set_transformation( _trafo_stack.top() ) ;

    return so_scene::result::ok ;
}

//*************************************************************************************
so_scene::result render_visitor::visit( so_scene::so_node::renderable_ptr_t nptr )  
{
    if( so_core::is_nullptr(nptr) )
        return so_scene::result::invalid_argument ;

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
    /*_gpx_system_ptr->execute( [this,nptr, varset]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        so_gpu::variable_set_ptr_t vs_ptr ;
        if( nptr->get_varset(varset, vs_ptr ) )
        {
            drv_ptr->load_variable( vs_ptr ) ;
            drv_ptr->execute( so_gpu::so_driver::render_config_info( nptr->get_config(), varset ) ) ;
        }
    } ) ; */
    return so_scene::result::ok ;
}

//*************************************************************************************
so_scene::result render_visitor::visit( so_scene::so_node::render_state_ptr_t nptr )  
{
    /*_gpx_system_ptr->execute( [this, nptr]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->push_state( nptr->get_attributes() ) ;
        drv_ptr->change_states( nptr->get_attributes(), nptr->get_states() ) ;
    } ) ;
    */
    return so_scene::result::ok ;
}

//*************************************************************************************
so_scene::result render_visitor::post_visit( so_scene::so_node::render_state_ptr_t nptr )  
{
    /*
    _gpx_system_ptr->execute( [this, nptr]( so_gpx::so_driver::driver_async_ptr_t drv_ptr )
    {
        drv_ptr->pop_state() ;
    } ) ;
    */
    return so_scene::result::ok ;
}

//*************************************************************************************
void_t render_visitor::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
