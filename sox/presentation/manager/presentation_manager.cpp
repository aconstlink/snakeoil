#include "presentation_manager.h"

using namespace sox_presentation ;

//*********************************************************
presentation_manager::presentation_manager( void_t ) noexcept 
{}

//*********************************************************
presentation_manager::presentation_manager( this_rref_t rhv ) noexcept
{
    _pages = std::move( rhv._pages ) ;
    _transitions = std::move( rhv._transitions ) ;

    so_move_member_ptr( _cur_page, rhv ) ;
    so_move_member_ptr( _nxt_page, rhv ) ;
    so_move_member_ptr( _transition, rhv ) ;
}

//*********************************************************
presentation_manager::~presentation_manager( void_t ) noexcept
{
    for( auto * ptr : _pages )
    {
        ptr->destroy() ;
    }

    for( auto * ptr : _transitions ) 
    {
        ptr->destroy() ;
    }

    _cur_page = nullptr ;
    _nxt_page = nullptr ;
    _transition = nullptr ;

}

//*********************************************************
presentation_manager::this_ptr_t presentation_manager::create( so_memory::purpose_cref_t p ) noexcept
{
    return so_memory::global_t::alloc( this_t(), p ) ;
}

//*********************************************************
void_t presentation_manager::destroy( this_ptr_t ptr ) noexcept
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*********************************************************
void_t presentation_manager::render( render_data_in_t rd ) noexcept
{
    // 1. do current page
    {
        if( so_core::is_nullptr( _cur_page ) )
        {
            return ;
        }

        if( _cur_page->is_loaded() )
            _cur_page->on_render( rd ) ;
    }

    // 2. do transition
    {
        if( so_core::is_not_nullptr( _transition ) )
        {
            std::chrono::seconds const dur = _transition->get_duration() ;

            _transition->on_render( rd ) ;
        }
    }

    // 3. do next page
    {
        if( so_core::is_not_nullptr( _nxt_page ) )
        {
            _nxt_page->on_render( rd ) ;
        }
    }
}

//*********************************************************
void_t presentation_manager::update( update_data_in_t ud ) noexcept
{
    // 1. do current page
    {
        if( so_core::is_nullptr( _cur_page ) )
        {
            if( _pages.size() == 0 ) return ;
            _cur_page = _pages[ 0 ] ;
            _cur_page->on_load() ;
        }

        if( _cur_page->is_loaded() )
            _cur_page->on_update( ud ) ;
    }

    // 2. do transition
    {
        if( so_core::is_not_nullptr( _transition ) )
        {
            so_core::seconds_t const dur = _transition->get_duration() ;

            _transition->on_update( ud ) ;
        }
    }

    // 3. do next page
    {
        if( so_core::is_not_nullptr( _nxt_page ) )
        {
            _nxt_page->on_update( ud ) ;
        }
    }
}

//*********************************************************
void_t presentation_manager::add_page( sox_presentation::ipage_utr_t pptr ) noexcept
{
    {
        auto const iter = std::find( _pages.begin(), _pages.end(), pptr ) ;
        if( iter != _pages.end() )
            return ;
    }

    _pages.emplace_back( pptr ) ;
}

//*********************************************************
void_t presentation_manager::add_transition( sox_presentation::itransition_utr_t ptr ) noexcept 
{
    {
        auto const iter = std::find( _transitions.begin(), _transitions.end(), ptr ) ;
        if( iter != _transitions.end() )
            return ;
    }

    _transitions.emplace_back( ptr ) ;
}