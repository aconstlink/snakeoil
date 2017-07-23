//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_FBX_FBX_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_FBX_FBX_MODULE_H_

#include "../iscene_module.h"

#include "c4d_import_context.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <snakeoil/std/container/map.hpp>

namespace so_imex
{
    class SNAKEOIL_IMEX_API c4d_module : public iscene_module
    {
        so_this_typedefs( c4d_module );

    private:

        c4d_module( this_cref_t ) {}

    public:

        c4d_module( void_t );
        c4d_module( this_rref_t );
        virtual ~c4d_module( void_t );

    public:

        static this_ptr_t create( so_memory::purpose_cref_t );
        static void_t destroy( this_ptr_t );

    public:

        virtual so_imex::result initialize( init_params_cref_t );

    public:

        virtual bool_t create_properties( property_map_inout_t ) const ;
        virtual bool_t is_format_supported( so_std::string_cref_t ) const;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t );

        virtual so_thread::task_graph_t import_scene( import_params_cref_t );
        virtual so_thread::task_graph_t export_scene( export_params_cref_t );

    public:

        virtual so_imex::result destroy( void_t ) ;

    private:

        void_t import_scene( so_std::string_cref_t prev_name, melange::BaseObject *, 
            so_imex::iscene_module::import_params_cref_t, so_io::path_cref_t,
            so_thread::task_graph_ref_t, c4d_import_context_ref_t );

        void_t import_the_graph( so_std::string_cref_t prev_name, melange::BaseObject *,
            so_imex::so_node::logic_group_ptr_t parent_ptr, 
            so_imex::iscene_module::import_params_cref_t params_in, 
            so_io::path_cref_t, c4d_import_context_ref_t ) ;

        void_t import_the_polygon_graph( so_std::string_cref_t prev_name, melange::PolygonObject *,
            so_imex::so_node::logic_group_ptr_t parent_ptr, 
            so_imex::iscene_module::import_params_cref_t params_in, 
            so_io::path_cref_t, c4d_import_context_cref_t ) ;

        void_t import_the_camera_graph( melange::BaseObject const *,
            so_imex::so_node::logic_group_ptr_t parent_ptr, c4d_import_context_cref_t ) ;

    private: // meshes

        void_t import_all_meshes( so_std::string_cref_t, melange::BaseObject *,
            so_imex::iscene_module::import_params_cref_t,
            so_thread::task_graph_ref_t, c4d_import_context_ref_t ) ;

        void_t import_mesh( so_std::string_cref_t, melange::PolygonObject *,
            so_imex::iscene_module::import_params_cref_t,
            so_thread::task_graph_ref_t, c4d_import_context_ref_t ) ;

    private: // material 

        void_t import_all_materials( so_std::string_cref_t, melange::BaseObject *,
            so_imex::iscene_module::import_params_cref_t, so_io::path_cref_t,
            so_thread::task_graph_ref_t, c4d_import_context_ref_t ) ;

        void_t import_material( so_std::string_cref_t, 
            melange::TextureTag * tx_tag, melange::BaseMaterial *,
            so_imex::iscene_module::import_params_cref_t, so_io::path_cref_t,
            c4d_import_context_ref_t ) ;

    private: // image

        void_t import_all_images( so_std::string_cref_t, melange::BaseObject *,
            so_imex::iscene_module::import_params_cref_t, so_io::path_cref_t,
            so_thread::task_graph_ref_t, c4d_import_context_ref_t ) ;

        so_imex::result import_image( so_std::string_inout_t, melange::BaseShader *, 
            so_imex::iscene_module::import_params_cref_t, so_io::path_cref_t, 
            so_thread::task_graph_ref_t, c4d_import_context_ref_t ) ;

    private: // spline

        void_t import_all_splines( so_std::string_cref_t, melange::BaseObject *,
            c4d_import_context_ref_t, so_thread::task_graph_ref_t ) ;

        void_t import_spline( so_std::string_cref_t, melange::SplineObject *,
            c4d_import_context_ref_t, so_thread::task_graph_ref_t ) ;


    private: // camera 

        void_t import_all_cameras( so_std::string_cref_t, melange::BaseObject *,
            c4d_import_context_ref_t ) ;

        void_t import_camera( so_std::string_cref_t, melange::BaseObject *,
            c4d_import_context_ref_t ) ;

    private:

        so_std::string_t to_string( melange::String const & ) const ;
        so_math::vec3f_t to_vector( melange::Vector const & ) const ;
    };
    so_typedef( c4d_module );
}

#endif
