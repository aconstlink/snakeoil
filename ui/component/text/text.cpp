//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "text.h"

#include <snakeoil/math/vector/vector2.hpp>

using namespace so_ui ;
using namespace so_ui::so_component ;

//****************************************************************************
text::text( void_t )
{}

//****************************************************************************
text::text( so_std::string_cref_t r )
{
    _text = r ;
}

//****************************************************************************
text::text( this_rref_t rhv )
{
    _text = std::move( rhv._text ) ;
}

//****************************************************************************
text::~text( void_t )
{}

//****************************************************************************
text::this_ptr_t text::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//****************************************************************************
text::this_ptr_t text::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//****************************************************************************
void_t text::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
void_t text::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//****************************************************************************
void_t text::set_text( so_std::string_cref_t t )
{
    _text = t ;
}

//****************************************************************************
so_std::string_cref_t text::get_text( void_t ) const
{
    return _text ;
}