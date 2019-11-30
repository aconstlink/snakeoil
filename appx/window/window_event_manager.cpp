
#include "window_event_manager.h"


using namespace so_appx ;

//***************************************************
window_event_manager::window_event_manager( void_t ) 
{
}

//***************************************************
window_event_manager::window_event_manager( this_rref_t rhv )
{
    _datas = std::move( rhv._datas ) ;
}

//***************************************************
window_event_manager::~window_event_manager( void_t )
{
}

//***************************************************
void_t window_event_manager::toggle_vsync( size_t const wid )
{
    auto iter = std::find_if( _datas.begin(), _datas.end(),
        [&] ( window_event_manager_t::data_in_t d )
    {
        return d.wid == wid ;
    } ) ;
    if( iter == _datas.end() )
    {
        _datas.emplace_back( this_t::data_t( wid ) ) ;
        iter = --_datas.end() ;
    }

    iter->toggle_vsync = !iter->toggle_vsync ;
}

//***************************************************
void_t window_event_manager::toggle_fullscreen( size_t const wid )
{
    auto iter = std::find_if( _datas.begin(), _datas.end(),
        [&] ( data_in_t d )
    {
        return d.wid == wid ;
    } ) ;

    if( iter == _datas.end() )
    {
        _datas.emplace_back( this_t::data_t( wid ) ) ;
        iter = --_datas.end() ;
    }

    iter->toggle_fullscreen = !iter->toggle_fullscreen ;
}

//***************************************************
window_event_manager::data_t window_event_manager::swap( size_t const wid )
{
    auto iter = std::find_if( _datas.begin(), _datas.end(),
        [&] ( data_in_t d )
    {
        return d.wid == wid ;
    } ) ;

    if( iter == _datas.end() )
        return this_t::data_t() ;

    this_t::data_t const ret = *iter ;
    *iter = this_t::data_t() ;

    return ret ;
}

//***************************************************
window_event_manager::this_ptr_t window_event_manager::create( void_t ) 
{
    return so_memory::global_t::alloc( this_t(), "[window_event_manager::create]" ) ;
}

//***************************************************
void_t window_event_manager::destroy( this_ptr_t ptr ) 
{
    so_memory::global_t::dealloc( ptr ) ;
}