//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "config.h"

using namespace so_gpu ;

//*****************************************************************************************************
config::this_ptr_t config::create( void_t ) 
{
    return this_t::create( "[so_gpu::config::create]" ) ;
}

//*****************************************************************************************************
config::this_ptr_t config::create( std::string const & purpose ) 
{
    return so_gpu::memory::alloc( this_t(), purpose ) ;
}

//*****************************************************************************************************
void_t config::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
config::config( void_t ) 
{

}

//*****************************************************************************************************
config::config( this_t && rhv ) : base_t( std::move(rhv) )
{
    _variable_sets = std::move( rhv._variable_sets ) ;
    _program_ptr = rhv._program_ptr ;
    rhv._program_ptr = nullptr ;

    _pt = rhv._pt ;
    rhv._pt = primitive_type::undefined ;
    _vb_ptr = rhv._vb_ptr ;
    rhv._vb_ptr = nullptr ;
    _ib_ptr = rhv._ib_ptr ;
    rhv._ib_ptr = nullptr ;

    _config_has_changed = rhv._config_has_changed ;

    _va_mappings = std::move( rhv._va_mappings ) ;
}

//*****************************************************************************************************
config::~config( void_t ) 
{
    this_t::set_program( nullptr ) ;
}

//*****************************************************************************************************
void_t config::set_program( program_ptr_t ptr ) 
{
    if( _program_ptr == ptr ) return ;
    if( _program_ptr != nullptr )
    {
        _program_ptr->detach_config( this ) ;
    }
    _program_ptr = ptr ;

    if( _program_ptr == nullptr ) return ;
    
    _program_ptr->add_config( this ) ;
    this_t::validate_change() ;
}

//*****************************************************************************************************
program_ptr_t config::get_program( void_t ) 
{
    return _program_ptr ;
}

//*****************************************************************************************************
void_t config::add_variable_set( variable_set_ptr_t ptr ) 
{
    if( ptr == nullptr ) return ;
    auto const iter = std::find_if( _variable_sets.begin(), _variable_sets.end(), 
        [&]( variable_set_info_cref_t item )
    {
        return ptr == item.ptr ;
    } ) ;

    if( iter == _variable_sets.end() )
    {
        variable_set_info vsi ;
        vsi.ptr = ptr ;

        _variable_sets.push_back( vsi ) ;
    }

    this_t::validate_varset_change( ptr ) ;
}

//*****************************************************************************************************
variable_set_ptr_t config::get_variable_set( size_t index ) 
{
    if( index >= _variable_sets.size() ) return nullptr ;
    return _variable_sets[index].ptr ;
}

//*****************************************************************************************************
size_t config::get_num_varsets( void_t ) const 
{
    return _variable_sets.size() ;
}

//*****************************************************************************************************
bool_t config::remove_variable_set( variable_set_ptr_t ptr ) 
{
    if( ptr == nullptr ) 
        return false ;

    auto const iter = std::find_if( _variable_sets.begin(), _variable_sets.end(),
        [&]( variable_set_info_cref_t item )
    {
        return ptr == item.ptr ;
    } ) ;
    if( iter == _variable_sets.end() ) 
        return false ;

    _variable_sets.erase( iter ) ;

    return true ;
}

//*****************************************************************************************************
so_gpu::result config::bind( so_gpu::vertex_attribute va, std::string const & name ) 
{
    auto iter = std::find_if( _va_mappings.begin(), _va_mappings.end(), 
        [=]( va_to_name_t const & mapping )
        {
            return mapping.first == va ;
        } ) ;

    if( iter != _va_mappings.end() ) iter->second = name ;
    else _va_mappings.push_back( va_to_name_t( va, name ) ) ;
    
    return so_gpu::ok ;
}

//*****************************************************************************************************
shader_variable_ptr_t config::get_input_variable( so_gpu::vertex_attribute va ) 
{
    if( _program_ptr == nullptr ) return nullptr ;
    auto iter = std::find_if( _va_mappings.begin(), _va_mappings.end(), [&]( va_to_name_t const & item )
    {
        return item.first == va ;
    } ) ;
    if( iter == _va_mappings.end() ) return nullptr ;

    return _program_ptr->find_shader_input_variable( iter->second ) ;
}

//*****************************************************************************************************
so_gpu::result config::bind( so_gpu::primitive_type pt, so_gpu::buffer_ptr_t vb_ptr, so_gpu::buffer_ptr_t ib_ptr ) 
{
    _vb_ptr = vb_ptr ;
    _ib_ptr = ib_ptr ;
    _pt = pt ;

    _config_has_changed = true ;

    return so_gpu::ok ;
}

//*****************************************************************************************************
void_t config::validate_shader_variables( void_t ) 
{
}

//*****************************************************************************************************
void_t config::validate_change( void_t ) 
{
    _config_has_changed = true ;
    for( auto & item : _variable_sets )
        item.changed = true ;
}

//*****************************************************************************************************
void_t config::validate_varset_change( so_gpu::variable_set_ptr_t ) 
{
    for( auto & item : _variable_sets )
        item.changed = true ;
}

//*****************************************************************************************************
void_t config::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

