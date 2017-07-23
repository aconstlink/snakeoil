//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "appx_context.h"

#include <snakeoil/core/assert.h>

#include <algorithm>

using namespace so_appx ;

//***********************************************************************
appx_context::appx_context( void_t ) 
{

}

//***********************************************************************
appx_context::appx_context( this_rref_t rhv ) 
{    
    so_move_member_ptr( _rsys_ptr, rhv ) ;
    so_move_member_ptr(_dev_ptr, rhv) ;
    so_move_member_ptr(_imexs_ptr, rhv) ;
}

//***********************************************************************
so_appx::result appx_context::set_render_system( so_gpx::render_system_ptr_t rsys_in )
{
    _rsys_ptr = rsys_in ;
    return so_appx::ok ;
}

//***********************************************************************
so_appx::result appx_context::set_dev( so_device::idevice_system_ptr_t ptr ) 
{
    _dev_ptr = ptr ;
    return so_appx::ok ;
}

//***********************************************************************
so_appx::result appx_context::set_imexs( so_imex::isystem_ptr_t ptr ) 
{
    _imexs_ptr = ptr ;
    return so_appx::ok ;
}

//***********************************************************************
so_gpx::render_system_ptr_t appx_context::render_system( void_t )
{
    return _rsys_ptr ;
}

//***********************************************************************
so_device::idevice_system_ptr_t appx_context::dev( void_t ) 
{
    return _dev_ptr ;
}

//***********************************************************************
so_imex::isystem_ptr_t  appx_context::imexsys( void_t ) 
{
    return _imexs_ptr ;
}
