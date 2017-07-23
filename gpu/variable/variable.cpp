//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variable.h"

using namespace so_gpu ;

//*****************************************************************************************************
variable::variable( void_t ) 
{

}

//*****************************************************************************************************
variable::variable( shader_variable_type type ) : _type(type)
{

}

//*****************************************************************************************************
variable::variable( variable && rhv ) : object( std::move(rhv) )
{
    _type = rhv._type ;
    rhv._type = shader_variable_type::undefined ;
}

//*****************************************************************************************************
variable::~variable( void_t ) 
{

}

//*****************************************************************************************************
bool_t variable::is_type( shader_variable_type type ) const 
{
    return _type == type ;
}

//*****************************************************************************************************
shader_variable_type variable::get_type( void_t ) const 
{
    return _type ;
}

//*****************************************************************************************************
void_t variable::set_shader_variable_type( shader_variable_type svt ) 
{
    _type = svt ;
}

