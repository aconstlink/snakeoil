//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "so_module.h"
#include "so_document.h"
#include "so_import_context.h"

#include "../../manager/manager_registry.h"

#include <snakeoil/shade/type.h>
#include <snakeoil/shade/code/ascii_code.h>
#include <snakeoil/shade/shader/vertex_shader.h>
#include <snakeoil/shade/shader/pixel_shader.h>
#include <snakeoil/shade/shader/effect.h>

#include <snakeoil/thread/task/task_graph.h>
#include <snakeoil/thread/task/tasks.h>

#include <snakeoil/io/io.h>
#include <snakeoil/std/filesystem/filesystem.hpp>
#include <snakeoil/log/log.h>

#include <snakeoil/core/execute_if.hpp>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

using namespace so_imex ;

namespace so_pimpl
{
    struct import
    {
        static so_shade::type_t make_type(so_imex::so_jso::shader::parameter_entry_cref_t pe )
        {
            so_shade::type_base tb = so_shade::type_base::invalid ;
            so_shade::type_struct ts = so_shade::type_struct::invalid ;

            if( pe.type == "int" )
            {
                tb = so_shade::type_base::tint ;
            }
            else if( pe.type == "uint" )
            {
                tb = so_shade::type_base::tuint ;
            }
            else if( pe.type == "float" )
            {
                tb = so_shade::type_base::tfloat ;
            }
            else if( pe.type == "byte" )
            {
                tb = so_shade::type_base::tbyte ;
            }
            else if( pe.type == "double" )
            {
                tb = so_shade::type_base::tdouble ;
            }

            if( pe.type_struct == "vec1" )
            {
                ts = so_shade::type_struct::vector1 ;
            }
            else if( pe.type_struct == "vec2" )
            {
                ts = so_shade::type_struct::vector2 ;
            }
            else if( pe.type_struct == "vec3" )
            {
                ts = so_shade::type_struct::vector3 ;
            }
            else if( pe.type_struct == "vec4" )
            {
                ts = so_shade::type_struct::vector4 ;
            }
            else if( pe.type_struct == "mat2" )
            {
                ts = so_shade::type_struct::matrix2 ;
            }
            else if( pe.type_struct == "mat3" )
            {
                ts = so_shade::type_struct::matrix3 ;
            }
            else if( pe.type_struct == "mat4" )
            {
                ts = so_shade::type_struct::matrix4 ;
            }

            return so_shade::type( tb, ts ) ;
        }

        static void_t scan_variable_entry( rapidjson::Value const & v, 
            so_imex::so_jso::shader::parameters_ref_t params )
        {
            rapidjson::Value::ConstMemberIterator iter_ = v.MemberBegin() ;
            while(iter_ != v.MemberEnd())
            {
                rapidjson::Value::ConstMemberIterator citer_ = iter_++ ;

                so_std::string_t name = citer_->name.GetString() ;
                {
                    auto const iiter = params.find( name ) ;
                    if(iiter != params.end())
                    {
                        so_log::log::error( "[] : input already exists : " + name ) ;
                        continue ;
                    }
                }

                auto const has_type = citer_->value.HasMember( "type" ) &&
                    citer_->value["type"].IsString() ;
                auto const has_typestruct = citer_->value.HasMember( "type_struct" ) &&
                    citer_->value["type_struct"].IsString() ;
                auto const has_bindingpoint = citer_->value.HasMember( "binding_point" ) &&
                    citer_->value["binding_point"].IsString() ;


                if(so_log::log::error( so_core::is_not( has_type ),
                    "[] : Missing or invalid type member in input variable : " + name ))
                continue ;

                if(so_log::log::error( so_core::is_not( has_typestruct ),
                    "[] : Missing or invalid type_struct member in input variable : " + name ))
                continue ;

                so_std::string_t const type_string = citer_->value["type"].GetString() ;
                so_std::string_t const type_struct_string = citer_->value["type_struct"].GetString() ;

                so_std::string_t const binding_point_string =
                    so_core::is_not( has_bindingpoint ) ? "custom" :
                    citer_->value["binding_point"].GetString() ;


                so_imex::so_jso::shader::parameter_entry_t ie ;
                ie.binding_point = binding_point_string ;
                ie.type = type_string ;
                ie.type_struct = type_struct_string ;

                params[name] = std::move(ie) ;
            }
        }

