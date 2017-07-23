//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "single_choice_property.h"

#include "../ivisitor.h"

using namespace so_property ;

//*************************************************************************************
single_choice_property::single_choice_property( so_std::utf8_cref_t name ) : base_t( name )
{}

//*************************************************************************************
single_choice_property::single_choice_property( this_rref_t rhv ) : base_t( std::move(rhv) )
{}

//*************************************************************************************
single_choice_property::~single_choice_property( void_t )
{}

//*************************************************************************************
single_choice_property::this_ptr_t single_choice_property::create( this_rref_t rhv , so_memory::purpose_cref_t p )
{
    return so_property::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t single_choice_property::destroy( this_ptr_t ptr )
{
    so_property::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t single_choice_property::apply( so_property::ivisitor_ptr_t ptr )
{
    if( so_core::is_nullptr( ptr ) )
        return ;

    ptr->visit( this ) ;
    base_t::traverse_children( ptr ) ;
    return ptr->post_visit( this ) ;
}

//*************************************************************************************
void_t single_choice_property::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************