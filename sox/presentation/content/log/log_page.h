
#pragma once

#include "../ipage.h"

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API log_page : public ipage
    {
        so_this_typedefs( log_page ) ;

    private:

        size_t _i = size_t( -1 ) ;
        bool_t _update = false ;
        bool_t _render = false ;

    public: 

        log_page( size_t const ) noexcept ;
        log_page( this_cref_t ) = delete ;
        log_page( this_rref_t ) noexcept ;
        virtual ~log_page( void_t ) noexcept ;

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_utr_t ) ;

    public:

        virtual bool_t on_load() noexcept  ;
        virtual bool_t on_unload() noexcept  ;

    public:

        virtual void_t on_update( sox_presentation::update_data_in_t ) noexcept ;
        virtual void_t on_render( sox_presentation::render_data_in_t ) noexcept  ;
        virtual void_t destroy( void_t ) noexcept ;
    };
    so_typedef( log_page ) ;
}