        static so_thread::task_graph_t load_and_parse_doc( snakeoil_import_context_ptr_t ctx_ptr, 
            so_io::path_cref_t path_to_file )
        {
            so_thread::itask_ptr_t t = so_thread::dyn_task_t::create( [=]( 
                so_thread::itask_ptr_t self_ptr )
            {
                so_thread::task_graph_dyn_t inner_tg(self_ptr) ;

                so_io::path_t p = path_to_file ;

                so_std::string_t loaded_data ;

                {
                    so_io::load_handle_t lh = so_io::io::load( p ) ;
                    auto const res = lh.wait_for_operation( [&]( 
                        char_cptr_t din, size_t sib, so_io::result res )
                    {
                        if( so_io::no_success( res ) ) return ;
                        loaded_data = so_std::string_t( din, sib ) ;
                    } ) ;
                    
                    if( so_io::no_success( res ) )
                    {
                        so_log::log::error( "[] : waiting for load operation failed : " + 
                            p.string() ) ;
                        return ;
                    }
                }

                so_imex::so_jso::document_t jso_doc ;
                rapidjson::Document adoc ;

                if(adoc.Parse( loaded_data.c_str() ).HasParseError())
                {
                    auto const err = adoc.GetParseError() ;
                    (void_t)err ; // do something with the error code
                    so_log::log::error( "[] : parse error in json file : " + p.string( ) ) ;
                    return ;
                }

                if(so_core::is_not( adoc.IsObject() ))
                {
                    so_log::log::error( "[] : document does not start with an object : " + p.string( ) ) ;
                    return ;
                }

                so_core::execute_if( adoc.HasMember( "files" ), [&]( void_t )
                {
                    rapidjson::Value & v_files = adoc["files"] ;
                    if(so_core::is_not( v_files.IsArray() ))
                    {
                        so_log::log::error( "[] : files is not an array : " + p.string( ) ) ;
                        return false ;
                    }

                    for(rapidjson::SizeType i = 0; i < v_files.Size(); ++i)
                    {
                        if(so_log::log::error( so_core::is_not( v_files[i].IsString() ),
                            "[] : files entry is not a string: " + p.string( ) ))
                            continue ;

                        so_io::path_t const new_p = v_files[i].GetString() ;
                        jso_doc.files_data.push_back( std::move( new_p ) ) ;
                    }

                    so_std::filesystem::path path_base(p) ;
                    path_base.remove_filename() ;

                    for( auto const & ipath : jso_doc.files_data )
                    {
                        so_std::filesystem::path new_path(path_base) ;
                        so_std::filesystem::path path_to_load( ipath ) ;
                        if( path_to_load.has_relative_path() )
                        {
                            new_path /= path_to_load ;
                        }
                        else
                        {
                            new_path = path_to_load ;
                        }

                        // @todo enable loading
                        //inner_tg.in_between( so_pimpl::import::load_and_parse_doc(ctx_ptr, new_path.string()) ) ;
                    }

                    return true ;
                } ) ;

                so_core::execute_if( adoc.HasMember( "assets" ), [&]( void_t )
                {
                    rapidjson::Value & v_assets = adoc["assets"] ;
                    if(so_core::is_not( v_assets.IsObject() ))
                    {
                        so_log::log::error( "[] : assets is not an object : " + p.string( ) ) ;
                        return false ;
                    }

                    so_core::execute_if( v_assets.HasMember( "sources" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_assets["sources"] ;
                        if(so_core::is_not( rj_entry.IsObject() ))
                        {
                            so_log::log::error( "[] : sources is not an object : " + p.string( ) ) ;
                            return false ;
                        }

                        so_std::filesystem::path path_base( p ) ;
                        path_base.remove_filename() ;

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while(iter != rj_entry.MemberEnd())
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if(so_log::log::error(
                                so_core::is_not( citer->name.IsString() && citer->value.IsString() ),
                                "[] : fragments entry is invalid: " + p.string( ) ))
                                continue ;

                            so_std::filesystem::path new_path( path_base ) ;
                            so_std::filesystem::path path_to_load( citer->value.GetString() ) ;
                            if( path_to_load.has_relative_path() )
                            {
                                new_path /= path_to_load ;
                            }
                            else
                            {
                                new_path = path_to_load ;
                            }

                            so_std::string_t const name = citer->name.GetString() ;
                            so_io::path_t const value = new_path ;

                            auto const iiter = jso_doc.assets_data.sources.find( name ) ;
                            if(so_log::log::error( iiter != jso_doc.assets_data.sources.end(),
                                "[] : fragments entry not unique: " + name + " in " + p.string( ) ))
                                continue ;

                            jso_doc.assets_data.sources[name] = value ;
                        }

                        return true ;
                    } ) ;

                    so_core::execute_if( v_assets.HasMember( "geos" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_assets["geos"] ;
                        if(so_core::is_not( rj_entry.IsObject() ))
                        {
                            so_log::log::error( "[] : geos is not an object : " + p.string( ) ) ;
                            return false ;
                        }

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while(iter != rj_entry.MemberEnd())
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if(so_log::log::error(
                                so_core::is_not( citer->name.IsString() && citer->value.IsString() ),
                                "[] : fragments entry is invalid: " + p.string( ) ))
                                continue ;

                            so_std::string_t const name = citer->name.GetString() ;
                            so_io::path_t const value = so_io::path_t( citer->value.GetString() ) ;

                            auto const iiter = jso_doc.assets_data.geos.find( name ) ;
                            if(so_log::log::error( iiter != jso_doc.assets_data.geos.end(),
                                "[] : geos entry not unique: " + name + " in " + p.string( ) ))
                                continue ;

                            jso_doc.assets_data.geos[name] = value ;
                        }

                        return true ;

                    } ) ;

                    so_core::execute_if( v_assets.HasMember( "images" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_assets["images"] ;
                        if(so_core::is_not( rj_entry.IsObject() ))
                        {
                            so_log::log::error( "[] : images is not an object : " + p.string( ) ) ;
                            return false ;
                        }

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while(iter != rj_entry.MemberEnd())
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if(so_log::log::error(
                                so_core::is_not( citer->name.IsString() && citer->value.IsString() ),
                                "[] : images entry is invalid: " + p.string( ) ))
                                continue ;

                            so_std::string_t const name = citer->name.GetString() ;
                            so_io::path_t const value = so_io::path_t( citer->value.GetString() ) ;

                            auto const iiter = jso_doc.assets_data.images.find( name ) ;
                            if(so_log::log::error( iiter != jso_doc.assets_data.images.end(),
                                "[] : images entry not unique: " + name + " in " + p.string( ) ))
                                continue ;

                            jso_doc.assets_data.images[name] = value ;
                        }

                        return true ;
                    } ) ;

                    so_core::execute_if( v_assets.HasMember( "scenes" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_assets["scenes"] ;
                        if(so_core::is_not( rj_entry.IsObject() ))
                        {
                            so_log::log::error( "[] : scenes is not an object : " + p.string( ) ) ;
                            return false ;
                        }

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while(iter != rj_entry.MemberEnd())
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if(so_log::log::error(
                                so_core::is_not( citer->name.IsString() && citer->value.IsString() ),
                                "[] : scenes entry is invalid: " + p.string( ) ))
                                continue ;

                            so_std::string_t const name = citer->name.GetString() ;
                            so_io::path_t const value = so_io::path_t( citer->value.GetString() ) ;

                            auto const iiter = jso_doc.assets_data.scenes.find( name ) ;
                            if(so_log::log::error( iiter != jso_doc.assets_data.scenes.end(),
                                "[] : scenes entry not unique: " + name + " in " + p.string( ) ))
                                continue ;

                            jso_doc.assets_data.scenes[name] = value ;
                        }

                        return true ;
                    } ) ;

                    return true ;
                } ) ;

