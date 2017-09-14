//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "obj_module.h"
#include "obj_document.h"

#include "../../manager/manager_registry.h"
#include "../../manager/mesh_manager.h"

#include <snakeoil/io/global.h>

#include <snakeoil/std/filesystem/filesystem.hpp>

#include <snakeoil/geometry/mesh/polygon_mesh.h>
#include <snakeoil/thread/task/task_graph.h>
#include <snakeoil/thread/task/tasks.h>

#include <snakeoil/log/global.h>

#include <snakeoil/core/cast.hpp>

using namespace so_imex ;

struct obj_export_helper
{
    so_this_typedefs( obj_export_helper ) ;

    static so_std::string_t vec3_to_string( so_math::vec3f_cref_t v )
    {
        return so_std::string_t(
            std::to_string(v.x()) +" "+ std::to_string( v.y() ) + " " + std::to_string( v.z() ) ) ;
    }

    static so_std::string_t compose_header( so_imex::so_obj::obj_document_cref_t doc )
    {
        so_std::string_t header ;

        header += "# \n" ;
        header += "# snakeoil obj exporter \n" ;
        header += "# \n" ;
        header += "## creation : 0000-00-00 \n" ;
        header += "## positions : " + std::to_string( doc.positions.size() ) + " \n";
        header += "## normals : " + std::to_string( doc.normals.size() ) + " \n" ;
        header += "## texcoords : " + std::to_string( doc.texcoords.size() ) + " \n" ;
        header += "## faces : " + std::to_string( doc.faces.size() ) + " \n" ;
        header += "# \n" ;

        return std::move( header ) ;
    }

    static void_t from_mesh( so_std::string_t name, so_geo::flat_tri_mesh_ptr_t mesh_ptr, 
        so_imex::so_obj::obj_document_inout_t doc )
    {
        size_t const pos_comp = so_geo::vector_component_format_to_number( mesh_ptr->position_format ) ;
        size_t const nrm_comp = so_geo::vector_component_format_to_number( mesh_ptr->normal_format ) ;
        size_t const txc_comp = so_geo::texcoords_component_format_to_number( mesh_ptr->texcoord_format ) ;

        bool_t const has_normals = mesh_ptr->normals.size() > 0 ;
        bool_t const has_texcoords = mesh_ptr->texcoords.size() > 0 ;

        size_t const num_positions = mesh_ptr->positions.size() / pos_comp ;
        size_t const num_normals = mesh_ptr->normals.size() / ( nrm_comp != 0 ? nrm_comp : 1 ) ;
        
        size_t const num_texcoords = so_core::is_not( has_texcoords ) ? 0 :
            mesh_ptr->texcoords[ 0 ].size() / ( txc_comp != 0 ? txc_comp : 1 ) ;

        doc.name = name ;

        // positions
        if( pos_comp != 0 && mesh_ptr->positions.size() != 0 )
        {
            doc.positions.resize( mesh_ptr->positions.size() / pos_comp ) ;

            for( size_t i = 0; i < mesh_ptr->positions.size() / pos_comp ; ++i )
            {
                size_t const index = i * pos_comp ;

                doc.positions[i].v = so_math::vec3f_t(
                    mesh_ptr->positions[ index + 0 ],
                    mesh_ptr->positions[ index + 1 ],
                    mesh_ptr->positions[ index + 2 ] ) ;
            }
        }

        // texcoords
        if( mesh_ptr->texcoords.size() > 0 )
        {
            if( txc_comp != 0 && mesh_ptr->texcoords[0].size() != 0 )
            {
                doc.texcoords.resize( mesh_ptr->texcoords[0].size() / txc_comp ) ;

                for( size_t i = 0; i < mesh_ptr->texcoords[0].size() / txc_comp ; ++i )
                {
                    size_t const index = i * txc_comp ;

                    auto third = txc_comp == 3 ? mesh_ptr->texcoords[ 0 ][ index + 2 ] : 0.0f ;

                    doc.texcoords[ i ].v = so_math::vec3f_t(
                        mesh_ptr->texcoords[ 0 ][ index + 0 ],
                        mesh_ptr->texcoords[ 0 ][ index + 1 ],
                        third ) ;
                }
            }

            so_log::global::warning( mesh_ptr->texcoords.size() > 1, "[obj_module::export] : "
                "obj format only supports one set of texture coordinates for mesh " + name ) ;
        }
        

        // normals
        if( nrm_comp != 0 && mesh_ptr->normals.size() != 0 )
        {
            doc.normals.resize( mesh_ptr->normals.size() / nrm_comp ) ;

            for( size_t i = 0; i < mesh_ptr->normals.size() / nrm_comp ; ++i )
            {
                size_t const index = i * nrm_comp ;

                doc.normals[ i ].v = so_math::vec3f_t(
                    mesh_ptr->normals[ index + 0 ],
                    mesh_ptr->normals[ index + 1 ],
                    mesh_ptr->normals[ index + 2 ] ) ;
            }
        }

        // faces
        if( mesh_ptr->indices.size() != 0 )
        {
            doc.faces.resize( mesh_ptr->indices.size() / 3 ) ;

            for( size_t i = 0; i < mesh_ptr->indices.size() / 3; ++i )
            {
                size_t const index = i * 3 ;

                
                doc.faces[ i ].num_vertices = 3 ;
                {
                    size_t const vi = mesh_ptr->indices[ index + 0 ] + 1 ;
                    doc.faces[ i ].vertices[ 0 ].v = vi ;
                    
                    if( has_texcoords )
                        doc.faces[ i ].vertices[ 0 ].vt = vi ;

                    if( has_normals )
                        doc.faces[ i ].vertices[ 0 ].vn = vi ;
                }
                
                {
                    size_t const vi = mesh_ptr->indices[ index + 1 ] + 1;
                    doc.faces[ i ].vertices[ 1 ].v = vi ;

                    if( has_texcoords )
                        doc.faces[ i ].vertices[ 1 ].vt = vi ;

                    if( has_normals )
                        doc.faces[ i ].vertices[ 1 ].vn = vi ;
                }

                {
                    size_t const vi = mesh_ptr->indices[ index + 2 ] + 1;
                    doc.faces[ i ].vertices[ 2 ].v = vi ;

                    if( has_texcoords )
                        doc.faces[ i ].vertices[ 2 ].vt = vi ;

                    if( has_normals )
                        doc.faces[ i ].vertices[ 2 ].vn = vi ;
                }
                
            }
        }
    }

