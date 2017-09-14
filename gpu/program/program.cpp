//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "program.h"
#include "config.h"

#include "../variable/shader_variable.h"

#include <snakeoil/log/global.h>
#include <snakeoil/core/macros/move.h>

#include <algorithm>

using namespace so_gpu ;

//**********************************************************************************************
program::this_ptr_t program::create( so_memory::purpose_cref_t purpose ) 
{
    return so_gpu::memory::alloc( this_t(), purpose ) ;
}

//**********************************************************************************************
program::this_ptr_t program::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_gpu::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t program::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
program::program( void_t ) 
{}

//**********************************************************************************************
program::program( this_rref_t rhv ) : base_t( std::move( rhv ) )
{
    so_move_member_ptr( _vertex_shader, rhv ) ;
    so_move_member_ptr( _geometry_shader, rhv ) ;
    so_move_member_ptr( _pixel_shader, rhv ) ;

    _input_variables = std::move( rhv._input_variables ) ;
    _shader_variables = std::move( rhv._shader_variables ) ;

    _feedback_names = std::move( rhv._feedback_names ) ;
    _fbt = rhv._fbt ;
    rhv._fbt = feedback_buffer_type::undefined ;

    _configs = std::move( rhv._configs ) ;
}

//**********************************************************************************************
program::~program( void_t )
{
    this_t::detach_all_configs() ;
}

//**********************************************************************************************
program::this_ref_t program::set_shader( shader_ptr_t shd_ptr ) 
{
    if( shd_ptr == nullptr ) return *this ;

    switch( shd_ptr->get_shader_type() )
    {
    case shader_type::vertex_shader:
        _vertex_shader = static_cast<vertex_shader_ptr_t>(shd_ptr) ;
        break ;
    case shader_type::geometry_shader:
        _geometry_shader = static_cast<geometry_shader_ptr_t>(shd_ptr) ;
        break ;
    case shader_type::pixel_shader:
        _pixel_shader = static_cast<pixel_shader_ptr_t>(shd_ptr) ;
        break ;
    case shader_type::undefined:
        break ;
    }
    return *this ;
}

//**********************************************************************************************
program::this_ref_t program::set_shader( vertex_shader_ptr_t shd_ptr )
{
    _vertex_shader = shd_ptr ;
    return *this ;
}

//**********************************************************************************************
program::this_ref_t program::set_shader( geometry_shader_ptr_t shd_ptr ) 
{
    _geometry_shader = shd_ptr ;
    return *this ;
}

//**********************************************************************************************
program::this_ref_t program::set_shader( pixel_shader_ptr_t shd_ptr )
{
    _pixel_shader = shd_ptr ;
    return *this ;
}

//**********************************************************************************************
bool_t program::has_pixel_shader( void_t ) const 
{
    return _pixel_shader != nullptr ;
}

//**********************************************************************************************
bool_t program::has_no_pixel_shader( void_t ) const 
{
    return !has_pixel_shader() ;
}

//**********************************************************************************************
bool_t program::has_vertex_shader( void_t ) const 
{
    return _vertex_shader != nullptr ;
}

//**********************************************************************************************
bool_t program::has_no_vertex_shader( void_t ) const 
{
    return !has_vertex_shader() ;
}

//**********************************************************************************************
bool_t program::has_geometry_shader( void_t ) const 
{
    return _geometry_shader != nullptr ;
}

//**********************************************************************************************
bool_t program::has_no_geometry_shader( void_t ) const 
{
    return !has_geometry_shader() ;
}

//**********************************************************************************************
program::this_ref_t program::add_feedback_name( std::string const & name ) 
{
    if( std::find(_feedback_names.begin(), _feedback_names.end(), name ) != _feedback_names.end() ) 
        return *this ;
    
    _feedback_names.push_back( name ) ;
    return *this ;
}

//**********************************************************************************************
size_t program::get_num_feedback_names( void_t ) const 
{
    return _feedback_names.size() ;
}

