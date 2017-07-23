//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_SHADE_MANAGER_H_
#define _SNAKEOIL_IMEX_MANAGER_SHADE_MANAGER_H_

#include "asset_manager.h"

#include <snakeoil/shade/protos.h>

#include <snakeoil/resource/handle/handle.hpp>
#include <snakeoil/resource/manager/resource_manager.hpp>

namespace so_imex
{
    /// managing shader assets
    class SNAKEOIL_IMEX_API shade_manager : public asset_manager
    {
        so_this_typedefs( shade_manager ) ;

    private:

        struct code_manage_item
        {
            so_shade::icode_ptr_t code_ptr = nullptr ;            
            so_io::path_t file_path ;
        };
        so_typedef( code_manage_item ) ;
        so_typedefs( so_resource::resource_manager<code_manage_item>, code_manager ) ;
        
    private: 

        struct material_manage_item
        {
            so_shade::so_material::imaterial_ptr_t mat_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( material_manage_item ) ;
        so_typedefs( so_resource::resource_manager<material_manage_item>, material_manager ) ;


    private:

        struct vertex_shader_manage_item
        {
            so_shade::so_shader::vertex_shader_ptr_t shd_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( vertex_shader_manage_item ) ;
        so_typedefs( so_resource::resource_manager<vertex_shader_manage_item>, vertex_shader_manager ) ;


    private:

        struct pixel_shader_manage_item
        {
            so_shade::so_shader::pixel_shader_ptr_t shd_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( pixel_shader_manage_item ) ;
        so_typedefs( so_resource::resource_manager<pixel_shader_manage_item> , pixel_shader_manager ) ;

    private:

        code_manager_t _code_res_mgr ;
        material_manager_t _material_mgr ;
        vertex_shader_manager_t _vs_mgr ;
        pixel_shader_manager_t _ps_mgr ;

    public:

        so_typedefs( code_manager_t::handle_t, code_handle ) ;
        so_typedefs( material_manager_t::handle_t, material_handle ) ;
        so_typedefs( vertex_shader_manager_t::handle_t, vertex_shader_handle ) ;
        so_typedefs( pixel_shader_manager_t::handle_t, pixel_shader_handle ) ;

    public:

        struct code_params
        {
            so_shade::icode_ptr_t code_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( code_params ) ;

        struct material_params
        {
            so_shade::so_material::imaterial_ptr_t mat_ptr = nullptr ;
        };
        so_typedef( material_params ) ;

        struct vertex_shader_params
        {
            so_shade::so_shader::vertex_shader_ptr_t shd_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( vertex_shader_params ) ;

        struct pixel_shader_params
        {
            so_shade::so_shader::pixel_shader_ptr_t shd_ptr = nullptr ;
            so_io::path_t file_path ;
        };
        so_typedef( pixel_shader_params ) ;

    private:

        so_thread::mutex_t _mtx_counter ;
        size_t _counter = 0 ;

    private:

        shade_manager( this_cref_t ) {}

    public:

        shade_manager( void_t ) ;
        shade_manager( this_rref_t ) ;
        virtual ~shade_manager( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public: // code acquire/release

        bool_t acquire_code_by_path( so_io::path_cref_t, so_resource::purpose_cref_t, code_handle_ref_t ) ;
        bool_t acquire_code( so_imex::key_cref_t, so_resource::purpose_cref_t, code_handle_ref_t ) ;
        bool_t release_code( code_handle_rref_t ) ;

    public:  // material acquire/release

        bool_t acquire_material_by_path( so_io::path_cref_t, 
            so_resource::purpose_cref_t, material_handle_out_t ) ;

        bool_t acquire_material( so_imex::key_cref_t, 
            so_resource::purpose_cref_t, material_handle_out_t ) ;

        bool_t release_material( material_handle_rref_t ) ;

    public: // vertex shader acquire/release

        bool_t acquire_shader_by_path( so_io::path_cref_t, 
            so_resource::purpose_cref_t, vertex_shader_handle_out_t ) ;

        bool_t acquire_shader( so_imex::key_cref_t, 
            so_resource::purpose_cref_t, vertex_shader_handle_ref_t ) ;

        bool_t release_shader( vertex_shader_handle_rref_t ) ;

    public: // pixel shader acquire/release

        bool_t acquire_shader_by_path( so_io::path_cref_t,
            so_resource::purpose_cref_t, pixel_shader_handle_out_t ) ;

        bool_t acquire_shader( so_imex::key_cref_t,
            so_resource::purpose_cref_t, pixel_shader_handle_ref_t ) ;

        bool_t release_shader( pixel_shader_handle_rref_t ) ;


    public: // insert/reserve/exchange

        /// does not acquire the resource. It only inserts it.
        so_imex::result insert_code( so_imex::key_cref_t, code_params_cref_t ) ;

        so_imex::result reserve_code_by_key( so_imex::key_cref_t ) ;
        so_imex::result reserve_code_by_path( so_io::path_cref_t, so_imex::key_ref_t ) ;

        /// only exchanged the resource. It does not acquire or release any resource.
        /// the exchanged image resource will be destroyed
        so_imex::result exchange_code( so_imex::key_cref_t, code_params_cref_t ) ;

    public:

        /// take all the data from the other manager and
        /// place it into this manager.
        so_imex::result take_and_clear( this_ref_t ) ;

    public:

        size_t unique_number( void_t ) ;
        so_std::string_t unique_number_as_string( void_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( shade_manager ) ;
}

#endif
