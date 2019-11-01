
#pragma once

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"
#include "../structs.h"

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API ipage
    {

    public:

        virtual void_t on_load() noexcept = 0 ;
        virtual bool_t is_loaded( void_t ) noexcept = 0 ;

        virtual void_t on_unload() noexcept = 0 ;
        virtual bool_t is_unloaded( void_t ) noexcept = 0 ;

    public:

        virtual void_t on_update( update_data_in_t ) noexcept = 0 ;
        virtual void_t on_render( render_data_in_t ) noexcept = 0 ;
        virtual void_t destroy( void_t ) noexcept = 0 ;
    };
    so_typedef( ipage ) ;
}