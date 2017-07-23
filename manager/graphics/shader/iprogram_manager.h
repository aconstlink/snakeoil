//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MANAGER_GRAPHICS_SHADER_IPROGRAM_MANAGER_H_
#define _SNAKEOIL_MANAGER_GRAPHICS_SHADER_IPROGRAM_MANAGER_H_

#include "../../api.h"
#include "../../result.h"
#include "../../typedefs.h"
#include "../../protos.h"

#include <snakeoil/gpu/result.h>
#include <snakeoil/gpu/protos.h>

namespace so_manager
{
    namespace so_graphics
    {
        class SNAKEOIL_MANAGER_API iprogram_manager
        {
        public:

            struct shader_keys
            {
                /// vertex shader key
                std::string vs_key ;
                /// geometry shader key
                std::string gs_key ;
                /// pixel shader key
                std::string ps_key ;
            };

            typedef shader_keys const & shader_keys_cref_t ;

        public:

            virtual so_manager::result compose_program( 
                so_manager::key_cref_t, shader_keys_cref_t shk ) = 0 ;

            virtual so_manager::result link_program( 
                so_manager::key_cref_t key_in )  = 0 ;
            
            virtual so_manager::result compose_and_link_program( 
                so_manager::key_cref_t, shader_keys_cref_t shk ) = 0;

            virtual so_gpu::result get_link_status( 
                so_manager::key_cref_t ) = 0 ;

            virtual so_gpu::result wait_for_link( 
                so_manager::key_cref_t ) = 0 ;

            virtual so_gpu::program_ptr_t get_program( 
                so_manager::key_cref_t ) = 0 ;

        public: // config

            virtual so_manager::key_t compose_unique_config_key( so_manager::key_cref_t ) = 0 ;

            virtual so_manager::result compose_config( 
                so_manager::key_cref_t config_key_in, so_manager::key_cref_t prog_key_in ) = 0 ;
            
            virtual so_gpu::config_ptr_t get_config( 
                so_manager::key_cref_t ) = 0 ;

        public: // shutdown

            virtual so_manager::result destroy( void_t ) = 0 ;
            virtual so_manager::result shutdown( void_t ) = 0 ;
            virtual so_manager::result has_shutdown( void_t ) = 0 ;
        };
    }
}

#endif
