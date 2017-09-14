//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "win32_application.h"

#include "../../system.h"

#include <snakeoil/device/global.h>
#include <snakeoil/device/system/idevice_system.h>
#include <snakeoil/device/api/win32/rawinput/rawinput_api.h>



#include <snakeoil/log/global.h>

#include <windows.h>

using namespace so_app ;
using namespace so_app::so_win32 ;

//***********************************************************************
win32_application::win32_application( void_t ) 
{
    _rawinput_ptr = so_device::so_win32::rawinput_api::create(
        "[win32_application::win32_application] : rawinput_api" ) ;

    so_device::global::device_system()->register_api( 
        reinterpret_cast<so_device::iapi_ptr_t>(_rawinput_ptr) ) ;
}

//***********************************************************************
win32_application::win32_application( this_rref_t rhv ) : base_t( std::move( rhv ) )
{
    so_move_member_ptr( _rawinput_ptr, rhv ) ;
}

//***********************************************************************
win32_application::~win32_application( void_t )
{
    // _rawinput_module_ptr will be auto deleted
}

//***********************************************************************
win32_application::this_ptr_t win32_application::create( void_t ) 
{
    return this_t::create( this_t() ) ;
}

//***********************************************************************
win32_application::this_ptr_t win32_application::create( this_rref_t rhv )
{
    return so_app::memory::alloc( std::move( rhv ), "[win32_application::create]" ) ;
}

//***********************************************************************
void_t win32_application::destroy( this_ptr_t ptr ) 
{
    so_app::memory::dealloc( ptr ) ;
}

//***********************************************************************
so_app::result win32_application::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_app::ok ;
}

//***********************************************************************
so_app::result win32_application::exec_derived( void_t )
{
    MSG msg ;
    while( GetMessage( &msg, 0,0,0) )
    {		
        TranslateMessage( &msg ) ;
        DispatchMessage( &msg ) ;

        // will be send if a win32_window is closed.
        if( msg.message == WM_USER )
        {
            break ;
        }

        _rawinput_ptr->handle_input_event( msg.hwnd, msg.message,
            msg.wParam, msg.lParam ) ;
    } 

    return ok ;
}

//***********************************************************************

