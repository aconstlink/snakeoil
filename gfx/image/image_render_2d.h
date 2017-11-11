//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#pragma once

#include "../protos.h"
#include "../result.h"
#include "../api.h"

#include "image_render_2d_shared_data.h"

#include <snakeoil/gpx/protos.h>
#include <snakeoil/gpx/window/window_id.h>
#include <snakeoil/gpx/technique/technique_id.h>
#include <snakeoil/gpu/viewport/viewport_2d.h>

#include <snakeoil/font/protos.h>

#include <snakeoil/imex/protos.h>

#include <snakeoil/io/typedefs.h>

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/math/vector/vector2.hpp>
#include <snakeoil/math/vector/vector3.hpp>

#include <functional>

namespace so_gfx
{
    class SNAKEOIL_GFX_API image_render_2d
    {
        so_this_typedefs( image_render_2d ) ;

    public:

        so_typedefs( std::function< so_math::vec2f_t ( so_math::vec2f_cref_t ) >, offset_funk ) ;
        
        class image_id
        {
            so_this_typedefs( image_id ) ;

            friend class image_render_2d ;

        private: 

            size_t _iid = size_t(-1) ;
            size_t _vpid = size_t(-1) ;

        private:

            image_id( void_t ) {}
            image_id( size_t const iid, size_t const vpid ) : _iid(iid), _vpid(vpid) {}

        public:

            image_id( this_cref_t rhv )
            {
                _iid = rhv._iid ;
                _vpid = rhv._vpid ;
            }

        public:
            
            bool_t operator == ( this_cref_t rhv ) const
            {
                return _iid == rhv._iid && _vpid == rhv._vpid ;
            }

            bool_t is_valid( void_t ) const
            {
                return _iid != size_t( -1 ) && _vpid != size_t( -1 ) ;
            }
        };
        so_typedef( image_id ) ;

    private:
        
        so_gpx::technique_id_t _t_rnd = so_gpx::technique_id_t( -1 ) ;
        so_gfx::image_render_2d_plug_factory_ptr_t _fac_ptr = nullptr ;

        so_gfx::image_render_2d_shared_data_ptr_t _sd_ptr = nullptr ;

        so_gpx::render_system_ptr_t _gpxr = nullptr ;

    private:

        struct image_info
        {
            image_id_t image_id ;
            so_math::vec2f_t pos ;
            so_math::vec2f_t scale ;
            float_t rot ;
            so_math::vec4f_t color ;
            bool_t dirty ;
        };
        so_typedef( image_info ) ;

        struct id_to_images
        {
            so_this_typedefs( id_to_images ) ;

            so_thread::mutex_t mtx ;
            size_t image_index ;
            so_std::vector< image_info > image_infos ;

            id_to_images( void_t ) {}
            id_to_images( this_cref_t rhv ) = delete ;

            id_to_images( this_rref_t rhv ) {
                image_index = rhv.image_index ;
                image_infos = std::move( rhv.image_infos ) ;
            }
            ~id_to_images( void_t ) {}
        };
        so_typedef( id_to_images ) ;

        struct group_info
        {
            so_this_typedefs( group_info ) ;

            so_thread::mutex_t mtx ;
            
            typedef so_std::vector< id_to_images_ptr_t > ids_and_images_t ;
            ids_and_images_t ids_and_images ;

            size_t group_id = 0 ;

            so_math::mat4f_t proj ;
            so_math::mat4f_t view ;

            group_info( void_t ) {}
            group_info( this_cref_t rhv ) {
                ids_and_images = rhv.ids_and_images ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
            }
            group_info( this_rref_t rhv ) {
                ids_and_images = std::move( rhv.ids_and_images ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
            }
            ~group_info( void_t ) {}

            this_ref_t operator = ( this_cref_t rhv ) {
                ids_and_images = rhv.ids_and_images ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                return *this ;
            }
            this_ref_t operator = ( this_rref_t rhv ) {
                ids_and_images = std::move( rhv.ids_and_images ) ;
                group_id = rhv.group_id ;
                proj = rhv.proj ;
                view = rhv.view ;
                return *this ;
            }

            bool_t operator == ( this_cref_t rhv ) const {
                return group_id == group_id ;
            }
        };
        so_typedef( group_info ) ;
        so_typedefs( so_std::vector< group_info_ptr_t >, group_infos ) ;

        so_thread::mutex_t _mtx_gis ;
        group_infos_t _gis ;

    private:

        struct image_data
        {
            so_imex::image_ptr_t img_ptr ;
            so_std::vector< so_gpu::viewport_2d_t > viewports ;
        };
        so_typedef( image_data ) ;

        so_typedefs( so_std::vector< image_data_t >, images ) ;
        
        so_thread::mutex_t _mtx_image ;
        images_t _images ;
        
        so_typedefs( so_std::vector< size_t >, render_group_ids ) ;
        render_group_ids_t _render_groups ;

    public:

        image_render_2d( so_gpx::render_system_ptr_t ) ;
        image_render_2d( this_cref_t ) = delete ;
        image_render_2d( this_rref_t ) ;
        ~image_render_2d( void_t ) ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t init( void_t ) ;

        image_id_t add_image( so_imex::image_ptr_t ) ;
        image_id_t add_image( so_imex::image_ptr_t, so_gpu::viewport_2d_cref_t ) ;

        void_t set_view_projection( size_t const, so_math::mat4f_cref_t view, so_math::mat4f_cref_t proj ) ;

        so_gfx::result draw_image( size_t const group, image_id_cref_t image_id, bool_t const dirty,
            so_math::vec2f_cref_t pos, so_math::vec2f_cref_t scale, float_t const rot,
            so_math::vec4f_cref_t color ) ;

        
        so_gfx::result prepare_for_rendering( void_t ) ;
        bool_t need_to_render( size_t const ) const ;
        so_gfx::result render( size_t const ) ;

        so_gfx::result release( void_t ) ;

    private:

        group_info_ptr_t insert_group( size_t const ) ;

    };
    so_typedef( image_render_2d ) ;
}