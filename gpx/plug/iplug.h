//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_API_IPLUG_H_
#define _SNAKEOIL_GPX_API_IPLUG_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"
#include "../result.h"

#include "plug_result.h"

#include <snakeoil/gpu/protos.h>

namespace so_gpx
{
    class SNAKEOIL_GPX_API iplug
    {
    public:

        struct create_info
        {
            so_gpx::iglobal_service_ptr_t gs_ptr = nullptr ;
            so_gpx::ilocal_service_ptr_t ls_ptr = nullptr ;
        };
        so_typedef( create_info ) ;

        struct execute_info
        {
            size_t rnd_id ;
            so_gpu::gpu_manager_ptr_t mgr = nullptr ;
        };
        so_typedef( execute_info ) ;

        struct update_info
        {
        };
        so_typedef( update_info ) ;

        struct init_info
        {
            bool_t reinit = false ;
            so_gpu::gpu_manager_ptr_t mgr = nullptr ;
        };
        so_typedef( init_info ) ;

        struct load_info
        {
            bool_t reload = false ;
        };
        so_typedef( load_info ) ;

    public:

        /// called right after the plug is created
        virtual so_gpx::plug_result on_create( create_info_cref_t ) = 0 ;

    public:

        /// load required assets
        virtual so_gpx::plug_result on_load( load_info_cref_t ) = 0 ;

        /// unload assets
        virtual so_gpx::plug_result on_unload( void_t ) = 0 ;

    public:

        /// do all api object init
        virtual so_gpx::plug_result on_initialize( init_info_cref_t ) = 0 ;

        /// release all api objects
        virtual so_gpx::plug_result on_release( void_t ) = 0 ;

        /// do all gpu object update and gpu/cpu transfer
        /// this is the place where user provided data from
        /// on_update can be transfered to/from the gpu.
        virtual so_gpx::plug_result on_update( update_info_cref_t ) = 0 ;
        
        

        /// do all api execution commands
        virtual so_gpx::plug_result on_execute( execute_info_cref_t ) = 0 ;
            

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iplug ) ;
}

#endif