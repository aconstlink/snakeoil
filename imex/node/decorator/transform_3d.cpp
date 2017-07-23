//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "transform_3d.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
transform_3d::transform_3d( void_t ) 
{}

//*************************************************************************************
transform_3d::transform_3d( trafo_cref_t t ) 
{
    _trafo = t ;
}

//*************************************************************************************
transform_3d::transform_3d( this_rref_t rhv ) : base_t( std::move(rhv))
{
    _trafo = rhv._trafo ;
}

//*************************************************************************************
transform_3d::transform_3d( node_ptr_t nptr ) : base_t( nptr ) 
{}

//*************************************************************************************
transform_3d::~transform_3d( void_t ) 
{}

//*************************************************************************************
transform_3d::this_ptr_t transform_3d::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p )  ;
}

//*************************************************************************************
transform_3d::this_ptr_t transform_3d::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p )  ;
}

//*************************************************************************************
void_t transform_3d::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
transform_3d::trafo_cref_t transform_3d::get_transformation( void_t ) const
{
    return _trafo ;
}

//*************************************************************************************
void_t transform_3d::set_decorated( node_ptr_t nptr ) 
{
    base_t::set_decorated( nptr ) ;
}

//*************************************************************************************
void_t transform_3d::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
void_t transform_3d::set_rotation( so_math::vec3f_cref_t v ) 
{ 
    _rotation = v ; 
    this_t::transform( _rotation, _scale, _translation ) ;
}

//*************************************************************************************
void_t transform_3d::set_scale( so_math::vec3f_cref_t v ) 
{ 
    _scale = v ; 
    this_t::transform( _rotation, _scale, _translation ) ;
}

//*************************************************************************************
void_t transform_3d::set_translation( so_math::vec3f_cref_t v ) 
{ 
    _translation = v ; 
    this_t::transform( _rotation, _scale, _translation ) ;
}

//*************************************************************************************
void_t transform_3d::transform( so_math::vec3f_cref_t rotation, 
    so_math::vec3f_cref_t scale, so_math::vec3f_cref_t translation ) 
{
    so_math::so_3d::trafof_t tscale ;
    tscale.scale_fl( scale ) ;

    so_math::vec3f_t const x = so_math::vec3f_t( so_math::so_vector::x_axis() ) ;
    so_math::vec3f_t const y = so_math::vec3f_t( so_math::so_vector::y_axis() ) ;
    so_math::vec3f_t const z = so_math::vec3f_t( so_math::so_vector::z_axis() ) ;

    so_math::so_3d::trafof_t trotate ;

    trotate.rotate_by_axis_fl( z, rotation.z() ) ;
    trotate.rotate_by_axis_fl( y, rotation.y() ) ;
    trotate.rotate_by_axis_fl( x, rotation.x() ) ;

    so_math::so_3d::trafof_t ttranslate ;
    ttranslate.translate_fl( translation ) ;

    _trafo = ttranslate * tscale * trotate ;

    _rotation = rotation ;
    _scale = scale ;
    _translation = translation ;
}