//**********************************************************************************************
std::string const & program::get_feedback_name( size_t index ) const 
{
    return _feedback_names[index] ;
}

//**********************************************************************************************
bool_t program::has_feedback_names( void_t ) const 
{
    return _feedback_names.size() != 0 ;
}

//**********************************************************************************************
bool_t program::has_no_feedback_names( void_t ) const 
{
    return _feedback_names.size() == 0 ;
}

//**********************************************************************************************
void_t program::add_input_variable( shader_variable_ptr_t var_ptr ) 
{
    if( var_ptr == nullptr ) return ;
    
    auto iter = std::find_if( _input_variables.begin(), _input_variables.end(), 
        [=](shader_variable_ptr_t int_var){
            return int_var->is_name( var_ptr ) ;
        } ) ;

    if( so_log::global::warning(iter != _input_variables.end(),
        "[so_gpu::program::add_input_variable] : input variable name already exists.") ) 
        return ;

    _input_variables.push_back( var_ptr ) ;
}

//**********************************************************************************************
void_t program::add_shader_variable( shader_variable_ptr_t var_ptr ) 
{
    if( var_ptr == nullptr ) return ;
    
    auto iter = std::find_if( _shader_variables.begin(), _shader_variables.end(), 
        [=](shader_variable_ptr_t int_var){
            return int_var->is_name( var_ptr ) ;
        } ) ;
    
    if( so_log::global::warning(iter != _shader_variables.end(), 
        "[so_gpu::program::add_shader_variable] : shader variable name already exists.") ) 
        return ;
    
    _shader_variables.push_back( var_ptr ) ;
}

//**********************************************************************************************
shader_variable_ptr_t program::find_shader_input_variable( std::string const & name ) 
{
    auto iter = std::find_if( _input_variables.begin(), _input_variables.end(), 
        [&]( shader_variable_ptr_t vptr )
    {
        return vptr->is_name( name ) ;        
    } ) ;

    return ( iter != _input_variables.end() ) ? *iter : nullptr ;
}

//**********************************************************************************************
shader_variable_ptr_t program::has_shader_variable( std::string const & name, shader_variable_type svt ) 
{
    auto iter = std::find_if( _shader_variables.begin(), _shader_variables.end(), 
        [=](shader_variable_ptr_t int_var){
            return int_var->is_name( name ) ;
        } ) ;

    if( iter == _shader_variables.end() ) return nullptr ;
    if( !(*iter)->is_type( svt) ) return nullptr ;
    return *iter ;
}

//**********************************************************************************************
shader_variable_ptr_t program::find_shader_variable( std::string const & name ) 
{
    auto iter = std::find_if( _shader_variables.begin(), _shader_variables.end(), 
        [=](shader_variable_ptr_t int_var){
            return int_var->is_name( name ) ;
        } ) ;

    if( iter == _shader_variables.end() ) return nullptr ;
    return *iter ;
}

//**********************************************************************************************
void_t program::validate_connected_configs( void_t ) 
{
    for( auto * item : _configs )
    {
        item->set_config_changed(true) ;
    }
}

//**********************************************************************************************
void_t program::add_config( so_gpu::config_ptr_t ptr ) 
{
    if( ptr == nullptr ) return ;
    auto const iter = std::find( _configs.begin(), _configs.end(), ptr ) ;
    if( iter != _configs.end() ) return ;

    _configs.push_back( ptr ) ;
}

//**********************************************************************************************
void_t program::detach_config( so_gpu::config_ptr_t ptr ) 
{
    if( ptr == nullptr ) return ;
    auto const iter = std::find( _configs.begin(), _configs.end(), ptr ) ;
    if( iter == _configs.end() ) return ;

    _configs.erase( iter ) ;
}

//**********************************************************************************************
void_t program::detach_all_configs( void_t ) 
{
    auto configs = std::move(_configs) ;
    for( auto * config : configs )
    {
        config->set_program( nullptr ) ;
    }
}

//**********************************************************************************************
void_t program::destroy( void_t )
{
    this_t::destroy( this ) ;
}

