//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "size_requirement.h"

using namespace so_ui ;
using namespace so_ui::so_component ;

//****************************************************************************
size_requirement::size_requirement( void_t )
{}

//****************************************************************************
size_requirement::size_requirement( size_data_cref_t id )
{
    _st_height = id.st_height ;
    _st_width = id.st_width ;
    _width = id.width ;
    _height = id.height ;
}

//****************************************************************************
size_requirement::size_requirement( this_rref_t rhv )
{
    _st_width = rhv._st_width ;
    _st_height = rhv._st_height ;
    _width = rhv._width ;
    _height = rhv._height ;
}

//****************************************************************************
size_requirement::~size_requirement( void_t )
{}

//****************************************************************************
size_requirement::this_ptr_t size_requirement::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//****************************************************************************
size_requirement::this_ptr_t size_requirement::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//****************************************************************************
void_t size_requirement::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//****************************************************************************
void_t size_requirement::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//****************************************************************************
size_requirement::size_data_t size_requirement::get_size_data( void_t ) const
{
    this_t::size_data_t sd ;
    sd.st_height = _st_height ;
    sd.st_width = _st_width ;
    sd.width = _width ;
    sd.height = _height ;

    return sd ;
}