                so_core::execute_if( adoc.HasMember( "objects" ), [&]( void_t )
                {
                    rapidjson::Value & v_objects = adoc["objects"] ;
                    so_core::execute_if( v_objects.HasMember( "shaders" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_objects["shaders"] ;
                        if(so_core::is_not( rj_entry.IsObject() ))
                        {
                            so_log::log::error( "[] : shaders is not an object : " + p.string( ) ) ;
                            return false ;
                        }

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while(iter != rj_entry.MemberEnd())
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if(so_log::log::error( so_core::is_not( citer->name.IsString() ),
                                "[] : shader entry is invalid: " + p.string( ) ))
                                continue ;

                            so_imex::so_jso::shader shd ;
                            so_std::string_t const shd_name = citer->name.GetString() ;

                            // check existence
                            {
                                auto iiter = jso_doc.objects_data.shaders.find( shd_name ) ;
                                if(iiter != jso_doc.objects_data.shaders.end())
                                {
                                    so_log::log::error( "[] : Shader already exists : " + shd_name ) ;
                                    continue ;
                                }
                            }

                            if(so_log::log::error( so_core::is_not( citer->value.IsObject() ),
                                "[] : shader entry is invalid: " + p.string( ) ))
                                continue ;

                            // type
                            {
                                auto const exec_res = so_core::execute_if(
                                citer->value.HasMember( "type" ) && citer->value["type"].IsString(), [&]( void_t )
                                {
                                    shd.type = so_std::string_t( citer->value["type"].GetString() ) ;
                                    return true ;
                                } ) ;

                                if(so_core::is_not( exec_res ))
                                {
                                    so_log::log::error( "[] : shader entry has no type: " + shd_name ) ;
                                    continue ;
                                }
                            }

                            // fragment_ids
                            {
                                auto const exec_res = so_core::execute_if(
                                citer->value.HasMember( "fragment_ids" ) &&
                                citer->value["fragment_ids"].IsArray(), [&]( void_t )
                                {
                                    rapidjson::Value const & the_array = citer->value["fragment_ids"] ;
                                    for(rapidjson::SizeType i = 0; i < the_array.Size(); ++i)
                                    {
                                        rapidjson::Value const & v = the_array[i] ;
                                        if(so_core::is_not( v.IsString() )) continue ;

                                        shd.fragment_ids.push_back( so_std::string_t( v.GetString() ) ) ;
                                    }
                                    return true ;
                                } ) ;

                                if(so_core::is_not( exec_res ))
                                {
                                    so_log::log::error( "[] : shader entry has no type: " + shd_name ) ;
                                    continue ;
                                }
                            }

                            // inputs
                            {
                                so_core::execute_if( citer->value.HasMember( "inputs" ) &&
                                citer->value["inputs"].IsObject(), [&]( void_t )
                                {
                                    rapidjson::Value const & v = citer->value["inputs"] ;
                                    so_pimpl::import::scan_variable_entry( v, shd.inputs ) ;
                                    return true ;
                                } ) ;
                            }

                            // outputs
                            {
                                so_core::execute_if( citer->value.HasMember( "outputs" ) &&
                                citer->value["outputs"].IsObject(), [&]( void_t )
                                {
                                    rapidjson::Value const & v = citer->value["outputs"] ;
                                    so_pimpl::import::scan_variable_entry( v, shd.outputs ) ;
                                    return true ;
                                } ) ;
                            }

                            // parameters
                            {
                                so_core::execute_if( citer->value.HasMember( "parameters" ) &&
                                citer->value["parameters"].IsObject(), [&]( void_t )
                                {
                                    rapidjson::Value const & v = citer->value["parameters"] ;
                                    so_pimpl::import::scan_variable_entry( v, shd.parameters ) ;
                                    return true ;
                                } ) ;
                            }

                            jso_doc.objects_data.shaders[shd_name] = std::move( shd ) ;
                        }

                        return true ;
                    } ) ;

                    so_core::execute_if( v_objects.HasMember( "effects" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_objects["effects"] ;
                        if(so_core::is_not( rj_entry.IsObject() ))
                        {
                            so_log::log::error( "[] : effects is not an object : " + p.string( ) ) ;
                            return false ;
                        }

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while(iter != rj_entry.MemberEnd())
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if(so_log::log::error( so_core::is_not( citer->name.IsString() ),
                                "[] : name is not a string : " + p.string( ) ))
                                continue ;

                            so_std::string_t const name = citer->name.GetString() ;

                            if(so_log::log::error( so_core::is_not( citer->value.IsObject() ),
                                "[] : value is not an object : " + name + " in " + p.string( ) ))
                                continue ;

                            so_imex::so_jso::effect_t effect ;

                            so_core::execute_if( citer->value.HasMember( "vertex_shader" ), [&]( void_t )
                            {
                                if(so_core::is_not( citer->value["vertex_shader"].IsString() ))
                                    return false ;

                                effect.vertex_shader = citer->value["vertex_shader"].GetString() ;
                                return true ;
                            } ) ;

                            so_core::execute_if( citer->value.HasMember( "geometry_shader" ), [&]( void_t )
                            {
                                if(so_core::is_not( citer->value["geometry_shader"].IsString() ))
                                    return false ;

                                effect.geometry_shader = citer->value["geometry_shader"].GetString() ;
                                return true ;
                            } ) ;

                            so_core::execute_if( citer->value.HasMember( "pixel_shader" ), [&]( void_t )
                            {
                                if(so_core::is_not( citer->value["pixel_shader"].IsString() ))
                                    return false ;

                                effect.pixel_shader = citer->value["pixel_shader"].GetString() ;
                                return true ;
                            } ) ;

                            {
                                auto const iiter = jso_doc.objects_data.effects.find( name ) ;
                                if(iiter != jso_doc.objects_data.effects.end())
                                {
                                    so_log::log::error( "[] : effect already exists : " + name ) ;
                                    continue ;
                                }
                            }

                            jso_doc.objects_data.effects[name] = effect ;
                        }
                        return true ;
                    } ) ;

