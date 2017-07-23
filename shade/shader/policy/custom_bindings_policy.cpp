//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "custom_bindings_policy.h"

using namespace so_shade ;
using namespace so_shade::so_shader ;

//*************************************************************************************
custom_bindings_policy::custom_bindings_policy( void_t ) 
{

}

//*************************************************************************************
custom_bindings_policy::custom_bindings_policy( this_cref_t rhv ) 
{
    _customs = rhv._customs ;
}

//*************************************************************************************
custom_bindings_policy::custom_bindings_policy( this_rref_t rhv )
{
    _customs = std::move( rhv._customs ) ;
}

//*************************************************************************************
custom_bindings_policy::~custom_bindings_policy( void_t )
{
    for( auto * ptr : _customs )
    {
        ptr->destroy() ;
    }
}

//*************************************************************************************
custom_bindings_policy::this_ref_t custom_bindings_policy::operator = ( this_cref_t rhv ) 
{
    _customs = rhv._customs ;
    return *this ;
}

//*************************************************************************************
so_shade::result custom_bindings_policy::add_binding( icustom_binding_ptr_t ptr )
{
    auto iter = std::find( _customs.begin(), _customs.end(), ptr ) ;
    if( iter != _customs.end() )
        return so_shade::invalid_argument ;

    _customs.push_back(ptr) ;

    return so_shade::ok ;
}

//*************************************************************************************
void_t custom_bindings_policy::for_each( for_each_funk_c_t funk ) const 
{
    for( auto const * ptr : _customs )
    {
        if( funk( ptr ) ) break ;
    }
}
