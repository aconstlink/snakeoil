//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "code.h"

using namespace so_shade ;

//*************************************************************************************
code::code( void_t ) 
{}

//*************************************************************************************
code::code( so_shade::code_type ct, so_shade::shader_type st ) :
    _type(ct), _stype(st)
{}

//*************************************************************************************
code::code( this_rref_t rhv ) : _type(rhv._type), _stype(rhv._stype)
{}

//*************************************************************************************
code::~code( void_t )
{}

//*************************************************************************************
so_shade::shader_type code::get_shader_type( void_t ) const 
{
    return _stype ;
}

//*************************************************************************************
so_shade::code_type code::get_type( void_t ) const 
{
    return _type ;
}


//*************************************************************************************
