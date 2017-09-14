//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "dae_module.h"

#include "../../manager/manager_registry.h"
#include "../../manager/mesh_manager.h"

#include "elements/dae_document.h"
#include "elements/dae_visitor.h"
#include "elements/dae_node.h"
#include "elements/dae_nodes.h"

#include <snakeoil/std/filesystem/filesystem.hpp>

#include <snakeoil/io/global.h>

#include <snakeoil/geometry/mesh/polygon_mesh.h>
#include <snakeoil/thread/task/task_graph.h>
#include <snakeoil/thread/task/tasks.h>

#include <snakeoil/math/utility/angle.hpp>
#include <snakeoil/math/euler/euler_angles.hpp>

#include <snakeoil/std/container/stack.hpp>

#include <snakeoil/log/global.h>

#include <snakeoil/core/cast.hpp>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_print.hpp>

using namespace so_imex ;

so_typedefs( rapidxml::xml_document<>, rxml_doc ) ;
so_typedefs( rapidxml::xml_node<>, rxml_node ) ;

namespace this_file
{
    //*************************************************************************************
    so_std::string_t float_vector_to_string( so_geo::floats_cref_t floats )
    {
        so_std::string_t sout ;

        for( auto f : floats )
        {
            sout += std::to_string( f ) + " ";
        }

        sout.push_back( '\0' ) ;

        return std::move( sout ) ;
    }

    //*************************************************************************************
    so_std::string_t uint_vector_to_string( so_geo::uints_cref_t values )
    {
        so_std::string_t sout ;

        for( auto v : values )
        {
            sout += std::to_string( v ) + " " ;
        }

        sout.push_back( '\0' ) ;

        return std::move( sout ) ;
    }
} ;

class rapidxml_visitor : public so_dae::visitor
{
    so_this_typedefs( rapidxml_visitor ) ;

    typedef so_std::stack< rxml_node_ptr_t, 20 > __stack_t ;
    so_typedefs( __stack_t, stack ) ;

private:

    rxml_doc_ptr_t _dptr = nullptr ;
    stack_t _node_stack ;

    rxml_node_ptr_t _cur_ptr = nullptr ;

private:

    rxml_doc_ref_t doc( void_t ) { return *_dptr ; }
    rxml_node_ptr_t top( void_t ) { return _node_stack.top() ; }

    void_t set_cur( rxml_node_ptr_t ptr ) 
    {
        _cur_ptr = ptr ;
        this_t::append( _cur_ptr ) ;
    }
    
    void_t append( rxml_node_ptr_t ptr ) 
    {
        if( so_core::is_nullptr( this_t::top() ) ) return ;
        this_t::top()->append_node( ptr ) ;
    }

public:

    rapidxml_visitor( rxml_doc_ptr_t xml_doc ) : _dptr( xml_doc )
    {
        so_log::global::error_and_exit( so_core::is_nullptr( xml_doc ),
            "[dae_module_export:rapidxml_visitor] : xml doc must not be nullptr" ) ;

        _node_stack.push( xml_doc ) ;
    }

public:

