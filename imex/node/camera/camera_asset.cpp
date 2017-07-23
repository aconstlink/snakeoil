//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "camera_asset.h"

#include "../../manager/camera_manager.h"

using namespace so_imex ;
using namespace so_imex::so_node ;

//*************************************************************************************
camera_asset::camera_asset( void_t ) 
{}

//*************************************************************************************
camera_asset::camera_asset( handle_rref_t hnd )
{
    _hnd = std::move(hnd) ;
}

//*************************************************************************************
camera_asset::camera_asset( this_rref_t rhv )
{
    _hnd = std::move(rhv._hnd) ;
}

//*************************************************************************************
camera_asset::~camera_asset( void_t )
{}

//*************************************************************************************
camera_asset::this_ptr_t camera_asset::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
camera_asset::this_ptr_t camera_asset::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t camera_asset::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t camera_asset::get_key( so_imex::key_out_t ko ) const 
{
    if( _hnd.is_valid() )
    {
        ko = _hnd.get_key() ;
        return true ;
    }
    return false ;
}

//*************************************************************************************
so_imex::icamera_ptr_t camera_asset::get_camera( void_t )
{
    return _hnd.is_valid() ? _hnd->data_ptr : nullptr ;
}

//*************************************************************************************
void_t camera_asset::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
