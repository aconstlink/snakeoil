#include "presentation.h"

using namespace sox_presentation ;

//*********************************************************
presentation::presentation( void_t ) noexcept 
{}

//*********************************************************
presentation::presentation( this_rref_t rhv ) noexcept
{
    _pages = std::move( rhv._pages ) ;
    _transitions = std::move( rhv._transitions ) ;

    _cur_index = rhv._cur_index ;
}

//*********************************************************
presentation::~presentation( void_t ) noexcept
{
    for( auto * ptr : _pages )
    {
        ptr->destroy() ;
    }

    for( auto * ptr : _transitions ) 
    {
        ptr->destroy() ;
    }
}

//*********************************************************
presentation::this_ptr_t presentation::create( so_memory::purpose_cref_t p ) noexcept
{
    return so_memory::global_t::alloc( this_t(), p ) ;
}

//*********************************************************
void_t presentation::destroy( this_ptr_t ptr ) noexcept
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*********************************************************
void_t presentation::render( render_data_in_t rd ) noexcept
{
    // 1. do current page
    {
        if( so_core::is_nullptr( this_t::cur_page() ) )
        {
            return ;
        }

        if( this_t::cur_page()->on_load() )
        {
            this_t::cur_page()->on_render( rd ) ;
        }
    }

    // 2. do transition
    {
        if( so_core::is_not_nullptr( this_t::cur_transition() ) )
        {
            std::chrono::seconds const dur = this_t::cur_transition()->get_duration() ;

            this_t::cur_transition()->on_render( rd ) ;
        }
    }

    // 3. do next page
    {
        if( so_core::is_not_nullptr( this_t::nxt_page() ) )
        {
            this_t::nxt_page()->on_render( rd ) ;
        }
    }
}

//*********************************************************
void_t presentation::update( update_data_in_t ud ) noexcept
{
    auto const dt = std::chrono::duration_cast<std::chrono::seconds>( 
        so_core::clock_t::now() - _utime )  ;

    _utime = so_core::clock_t::now() ;


    
    // 1. do current page
    {
        if( so_core::is_nullptr( this_t::cur_page() ) )
        {
            return ;
        }

        if( this_t::cur_page()->on_load() )
            this_t::cur_page()->on_update( ud ) ;
    }

    // 2. do transition
    {
        if( so_core::is_not_nullptr( this_t::cur_transition() ) )
        {
            if( this_t::cur_transition()->on_load() )
            {
                so_core::seconds_t const dur = this_t::cur_transition()->get_duration() ;

                this_t::cur_transition()->on_update( ud ) ;
            }
        }
    }

    // 3. do next page
    {
        if( so_core::is_not_nullptr( this_t::nxt_page() ) )
        {
            if( this_t::nxt_page()->on_load() )
            {
                // if transition ongoing and loaded, call on_update on next page
            }
        }
    }
}

//*********************************************************
void_t presentation::add_page( sox_presentation::ipage_utr_t pptr ) noexcept
{
    {
        auto const iter = std::find( _pages.begin(), _pages.end(), pptr ) ;
        if( iter != _pages.end() )
            return ;
    }

    _pages.emplace_back( pptr ) ;

    if( _cur_index == size_t( -1 ) )
        _cur_index = 0 ;
}

//*********************************************************
void_t presentation::add_transition( sox_presentation::itransition_utr_t ptr ) noexcept 
{
    {
        auto const iter = std::find( _transitions.begin(), _transitions.end(), ptr ) ;
        if( iter != _transitions.end() )
            return ;
    }

    _transitions.emplace_back( ptr ) ;
}

//*********************************************************
bool_t presentation::next_page( void_t ) noexcept 
{
    _tgt_index = this_t::nxt_index() ;
    _ttime = so_core::clock_t::time_point( std::chrono::seconds( 0 ) ) ;

    return _tgt_index != size_t( -1 ) ;
}

//*********************************************************
bool_t presentation::prev_page( void_t ) noexcept 
{
    _tgt_index = this_t::prv_index() ;
    _ttime = so_core::clock_t::time_point( std::chrono::seconds( 0 ) ) ;

    return _tgt_index != size_t( -1 ) ;
}

//*********************************************************
bool_t presentation::change_page( size_t const ) noexcept 
{
    return false ;
}