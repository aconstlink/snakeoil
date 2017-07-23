//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "pinhole_lens.h"

#include <snakeoil/math/utility/3d/look_at.hpp>
#include <snakeoil/math/utility/3d/perspective_fov.hpp>

using namespace so_gfx ;

//**********************************************************************************************
pinhole_lens::pinhole_lens( void_t ) 
{

}

//**********************************************************************************************
pinhole_lens::pinhole_lens( this_rref_t rhv )
{
    _lens_frame = rhv._lens_frame ; 
    _lens_matrix = rhv._lens_matrix ;
    _view_matrix = rhv._view_matrix ;
    _proj_matrix = rhv._proj_matrix ;

    _projection_mode = rhv._projection_mode ;
}

//**********************************************************************************************
pinhole_lens::~pinhole_lens( void_t )
{

}

//**********************************************************************************************
pinhole_lens::this_ptr_t pinhole_lens::create( so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( this_t(), p ) ;
}

//**********************************************************************************************
pinhole_lens::this_ptr_t pinhole_lens::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t pinhole_lens::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
void_t pinhole_lens::create_orthographic( float_t aspect, float_t near, float_t far ) 
{
    // @todo
}

//**********************************************************************************************
void_t pinhole_lens::create_perspective_fov( float_t fov, float_t aspect, float_t near, float_t far ) 
{
    so_math::so_3d::perspective<float_t>::create_by_fovv_aspect( 
        fov, aspect, near, far, _proj_matrix ) ;

    _projection_mode = projection_type::perspective ;
}

//**********************************************************************************************
bool_t pinhole_lens::is_perspective( void_t ) const 
{
    return _projection_mode == projection_type::perspective ;
}

//**********************************************************************************************
bool_t pinhole_lens::is_orthographic( void_t ) const 
{
    return _projection_mode == projection_type::orthographic ;
}

//**********************************************************************************************
void_t pinhole_lens::get_position( so_math::vec3f_ref_t vec_out ) const
{
    vec_out = _lens_matrix.get_column3( 3 ) ;
}

//**********************************************************************************************
void_t pinhole_lens::get_lens_frame( so_math::mat3f_ref_t mat_out ) const 
{
    mat_out = _lens_frame ;
}

//**********************************************************************************************
void_t pinhole_lens::get_lens_matrix( so_math::mat4f_ref_t mat_out ) const
{
    mat_out = _lens_matrix ;
}

//**********************************************************************************************
void_t pinhole_lens::get_view_matrix( so_math::mat4f_ref_t mat_out ) const
{
    mat_out = _view_matrix ;
}

//**********************************************************************************************
void_t pinhole_lens::get_proj_matrix( so_math::mat4f_ref_t mat_out ) const
{
    mat_out = _proj_matrix ;
}

//**********************************************************************************************
void_t pinhole_lens::transform_by( so_math::so_3d::trafof_cref_t trafo_in ) 
{
    so_math::so_3d::trafof_t const trafo = 
        so_math::so_3d::trafof_t(_lens_matrix) * trafo_in ;

    so_math::mat4f_t const mat_comp = trafo.get_transformation() * _lens_matrix ;
    
    so_math::so_3d::create_view_matrix( mat_comp, _view_matrix ) ;
}

//**********************************************************************************************
void_t pinhole_lens::set_transformation( so_math::so_3d::trafof_cref_t trafo_in ) 
{
    _lens_matrix = trafo_in.get_transformation() ;

    so_math::so_3d::create_view_matrix( _lens_matrix, _view_matrix ) ;
}

//**********************************************************************************************
void_t pinhole_lens::look_at( so_math::vec3f_cref_t pos, 
    so_math::vec3f_cref_t up, so_math::vec3f_cref_t at ) 
{
    so_math::so_3d::create_lookat_rh( pos, up, at, _lens_matrix ) ;
    so_math::so_3d::create_view_matrix( _lens_matrix, _view_matrix ) ;
}

//**********************************************************************************************
void_t pinhole_lens::set_camera_matrix( so_math::mat4f_cref_t mat ) 
{
    _lens_matrix = mat ;
    so_math::so_3d::create_view_matrix( _lens_matrix, _view_matrix ) ;
}

//**********************************************************************************************
void_t pinhole_lens::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
