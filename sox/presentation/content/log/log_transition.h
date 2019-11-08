
#pragma once

#include "../itransition.h"

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API log_transition : public itransition
    {
        so_this_typedefs( log_transition ) ;

    private:

        size_t _i = size_t( -1 ) ;
        bool_t _update = false ;
        bool_t _render = false ;

    public:

        log_transition( size_t const ) noexcept ;
        log_transition( this_cref_t ) = delete ;
        log_transition( this_rref_t ) noexcept ;
        virtual ~log_transition( void_t ) noexcept ;

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_utr_t ) ;

    public:

        virtual bool_t on_load() noexcept  ;
        virtual bool_t on_unload() noexcept  ;

        virtual bool_t on_init() noexcept ;
        virtual bool_t on_release() noexcept ;

    public:

        virtual std::chrono::milliseconds get_duration( void_t ) noexcept ;
        virtual void_t on_update( sox_presentation::update_data_in_t ) noexcept ;
        virtual void_t on_render( render_type const, sox_presentation::render_data_in_t ) noexcept  ;
        virtual void_t destroy( void_t ) noexcept ;
    };
    so_typedef( log_transition ) ;
}