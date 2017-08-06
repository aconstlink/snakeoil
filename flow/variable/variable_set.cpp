//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "variable_set.h"

#include "ivariable.h"

#include <algorithm>

using namespace so_flow ;

//*************************************************************************************
variable_set::variable_set( void_t ) 
{}

//*************************************************************************************
variable_set::variable_set( this_rref_t rhv )
{
    _variables = std::move( rhv._variables ) ;
}

//*************************************************************************************
variable_set::this_ref_t variable_set::operator = ( this_rref_t rhv )
{
    _variables = std::move( rhv._variables ) ;
    return *this ;
}

//*************************************************************************************
variable_set::~variable_set( void_t )
{
    for( auto & item : _variables )
    {
        item.var_ptr->destroy() ;
    }
}

//*************************************************************************************
variable_set::this_ptr_t variable_set::create( so_memory::purpose_cref_t p ) 
{
    return so_flow::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t variable_set::destroy( this_ptr_t ptr ) 
{
    so_flow::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_flow::result variable_set::add( so_flow::key_in_t key, so_flow::ivariable_ptr_t var_ptr )
{
    if( so_core::is_nullptr(var_ptr) )
        return so_flow::invalid_argument ;

    auto iter = std::find_if( _variables.begin(), _variables.end(), 
        [&]( this_t::store_data_cref_t item )
    {
        return item.name == key ;
    } ) ;

    if( iter != _variables.end() )
    {
        return so_flow::already_in_use ;
    }

    this_t::store_data_t sd ;
    sd.name = key ;
    sd.var_ptr = var_ptr ;

    _variables.push_back( sd ) ;

    return so_flow::ok ;
}

//*************************************************************************************
so_flow::result variable_set::remove( so_flow::key_in_t key )
{
    auto iter = std::find_if( _variables.begin(), _variables.end(),
        [&]( this_t::store_data_cref_t item )
    {
        return item.name == key ;
    } ) ;

    if( iter == _variables.end() )
    {
        return so_flow::invalid_argname ;
    }

    _variables.erase( iter ) ;

    return so_flow::ok ;
}

//*************************************************************************************
bool_t variable_set::get_variable( so_flow::key_in_t key, so_flow::ivariable_ptr_t & ptr_out )
{
    auto iter = std::find_if( _variables.begin(), _variables.end(),
        [&]( this_t::store_data_cref_t item )
    {
        return item.name == key ;
    } ) ;

    if( iter == _variables.end() )
    {
        return false ;
    }

    ptr_out = iter->var_ptr ;

    return true ;
}

//*************************************************************************************
void_t variable_set::for_each( this_t::for_each_funk_t funk ) const
{
    for( auto & item : _variables )
    {
        funk( item.name, item.var_ptr ) ;
    }
}