                    //
                    // At the moment, NO render configs
                    // 
                    so_core::execute_if( v_objects.HasMember( "render_configs" ), [&]( void_t )
                    {
                        rapidjson::Value & rj_entry = v_objects[ "render_configs" ] ;
                        if( so_core::is_not( rj_entry.IsObject() ) )
                        {
                            so_log::log::error( "[] : render_configs is not an object : " + p.string() ) ;
                            return false ;
                        }

                        rapidjson::Value::ConstMemberIterator iter = rj_entry.MemberBegin() ;
                        while( iter != rj_entry.MemberEnd() )
                        {
                            rapidjson::Value::ConstMemberIterator citer = iter++ ;

                            if( so_log::log::error( so_core::is_not( citer->name.IsString() ),
                                "[] : name is not a string : " + p.string() ) )
                                continue ;

                            so_std::string_t const name = citer->name.GetString() ;

                            if( so_log::log::error( so_core::is_not( citer->value.IsObject() ),
                                "[] : value is not an object : " + name + " in " + p.string() ) )
                                continue ;

                            {
                                auto const iiter = jso_doc.objects_data.render_configs.find( name ) ;
                                if( iiter != jso_doc.objects_data.render_configs.end() )
                                {
                                    so_log::log::error( "[] : render_config already exists : " + name ) ;
                                    continue ;
                                }
                            }

                            so_imex::so_jso::render_config_t rc ;

                            so_core::execute_if( citer->value.HasMember( "effect" ), [&]( void_t )
                            {
                                if( so_core::is_not( citer->value[ "effect" ].IsString() ) )
                                    return false ;

                                rc.effect_name = citer->value[ "effect" ].GetString() ;
                                return true ;
                            } ) ;

                            so_core::execute_if( citer->value.HasMember( "geometry" ), [&]( void_t )
                            {
                                if( so_core::is_not( citer->value[ "geometry" ].IsString() ) )
                                    return false ;

                                rc.geometry_name = citer->value[ "geometry" ].GetString() ;
                                return true ;
                            } ) ;

                            so_core::execute_if( citer->value.HasMember( "variable_sets" ), [&]( void_t )
                            {
                                rapidjson::Value const & vs_entry = citer->value[ "variable_sets" ] ;
                                if( so_core::is_not( vs_entry.IsObject() ) )
                                {
                                    so_log::log::error( "[] : variable_sets is not an object : " 
                                        + p.string() ) ;
                                    return false ;
                                }

                                rapidjson::Value::ConstMemberIterator iter = vs_entry.MemberBegin() ;
                                while( iter != vs_entry.MemberEnd() )
                                {
                                    rapidjson::Value::ConstMemberIterator iciter = iter++ ;

                                    if( so_log::log::error( so_core::is_not( iciter->name.IsString() ),
                                        "[] : name is not a string : " + p.string() ) )
                                        continue ;

                                    so_std::string_t const name = iciter->name.GetString() ;

                                    {
                                        auto const iiter = rc.variable_sets.find( name ) ;
                                        if( iiter != rc.variable_sets.end() )
                                        {
                                            so_log::log::error( "[] : variable_set already exists : " + name ) ;
                                            continue ;
                                        }
                                    }

                                    so_imex::so_jso::render_config_t::variable_set_t vs ;

                                    // fill in variable_set

                                    rc.variable_sets[ name ] = std::move( vs ) ;
                                }

                                return true ;
                            } ) ;
                            
                            jso_doc.objects_data.render_configs[ name ] = std::move( rc ) ;
                        }
                        return true ;
                    } ) ;

