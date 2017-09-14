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

using namespace so_scene ;
using namespace so_scene::so_node ;

//*************************************************************************************
renderable::renderable( void_t ) 
{
}

//*************************************************************************************
renderable::renderable( this_rref_t rhv ) : base_t(std::move(rhv))
{
    so_move_member_ptr( _config_ptr, rhv ) ;
    
    so_move( _gpu_varsets, rhv ) ;

    so_move_member_ptr( _object_ptr, rhv ) ;
    so_move_member_ptr( _world_ptr, rhv ) ;
    so_move_member_ptr( _view_ptr, rhv ) ;
    so_move_member_ptr( _proj_ptr, rhv ) ;
    so_move_member_ptr( _mvp_ptr, rhv ) ;

    so_move_member_ptr( _proj0_ptr, rhv ) ;
}

//*************************************************************************************
renderable::renderable( iparent_ptr_t pptr ) : base_t(pptr)
{}

//*************************************************************************************
renderable::renderable( so_gpu::config_ptr_t cptr ) : _config_ptr(cptr)
{}

//*************************************************************************************
renderable::renderable( so_gpu::config_ptr_t cptr, iparent_ptr_t pptr ) : 
    base_t(pptr), _config_ptr(cptr)
{}

//*************************************************************************************
renderable::~renderable( void_t )
{
}