    //***********************************************************************************
    virtual void_t visit( so_dae::collada_ptr_t /*nptr*/ )
    {
        auto * root = doc().allocate_node( rapidxml::node_element, "COLLADA" ) ;

        {
            auto * attr = doc().allocate_attribute( 
                "xmlns", "http://www.collada.org/2005/11/COLLADASchema" ) ;
            root->append_attribute( attr ) ;
        }

        {
            auto * attr = doc().allocate_attribute( "version", "1.4.1" ) ;
            root->append_attribute( attr ) ;
        }

        this_t::set_cur( root ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::asset_ptr_t nptr_in )
    {
        auto * asset_node = doc().allocate_node( rapidxml::node_element, "asset" ) ;

        // contributor
        {
            auto * con_node = doc().allocate_node( rapidxml::node_element, "contributor" ) ;

            {
                auto * tool_node = doc().allocate_node( rapidxml::node_element,
                    "authoring_tool", "snakeoil COLLADA exporter 0.0.0 - very experimental" ) ;

                con_node->append_node( tool_node ) ;
            }

            asset_node->append_node( con_node ) ;
        }

        // created
        {
            auto * node = doc().allocate_node( rapidxml::node_element, "created", "0000-00-00" ) ;
            asset_node->append_node( node ) ;
        }

        // modified
        {
            auto * node = doc().allocate_node( rapidxml::node_element, "modified", "0000-00-00" ) ;
            asset_node->append_node( node ) ;
        }

        {
            auto * node = doc().allocate_node( rapidxml::node_element, "up_axis", "Y_UP" ) ;
            asset_node->append_node( node ) ;
        }

        this_t::set_cur( asset_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::unit_ptr_t nin )
    {
        auto * node = doc().allocate_node( rapidxml::node_element, "unit" ) ;
        {
            auto * attr = doc().allocate_attribute( "meter", nin->meter.c_str() ) ;
            node->append_attribute( attr ) ;
        }

        {
            auto * attr = doc().allocate_attribute( "name", nin->name.c_str() ) ;
            node->append_attribute( attr ) ;
        }

        this_t::set_cur( node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::geometry_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "geometry" ) ;
        
        // id
        {
            auto * attr = doc().allocate_attribute( "id", nin->id.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        // name
        {
            auto * attr = doc().allocate_attribute( "name", nin->name.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::mesh_ptr_t )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "mesh" ) ;
        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::source_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "source" ) ;
        
        // id
        {
            auto * attr = doc().allocate_attribute( "id", nin->id.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::float_array_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "float_array" ) ;

        // id
        {
            auto * attr = doc().allocate_attribute( "id", nin->id.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        // count
        {
            auto * attr = doc().allocate_attribute( "count", nin->count.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::technique_common_ptr_t )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "technique_common" ) ;
        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::accessor_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "accessor" ) ;

        // source
        {
            auto * attr = doc().allocate_attribute( "source", nin->src.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        // count
        {
            auto * attr = doc().allocate_attribute( "count", nin->count.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        // stride
        {
            auto * attr = doc().allocate_attribute( "stride", nin->stride.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::param_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "param" ) ;

        // name
        {
            auto * attr = doc().allocate_attribute( "name", nin->name.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        // type
        {
            auto * attr = doc().allocate_attribute( "type", nin->type.c_str() ) ;
            xml_node->append_attribute( attr ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::vertices_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "vertices" ) ;

        // id
        {
            xml_node->append_attribute( doc().allocate_attribute( "id", nin->id.c_str() ) ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::input_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "input" ) ;

        // semantic
        {
            xml_node->append_attribute( doc().allocate_attribute( "semantic", nin->semantic.c_str() ) ) ;
        }

        // src
        {
            xml_node->append_attribute( doc().allocate_attribute( "source", nin->src.c_str() ) ) ;
        }

        // offset
        if( so_core::is_not( nin->offset.empty() ) )
        {
            xml_node->append_attribute( doc().allocate_attribute( "offset", nin->offset.c_str() ) ) ;
        }

        // set
        if( so_core::is_not( nin->set.empty() ) )
        {
            xml_node->append_attribute( doc().allocate_attribute( "set", nin->set.c_str() ) ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::polygons_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "polygons" ) ;

        // count
        {
            xml_node->append_attribute( doc().allocate_attribute( "count", nin->count.c_str() ) ) ;
        }

        // material
        {
            xml_node->append_attribute( doc().allocate_attribute( "material", nin->material.c_str() ) ) ;
        }

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::primitives_ptr_t nin )
    {
        // primitives
        for( auto const & primitive : nin->indices )
        {
            this_t::set_cur( doc().allocate_node( rapidxml::node_element, "p", primitive.c_str() ) ) ;
        }
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::data_ptr_t nin )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_data, "data", 
            nin->the_data.c_str() ) ;

        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::library_effects_ptr_t )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "library_effects" ) ;
        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::library_materials_ptr_t )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "library_materials" ) ;
        this_t::set_cur( xml_node ) ;
    }

    //***********************************************************************************
    virtual void_t visit( so_dae::library_geometries_ptr_t )
    {
        auto * xml_node = doc().allocate_node( rapidxml::node_element, "library_geometries" ) ;
        this_t::set_cur( xml_node ) ;
    }

private:

    //***********************************************************************************
    virtual void_t descent( so_dae::node_ptr_t )
    {
        _node_stack.push( _cur_ptr ) ;
    }

    //***********************************************************************************
    virtual void_t ascent( so_dae::node_ptr_t )
    {
        _cur_ptr = _node_stack.pop() ;
    }

};

//*************************************************************************************
void_t create_accessor( so_dae::node_ptr_t parent, so_std::string_cref_t src, size_t const elem_count,
    size_t const fmt_n )
{
    if( fmt_n == 0 )
    {
        so_log::global::error("[dae_export] : invalid vector format for source: " + src ) ;
        return ;
    }

    // accessor
    {
        so_dae::accessor_t ac ;

        {
            ac.src = src ;
            ac.count = std::to_string(elem_count / fmt_n) ;
            ac.stride = std::to_string( fmt_n ) ;
        }

        so_std::string_t const names[] = { "X", "Y", "Z", "W" } ;

        for( size_t i=0; i<fmt_n; ++i )
        {
            
            so_dae::param_t par ;

            par.name = names[ i % 4 ] ;
            par.type = "float" ;

            ac.add_node( so_imex::memory::alloc( std::move( par ),
                "[dae_export] : param" ) ) ;
            
        }
        

        parent->add_node( so_imex::memory::alloc( std::move( ac ),
            "[dae_export] : accessor" ) ) ;
    }
}

//*************************************************************************************
void_t create_geometry_node( so_std::string_cref_t name, so_geo::flat_tri_mesh_ptr_t mesh_ptr, 
    so_dae::library_geometries_ptr_t parent_ptr, dae_module::export_params_cref_t params_in )
{
    so_dae::geometry_t geo ;
    
    // geo attribs
    {
        geo.name = name ;
        geo.id = name ;
    }

    {
        so_dae::mesh_t mesh ;

        // position
        {
            so_dae::source_t src ;
            src.id = name + ".pos";            

            size_t const elem_count = mesh_ptr->get_num_positions() ;

            // float array
            {
                so_dae::float_array_t fa ;

                {
                    fa.id = src.id + ".array";
                    fa.count = std::to_string( elem_count ) ;
                }

                {
                    so_dae::data_t d ;
                    d.the_data = this_file::float_vector_to_string( mesh_ptr->positions ) ;
                    fa.add_node( so_imex::memory::alloc( std::move( d ),
                        "[dae_export] : data" ) ) ;
                }

                src.add_node( so_imex::memory::alloc( std::move( fa ),
                    "[dae_export] : float_array" ) ) ;
            }

            // techique_common
            {
                so_dae::technique_common_t tc ;

                so_std::string_t acc_src = "#" + geo.name + ".pos.array" ;
                create_accessor( &tc, acc_src, elem_count, 
                    so_geo::vector_component_format_to_number( mesh_ptr->position_format ) ) ;

                src.add_node( so_imex::memory::alloc( std::move( tc ),
                    "[dae_export] : technique_common" ) ) ;
            }

            mesh.add_node( so_imex::memory::alloc( std::move( src ),
                "[dae_export] : source" ) ) ;
        }

        // normal
        if( mesh_ptr->normals.size() != 0 )
        {
            so_dae::source_t src ;
            src.id = name + "-nrm";

            size_t const elem_count = mesh_ptr->get_num_normals() ;

            // float array
            {
                so_dae::float_array_t fa ;

                {
                    fa.id = src.id + ".array";
                    fa.count = std::to_string( elem_count ) ;
                }

                {
                    so_dae::data_t d ;
                    d.the_data = this_file::float_vector_to_string( mesh_ptr->normals ) ;
                    fa.add_node( so_imex::memory::alloc( std::move( d ),
                        "[dae_export] : data" ) ) ;
                }

                src.add_node( so_imex::memory::alloc( std::move( fa ),
                    "[dae_export] : float_array" ) ) ;
            }
            
            // techique_common
            {
                so_dae::technique_common_t tc ;

                so_std::string_t acc_src = "#" + geo.name + ".nrm.array" ;
                create_accessor( &tc, acc_src, elem_count, 
                    so_geo::vector_component_format_to_number( mesh_ptr->normal_format ) ) ;

                src.add_node( so_imex::memory::alloc( std::move( tc ),
                    "[dae_export] : technique_common" ) ) ;
            }

            mesh.add_node( so_imex::memory::alloc( std::move( src ),
                "[dae_export] : source" ) ) ;
        }

        // texcoords
        for( size_t txc=0; txc < mesh_ptr->texcoords.size(); ++txc )
        {
            so_dae::source_t src ;
            src.id = name + ".txc." + std::to_string(txc) ;

            size_t const elem_count = mesh_ptr->get_num_texcoords(txc) ;

            // float array
            {
                so_dae::float_array_t fa ;

                {
                    fa.id = src.id + ".array";
                    fa.count = std::to_string( elem_count ) ;
                }

                {
                    so_dae::data_t d ;
                    d.the_data = this_file::float_vector_to_string( mesh_ptr->texcoords[txc] ) ;
                    fa.add_node( so_imex::memory::alloc( std::move( d ),
                        "[dae_export] : data" ) ) ;
                }

                src.add_node( so_imex::memory::alloc( std::move( fa ),
                    "[dae_export] : float_array" ) ) ;
            }

            // techique_common
            {
                so_dae::technique_common_t tc ;

                so_std::string_t acc_src = "#" + src.id + ".array" ;
                create_accessor( &tc, acc_src, elem_count, 
                    so_geo::texcoords_component_format_to_number(mesh_ptr->texcoord_format) ) ;

                src.add_node( so_imex::memory::alloc( std::move( tc ),
                    "[dae_export] : technique_common" ) ) ;
            }

            mesh.add_node( so_imex::memory::alloc( std::move( src ),
                "[dae_export] : source" ) ) ;
        }

        // vertices
        {
            so_dae::vertices vtx ;
            vtx.id = name + ".vtx" ;

            {
                so_dae::input inp ;
                inp.semantic = "POSITION" ;
                inp.src = "#"+name+".pos" ;

                vtx.add_node( so_imex::memory::alloc( std::move( inp ),
                    "[dae_export] : input" ) ) ;
            }

            mesh.add_node( so_imex::memory::alloc( std::move( vtx ),
                "[dae_export] : vertices" ) ) ;
        }

        // polygons
        {
            so_dae::polygons polys ;
            polys.count = std::to_string( mesh_ptr->indices.size() / 3 ) ;
            polys.material = "undefined" ;

            // vertex
            {
                so_dae::input inp ;
                inp.semantic = "VERTEX" ;
                inp.src = "#" + name + ".vtx" ;
                inp.offset = "0" ;

                polys.add_node( so_imex::memory::alloc( std::move( inp ),
                    "[dae_export] : input" ) ) ;
            }

            // normals
            if( mesh_ptr->normals.size() != 0 )
            {
                so_dae::input inp ;
                inp.semantic = "NORMAL" ;
                inp.src = "#" + name + ".nrm" ;
                inp.offset = "0" ;

                polys.add_node( so_imex::memory::alloc( std::move( inp ),
                    "[dae_export] : input" ) ) ;

            }

            // texcoords
            for( size_t txc = 0; txc < mesh_ptr->texcoords.size() ; ++txc )
            {
                so_dae::input inp ;
                inp.semantic = "TEXCOORD" ;
                inp.src = "#" + name + ".txc." + std::to_string( txc ) ;
                inp.set = std::to_string( txc ) ;
                inp.offset = "0" ;

                polys.add_node( so_imex::memory::alloc( std::move( inp ),
                    "[dae_export] : input" ) ) ;
            }

            // indices
            {
                so_dae::primitives prims ;

                prims.indices.resize( mesh_ptr->indices.size() / 3 ) ;

                for( size_t i = 0; i < mesh_ptr->indices.size() / 3; ++i )
                {
                    size_t const index = i * 3 ;
                    so_std::string_t primitive =
                        std::to_string( mesh_ptr->indices[ index + 0 ] ) + " " +
                        std::to_string( mesh_ptr->indices[ index + 1 ] ) + " " +
                        std::to_string( mesh_ptr->indices[ index + 2 ] ) ;

                    prims.indices[ i ] = std::move( primitive ) ;
                }

                polys.add_node( so_imex::memory::alloc( std::move( prims ),
                    "[dae_export] : polygons" ) ) ;
            }

            mesh.add_node( so_imex::memory::alloc( std::move( polys ),
                "[dae_export] : polygons" ) ) ;
        }

        geo.add_node( so_imex::memory::alloc( std::move( mesh ), 
            "[dae_export] : mesh" ) ) ;
    }

    parent_ptr->add_node( so_imex::memory::alloc( std::move( geo ),
        "[dae_export] : geometry" ) ) ;
}

//*************************************************************************************
void_t create_libary_geometries( so_dae::node_ptr_t parent_ptr, dae_module::export_params_cref_t params_in )
{
    so_dae::library_geometries_t lib_geos ;

    auto * mmgr = params_in.manager_reg_ptr->get_mesh_manager() ;

    so_imex::mesh_manager_t::handles_t hnds ;
    mmgr->acquire_all( "[dae_export] : iterate all meshes for geometry library", hnds ) ;

    for( auto & hnd : hnds )
    {
        if( so_core::can_cast< so_geo::flat_tri_mesh_ptr_t >( hnd->data_ptr ) )
        {
            auto * mesh_ptr = static_cast< so_geo::flat_tri_mesh_ptr_t >( hnd->data_ptr ) ;
            create_geometry_node( hnd.get_key(), mesh_ptr, &lib_geos, params_in ) ;
        }
        /*else if( so_core::can_cast< so_geo::polygon_mesh_ptr_t >( hnd->data_ptr ) )
        {
        auto * mesh_ptr = static_cast< so_geo::polygon_mesh_ptr_t >( hnd->data_ptr ) ;
        ( void_t ) mesh_ptr ;
        }*/
        else
        {
            so_log::global::warning( "[dae_export] : mesh type currently not supported" ) ;
        }
    }

    parent_ptr->add_node( so_imex::memory::alloc( std::move( lib_geos ),
        "[dae_export] : library_geometries" ) ) ;
}

//*************************************************************************************
so_thread::task_graph_t dae_module::export_scene( export_params_cref_t params_in )
{
    so_imex::sync_object_t::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    so_thread::itask_ptr_t tt_end = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_imex::sync_object_t::set_and_signal( params_in.sync_ptr, so_imex::ok ) ;
    } ) ;

    so_thread::itask_ptr_t tt_begin = so_thread::dyn_task_t::create(
        [=]( so_thread::itask_ptr_t self_ptr )
    {
        so_dae::document dae_doc ;

        //
        // 1. construct dae document
        //
        {
            so_dae::collada_t col ;
            {
                so_dae::unit_t u ;
                u.meter = "1.0" ;
                u.name = "cm" ;

                so_dae::asset_t as ;
                as.add_node( so_imex::memory::alloc( std::move( u ), 
                    "[dae_export] : unit node" ) ) ;
                as.up = "Y-UP" ;
                
                col.add_node( so_imex::memory::alloc( std::move( as ), 
                    "[dae_export] : asset node" ) ) ;
            }

            create_libary_geometries( &col, params_in ) ;

            dae_doc.collada_node = so_imex::memory::alloc( std::move( col ), 
                "[dae_export] : collada node" ) ;
        }

        rapidxml::xml_document<> xml_doc ;

        //
        // 2. export xml line
        //
        {
            auto * node = xml_doc.allocate_node( rapidxml::node_declaration, "xml" ) ;
            {
                auto * attr = xml_doc.allocate_attribute( "version", "1.0" ) ;
                node->append_attribute( attr ) ;
            }
            {
                auto * attr = xml_doc.allocate_attribute( "encoding", "utf-8" ) ;
                node->append_attribute( attr ) ;
            }
            xml_doc.append_node( node ) ;
        }

        // 
        // 3. export the collada document
        //
        {
            rapidxml_visitor v( &xml_doc ) ;
            dae_doc.collada_node->apply( &v ) ;
        }

        //
        // 4. write xml string to file
        //
        {
            so_std::string_t s ;
            rapidxml::print( std::back_inserter( s ), xml_doc, 0 ) ;

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

                auto const res = so_io::global::store( 
                    path, s.c_str(), s.size() * sizeof( char_t ) ).wait_for_operation() ;
                
                if( so_io::no_success( res ) )
                {
                    so_log::global::error( "[dae_module] : unable to write xml document to : " + 
                        path.string() ) ;
                }
            }
        }
    } ) ;

    return so_thread::task_graph_t( tt_begin, tt_end ) ;
}