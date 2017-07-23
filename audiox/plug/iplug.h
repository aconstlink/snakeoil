//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_API_IPLUG_H_
#define _SNAKEOIL_AUDIOX_API_IPLUG_H_

#include "../protos.h"
#include "../typedefs.h"
#include "../api.h"
#include "../result.h"

#include "plug_result.h"

namespace so_audiox
{
    class SNAKEOIL_AUDIOX_API iplug
    {

    public:

        struct execute_info
        {
            double_t dt ;
        };
        so_typedef( execute_info ) ;

    public:

        /// load required assets
        virtual so_audiox::plug_result on_load( void_t ) = 0 ;

        /// unload assets
        virtual so_audiox::plug_result on_unload( void_t ) = 0 ;

    public:

        /// do all api object init
        virtual so_audiox::plug_result on_initialize( void_t ) = 0 ;

        /// release all api objects
        virtual so_audiox::plug_result on_release( void_t ) = 0 ;

        /// do all gpu/cpu transfer
        /// this is the place where user provided data from
        /// on_update can be transfered to/from the gpu.
        virtual so_audiox::plug_result on_transfer( void_t ) = 0 ;

        /// do all api execution commands
        virtual so_audiox::plug_result on_execute( execute_info_cref_t ) = 0 ;

    public: // user/api side

            /// update api buffers/images with user data
            /// this function gives the user a time slice where
            /// all its data can be brought into the api space
        virtual so_audiox::plug_result on_update( void_t ) = 0 ;


    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iplug ) ;
}

#endif