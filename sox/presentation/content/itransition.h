#pragma once

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../structs.h"

#include <snakeoil/gpx/protos.h>

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

        virtual std::chrono::milliseconds get_duration( void_t ) const noexcept = 0 ;

    public:

        virtual bool_t on_load() noexcept = 0 ;
        virtual bool_t on_unload() noexcept = 0 ;

        // before update starts
        virtual bool_t on_init() noexcept = 0 ;

        // after update ends
        virtual bool_t on_release() noexcept = 0 ;

    public:

        virtual void_t on_update( update_data_in_t ) noexcept = 0 ;
        virtual bool_t on_clear( void_t ) const noexcept = 0 ;
        virtual void_t on_render( render_type const, render_data_in_t ) noexcept = 0 ;
        virtual void_t destroy( void_t ) noexcept = 0 ;
    };
    so_typedef( itransition ) ;


    class SNAKEOILX_PRESENTATION_API transition : public itransition
    {
        so_this_typedefs( transition ) ;

    private:

        so_gpx::render_system_ptr_t _rs = nullptr ;

    protected:

        so_gpx::render_system_ptr_t render_system( void_t ) noexcept
        {
            return _rs ;
        }

    public:

        transition( so_gpx::render_system_ptr_t rs ) noexcept : _rs( rs ) {}
        transition( this_cref_t ) = delete ;
        transition( this_rref_t rhv ) noexcept { so_move_member_ptr( _rs, rhv ) ; }
        virtual ~transition( void_t ) noexcept {}

    public:

        virtual std::chrono::milliseconds get_duration( void_t ) const noexcept 
        { 
            return std::chrono::milliseconds( 0 ) ;
        }

        // early call for loading the page's assets
        virtual bool_t on_load() noexcept { return true ; }
        virtual bool_t on_unload() noexcept { return true ; }

        // before update starts
        virtual bool_t on_init() noexcept { return true ; }

        // after update ends
        virtual bool_t on_release() noexcept { return true ; }

    public:

        virtual void_t on_update( update_data_in_t ) noexcept {}
        virtual void_t on_render( render_type const, render_data_in_t ) noexcept {}
        virtual void_t destroy( void_t ) noexcept {}
    };
    so_typedef( transition ) ;
}