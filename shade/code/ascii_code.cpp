//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "ascii_code.h"

using namespace so_shade ;

//**********************************************************************************************
ascii_code::ascii_code( void_t ) 
{

}

//**********************************************************************************************
ascii_code::ascii_code( so_std::string_cref_t source ) : _code(source)
{

}

//**********************************************************************************************
ascii_code::ascii_code( so_shade::code_type ct,  so_shade::shader_type st, 
    so_std::string_cref_t source ) : base_t(ct, st), _code(source)
{

}

//**********************************************************************************************
ascii_code::ascii_code( this_cref_t rhv )
{
    _code = rhv._code ;
}

//**********************************************************************************************
ascii_code::ascii_code( this_rref_t rhv ) : base_t( std::move(rhv) )
{
    _code = std::move( rhv._code ) ;
}

//**********************************************************************************************
ascii_code::~ascii_code( void_t )
{}

//**********************************************************************************************
ascii_code::this_ref_t ascii_code::operator = ( this_cref_t rhv )
{
    _code = rhv._code ;
    return *this ;
}

//**********************************************************************************************
ascii_code::this_ref_t ascii_code::operator = ( this_rref_t rhv )
{
    _code = std::move( rhv._code ) ;
    return *this ;
}

//**********************************************************************************************
ascii_code::this_ptr_t ascii_code::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_shade::memory::alloc( std::move(rhv), p ) ;
}

//**********************************************************************************************
void_t ascii_code::destroy( this_ptr_t ptr )
{
    so_shade::memory::dealloc( ptr ) ;
}

//**********************************************************************************************
void_t ascii_code::set_code( so_std::string_cref_t code ) 
{
    _code = code ;
}

//**********************************************************************************************
so_std::string_cref_t ascii_code::get_code( void_t ) const
{
    return _code ;
}

//**********************************************************************************************
void_t ascii_code::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//**********************************************************************************************