    static so_std::string_t doc_to_string( so_imex::so_obj::obj_document_cref_t doc )
    {
        so_std::string_t doc_s ;

        doc_s += "o " + doc.name + "\n" ;

        // fill in

        // positions
        for( auto const & v : doc.positions )
        {
            doc_s += "v " + this_t::vec3_to_string( v.v ) + " \n" ;
        }
        
        // texcoords
        for( auto const & vt : doc.texcoords )
        {
            doc_s += "vt " + this_t::vec3_to_string( vt.v ) + " \n" ;
        }

        // normals
        for( auto const & vn : doc.normals )
        {
            doc_s += "vn " + this_t::vec3_to_string( vn.v ) + " \n" ;
        }

        // faces
        for( auto const & f : doc.faces )
        {
            doc_s += "f " ;

            for( auto i = 0; i < f.num_vertices; ++i )
            {                
                doc_s += std::to_string( f.vertices[ i ].v ) ;
                
                if( f.vertices[ i ].vt != size_t( -1 ) )
                    doc_s += "/" + std::to_string( f.vertices[ i ].vt ) ;
                else
                    doc_s += "/" ;

                if( f.vertices[ i ].vn != size_t( -1 ) )
                    doc_s += "/" + std::to_string( f.vertices[ i ].vn ) ;

                doc_s += " " ;
            }

            doc_s += "\n" ;
        }
        

        return std::move( doc_s ) ;
    }
};


//*************************************************************************************
so_thread::task_graph_t obj_module::export_scene( export_params_cref_t params_in )
{
    so_imex::sync_object_t::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    so_thread::itask_ptr_t tt_end = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_imex::sync_object_t::set_and_signal( params_in.sync_ptr, so_imex::ok ) ;
    } ) ;

    so_thread::itask_ptr_t tt_begin = so_thread::dyn_task_t::create(
        [=]( so_thread::itask_ptr_t self_ptr )
    {

        auto * mmgr = params_in.manager_reg_ptr->get_mesh_manager() ;

        so_imex::mesh_manager_t::handles_t hnds ;
        mmgr->acquire_all( "[obj_module::export] : acquire all meshes for geometry", hnds ) ;

        so_imex::so_obj::obj_document doc ;
        
        //
        // 1. assemble doc
        //
        for( auto & hnd : hnds )
        {
            if( so_core::can_cast< so_geo::flat_tri_mesh_ptr_t >( hnd->data_ptr ) )
            {
                auto * mesh_ptr = static_cast< so_geo::flat_tri_mesh_ptr_t >( hnd->data_ptr ) ;
                obj_export_helper::from_mesh( hnd.get_key(), mesh_ptr, doc ) ;
            }
            /*else if( so_core::can_cast< so_geo::polygon_mesh_ptr_t >( hnd->data_ptr ) )
            {
            auto * mesh_ptr = static_cast< so_geo::polygon_mesh_ptr_t >( hnd->data_ptr ) ;
            ( void_t ) mesh_ptr ;
            }*/
            else
            {
                so_log::global::warning( "[obj_module::export] : mesh type currently not supported" ) ;
            }

            // @note at the moment, only one mesh
            break ;
        }

        //
        // 2. write to file
        //
        so_std::string_t s = obj_export_helper::compose_header( doc ) ;
        
        s += obj_export_helper::doc_to_string( doc ) ;

        // check if path exists to see if it must be created
        {
            so_io::path_t const path = params_in.path_to_file ;

            if( so_core::is_not( so_std::filesystem::exists( path.parent_path() ) ) )
            {
                so_std::filesystem::create_directories( path.parent_path() ) ;
            }
        }

        {
            so_io::path_t const path = params_in.path_to_file ;

            so_io::store_handle_t hnd = so_io::global::store( 
                path, s.c_str(), s.size() * sizeof( char_t ) ) ;

            auto const res = hnd.wait_for_operation() ;
            if( so_io::no_success( res ) )
            {
                so_log::global::error( "[obj_module::export] : unable to write obj document to : " 
                    + path.string() ) ;
            }
        }

    } ) ;

    return so_thread::task_graph_t( tt_begin, tt_end ) ;
}