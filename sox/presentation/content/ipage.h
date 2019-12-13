
#pragma once

#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"
#include "../structs.h"

#include <snakeoil/gpx/protos.h>

namespace sox_presentation
{
    class SNAKEOILX_PRESENTATION_API ipage
    {

    public:

        // early call for loading the page's assets
        virtual bool_t on_load() noexcept = 0 ;
        virtual bool_t on_unload() noexcept = 0 ;

        // before update starts
        virtual bool_t on_init() noexcept = 0 ;

        // after update ends
        virtual bool_t on_release() noexcept = 0 ;

    public:

        virtual void_t on_update( update_data_in_t ) noexcept = 0 ;
        virtual void_t on_render( render_data_in_t ) noexcept = 0 ;
        virtual void_t destroy( void_t ) noexcept = 0 ;
    };
    so_typedef( ipage ) ;

    class SNAKEOILX_PRESENTATION_API page : public ipage
    {
        so_this_typedefs( page ) ;

    private:

        so_gpx::render_system_ptr_t _rs = nullptr ;

    protected:

        so_gpx::render_system_ptr_t render_system( void_t ) noexcept 
        {
            return _rs ;
        }

    public:

        page( so_gpx::render_system_ptr_t rs ) noexcept : _rs(rs) {}
        page( this_cref_t ) = delete ;
        page( this_rref_t rhv ) noexcept { so_move_member_ptr( _rs, rhv ) ; }
        virtual ~page( void_t ) noexcept {}

    public:

        // early call for loading the page's assets
        virtual bool_t on_load() noexcept { return true ; }
        virtual bool_t on_unload() noexcept { return true ; }

        // before update starts
        virtual bool_t on_init() noexcept { return true ; }

        // after update ends
        virtual bool_t on_release() noexcept { return true ; }

    public:

        virtual void_t on_update( update_data_in_t ) noexcept {}
        virtual void_t on_render( render_data_in_t ) noexcept {}
        virtual void_t destroy( void_t ) noexcept {}
    };
    so_typedef( page ) ;
}