                    return true ;
                } ) ;

                // construct data struct
                // check for more loading

                auto * doc_out = so_memory::memory::alloc( 
                    so_imex::so_jso::document_t(std::move(jso_doc)), 
                    "[] : parsed jso document" ) ;

                if( so_core::is_not( ctx_ptr->add_document( path_to_file, doc_out ) ) )
                {
                    so_log::log::error("[so_imex::so_jso::load_and_parse_doc] : "
                        "document already exists : " + path_to_file.string( ) ) ;

                    so_memory::memory::dealloc( doc_out ) ;
                }

            } ) ;

            return so_thread::task_graph_t( t ) ;
        }

        static so_thread::task_graph_t interpret_docs( snakeoil_import_context_ptr_t ctx_ptr ) 
        {
            so_thread::itask_ptr_t end = so_thread::void_funk_task_t::create_noop(
                "[so_imex::so_jso::interpret_docs] : merge task" ) ;

            so_thread::itask_ptr_t beg = so_thread::dyn_task_t::create( 
                [=]( so_thread::itask_ptr_t self )
            {
                so_thread::task_graph_dyn_t tg( self, end ) ;

                auto docs = ctx_ptr->get_documents() ;

                //
                // load all sources
                //
                so_thread::task_graph_t tg_sources ;
                for( auto & item : docs )
                {
                    so_imex::so_jso::document_ptr_t dptr = item.second ;
                    for( auto & source_item : dptr->assets_data.sources )
                    {
                        tg_sources.in_between( so_thread::void_funk_task_t::create( [=]( void_t )
                        {
                            so_std::string_t loaded_data ;
                            
                            {
                                so_io::load_handle_t lh = so_io::io::load( source_item.second ) ;
                                auto const res = lh.wait_for_operation( [&]( 
                                    char_cptr_t din, size_t sib, so_io::result res )
                                {
                                    if( so_io::no_success( res ) ) return ;
                                    loaded_data = so_std::string_t( din, sib ) ;
                                } ) ;

                                if( so_log::log::error( so_io::no_success( res ),
                                    "[] : failed to load file : " + source_item.second.string() ) )
                                    return ;
                            }

                            {
                                auto const code_res = ctx_ptr->add_code( source_item.first,
                                    so_shade::ascii_code_t( loaded_data.c_str() ) ) ;

                                if( so_core::is_not(code_res) )
                                {
                                    so_log::log::error( "[so_imex::so_jso::interpret_docs] : "
                                        "failed to add code into context : " + source_item.first ) ;

                                    return  ;
                                }
                            }

                        }, "[so_imex::so_jso::interpret_docs] : load source task" ) );
                    }
                }
                tg.in_between( std::move( tg_sources ) ) ;

                //
                // create all shaders
                // 
                so_thread::task_graph_t tg_shaders ;
                for( auto & item : docs )
                {
                    tg_shaders.in_between( so_thread::dyn_task_t::create(
                        [=]( so_thread::itask_ptr_t self )
                    {
                        so_shade::so_shader::effect_t effect ;

                        so_imex::so_jso::document_ptr_t dptr = item.second ;
                        for( auto & shader_item : dptr->objects_data.shaders )
                        {
                        
                            so_imex::so_jso::shader const & shd = shader_item.second ;
                            if( shd.type == "vertex_shader" )
                            {
                                so_shade::so_shader::vertex_shader vs ;
                            
                                for( auto const & frag_item : shd.fragment_ids )
                                {
                                    so_shade::ascii_code_t code ;
                                    auto const cres = ctx_ptr->get_code( frag_item, code ) ;
                                    if( so_core::is_not( cres ) )
                                    {
                                        so_log::log::error( "[so_imex::so_jso::interpret_docs] : "
                                            "fragment id not found : " + frag_item ) ;
                                        continue ;
                                    }

                                    vs.set_code( code ) ;
                                }

                                for( auto const & input_item : shd.inputs )
                                {
                                
                                    so_imex::so_jso::shader::parameter_entry_cref_t pe =
                                        input_item.second ;

                                    so_shade::type_t const type = so_pimpl::import::make_type( pe ) ;

                                    if( pe.binding_point == "position" )
                                    {
                                        vs.set( so_shade::so_shader::position_binding{
                                            so_shade::variable_t( input_item.first, type )} ) ;
                                    }
                                    else if( pe.binding_point == "normal" )
                                    {
                                        vs.set( so_shade::so_shader::normal_binding{
                                            so_shade::variable_t( input_item.first, type ) } ) ;
                                    }
                                    else if( pe.binding_point == "texcoord0" )
                                    {
                                        vs.add( so_shade::so_shader::texcoord_binding{
                                            so_shade::variable_t( input_item.first, type ) } ) ;
                                    }
                                    else if( pe.binding_point == "texcoord1" )
                                    {
                                        vs.add( so_shade::so_shader::texcoord_binding{
                                            so_shade::variable_t( input_item.first, type ) } ) ;
                                    }
                                    else if( pe.binding_point == "texcoord2" )
                                    {
                                        vs.add( so_shade::so_shader::texcoord_binding{
                                            so_shade::variable_t( input_item.first, type ) } ) ;
                                    }
                                    else if( pe.binding_point == "texcoord3" )
                                    {
                                        vs.add( so_shade::so_shader::texcoord_binding{
                                            so_shade::variable_t( input_item.first, type ) } ) ;
                                    }
                                    else
                                    {
                                        so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                            "unknown vertex shader input" ) ;
                                    }
                                }

                                for( auto const & param_item : shd.parameters )
                                {
                                    so_imex::so_jso::shader::parameter_entry_cref_t pe =
                                        param_item.second ;

                                    so_shade::type_t const type = so_pimpl::import::make_type( pe ) ;
                                
                                    if( pe.binding_point == "custom" )
                                    {
                                        // custom binding may not be required
                                        so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                            "custom parameter not implemented" ) ;
                                    }
                                    else
                                    {
                                        so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                            "unknown vertex shader parameter" ) ;
                                    }

                                }

                                for( auto const & out_item : shd.outputs )
                                {
                                    so_imex::so_jso::shader::parameter_entry_cref_t pe =
                                        out_item.second ;

                                    so_shade::type_t const type = so_pimpl::import::make_type( pe ) ;

                                    so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                        "output variable not implemented" ) ;
                                }
                                effect.set_vertex_shader( so_shade::so_shader::vertex_shader_t::create(
                                    std::move( vs ), "[so_imex::so_jso::interpret_docs] : vertex_shader" ) ) ;
                            }
                            else if( shd.type == "geometry_shader" ) {}
                            else if( shd.type == "pixel_shader" ) 
                            {
                                so_shade::so_shader::pixel_shader ps ;

                                for( auto const & input_item : shd.inputs )
                                {
                                    so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                        "input variable in pixel shader not implemented : " + 
                                        input_item.first ) ;
                                }

                                for( auto const & param_item : shd.parameters )
                                {
                                    so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                        "parameter variable in pixel shader not implemented : " + 
                                        param_item.first ) ;
                                }

                                for( auto const & out_item : shd.outputs )
                                {
                                    so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                        "output variable in pixel shader not implemented : " +
                                    out_item.first ) ;
                                }
                                effect.set_pixel_shader( so_shade::so_shader::pixel_shader_t::create(
                                    std::move(ps), "[so_imex::so_jso::interpret_docs] : pixel_shader" ) ) ;
                            }
                            else 
                            {
                                so_log::log::warning( "[so_imex::so_jso::interpret_docs] : "
                                    "unsupported shader type" ) ;
                            }

                        

                            /*for( auto const & output_item : shd.outputs )
                            {
                            }

                            for( auto const & param_item : shd.parameters )
                            {
                            }*/
                        }
                    } ) ) ;
                }

                tg.then( std::move(tg_shaders) ) ;
                
            } ) ;

            return so_thread::task_graph_t( beg, end ) ;
        }
    };
}

