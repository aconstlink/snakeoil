#pragma once

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../structs.h"

#include <chrono>

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API itransition
    {
    public:

        enum class render_type
        {
            scene,
            mask
        };

    public:

        virtual std::chrono::milliseconds get_duration( void_t ) noexcept = 0 ;

    public:

        virtual bool_t on_load() noexcept = 0 ;
        virtual bool_t on_unload() noexcept = 0 ;

        // before update starts
        virtual bool_t on_init() noexcept = 0 ;

        // after update ends
        virtual bool_t on_release() noexcept = 0 ;

    public:

        virtual void_t on_update( update_data_in_t ) noexcept = 0 ;
        virtual void_t on_render( render_type const, render_data_in_t ) noexcept = 0 ;
        virtual void_t destroy( void_t ) noexcept = 0 ;
    };
    so_typedef( itransition ) ;
}