#pragma once

#include "../content/ipage.h"
#include "../content/itransition.h"

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API presentation
    {
        so_this_typedefs( presentation ) ;

    private:

        struct page_info
        {
            ipage_ptr_t pptr = nullptr ;
            bool_t loaded = false ;
            bool_t inited = false ;

            bool_t on_load( void_t ) ;
            void_t on_unload( void_t ) ;
            bool_t do_update( update_data_in_t ) ;
            bool_t do_render( render_data_in_t ) ;
            bool_t on_init( void_t ) ;
            bool_t on_release( void_t ) ;
            
        };
        so_typedef( page_info ) ;

        typedef std::function< void_t ( page_info_ref_t ) > on_page_t ;

        so_typedefs( so_std::vector< page_info_t >, pages ) ;
        pages_t _pages ;


        struct transition_info
        {
            itransition_ptr_t pptr = nullptr ;
            bool_t loaded = false ;

            bool_t on_load( void_t ) ;
            void_t on_unload( void_t ) ;
            bool_t do_update( update_data_in_t ) ;
            bool_t do_render( render_data_in_t ) ;

        };
        so_typedef( transition_info ) ;

        typedef std::function< void_t ( transition_info_ref_t ) > on_transition_t ;

        so_typedefs( so_std::vector< transition_info_t >, transitions ) ;
        transitions_t _transitions ;

        so_core::clock_t::time_point _transition_started ;

        size_t _cur_index = size_t( -1 ) ;
        size_t _tgt_index = size_t( -1 ) ;

        // the current transition time.
        std::chrono::microseconds _tdur = std::chrono::microseconds(0) ;


        so_core::clock_t::time_point _utime = 
            so_core::clock_t::now() ;

        bool_t _abort_transition = false ;

    private:

        bool_t cur_index( size_t & i ) const noexcept
        {
            if( _cur_index == size_t( -1 ) )
                return false ;

            i = _cur_index ;

            return true ;
        }

        bool_t nxt_index( size_t & i ) const noexcept
        {
            if( _cur_index == (_pages.size() - 1) )
            {
                return false ;
            }

            i = _cur_index + 1 ;

            return true ;
        }

        bool_t prv_index( size_t & i ) const noexcept
        {
            if( _cur_index == 0 )
            {
                return false ;
            }
            i = _cur_index - 1 ;
            return true ;
        }

        bool_t cur_page( on_page_t f ) noexcept
        {
            size_t i ;
            if( so_core::is_not( this_t::cur_index( i ) ) )
                return false ;

            f( _pages[ i ] ) ;
            
            return true ;
        }

        bool_t nxt_page( on_page_t f ) noexcept
        {
            size_t i ;
            if( so_core::is_not( this_t::nxt_index( i ) ) )
                return false ;

            f( _pages[ i ] ) ;

            return true ;
        }

        bool_t prv_page( on_page_t f ) noexcept
        {
            size_t i ;
            if( so_core::is_not( this_t::prv_index( i ) ) )
                return false ;

            f( _pages[ i ] ) ;

            return true ;
        }

        bool_t tgt_page( on_page_t f ) noexcept
        {
            if( so_core::is_not( this_t::in_transition() ) )
                return false ;

            size_t const i = _tgt_index ;
            if( i == size_t(-1) )
                return false ;

            f( _pages[ i ] ) ;

            return true ;
        }

        bool_t cur_transition( on_transition_t f ) noexcept
        {
            size_t i ;
            if( so_core::is_not( this_t::cur_index( i ) ) )
                return false ;

            if( _transitions.size() == 0 )
                return nullptr ;

            if( _transitions.size() >= i )
            {
                i = 0 ;
            }

            f( _transitions[ i ] ) ;

            return true ;
        }

        bool_t in_transition( void_t ) const noexcept
        {
            return _cur_index != _tgt_index ;
        }

        void_t change_to_target( void_t ) noexcept ;

    public:

        presentation( void_t ) noexcept ;
        presentation( this_cref_t ) = delete ;
        presentation( this_rref_t ) noexcept ;
        ~presentation( void_t ) noexcept ;

        static this_ptr_t create( so_memory::purpose_cref_t ) noexcept ;
        static void_t destroy( this_ptr_t ) noexcept ;

    public:

        void_t render( void_t ) noexcept ;
        void_t update( void_t ) noexcept ;

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