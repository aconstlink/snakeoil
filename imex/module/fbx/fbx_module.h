//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MODULE_FBX_FBX_MODULE_H_
#define _SNAKEOIL_IMEX_MODULE_FBX_FBX_MODULE_H_

#include "../iscene_module.h"

#include <snakeoil/thread/mutex.h>
#include <snakeoil/math/matrix/matrix4.hpp>
#include <snakeoil/math/vector/vector3.hpp>
#include <snakeoil/math/vector/vector4.hpp>

#include <fbxsdk.h>

#include <map>

namespace so_imex
{
    class SNAKEOIL_IMEX_API fbx_module : public iscene_module
    {
        so_this_typedefs( fbx_module ) ;

        struct file_paths
        {
            /// the key used to store this path in the manager
            so_imex::key_t key ;
            so_std::string_t absolute_path ;
            so_std::string_t relative_path ;
        };
        so_typedef( file_paths ) ;

        typedef std::map< so_std::string_t, file_paths > __path_map_t ;
        so_typedefs( __path_map_t, path_map ) ;

        struct path_collection
        {
            path_map_t image_paths ;
        };
        so_typedef( path_collection ) ;

    private:
        
        /// the fbx sdk is not thread safe, so it needs to
        /// be locked during import. It also needs to be locked
        /// during shutdown.
        /// @note at the moment, during scene reading, the lock is
        /// released for better parallel performance.
        static so_thread::mutex_t _fbx_mtx ;
        static FbxManager * _fbx_manager ;

        static void_t create_fbx_manager( void_t ) ;

    private:

        fbx_module( this_cref_t ) {}

    public:

        fbx_module( void_t ) ;
        fbx_module( this_rref_t ) ;
        virtual ~fbx_module( void_t ) ;
        
    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        virtual so_imex::result initialize( init_params_cref_t ) ;

    public:

        virtual bool_t is_format_supported( so_std::string_cref_t ) const ;
        virtual so_imex::file_extension_names_t get_file_extension_names( void_t ) ;

        virtual so_thread::task_graph_t import_scene( import_params_cref_t ) ;
        virtual so_thread::task_graph_t export_scene( export_params_cref_t ) ;

    public:

        virtual so_imex::result destroy( void_t ) ;

    private: // attribute

        /// used to construct a unique key by using Fbx names 
        /// with the current scene key.
        so_imex::key_t construct_key( so_imex::key_cref_t, FbxNode * ) ;

    private: // scene graph

        so_imex::result import_the_root( so_imex::key_cref_t, FbxNode *, 
            import_params_cref_t ) ;
        so_imex::result import_the_graph( so_imex::key_cref_t, FbxNode *, 
            so_imex::so_node::logic_group_ptr_t, import_params_cref_t ) ;

        /// import one mesh per material - make a mesh scene per material id
        so_imex::so_node::node_ptr_t import_mesh_scenes( so_imex::key_cref_t, 
            FbxNode *, FbxMesh *, import_params_cref_t ) ;

        /// import a mesh scene. 
        so_imex::so_node::mesh_ptr_t import_mesh_scene( so_imex::key_cref_t, 
            uint_t, FbxNode *, FbxMesh *, import_params_cref_t ) ;

        so_imex::so_node::camera_asset_ptr_t import_camera_scene( so_imex::key_cref_t, 
            FbxNode *, FbxCamera *, import_params_cref_t ) ;


    private: // mesh
        
        so_imex::result import_all_mesh_geometries( so_imex::key_cref_t, FbxNode *,
            so_imex::mesh_manager_ptr_t ) ;
        so_imex::result import_mesh_geometry( so_imex::key_cref_t, FbxNode *, FbxMesh *, 
            so_imex::mesh_manager_ptr_t ) ;

    private: // camera

        so_imex::result import_all_cameras( so_imex::key_cref_t, FbxNode *, 
            so_imex::camera_manager_ptr_t ) ;

        so_imex::result import_camera( so_imex::key_cref_t, FbxCamera *, 
            so_imex::camera_manager_ptr_t ) ;

    private: // materials

        so_imex::result import_all_materials( so_imex::key_cref_t, FbxNode * ) ;

    private: // texture/image

        so_imex::result import_all_images( so_imex::key_cref_t, FbxNode *, 
            so_imex::image_manager_ptr_t, import_params_cref_t, so_thread::task_graph_ref_t ) ;

    private: // helper

        so_math::mat4f_t conv_matrix( FbxAMatrix const & ) const ;
        so_math::vec4f_t conv_vector( FbxVector4 const & ) const ;
        so_math::vec3f_t conv_vector( FbxDouble3 const & ) const ;

        so_math::vec3f_t fbx_angles_vector( FbxDouble3 const & ) const ;
        so_math::vec3f_t fbx_vector_swizzle( so_math::vec3f_cref_t ) const ;
        so_math::vec3f_t fbx_angles_swizzle( so_math::vec3f_cref_t ) const ;
        so_math::vec3f_t fbx_scale_swizzle( so_math::vec3f_cref_t ) const ;
    };
    so_typedef( fbx_module ) ;
}

#endif