//*************************************************************************************
renderable::this_ptr_t renderable::create( so_memory::purpose_cref_t purp ) 
{
    return so_scene::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
renderable::this_ptr_t renderable::create( this_rref_t rhv, so_memory::purpose_cref_t purp) 
{
    return so_scene::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************************
void_t renderable::destroy( this_ptr_t ptr ) 
{
    so_scene::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_gpu::config_ptr_t renderable::get_config( void_t ) 
{
    return _config_ptr ;
}

//*************************************************************************************
so_gpu::config_cptr_t renderable::get_config( void_t ) const 
{
    return _config_ptr ;
}

//*************************************************************************************
size_t renderable::get_num_gpu_varsets( void_t ) const 
{
    return _gpu_varsets.size() ;
}

//*************************************************************************************
bool_t renderable::get_varset( size_t const i, so_gpu::variable_set_ptr_t & ptr_out ) 
{
    if( i >= _gpu_varsets.size() )
        return false ;

    ptr_out = _gpu_varsets[i] ;

    return true ;
}

//*************************************************************************************
so_gpu::config_ptr_t renderable::set_config( so_gpu::config_ptr_t cptr ) 
{
    auto * old_config_ptr = _config_ptr ;

    // disconnect
    // remove variable set 
    if( _config_ptr != nullptr )
    {
        for( auto * ptr : _gpu_varsets )
            _config_ptr->remove_variable_set( ptr ) ;
    }

    _config_ptr = cptr ;

    // connect
    {
        for( auto * ptr : _gpu_varsets )
            _config_ptr->add_variable_set( ptr ) ;
    }

    return old_config_ptr ;
}

//*************************************************************************************
so_scene::result renderable::add_variable_set( so_gpu::variable_set_ptr_t ptr ) 
{
    auto const iter = std::find( _gpu_varsets.begin(), _gpu_varsets.end(), ptr ) ;
    if( iter != _gpu_varsets.end() )
        return so_scene::failed ;

    _gpu_varsets.push_back( ptr ) ;

    if( so_core::is_not_nullptr( _config_ptr ) )
        _config_ptr->add_variable_set( ptr ) ;

    return so_scene::ok ;
}

//*************************************************************************************
bool_t renderable::set_object_matrix( so_math::mat4f_cref_t mat_in ) 
{
    if( so_core::is_nullptr(_object_ptr) )
        return false ;

    _object_ptr->set_data( mat_in ) ;

    return true ;
}

//*************************************************************************************
bool_t renderable::set_world_matrix( so_math::mat4f_cref_t mat_in ) 
{
    if(so_core::is_nullptr( _world_ptr ))
        return false ;

    _world_ptr->set_data( mat_in ) ;

    return true ;
}

//*************************************************************************************
bool_t renderable::set_view_matrix( so_math::mat4f_cref_t mat_in )  
{
    if(so_core::is_nullptr( _view_ptr ))
        return false ;

    _view_ptr->set_data( mat_in ) ;

    return true ;
}

//*************************************************************************************
bool_t renderable::set_proj_matrix( so_math::mat4f_cref_t mat_in )  
{
    if( so_core::is_nullptr( _proj_ptr ) )
        return false ;

    _proj_ptr->set_data( mat_in ) ;

    return true ;
}

//*************************************************************************************
bool_t renderable::set_mvp_matrix( so_math::mat4f_cref_t mat_in ) 
{
    if( so_core::is_nullptr( _view_ptr ) )
        return false ;

    _view_ptr->set_data( mat_in ) ;

    return true ; 
}

//*************************************************************************************
bool_t renderable::set_proj_param0( so_math::vec4f_cref_t vec_in ) 
{
    if( so_core::is_nullptr( _proj0_ptr ) )
        return false ;

    _proj0_ptr->set_data( vec_in ) ;

    return true ;
}

//*************************************************************************************
bool_t renderable::bind_object_matrix( so_std::string_cref_t bp, this_t::var_mat4f_ptr_t ptr ) 
{
    if( so_core::is_not( this_t::bind_variable(bp, ptr) ) )
        return false ;
    
    _object_ptr = ptr ;

    return true ;
}

//*************************************************************************************
bool_t renderable::bind_world_matrix( so_std::string_cref_t bp, this_t::var_mat4f_ptr_t ptr )
{
    if( so_core::is_not(this_t::bind_variable( bp, ptr )) )
        return false ;

    _world_ptr = ptr ;

    return true ;
}

//*************************************************************************************
bool_t renderable::bind_view_matrix( so_std::string_cref_t bp, this_t::var_mat4f_ptr_t ptr )
{
    if( so_core::is_not(this_t::bind_variable( bp, ptr )) )
        return false ;

    _view_ptr = ptr ;

    return true ;
}

//*************************************************************************************
bool_t renderable::bind_proj_matrix( so_std::string_cref_t bp, this_t::var_mat4f_ptr_t ptr )
{
    if( so_core::is_not( this_t::bind_variable( bp, ptr ) ) )
        return false ;

    _proj_ptr = ptr ;

    return true ;
}

//*************************************************************************************
bool_t renderable::bind_mvp_matrix( so_std::string_cref_t bp, this_t::var_mat4f_ptr_t ptr )
{
    if( so_core::is_not(this_t::bind_variable( bp, ptr )) )
        return false ;

    _mvp_ptr = ptr ;

    return true ;
}

//*************************************************************************************
bool_t renderable::bind_proj_param0( so_std::string_cref_t bp, this_t::var_vec4f_ptr_t ptr )
{
    if( so_core::is_not(this_t::bind_variable( bp, ptr )) )
        return false ;

    _proj0_ptr = ptr ;

    return true ;
}

//*************************************************************************************
bool_t renderable::compute_mvp_matrix( void_t ) 
{
    if( so_core::is_nullptr( _mvp_ptr ) )
        return false ;

    so_math::mat4f_t mvp = so_math::mat4f_t(so_math::so_matrix::with_identity()) ;

    if( so_core::is_not_nullptr(_proj_ptr) )
    {
        mvp *= _proj_ptr->get_data() ;
    }

    if( so_core::is_not_nullptr( _view_ptr ) )
    {
        mvp *= _view_ptr->get_data() ;
    }

    if( so_core::is_not_nullptr( _world_ptr ) )
    {
        mvp *= _world_ptr->get_data() ;
    }

    if( so_core::is_not_nullptr( _object_ptr ) )
    {
        mvp *= _object_ptr->get_data() ;
    }

    *_mvp_ptr = mvp ;

    return true ;
}

//*************************************************************************************
bool_t renderable::reconnect_flow_to_variable_set( void_t ) 
{
    return this_t::reconnect_flow_node() ;
}

//*************************************************************************************
bool_t renderable::get_object_matrix( so_math::mat4f_out_t o ) const 
{
    if(so_core::is_nullptr( _object_ptr ))
        return false ;

    o = _object_ptr->get_data() ;

    return true ;
}

//*************************************************************************************
bool_t renderable::get_world_matrix( so_math::mat4f_out_t o ) const  
{
    if(so_core::is_nullptr( _world_ptr ))
        return false ;

    o = _world_ptr->get_data() ;

    return true ;
}

//*************************************************************************************
bool_t renderable::get_view_matrix( so_math::mat4f_out_t o ) const  
{
    if(so_core::is_nullptr( _view_ptr ))
        return false ;

    o = _view_ptr->get_data() ;

    return true ;
}

//*************************************************************************************
bool_t renderable::get_proj_matrix( so_math::mat4f_out_t o ) const  
{
    if(so_core::is_nullptr( _proj_ptr ))
        return false ;

    o = _proj_ptr->get_data() ;

    return true ;
}

//*************************************************************************************
bool_t renderable::get_mvp_matrix( so_math::mat4f_out_t o ) const 
{
    if(so_core::is_nullptr( _mvp_ptr ))
        return false ;

    o = _mvp_ptr->get_data() ;

    return true ;
}

//*************************************************************************************
bool_t renderable::get_proj_param0( so_math::vec4f_out_t o ) const 
{
    if( so_core::is_nullptr(_proj0_ptr) )
        return false ;

    o = _proj0_ptr->get_data() ;

    return true ;
}

//*************************************************************************************
so_scene::result renderable::apply( so_scene::so_visitor::ivisitor_ptr_t ptr ) 
{
    auto res = ptr->visit( this ) ;
    if( so_scene::no_success(res) )
    {
        so_log::global::error( "[so_scene::render_config::apply] : visit" ) ;
    }
    return ptr->post_visit( this ) ;
}

//*************************************************************************************
void_t renderable::destroy( void_t ) 
{
    this_t::destroy( this )  ;
}

//*************************************************************************************
