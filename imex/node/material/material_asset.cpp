//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "material_asset.h"
#include "../../manager/shade_manager/material_manager.h"

#include <snakeoil/core/macros/move.h>

using namespace so_imex ;
using namespace so_imex::so_node ;


//*************************************************************************************
material_asset::material_asset( void_t ) 
{}

//*************************************************************************************
material_asset::material_asset( this_rref_t rhv ) 
{
    _hnd = std::move( rhv._hnd ) ;
}

//*************************************************************************************
material_asset::material_asset( handle_rref_t hnd ) 
{
    _hnd = std::move( hnd ) ;
}

//*************************************************************************************
material_asset::~material_asset( void_t )
{}

//*************************************************************************************
material_asset::this_ref_t material_asset::operator = ( this_rref_t rhv )
{
    _hnd = std::move( rhv._hnd ) ;
    return *this ;
}

//*************************************************************************************
material_asset::this_ptr_t material_asset::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
material_asset::this_ptr_t material_asset::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t material_asset::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_resource::key_t material_asset::get_key( void_t ) const 
{
    return _hnd.get_key() ;
}

//*************************************************************************************
so_shade::so_material::imaterial_ptr_t material_asset::get_material( void_t )
{
    return _hnd.is_valid() ? _hnd->data_ptr : nullptr ; 
}

//*************************************************************************************
void_t material_asset::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
