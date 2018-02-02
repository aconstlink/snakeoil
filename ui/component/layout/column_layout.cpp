//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "column_layout.h"

using namespace so_ui ;
using namespace so_ui::so_component ;

//*****************************************************************************************
column_layout::column_layout( void_t )
{

}

//*****************************************************************************************
column_layout::column_layout( this_rref_t rhv )
{

}

//*****************************************************************************************
column_layout::~column_layout( void_t )
{

}

//*****************************************************************************************
column_layout::this_ptr_t column_layout::create( so_memory::purpose_cref_t p )
{
    return this_t::create( this_t(), p ) ;
}

//*****************************************************************************************
column_layout::this_ptr_t column_layout::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_ui::memory::alloc( std::move( rhv ), p ) ;
}

//*****************************************************************************************
void_t column_layout::destroy( this_ptr_t ptr )
{
    so_ui::memory::dealloc( ptr ) ;
}

//*****************************************************************************************
void_t column_layout::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*****************************************************************************************