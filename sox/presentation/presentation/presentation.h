#pragma once

#include "../content/ipage.h"
#include "../content/itransition.h"

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API presentation
    {
        so_this_typedefs( presentation ) ;

    private:

        so_typedefs( so_std::vector< sox_presentation::ipage_utr_t >, pages ) ;
        pages_t _pages ;

        so_typedefs( so_std::vector< sox_presentation::itransition_utr_t >, transitions ) ;
        transitions_t _transitions ;

        so_core::clock_t::time_point _transition_started ;

        size_t _cur_index = size_t( -1 ) ;
        size_t _tgt_index = size_t( -1 ) ;

        // the current transition time.
        so_core::clock_t::time_point _ttime = 
            so_core::clock_t::time_point( std::chrono::seconds(0) ) ;


        so_core::clock_t::time_point _utime = 
            so_core::clock_t::now() ;

    private:

        size_t cur_index( void_t ) const noexcept
        {
            return _cur_index ;
        }

        size_t nxt_index( void_t ) const noexcept
        {
            if( _cur_index == (_pages.size() - 1) )
            {
                return size_t( -1 ) ;
            }
            return _cur_index + 1 ;
        }

        size_t prv_index( void_t ) const noexcept
        {
            if( _cur_index == 0 )
            {
                return size_t( -1 ) ;
            }
            return _cur_index - 1 ;
        }

        ipage_utr_t cur_page( void_t ) noexcept
        {
            if( this_t::cur_index() == size_t( -1 ) )
                return nullptr ;

            return _pages[ this_t::cur_index() ] ;
        }

        ipage_utr_t nxt_page( void_t ) noexcept
        {
            if( this_t::nxt_index() == size_t( -1 ) )
                return nullptr ;

            return _pages[ this_t::nxt_index() ] ;
        }

        ipage_utr_t prc_page( void_t ) noexcept
        {
            if( this_t::prv_index() == size_t( -1 ) )
                return nullptr ;

            return _pages[ this_t::prv_index() ] ;
        }

        itransition_utr_t cur_transition( void_t ) noexcept
        {
            if( this_t::cur_index() == size_t( -1 ) )
                return nullptr ;

            if( _transitions.size() == 0 )
                return nullptr ;

            return _transitions[ this_t::cur_index() ] ;
        }

        bool_t in_transition( void_t ) const noexcept
        {
            return _cur_index != _tgt_index ;
        }

    public:

        presentation( void_t ) noexcept ;
        presentation( this_cref_t ) = delete ;
        presentation( this_rref_t ) noexcept ;
        ~presentation( void_t ) noexcept ;

        static this_ptr_t create( so_memory::purpose_cref_t ) noexcept ;
        static void_t destroy( this_ptr_t ) noexcept ;

    public:

        void_t render( render_data_in_t ) noexcept ;
        void_t update( update_data_in_t ) noexcept ;

    public:

        void_t add_page( sox_presentation::ipage_utr_t ) noexcept ;
        void_t add_transition( sox_presentation::itransition_utr_t ) noexcept ;

    public:

        /// change to the next page
        bool_t next_page( void_t ) noexcept ;


        /// change to the previous page
        bool_t prev_page( void_t ) noexcept ;

        /// change to a specific page 
        bool_t change_page( size_t const ) noexcept ;

    };
    so_typedef( presentation ) ;
}