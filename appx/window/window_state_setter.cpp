
#include "window_state_setter.h"
#include "window_event_manager.h"

using namespace so_appx ;

//******************************************************
window_state_setter::window_state_setter( window_event_manager_str_t ptr )
{
    _mgr = ptr ;
}

//******************************************************
window_state_setter::window_state_setter( this_rref_t rhv )
{
    so_move_member_ptr( _mgr, rhv ) ;
}

//******************************************************
window_state_setter::~window_state_setter( void_t )
{}

//******************************************************
void_t window_state_setter::toggle_vsync( size_t const wid )
{
    _mgr->toggle_vsync( wid ) ;
}

//******************************************************
void_t window_state_setter::toggle_fullscreen( size_t const wid )
{
    _mgr->toggle_fullscreen( wid ) ;
}