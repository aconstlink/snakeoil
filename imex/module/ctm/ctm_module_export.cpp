//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "ctm_module.h"
#include "ctm_helper.h"

#include "../../manager/mesh_manager.h"

#include <snakeoil/geometry/mesh/tri_mesh.h>
#include <snakeoil/geometry/mesh/flat_tri_mesh.h>
#include <snakeoil/geometry/mesh/polygon_mesh.h>

#include <snakeoil/memory/guards/malloc_guard.hpp>
#include <snakeoil/log/log.h>

#include <snakeoil/core/cast.hpp>

#include <openctm.h>

using namespace so_imex ;

//*************************************************************************************
so_thread::task_graph_t ctm_module::export_mesh( export_params_cref_t params_in )
{
    so_imex::sync_object::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    if( so_log::log::error( params_in.mesh_mgr_ptr == nullptr,
        "[so_imex::ctm_module::export_mesh] : invalid mesh store ptr" ) )
    {
        so_imex::sync_object::set_and_signal( 
            params_in.sync_ptr, so_imex::invalid_argument ) ;
        
        return so_thread::task_graph_t() ;
    }

    so_thread::itask_ptr_t tt_begin = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        // we need to track if the mesh comes from the manager
        // or if its copy from the manager during conversion
        bool_t is_copy = false ;
        so_geo::flat_tri_mesh_cptr_t mesh_ptr = nullptr ;
        
        so_imex::mesh_manager_t::handle_t mhnd ;

        // first, try to get a proper mesh for exporting
        {
            bool_t const res = params_in.mesh_mgr_ptr->acquire( params_in.mesh_key, 
                "[so_imex::ctm_module::export_mesh]", mhnd ) ;

            if( so_core::is_not(res) )
            {
                so_log::log::error( "[so_imex::ctm_module::export_mesh] : no mesh for key : " +
                    params_in.mesh_key );

                return so_imex::sync_object::set_and_signal(
                    params_in.sync_ptr, so_imex::invalid_argument );
            }

            so_geo::imesh_cptr_t managed_mesh_ptr = mhnd->data_ptr ;

            
            if( so_core::can_cast<so_geo::polygon_mesh_cptr_t>(managed_mesh_ptr) )
            {
                is_copy = true ;

                auto const * tmp_ptr = static_cast<so_geo::polygon_mesh_cptr_t>(managed_mesh_ptr) ;
                
                so_geo::flat_tri_mesh ftm ;
                so_geo::result flatten_res = tmp_ptr->flatten( ftm ) ;
                if( so_log::log::error( so_geo::no_success( flatten_res ),
                    "[so_imex::ctm_module::export_mesh] : tri_mesh -> flat_tri_mesh failed" ) )
                {
                    return so_imex::sync_object::set_and_signal(
                        params_in.sync_ptr, so_imex::failed ) ;
                }
                mesh_ptr = so_geo::flat_tri_mesh::create( std::move( ftm ),
                    "[so_imex::ctm_module::export_mesh] : created during export from polygon mesh" ) ;
            }

            else if(so_core::can_cast<so_geo::tri_mesh_cptr_t>(managed_mesh_ptr))
            {
                is_copy = true ;

                auto const * tri_mesh = static_cast<so_geo::tri_mesh_cptr_t>(managed_mesh_ptr) ;
                
                so_geo::flat_tri_mesh ftm ;
                so_geo::result flatten_res = tri_mesh->flatten( ftm ) ;
                if( so_log::log::error( so_geo::no_success( flatten_res ),
                    "[so_imex::ctm_module::export_mesh] : tri_mesh -> flat_tri_mesh failed" ) )
                {
                    return so_imex::sync_object::set_and_signal(
                        params_in.sync_ptr, so_imex::failed ) ;
                }
                mesh_ptr = so_geo::flat_tri_mesh::create( std::move( ftm ),
                    "[so_imex::ctm_module::export_mesh] : created during export from tri-mesh" ) ;
            }

            else if( so_core::can_cast<so_geo::flat_tri_mesh_cptr_t>(managed_mesh_ptr) )
            {
                mesh_ptr = dynamic_cast<so_geo::flat_tri_mesh_cptr_t>( managed_mesh_ptr ) ;
            }

            else
            {
                so_log::log::error( "[so_imex::ctm_module::export_mesh] : mesh type is not flat_tri_mesh." ) ;
                return so_imex::sync_object::set_and_signal(
                    params_in.sync_ptr, so_imex::invalid_argument ) ;
            }
        }

        so_imex::sync_object::set_and_signal( params_in.sync_ptr, 
            this_t::do_export( params_in.path_to_file, *mesh_ptr ) ) ;

        // this is the mesh created in this function
        // so it is safe to const_cast it and destroy it.
        if( is_copy ) 
            const_cast<so_geo::flat_tri_mesh_ptr_t>(mesh_ptr)->destroy() ;
    }) ;
    
    return  so_thread::task_graph_t( tt_begin ) ;
}

