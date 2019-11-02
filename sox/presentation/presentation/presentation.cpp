#include "presentation.h"

using namespace sox_presentation ;

//*********************************************************
bool_t presentation::page_info::on_load( void_t ) 
{
    if( !loaded ) loaded = pptr->on_load() ;
    return loaded ;
}

//*********************************************************
void_t presentation::page_info::on_unload( void_t ) 
{
    if( loaded ) 
    {
        loaded = so_core::is_not( pptr->on_unload() ) ;
    }
}

//*********************************************************
bool_t presentation::page_info::do_update( update_data_in_t ud ) 
{
    if( !loaded ) 
    {
        loaded = pptr->on_load() ;
        return loaded ;
    }
    
    pptr->on_update( ud ) ;

    return true ;
}

//*********************************************************
bool_t presentation::page_info::do_render( render_data_in_t rd ) 
{
    if( loaded ) 
    {
        pptr->on_render( rd ) ;
    }
    return loaded ;
}

//*********************************************************
presentation::presentation( void_t ) noexcept 
{}

//*********************************************************
presentation::presentation( this_rref_t rhv ) noexcept
{
    _pages = std::move( rhv._pages ) ;
    _transitions = std::move( rhv._transitions ) ;

    _cur_index = rhv._cur_index ;
    _tgt_index = rhv._tgt_index ;
}

//*********************************************************
presentation::~presentation( void_t ) noexcept
{
    for( auto & item : _pages )
    {
        item.pptr->destroy() ;
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
void_t presentation::render( void_t ) noexcept
{
    render_data_t rd ;

    // 1. do current page
    {
        this_t::cur_page( [&] ( page_info_ref_t pi )
        {
            pi.do_render( rd ) ;
        } ) ;
    }

    // 2. do transition
    {
        if( so_core::is_not_nullptr( this_t::cur_transition() ) && this_t::in_transition() )
        {
            std::chrono::seconds const dur = this_t::cur_transition()->get_duration() ;

            this_t::cur_transition()->on_render( rd ) ;
        }
    }

    // 3. do next page
    {
        this_t::nxt_page( [&] ( page_info_ref_t pi )
        {
            if( this_t::in_transition() )
                pi.do_render( rd ) ;
        } ) ;
    }
}

//*********************************************************
void_t presentation::update( void_t ) noexcept
{
    auto const dt = std::chrono::duration_cast<std::chrono::seconds>( 
        so_core::clock_t::now() - _utime )  ;

    _utime = so_core::clock_t::now() ;

    update_data_t ud ;

    // 1. do current page
    {
        this_t::cur_page( [&] ( page_info_ref_t pi )
        {
            pi.do_update( ud ) ;
        } ) ;
    }

    // 2. do transition
    {
        so_core::seconds_t dur = std::chrono::seconds(0) ;

        if( so_core::is_not_nullptr( this_t::cur_transition() ) )
        {
            if( this_t::cur_transition()->on_load() && this_t::in_transition() )
            {
                dur = this_t::cur_transition()->get_duration() ;

                this_t::cur_transition()->on_update( ud ) ;
            }
        }

        // is transition done?
        if( dur <= _tdur )
        {
            _cur_index = _tgt_index ;
        }
    }

    // 3. do next page
    {
        this_t::nxt_page( [&] ( page_info_ref_t pi )
        {
            pi.on_load() ;

            if( this_t::in_transition() )
                pi.do_update( ud ) ;
        } ) ;
    }

    // 4. do unload
    {
        size_t i ;
        if( this_t::cur_index( i ) && i > 2 )
        {
            _pages[ i - 2 ].on_unload() ;
        }
    }

    // 5. update transition duration
    {
        _tdur += dt ;
    }
    
}

//*********************************************************
void_t presentation::add_page( sox_presentation::ipage_utr_t pptr ) noexcept
{
    {
        auto const iter = std::find_if( _pages.begin(), _pages.end(), [&]( page_info_cref_t item )
        {
            return item.pptr == pptr ;
        } ) ;
        if( iter != _pages.end() )
            return ;
    }

    {
        this_t::page_info_t pi ;
        pi.loaded = false ;
        pi.pptr = pptr ;
        _pages.emplace_back( pi ) ;
    }

    if( _cur_index == size_t( -1 ) )
    {
        _cur_index = 0 ;
        _tgt_index = 0 ;
    }
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
    so_core::is_not( this_t::nxt_index( _tgt_index ) ) ;
    _tdur = std::chrono::seconds( 0 ) ;

    return _tgt_index != size_t( -1 ) ;
}

//*********************************************************
bool_t presentation::prev_page( void_t ) noexcept 
{
    this_t::prv_index( _tgt_index ) ;

    // no transition if going backwards
    _cur_index = _tgt_index ;
    _tdur = std::chrono::seconds( 0 ) ;

    return _tgt_index != size_t( -1 ) ;
}

//*********************************************************
bool_t presentation::change_page( size_t const ) noexcept 
{
    return false ;
}