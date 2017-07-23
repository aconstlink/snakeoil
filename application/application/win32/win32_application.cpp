//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "win32_application.h"

#include "../../system.h"

//#include <snakeoil/device/device.h>
//#include <snakeoil/device/system/isystem.h>
//#include <snakeoil/device/module/win32/rawinput/rawinput_module.h>

#include <snakeoil/log/log.h>

#include <windows.h>

using namespace so_app ;
using namespace so_app::so_win32 ;

//***********************************************************************
win32_application::win32_application( void_t ) 
{
#if 0
    _rawinput_module_ptr = so_device::so_module::so_win32::rawinput::
        create_and_initialize( NULL ) ;

    so_log::log::warning( so_device::device::has_no_system(), 
        "[win32_window::win32_window] : No device system initialized. Can not add raw input module" ) ;

    if( so_device::device::has_system() )
    {
        // enable auto deletion!
        so_device::device::get()->add_module( _rawinput_module_ptr ) ;
    }
#endif
}

//***********************************************************************
win32_application::win32_application( this_rref_t rhv ) : base_t( std::move( rhv ) )
{
    //_rawinput_module_ptr = rhv._rawinput_module_ptr ;
    //rhv._rawinput_module_ptr = nullptr ;
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

#if 0
        _rawinput_module_ptr->handle_input_event( msg.hwnd, msg.message, 
            msg.wParam, msg.lParam ) ;        
#endif
        
    } 

    return ok ;
}

//***********************************************************************

