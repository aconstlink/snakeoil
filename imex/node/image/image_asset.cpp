//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "image_asset.h"
#include "../../manager/image_manager.h"

#include <snakeoil/core/macros/move.h>

using namespace so_imex ;
using namespace so_imex::so_node ;


//*************************************************************************************
image_asset::image_asset( void_t ) 
{}

//*************************************************************************************
image_asset::image_asset( this_rref_t rhv ) 
{
    _hnd = std::move( rhv._hnd ) ;
}

//*************************************************************************************
image_asset::image_asset( handle_rref_t hnd ) 
{
    _hnd = std::move( hnd ) ;
}

//*************************************************************************************
image_asset::~image_asset( void_t )
{}

//*************************************************************************************
image_asset::this_ptr_t image_asset::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
image_asset::this_ptr_t image_asset::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t image_asset::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_resource::key_t image_asset::get_key( void_t ) const 
{
    return _hnd.get_key() ;
}

//*************************************************************************************
so_imex::iimage_ptr_t image_asset::get_image( void_t )
{
    return _hnd.is_valid() ? _hnd->data_ptr : nullptr ; 
}

//*************************************************************************************
void_t image_asset::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
