//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "ctm_module.h"

#include "../../manager/mesh_manager.h"

#include <snakeoil/geometry/mesh/flat_tri_mesh.h>

#include <snakeoil/log/log.h>

#include <openctm.h>

using namespace so_imex ;

//*************************************************************************************
so_thread::task_graph_t ctm_module::import_mesh( import_params_cref_t params_in )
{
    so_imex::sync_object::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    if( so_log::log::error( params_in.mesh_mgr_ptr == nullptr, 
        "[so_imex::ctm_module::initialize] : mesh manager == nullptr" ) )
    {
        so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::failed ) ;
        return so_thread::task_graph_t() ;
    }

    so_thread::itask_ptr_t tt_begin = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_geo::flat_tri_mesh the_mesh ;

        so_imex::result res = this_t::do_import( params_in.path_to_file, the_mesh ) ;
        if( so_log::log::error(so_imex::no_success(res),
            "[so_imex::ctm_module::import_mesh] : import failed") )
            return ;

        so_geo::flat_tri_mesh_ptr_t mesh_gen_ptr = so_geo::flat_tri_mesh::create( 
            std::move(the_mesh), "[ctm_module::import_mesh] : store mesh" ) ;
                
        {
            so_imex::mesh_manager_t::manage_params mp ;
            mp.data_ptr = mesh_gen_ptr ;
            mp.file_path = params_in.path_to_file ;

            // @note not 100 % bullet proof. between reserve and exchange, the data
            // could be changed from somewhere else.
            auto const ires = params_in.mesh_mgr_ptr->reserve_by_key( params_in.key ) ;
            if( ires == so_imex::found || ires == so_imex::reserved )
            {
                auto const res2 = params_in.mesh_mgr_ptr->exchange( params_in.key, mp ) ;
                if( so_log::log::error( so_imex::no_success( res2 ),
                    "[so_imex::ctm_module::import_mesh] : exchange mesh" ))
                {
                    mesh_gen_ptr->destroy() ;
                    so_imex::sync_object::set_and_signal( params_in.sync_ptr, so_imex::failed ) ;
                    return ;
                }
            }
        }

        so_imex::sync_object::set_and_signal( params_in.sync_ptr, res ) ;        
    }) ;
    
    return  so_thread::task_graph_t( tt_begin ) ;
}

//*************************************************************************************
so_imex::result ctm_module::do_import( so_io::path_cref_t path_to_file, 
    so_geo::flat_tri_mesh_ref_t mesh_out ) 
{
    so_geo::flat_tri_mesh the_mesh ;

    CTMcontext context = ctmNewContext( CTM_IMPORT ) ;

    ctmLoad( context, path_to_file.string().c_str() ) ;
    if( ctmGetError(context) != CTM_NONE )
    {
        so_log::log::error( "[so_openctm::importer::from_file] : loading file failed." ) ;
        return so_imex::failed ;
    }

    CTMuint const num_verts = ctmGetInteger( context, CTM_VERTEX_COUNT ) ;
    CTMuint const num_tris = ctmGetInteger( context, CTM_TRIANGLE_COUNT ) ;
    CTMuint const num_uvms = ctmGetInteger( context, CTM_UV_MAP_COUNT ) ;

    CTMfloat const * vertices = ctmGetFloatArray( context, CTM_VERTICES ) ;
    CTMuint const * indices = ctmGetIntegerArray( context, CTM_INDICES ) ;

    // indices
    {
        size_t const num_indices = num_tris * 3 ;
        the_mesh.indices.resize( num_indices ) ;

        for( CTMuint i=0; i<num_indices; ++i )
        {
            the_mesh.indices[i] = indices[i] ;
        }
    }

    // positions (vertices in openctm)
    {
        // 3 components per vertex position
        size_t const num_positions = num_verts * 3 ;
        the_mesh.position_format = so_geo::vector_component_format::xyz ;
                
        the_mesh.positions.resize( num_positions ) ;
        for( CTMuint i=0; i<num_positions; ++i )
        {
            the_mesh.positions[i] = vertices[i] ;
        }
    }

    // normals
    if( ctmGetInteger(context, CTM_HAS_NORMALS) == CTM_TRUE ) 
    {
        // 3 components per vertex normal
        size_t const num_normals = num_verts * 3 ;
        the_mesh.normal_format = so_geo::vector_component_format::xyz ;

        CTMfloat const * normals = ctmGetFloatArray( context, CTM_NORMALS ) ;

        so_geo::floats_t store_normals(num_normals) ;
        for( CTMuint i=0; i<num_normals; ++i )
        {
            store_normals[i] = normals[i] ;
        }
        the_mesh.normals = std::move(store_normals) ;
    }

    // texcoords
    {
        // 2 components per texcoord
        size_t const num_texcoords = num_verts * 2 ;
        the_mesh.texcoord_format = so_geo::texcoord_component_format::uv ;

        for( CTMuint uvm=0; uvm<num_uvms; ++uvm )
        {
            CTMfloat const * uvs = ctmGetFloatArray( context, CTMenum(CTM_UV_MAP_1 + uvm) ) ;
            so_geo::floats_t store_uvs(num_texcoords) ;

            for( CTMuint i=0; i<num_texcoords; ++i )
            {
                store_uvs[i] = uvs[i] ;
            }
            the_mesh.texcoords.push_back( std::move(store_uvs) ) ;
        }
    }

    ctmFreeContext( context ) ;

    mesh_out = std::move(the_mesh) ;

    return so_imex::ok ;
}