//**********************************************************************************************
so_thread::task_graph_t snakeoil_module::import_scene( import_params_cref_t params_in )
{
    so_imex::sync_object_t::set_not_signal( params_in.sync_ptr, so_imex::not_ready ) ;

    so_thread::itask_ptr_t tt_end = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        so_imex::sync_object_t::set_and_signal( params_in.sync_ptr, so_imex::ok ) ;
    } ) ;

    so_thread::itask_ptr_t tt_begin = so_thread::dyn_task_t::create(
        [=]( so_thread::itask_ptr_t self_ptr )
    {
        so_thread::task_graph_dyn_t tg_dyn( self_ptr, tt_end ) ;

        so_imex::snakeoil_import_context_ptr_t imp_ctx = so_memory::memory::alloc(
            so_imex::snakeoil_import_context_t(params_in), "[] : snakeoil import context" ) ;
        
        so_thread::task_graph_t the_tg = so_pimpl::import::load_and_parse_doc( 
            imp_ctx, params_in.path_to_file ) ;

        // interpret here
        the_tg.then( so_pimpl::import::interpret_docs(imp_ctx) ) ;

        tg_dyn.in_between( std::move(the_tg) ) ;
    } ) ;

    return so_thread::task_graph_t( tt_begin, tt_end ) ;
}

//**********************************************************************************************
so_thread::itask_ptr_t snakeoil_module::load_file( 
    so_io::path_cref_t, snakeoil_import_context_ref_t ) 
{
    return nullptr ;
}

