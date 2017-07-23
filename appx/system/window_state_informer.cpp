//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "window_state_informer.h"

#include <snakeoil/application/window/iwindow_message_listener.h>

using namespace so_appx ;

//*************************************************************************************
window_state_informer::window_state_informer( void_t ) 
{
}

//*************************************************************************************
window_state_informer::window_state_informer( so_app::iwindow_message_listener_ptr_t lptr ) 
{
    window_state_informer::states_t st ;

    st.resize_changed = lptr->has_resize_changed( st.resize_msg ) ;
    st.show_changed = lptr->has_visible_changed( st.show_msg ) ;
    st.dpi_msg_changed = lptr->has_screen_changed( st.dpi_msg ) ;
    st.msize_msg_changed = lptr->has_screen_changed( st.msize_msg ) ;

    _states = st ;
}

//*************************************************************************************
window_state_informer::window_state_informer( this_rref_t rhv ) 
{
    *this = std::move( rhv ) ;
}

//*************************************************************************************
window_state_informer::window_state_informer( this_cref_t rhv ) 
{
    _states = rhv._states ;
}

//*************************************************************************************
window_state_informer::~window_state_informer( void_t ) 
{

}

//*************************************************************************************
window_state_informer::this_ref_t window_state_informer::operator = ( this_rref_t rhv ) 
{
    _states = rhv._states ;
    return *this ;
}

//*************************************************************************************
bool_t window_state_informer::has_screen_message( so_app::screen_dpi_message_ref_t msg ) const 
{
    msg = _states.dpi_msg ;
    return _states.dpi_msg_changed ;
}

//*************************************************************************************
bool_t window_state_informer::has_screen_message( so_app::screen_size_message_ref_t msg ) const 
{
    msg = _states.msize_msg ;
    return _states.msize_msg_changed ;
}

//*************************************************************************************
bool_t window_state_informer::has_resize_message( so_app::resize_message_ref_t msg ) const 
{
    msg = _states.resize_msg ;
    return _states.resize_changed ;
}

//*************************************************************************************
bool_t window_state_informer::has_show_message( so_app::show_message_ref_t /*msg*/ ) const
{
    return false ;
    //msg = _states.show_msg ;
    //return _states.show_changed ;
}

//*************************************************************************************
void_t window_state_informer::reset( bool_t b ) 
{
    _states.resize_changed = b ;
    _states.show_changed = b ;
    _states.dpi_msg_changed = b ;
    _states.msize_msg_changed = b ;
}

