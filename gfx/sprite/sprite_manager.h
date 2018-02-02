//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../typedefs.h"

#include "../image/image_render_2d.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/map.hpp>
#include <snakeoil/math/vector/vector2.hpp>

namespace so_gfx
{
    class SNAKEOIL_GFX_API sprite_manager
    {
        so_this_typedefs( sprite_manager ) ;

    private:

        so_gfx::image_render_2d_ptr_t _img_rnd = nullptr ;

    public:

        struct sprite_info
        {
            so_math::vec4ui_t rect ;
            so_math::vec2i_t pivot ;

            uint_t xl( void_t ) const { return rect.x() ; }
            uint_t yb( void_t ) const { return rect.y() ; }
            uint_t xr( void_t ) const { return rect.z() ; }
            uint_t yt( void_t ) const { return rect.w() ; }
        };
        so_typedef( sprite_info ) ;
        so_typedefs( so_std::vector< sprite_info_t >, sprite_infos ) ;

        struct sprite_data
        {
            so_gfx::image_render_2d_t::image_id_t img_id ;
            so_math::vec2f_t scale ;
            so_math::vec2f_t pivot ;
        };
        so_typedef( sprite_data ) ;
        so_typedefs( so_std::vector< sprite_data_t >, sprite_datas ) ;

    private:

        struct sprite_sequence_data
        {
            sprite_datas_t sds ;
        };
        so_typedef( sprite_sequence_data ) ;
        typedef so_std::map< so_std::string_t, sprite_sequence_data > __sprite_sequence_datas_t ;
        so_typedefs( __sprite_sequence_datas_t, sprite_sequence_datas ) ;

        so_thread::mutex_t _mtx_ssds ;
        sprite_sequence_datas_t _ssds ;

    public:

        sprite_manager( so_gfx::image_render_2d_ptr_t ) ;
        sprite_manager( this_cref_t ) = delete ;
        sprite_manager( this_rref_t ) ;
        ~sprite_manager( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_gfx::result add_sequence( so_imex::image_ptr_t, so_std::string_cref_t, sprite_infos_in_t ) ;

        bool_t get_sequence( so_std::string_cref_t, sprite_datas_out_t ) const ;
    };
    so_typedef( sprite_manager ) ;
}