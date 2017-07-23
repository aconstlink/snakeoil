//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "mesh.h"
#include "mesh_asset.h"
#include "../material/material_asset.h"
#include "../image/image_asset.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
mesh::mesh( void_t ) 
{}

//*************************************************************************************
mesh::mesh( this_rref_t rhv ) : base_t( std::move(rhv) )
{
    so_move_member_ptr( _asset_ptr, rhv ) ;
    so_move_member_ptr( _material_ptr, rhv ) ;
    _images = std::move( rhv._images ) ;
}

//*************************************************************************************
mesh::~mesh( void_t )
{
    // @note these nodes are destroyed via the group
    //so_imex::so_node::mesh_asset_t::destroy( _asset_ptr ) ;
    //so_imex::so_node::material_asset_t::destroy( _material_ptr ) ;
    /*
    for( auto & item : _images )
    {
        so_imex::so_node::image_asset_t::destroy(item.img_ptr) ;  
    }    
    _images.clear() ;
    */
}

//*************************************************************************************
mesh::this_ptr_t mesh::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
mesh::this_ptr_t mesh::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t mesh::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
mesh_asset_ptr_t mesh::set_asset( mesh_asset_ptr_t asset_ptr )
{
    auto * old_ptr = _asset_ptr ;
    if( base_t::exchange_child( old_ptr, asset_ptr ) )
    {
        _asset_ptr = asset_ptr ;
        return old_ptr ;
    }

    return nullptr ;
}

//*************************************************************************************
mesh_asset_ptr_t mesh::get_asset( void_t ) 
{
    return _asset_ptr ;
}

//*************************************************************************************
bool_t mesh::set_material( material_asset_ptr_t mptr )
{
    if( mptr == nullptr )
    {
        this_t::remove_child( _material_ptr ) ;
        _material_ptr = nullptr ;
    }
    else if( _material_ptr != nullptr )
    {
        this_t::exchange_child( _material_ptr, mptr ) ;
    }
    else if( this_t::add_child(mptr) )
    {
        _material_ptr = mptr ;
        return true ;
    }

    return false ;
}

//*************************************************************************************
material_asset_ptr_t mesh::get_material( void_t ) 
{
    return _material_ptr ;
}

//*************************************************************************************
bool_t mesh::add_image( so_std::string_cref_t key_in, 
    so_imex::so_node::image_asset_ptr_t aptr )  
{
    auto iter = std::find_if( _images.begin(), _images.end(), [&]( image_store_cref_t item )
    {
        return item.key == key_in ;
    } )  ;

    if( iter != _images.end() ) return false ;

    if( so_core::is_not( this_t::add_child( aptr ) ) )
    {
        return false ;
    }

    this_t::image_store_t is ;
    is.img_ptr = aptr ;
    is.key = key_in ;

    _images.push_back( is ) ;

    
    return true ;
}

//*************************************************************************************
bool_t mesh::get_image( so_std::string_cref_t key_in, 
    so_imex::so_node::image_asset_ptr_t & aptr_out )  
{
    auto iter = std::find_if( _images.begin(), _images.end(), [&]( image_store_cref_t item )
    {
        return item.key == key_in ;
    } )  ;
    if( iter == _images.end() ) return false ;

    aptr_out = iter->img_ptr ;

    return true ;
}

//*************************************************************************************
void_t mesh::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
