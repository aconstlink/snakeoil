//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "fbx_module.h"

#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/math/vector/vector4.hpp>
#include <snakeoil/math/utility/angle.hpp>

using namespace so_imex ;


FbxManager * fbx_module::_fbx_manager = nullptr ;
so_thread::mutex_t fbx_module::_fbx_mtx ;

//*************************************************************************************
void_t fbx_module::create_fbx_manager( void_t ) 
{
    so_thread::lock_guard_t lk( _fbx_mtx ) ;
    if(_fbx_manager == nullptr)
    {
        _fbx_manager = FbxManager::Create() ;
        FbxIOSettings * ios = FbxIOSettings::Create( _fbx_manager, IOSROOT ) ;
        _fbx_manager->SetIOSettings( ios ) ;
    }
}

//*************************************************************************************
fbx_module::fbx_module( void_t ) 
{
    this_t::create_fbx_manager() ;
}

//*************************************************************************************
fbx_module::fbx_module( this_rref_t rhv )
{}

//*************************************************************************************
fbx_module::~fbx_module( void_t )
{
    // we let the manager dy at application shutdown. 
    // there are some multi-threaded issues.
    if( _fbx_manager != nullptr )
    {
        so_thread::lock_guard_t lk( _fbx_mtx ) ;
        //_fbx_manager->Destroy() ;
        //_fbx_manager = nullptr ;
    }
        
}

//*************************************************************************************
fbx_module::this_ptr_t fbx_module::create( so_memory::purpose_cref_t purp )
{
    return so_imex::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
void_t fbx_module::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_imex::key_t fbx_module::construct_key( so_imex::key_cref_t key_in, FbxNode * node ) 
{
    std::string const node_name( node->GetName() ) ;
    return std::string( "scene." + key_in + "." + node_name ) ;
}

//*************************************************************************************
so_imex::result fbx_module::initialize( init_params_cref_t params_in )
{
    return so_imex::ok ;
}

//*************************************************************************************
so_thread::task_graph_t fbx_module::export_scene( export_params_cref_t params_in )
{
    so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::not_implemented ) ;
    return so_thread::task_graph_t() ;
}

//*************************************************************************************
so_imex::result fbx_module::destroy( void_t )
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//*************************************************************************************
so_math::vec4f_t fbx_module::conv_vector( FbxVector4 const & vec_in ) const
{
    return so_math::vec4f_t(
            float_t(vec_in.Buffer()[0]), float_t(vec_in.Buffer()[1]), 
            float_t(vec_in.Buffer()[2]), float_t(vec_in.Buffer()[3])
            ) ;
}

//*************************************************************************************
so_math::vec3f_t fbx_module::conv_vector( FbxDouble3 const & vec_in ) const 
{
    return so_math::vec3f_t(
            float_t(vec_in.Buffer()[0]), 
            float_t(vec_in.Buffer()[1]), 
            float_t(vec_in.Buffer()[2]) ) ;
}

//*************************************************************************************
so_math::mat4f_t fbx_module::conv_matrix( FbxAMatrix const & mat_in ) const 
{
    so_math::mat4f_t mat_out ;

    for( size_t i=0; i<4; ++i )
    {
        FbxVector4 const row = mat_in.GetRow( int(i) ) ;

        mat_out.set_column( i, this_t::conv_vector(row) ) ;
    }

    return mat_out ;
}

//*************************************************************************************
so_math::vec3f_t fbx_module::fbx_angles_vector( FbxDouble3 const & fbx_in ) const 
{
    return so_math::vec3f_t(
        so_math::angle<float_t>::degree_to_radian(float_t(fbx_in.Buffer()[0])),
        so_math::angle<float_t>::degree_to_radian(float_t(fbx_in.Buffer()[1])),
        so_math::angle<float_t>::degree_to_radian(float_t(fbx_in.Buffer()[2]))
        ) ;
}

//*************************************************************************************
so_math::vec3f_t fbx_module::fbx_vector_swizzle( so_math::vec3f_cref_t vec_in ) const 
{
    return so_math::vec3f_t( vec_in.x(), vec_in.z(), -vec_in.y() ) ;
}

//*************************************************************************************
so_math::vec3f_t fbx_module::fbx_angles_swizzle( so_math::vec3f_cref_t vec_in ) const
{
    return so_math::vec3f_t( vec_in.x(), vec_in.z(), -vec_in.y() ) ;
}

//*************************************************************************************
so_math::vec3f_t fbx_module::fbx_scale_swizzle( so_math::vec3f_cref_t vec_in ) const
{
    return so_math::vec3f_t( vec_in.x(), vec_in.z(), vec_in.y() ) ;
}

//*************************************************************************************
bool_t fbx_module::is_format_supported( so_std::string_cref_t ext ) const 
{
    return ext == "fbx" ? true : false ;        
}

//*************************************************************************************
so_imex::file_extension_names_t fbx_module::get_file_extension_names( void_t ) 
{
    return so_imex::file_extension_names_t( {"fbx"} ) ;
}