//*************************************************************************************
so_imex::result ctm_module::do_export( so_io::path_cref_t path, so_geo::flat_tri_mesh_cref_t mesh_in ) 
{
    if( so_core::is_not(mesh_in.are_array_numbers_valid()) )
    {
        so_log::log::error( "[so_openctm::exporter::to_file] : arrays are not flat" ) ;
        return so_imex::failed ;
    }

    CTMcontext context = ctmNewContext( CTM_EXPORT ) ;

    CTMuint const num_positions = (CTMuint)mesh_in.positions.size() ;
    CTMuint const num_indices = (CTMuint)mesh_in.indices.size() ;
    CTMuint const num_tris = num_indices / 3 ;

    so_memory::malloc_guard< CTMfloat > positions( num_positions ) ;
    so_memory::malloc_guard< CTMfloat > normals ;
    so_memory::malloc_guard< CTMuint > indices( num_indices ) ;
    
    for( size_t i=0; i<num_positions; ++i )
    {
        positions[i] = mesh_in.positions[i] ;
    }

    for(CTMuint i = 0; i < num_indices/3; ++i)
    {
        size_t const index = i*3 ;
        
        indices[index+0] = mesh_in.indices[index+0] ;
        indices[index+1] = mesh_in.indices[index+1] ;
        indices[index+2] = mesh_in.indices[index+2] ;
    }

    {
        size_t const num_normals = mesh_in.normals.size() ;
        if( num_normals > 0 )
        {
            normals = so_memory::malloc_guard< CTMfloat >( num_normals ) ;

            for(size_t i = 0; i < num_normals; ++i)
            {
                normals[i] = mesh_in.normals[i] ;
            }
        }
    }
    
    {
        CTMuint const num_vertices = (CTMuint)mesh_in.get_num_vertices() ;
        ctmDefineMesh( context, positions, num_vertices, indices, num_tris, normals ) ;
        
        if( ctmGetError( context ) != CTM_NONE )
        {
            so_log::log::error( "[so_openctm::exporter::to_file] : ctmDefineMesh with " + 
                so_imex::ctm_helper_t::error_to_string( ctmGetError( context ) ) ) ;

            return so_imex::failed ;
        }
    }

    typedef so_memory::malloc_guard< CTMfloat > mg_texcoord_t ;
    so_std::vector<mg_texcoord_t> texcoord_mallocs( mesh_in.texcoords.size() ) ;

    // do texcoords
    // note: we can have multi-layer texcoords
    {
        CTMuint const num_texcoord_layers = (CTMuint)mesh_in.texcoords.size() ;

        if( num_texcoord_layers > 0 )
        {
            for( CTMuint l = 0; l < num_texcoord_layers; ++l )
            {
                size_t const num_texcoords = mesh_in.texcoords[l].size() ;
                mg_texcoord_t ctmtexcoords( num_texcoords ) ;

                for(size_t i = 0; i < num_texcoords; ++i)
                {
                    ctmtexcoords[i] = mesh_in.texcoords[l][i] ;
                }

                so_std::string_t const uv_name = "a_TextureCoordinate" + (l > 0 ? std::to_string( l ) : "") ;
                auto const res = ctmAddUVMap( context, ctmtexcoords, uv_name.c_str(), NULL ) ;

                so_log::log::error( res == CTM_NONE,
                    "[so_openctm::exporter::to_file] : ctmAddUVMap with " + 
                    so_imex::ctm_helper_t::error_to_string( res ) ) ;

                texcoord_mallocs[l] = std::move(ctmtexcoords) ;
            }
        }
    }

    // check if path exists to see if it must be created
    {
        so_io::path_t base_path = path ;
        base_path.remove_filename() ;

        if( so_core::is_not( so_std::filesystem::exists( base_path ) ) )
        {
            so_std::filesystem::create_directories( base_path ) ;
        }
    }

    ctmSave( context, path.string().c_str() ) ;
    CTMenum const res = ctmGetError( context ) ;
    so_log::log::error( res != CTM_NONE, "[so_openctm::exporter::to_file] : ctmSave with " + 
        so_imex::ctm_helper_t::error_to_string(res) + " for " + path.string() ) ;

    ctmFreeContext( context ) ;

    return so_imex::ok ;
}
