//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_SHADER_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_SHADER_MANAGER_H_

#include "asset_manager.h"
#include "resource_manager.hpp"

#include "../shader/vertex_shader.h"
#include "../shader/pixel_shader.h"
#include "../shader/effect.h"

#include <snakeoil/shade/protos.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API shader_manager : public so_imex::asset_manager
    {
        so_this_typedefs( shader_manager ) ;
        
        so_typedefs( so_imex::resource_manager<so_imex::vertex_shader>, vs_manager ) ;
        so_typedefs( vs_manager_t::handle_t, handle_vs ) ;

    private:

        vs_manager_t _vs_manager ;

    public:

        shader_manager( void_t ) ;
        shader_manager( this_rref_t  ) ;
        virtual ~shader_manager( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // vertex shader

        bool_t acquire_by_path( so_io::path_cref_t path_in,
            so_resource::purpose_cref_t p, handle_vs_ref_t hnd_out ) ;

        bool_t acquire( so_imex::key_cref_t key_in,
            so_resource::purpose_cref_t p, handle_vs_ref_t hnd_out ) ;

        bool_t release( handle_vs_rref_t hnd ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
}

#endif
