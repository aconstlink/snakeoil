//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPX_MICRO_CONTEXT_H_
#define _SNAKEOIL_APPX_MICRO_CONTEXT_H_

#include "iappx_context.h"

namespace so_appx
{
    class appx_context : public so_appx::iappx_context
    {
        so_this_typedefs( appx_context ) ;
        
    private:

        so_gpx::render_system_ptr_t _rsys_ptr = nullptr ;
        so_imex::isystem_ptr_t _imexs_ptr = nullptr ;

    private:

        appx_context( this_cref_t ) {}

    public:

        appx_context( void_t ) ;
        appx_context( this_rref_t ) ;
        
        so_appx::result set_render_system( so_gpx::render_system_ptr_t ) ;
        so_appx::result set_imexs( so_imex::isystem_ptr_t ) ;

    public: // interface

        virtual so_gpx::render_system_ptr_t render_system( void_t ) ;
        virtual so_imex::isystem_ptr_t  imexsys( void_t ) ;
        
    };
}

#endif

