//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "shader.h"

using namespace so_gpu ;

//*****************************************************************************************************
bool_t shader::has_no_code( void_t ) const 
{
    return _code.empty() ;
}

//*****************************************************************************************************
bool_t shader::has_code( void_t ) const 
{
    return !_code.empty() ;
}

