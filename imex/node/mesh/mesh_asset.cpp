//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mesh_asset.h"
#include "../../manager/mesh_manager.h"

#include <snakeoil/log/log.h>

using namespace so_imex ;
using namespace so_imex::so_node ;


//*************************************************************************************
mesh_asset::this_ptr_t mesh_asset::create( so_memory::purpose_cref_t purp ) 
{
    return so_imex::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
mesh_asset::this_ptr_t mesh_asset::create( this_rref_t rhv, so_memory::purpose_cref_t purp ) 
{
    return so_imex::memory::alloc( std::move(rhv), purp ) ;
}

//*************************************************************************************
void_t mesh_asset::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
mesh_asset::mesh_asset( void_t ) 
{}

//*************************************************************************************
mesh_asset::mesh_asset( this_rref_t rhv ) 
{
    _hnd = std::move( rhv._hnd ) ;
}

//*************************************************************************************
mesh_asset::mesh_asset( handle_rref_t hnd ) 
{
    _hnd = std::move(hnd) ;
}

//*************************************************************************************
mesh_asset::~mesh_asset( void_t ) 
{}

//*************************************************************************************
so_geo::imesh_cptr_t mesh_asset::get_mesh( void_t ) const
{
    return _hnd->data_ptr ;
}

//*************************************************************************************
so_imex::key_cref_t mesh_asset::get_key( void_t ) const 
{
    return _hnd.get_key() ;
}

//*************************************************************************************
void_t mesh_asset::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}


