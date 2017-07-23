//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_C4D_C4D_IMPORT_CONTEXT_H_
#define _SNAKEOIL_IMEX_MODULE_C4D_C4D_IMPORT_CONTEXT_H_

#include "../../typedefs.h"
#include "../../api.h"
#include "../../protos.h"
#include "../iscene_module.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/string/string.hpp>
#include <snakeoil/std/container/map.hpp>
#include <snakeoil/std/container/stack.hpp>
#include <snakeoil/std/container/vector.hpp>

#include <c4d_file.h>

namespace so_imex
{
    class SNAKEOIL_IMEX_API c4d_import_context
    {
        so_this_typedefs( c4d_import_context ) ;

        typedef so_std::map< melange::PolygonObject const *, so_std::string_t > __polygon_to_name_t ;
        so_typedefs( __polygon_to_name_t, polygon_to_name ) ;

        typedef so_std::map< melange::BaseMaterial const *, so_std::string_t > __mat_to_name_t ;
        so_typedefs( __mat_to_name_t, mat_to_name ) ;

        typedef so_std::map< melange::BaseShader const *, so_std::string_t > __bitmap_to_name_t ;
        so_typedefs( __bitmap_to_name_t, bitmap_to_name ) ;

        typedef so_std::map< melange::BaseObject const *, so_std::string_t > __camera_to_name_t ;
        so_typedefs( __camera_to_name_t, camera_to_name ) ;

        typedef so_std::stack< melange::BaseMaterial const *, 20 > __mat_stack_t ;
        so_typedefs( __mat_stack_t, mat_stack ) ;

        struct visibility_item
        {
            byte_t editor = melange::OBJECT_UNDEF ;
            byte_t render = melange::OBJECT_UNDEF ;
        };
        so_typedef( visibility_item ) ;
        typedef so_std::stack< visibility_item_t, 20 > __visibility_stack_t ;
        so_typedefs( __visibility_stack_t, visibility_stack ) ;

        so_typedefs( so_std::vector<so_std::string_t>, strings ) ;

    private: // polygon

        mutable so_thread::mutex_t _mtx_polygon ;
        size_t _unique_polygon = 0 ;

        polygon_to_name_t _polygon_to_name ;

    private: // material
    
        mutable so_thread::mutex_t _mtx_material ;
        size_t _unique_material = 0 ;

        mat_to_name_t _mat_to_name ;

    private: // textures

        mutable so_thread::mutex_t _mtx_bitmap ;
        size_t _unique_bitmap = 0 ;

        bitmap_to_name_t _bitmap_to_name ;

    private: // camera

        mutable so_thread::mutex_t _mtx_camera ;
        size_t _unique_camera = 0 ;

        camera_to_name_t _camera_to_name ;
        strings_t _camera_names ;

    private: // material stack

        mutable so_thread::mutex_t _mtx_mat_stack ;
        mat_stack_t _mat_stack ;

    private:

        // mutex intentionally omitted
        visibility_stack_t _vis_stack ;

    private: // 

        so_io::path_t _doc_path ;
        so_imex::iscene_module::import_params_t _import_params ;

    private:

        c4d_import_context( this_rref_t ) {}

    public:

        c4d_import_context( so_io::path_cref_t, so_imex::iscene_module::import_params_cref_t ) ;
        c4d_import_context( this_cref_t ) ;
        ~c4d_import_context( void_t ) ;

    public:

        bool_t add_polygon( melange::PolygonObject const*, so_std::string_inout_t key ) ;
        bool_t get_polygon_key( melange::PolygonObject const*, so_std::string_out_t key ) const ;

    public: 

        /// add a material to the context and let the context create a unique key
        bool_t add_material( melange::BaseMaterial const*, so_std::string_inout_t key ) ;      
        bool_t get_material_key( melange::BaseMaterial const*, so_std::string_out_t key ) const ;

    public:

        bool_t add_bitmap( melange::BaseShader const *, so_std::string_inout_t key ) ;
        bool_t get_bitmap_key( melange::BaseShader const *, so_std::string_out_t key ) const ;
        bool_t change_bitmap_key( melange::BaseShader const *, so_std::string_in_t key ) ;
        bool_t remove_bitmap( melange::BaseShader const * ) ;

    public:

        bool_t add_camera( melange::BaseObject const *, so_std::string_inout_t ) ;
        bool_t get_camera_key( melange::BaseObject const *, so_std::string_out_t ) const ;

    public:

        void_t push_material( melange::BaseMaterial const * ) ;
        melange::BaseMaterial const * top_material( void_t ) const ;
        void_t pop_material( void_t ) ;
        bool_t has_top_material( void_t ) const ;

    public:

        void_t push_visibility( visibility_item_in_t ) ;
        bool_t top_visibility( visibility_item_out_t ) ;
        void_t pop_visibility( void_t ) ;

    public:

        so_io::path_cref_t doc_path( void_t ) const ;
        so_imex::iscene_module::import_params_cref_t import_params( void_t ) const ;

    };
    so_typedef( c4d_import_context ) ;
}

#endif
