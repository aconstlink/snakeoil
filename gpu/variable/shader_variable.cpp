//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "shader_variable.h"

using namespace so_gpu ;


//*****************************************************************************************************
shader_variable::this_ptr_t shader_variable::create( void_t ) 
{
    return this_t::create( this_t(), "[so_gpu::shader_variable::create]" ) ;
}

//*****************************************************************************************************
shader_variable::this_ptr_t shader_variable::create( this_rref_t rhv, std::string const & purpose ) 
{
    return so_gpu::memory::alloc( std::move(rhv), purpose ) ;
}

//*****************************************************************************************************
void_t shader_variable::destroy( this_ptr_t ptr ) 
{
    so_gpu::memory::dealloc( ptr ) ;
}

//*****************************************************************************************************
shader_variable::shader_variable( void_t ) 
{

}

//*****************************************************************************************************
shader_variable::shader_variable( std::string const & name, shader_variable_type type ) : 
    _name(name), _type(type)
{

}

//*****************************************************************************************************
shader_variable::shader_variable( shader_variable && rhv ) : base_t( std::move(rhv) )
{
    _name = std::move( rhv._name ) ;
    _type = rhv._type ;
    rhv._type = shader_variable_type::undefined ;
}

//*****************************************************************************************************
shader_variable::~shader_variable( void_t ) 
{

}

//*****************************************************************************************************
bool_t shader_variable::is_name( std::string const & name ) const 
{
    return _name == name ;
}

//*****************************************************************************************************
bool_t shader_variable::is_name( this_ptr_t rhv ) const 
{
    return this_t::is_name( rhv->get_name() ) ;
}

//*****************************************************************************************************
std::string const & shader_variable::get_name( void_t ) const 
{ 
    return _name ; 
}

//*****************************************************************************************************
bool_t shader_variable::is_type( shader_variable_type type ) const 
{
    return _type == type ;
}

//*****************************************************************************************************
shader_variable_type shader_variable::get_type( void_t ) const 
{
    return _type ;
}

//*****************************************************************************************************
void_t shader_variable::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

