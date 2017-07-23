//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_SCENE_SCENE_CREATOR_CONTEXT_H_
#define _SNAKEOIL_IMEX_SCENE_SCENE_CREATOR_CONTEXT_H_

#include "../typedefs.h"

#include <snakeoil/shade/protos.h>
#include <snakeoil/std/container/map.hpp>

#include <snakeoil/thread/mutex.h>

namespace so_scene
{
    class scene_creator_context
    {
        so_this_typedefs( scene_creator_context ) ;

        typedef so_std::map< so_std::string_t, so_shade::so_shader::effect_ptr_t > __effects_t ;
        so_typedefs( __effects_t, effects ) ;

    private:

        mutable so_thread::mutex_t _mtx_effects ;
        effects_t _effects ;

    public:

        scene_creator_context( void_t ) ;
        scene_creator_context( this_rref_t ) ;
        ~scene_creator_context( void_t ) ;

    public:

        bool_t reserve_effect( so_std::string_in_t ) ;
        bool_t set_reserved_effect( so_std::string_in_t, so_shade::so_shader::effect_ptr_t ) ;

        bool_t add_effect( so_std::string_in_t, so_shade::so_shader::effect_ptr_t ) ;
        bool_t get_effect( so_std::string_in_t, so_shade::so_shader::effect_ptr_t & ) ;
    };
    so_typedef( scene_creator_context ) ;
}

#endif
