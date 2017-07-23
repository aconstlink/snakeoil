//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _VERZERT_IMEX_SYSTEM_SYSTEM_H_
#define _VERZERT_IMEX_SYSTEM_SYSTEM_H_

#include "isystem.h"



#include <vector>

namespace so_imex
{
    class SNAKEOIL_IMEX_API system : public isystem 
    {
        so_this_typedefs( system ) ;

        so_imex::module_registry_ptr_t _mod_reg ;
        so_imex::manager_registry_ptr_t _mgr_reg ;

    private:

        so_imex::camera_manager_ptr_t _camera_mgr_ptr = nullptr ;

    private: 

        system( this_cref_t ) {}

    public:

        system( void_t  ) ;
        system( this_rref_t ) ;
        virtual ~system( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

        static this_ptr_t create_with_default_modules( void_t ) ;
        static this_ptr_t create_with_default_modules( so_memory::purpose_cref_t ) ;

    public: // register

        virtual so_imex::result register_module( so_imex::iscene_module_ptr_t ) ;
        virtual so_imex::result register_module( so_imex::iimage_module_ptr_t ) ;
        virtual so_imex::result register_module( so_imex::imesh_module_ptr_t ) ;
        virtual so_imex::result register_module( so_imex::icode_module_ptr_t ) ;
        virtual so_imex::result register_module( so_imex::iaudio_module_ptr_t ) ;

        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::iscene_module_ptr_t & ) ;
        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::iimage_module_ptr_t & ) ;
        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::imesh_module_ptr_t & ) ;
        virtual bool_t find_module_for_path( so_io::path_cref_t, so_imex::iaudio_module_ptr_t & ) ;

    public: // registry

        virtual module_registry_ptr_t get_module_registry( void_t ) ;
        virtual manager_registry_ptr_t get_manager_registry( void_t ) ;

    public: // access

        virtual so_imex::graph_manager_ptr_t get_graph_manager( void_t ) ;
        virtual so_imex::camera_manager_ptr_t get_camera_manager( void_t ) ;
        virtual so_imex::mesh_manager_ptr_t get_mesh_manager( void_t ) ;
        virtual so_imex::image_manager_ptr_t get_image_manager( void_t ) ;
        virtual so_imex::audio_manager_ptr_t get_audio_manager( void_t ) ;

    public:

        virtual so_imex::result destroy( void_t ) ;

    public:

    };
    so_typedef( system ) ;
}

#endif
