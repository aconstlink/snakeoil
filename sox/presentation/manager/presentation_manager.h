#pragma once

#include "../content/ipage.h"
#include "../content/itransition.h"

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API presentation_manager
    {
        so_this_typedefs( presentation_manager ) ;

    private:

        so_typedefs( so_std::vector< sox_presentation::ipage_utr_t >, pages ) ;
        pages_t _pages ;

        so_typedefs( so_std::vector< sox_presentation::itransition_utr_t >, transitions ) ;
        transitions_t _transitions ;

        sox_presentation::ipage_str_t _cur_page = nullptr ;
        sox_presentation::ipage_str_t _nxt_page = nullptr ;
        sox_presentation::itransition_str_t _transition = nullptr ;

        so_core::clock_t::time_point _transition_started ;

    public:

        presentation_manager( void_t ) noexcept ;
        presentation_manager( this_cref_t ) = delete ;
        presentation_manager( this_rref_t ) noexcept ;
        ~presentation_manager( void_t ) noexcept ;

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
        void_t next_page( void_t ) noexcept ;


        /// change to the previous page
        void_t prev_page( void_t ) noexcept ;

        /// change to a specific page 
        void_t change_page( size_t const ) noexcept ;

    };
    so_typedef( presentation_manager ) ;
}