#pragma once

#include "window_state_setter.h"

#include <snakeoil/std/container/vector.hpp>
#include <algorithm>

namespace so_appx
{
    class  window_event_manager
    {
        so_this_typedefs( window_event_manager ) ;
        friend class window_state_setter ;

    private:

        struct data
        {
            size_t wid = size_t( -1 );
            bool_t toggle_fullscreen = false ;
            bool_t toggle_vsync = false ;

            data( void_t ) {}
            data( size_t const wid_ ) { wid = wid_ ; }

            data & operator = ( data const & rhv )
            {
                wid = rhv.wid ;
                toggle_vsync = rhv.toggle_vsync ;
                toggle_fullscreen = rhv.toggle_fullscreen ;
                return *this ;
            }
        };
        so_typedef( data ) ;
        std::vector< data_t > _datas ;
        
    private:

        void_t toggle_vsync( size_t const wid ) ;
        void_t toggle_fullscreen( size_t const wid ) ;

    public:

        window_event_manager( void_t ) ;
        window_event_manager( this_rref_t ) ;
        window_event_manager( this_cref_t ) = delete ;
        ~window_event_manager( void_t ) ;

        this_t::data_t swap( size_t const wid ) ;

    public:

        static this_ptr_t create( void_t ) ;
        static void_t destroy( this_ptr_t ) ;

    };
    so_typedef( window_event_manager ) ;
}