//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "module_registry.h"

#include "iscene_module.h"
#include "imesh_module.h"
#include "iimage_module.h"
#include "iaudio_module.h"

#include "../property/property_map.h"

#include <snakeoil/property/property_sheet.h>
#include <snakeoil/property/properties/group_property.h>

#include <snakeoil/thread/task/tasks.h>
#include <snakeoil/thread/scheduler.h>
#include <snakeoil/log/log.h>

using namespace so_imex ;

//*************************************************************************************
module_registry::module_registry( void_t )
{}

//*************************************************************************************
module_registry::module_registry( this_rref_t rhv )
{
    _scene_registry = std::move( rhv._scene_registry ) ;
    _image_registry = std::move( rhv._image_registry ) ;
    _mesh_registry = std::move( rhv._mesh_registry ) ;
}

//*************************************************************************************
module_registry::~module_registry( void_t )
{

    {
        scene_modules_t mods ;

        for(auto item : _scene_registry)
        {
            for(auto * mod_ptr : item.second)
            {
                if( std::find( mods.begin(), mods.end(), mod_ptr ) == mods.end() ) 
                {
                    mod_ptr->destroy() ;
                    mods.push_back( mod_ptr ) ;
                }
            }
        }
    }

    {
        image_modules_t mods ;

        for(auto item : _image_registry)
        {
            for(auto * mod_ptr : item.second)
            {
                if(std::find( mods.begin(), mods.end(), mod_ptr ) == mods.end())
                {
                    mod_ptr->destroy() ;
                    mods.push_back( mod_ptr ) ;
                }
            }
        }
    }

    {
        mesh_modules_t mods ;

        for(auto item : _mesh_registry)
        {
            for(auto * mod_ptr : item.second)
            {
                if(std::find( mods.begin(), mods.end(), mod_ptr ) == mods.end())
                {
                    mod_ptr->destroy() ;
                    mods.push_back( mod_ptr ) ;
                }
            }
        }
    }
}

//*************************************************************************************
module_registry::this_ptr_t module_registry::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
module_registry::this_ptr_t module_registry::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t module_registry::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_imex::result module_registry::register_module( so_imex::iscene_module_ptr_t mod_ptr )
{
    if( so_core::is_nullptr(mod_ptr) )
        return so_imex::invalid_argument ;

    auto const exts = std::move(mod_ptr->get_file_extension_names()) ;
    for( auto ext : exts ) 
    {
        auto iter = _scene_registry.find( ext ) ;
        if( iter == _scene_registry.end() )
        {
            _scene_registry[ext] = scene_modules_t({mod_ptr});
            continue ;
        }

        auto iter2 = std::find( iter->second.begin(), iter->second.end(), mod_ptr ) ;
        if( iter2 != iter->second.end() )
        {
            so_log::log::error("[so_imex::module_registry::register_module] : module already contained") ;
            continue ;
        }
        iter->second.push_back( mod_ptr ) ;
    }
    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result module_registry::register_module( so_imex::imesh_module_ptr_t mod_ptr )
{
    if( so_core::is_nullptr( mod_ptr ) )
        return so_imex::invalid_argument;

    auto const exts = std::move( mod_ptr->get_file_extension_names() );
    for( auto ext : exts )
    {
        auto iter = _mesh_registry.find( ext );
        if( iter == _mesh_registry.end() )
        {
            _mesh_registry[ext] = mesh_modules_t({mod_ptr});
            continue;
        }

        auto iter2 = std::find( iter->second.begin(), iter->second.end(), mod_ptr );
        if( iter2 != iter->second.end() )
        {
            so_log::log::error( "[so_imex::module_registry::register_module] : module already contained" );
            continue;
        }
        iter->second.push_back( mod_ptr );
    }
    return so_imex::ok;
}

//*************************************************************************************
so_imex::result module_registry::register_module( so_imex::iimage_module_ptr_t mod_ptr )
{
    if( so_core::is_nullptr( mod_ptr ) )
        return so_imex::invalid_argument;

    auto const exts = std::move( mod_ptr->get_file_extension_names() );
    for( auto ext : exts )
    {
        auto iter = _image_registry.find( ext );
        if( iter == _image_registry.end() )
        {
            _image_registry[ext] = image_modules_t({mod_ptr});
            continue;
        }

        auto iter2 = std::find( iter->second.begin(), iter->second.end(), mod_ptr );
        if( iter2 != iter->second.end() )
        {
            so_log::log::error( "[so_imex::module_registry::register_module] : module already contained" );
            continue;
        }
        iter->second.push_back( mod_ptr );
    }
    return so_imex::ok;
}

//*************************************************************************************
so_imex::result module_registry::register_module( so_imex::iaudio_module_ptr_t mod_ptr )
{
    if( so_core::is_nullptr( mod_ptr ) )
        return so_imex::invalid_argument;

    auto const exts = std::move( mod_ptr->get_file_extension_names() );
    for( auto ext : exts )
    {
        auto iter = _audio_registry.find( ext );
        if( iter == _audio_registry.end() )
        {
            _audio_registry[ ext ] = audio_modules_t( { mod_ptr } );
            continue;
        }

        auto iter2 = std::find( iter->second.begin(), iter->second.end(), mod_ptr );
        if( iter2 != iter->second.end() )
        {
            so_log::log::error( "[so_imex::module_registry::register_module] : " 
                "audio module already contained" );
            continue;
        }
        iter->second.push_back( mod_ptr );
    }
    return so_imex::ok;
}

//*************************************************************************************
bool_t module_registry::find_module_for_path( so_io::path_cref_t path_in, 
    so_imex::iscene_module_ptr_t & ptr_out )
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _scene_registry.find( ext ) ;
    if( iter == _scene_registry.end() )
        return false ;

    if( iter->second.size() == 0 )
    {
        ptr_out = nullptr ;
        return false ;
    }

    ptr_out = iter->second[0] ;
    return true ;
}

//*************************************************************************************
bool_t module_registry::find_module_for_path( so_io::path_cref_t path_in, 
    so_imex::imesh_module_ptr_t & ptr_out )
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _mesh_registry.find( ext );
    if( iter == _mesh_registry.end() )
        return false ;

    if( iter->second.size() == 0 )
    {
        ptr_out = nullptr;
        return false;
    }

    ptr_out = iter->second[0];
    return true;
}

//*************************************************************************************
bool_t module_registry::find_module_for_path( so_io::path_cref_t path_in , 
    so_imex::iimage_module_ptr_t & ptr_out )
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _image_registry.find( ext );
    if( iter == _image_registry.end() )
        return false ;

    if( iter->second.size() == 0 )
    {
        ptr_out = nullptr;
        return false;
    }

    ptr_out = iter->second[0];
    return true;
}

//*************************************************************************************
bool_t module_registry::find_module_for_path( so_io::path_cref_t path_in, 
    so_imex::iaudio_module_ptr_t & ptr_out )
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _audio_registry.find( ext );
    if( iter == _audio_registry.end() )
        return false ;

    if( iter->second.size() == 0 )
    {
        ptr_out = nullptr;
        return false;
    }

    ptr_out = iter->second[ 0 ];
    return true;
}

//*************************************************************************************
bool_t module_registry::find_modules_for_path( so_io::path_cref_t path_in, 
    this_t::scene_modules_ref_t mods_out ) 
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _scene_registry.find( ext );
    if(iter == _scene_registry.end())
        return false ;

    if(iter->second.size() == 0)
    {
        return false;
    }

    mods_out = iter->second ;

    return true ;
}

//*************************************************************************************
bool_t module_registry::find_modules_for_path( so_io::path_cref_t path_in, 
    this_t::mesh_modules_ref_t mods_out ) 
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _mesh_registry.find( ext );
    if(iter == _mesh_registry.end())
        return false ;

    if(iter->second.size() == 0)
    {
        return false;
    }

    mods_out = iter->second ;

    return true ;
}

//*************************************************************************************
bool_t module_registry::find_modules_for_path( so_io::path_cref_t path_in, 
    this_t::image_modules_ref_t mods_out ) 
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _image_registry.find( ext );
    if(iter == _image_registry.end())
        return false ;

    if(iter->second.size() == 0)
    {
        return false;
    }

    mods_out = iter->second ;

    return true ;
}

//*************************************************************************************
bool_t module_registry::find_modules_for_path( so_io::path_cref_t path_in, 
    this_t::audio_modules_ref_t mods_out )
{
    so_std::string_t const ext = path_in.extension().string().substr( 1 ) ;

    auto iter = _audio_registry.find( ext );
    if( iter == _audio_registry.end() )
        return false ;

    if( iter->second.size() == 0 )
    {
        return false;
    }

    mods_out = iter->second ;

    return true ;
}

//*************************************************************************************
bool_t module_registry::create_properties( property_map_out_t mo ) const
{
    mo = property_map_t() ;
    // access every module and let them add their sheet
    return true ;
}

//*************************************************************************************
so_thread::task_graph_t module_registry::import_scene( 
    so_imex::iscene_module::import_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        // 1. get all modules for the passed path
        this_t::scene_modules_t all_mods ;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal( 
                params_in.sync_ptr, so_imex::extension_not_supported ) ;
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr ;
                
        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so ;
            auto params_copy = params_in ;
            params_copy.sync_ptr = &so ;
            
            if( so_core::is_nullptr(params_copy.module_reg_ptr) )
                params_copy.module_reg_ptr = this ;
                 

            auto tg = mod_ptr->import_scene( params_copy ) ;
            so_thread::scheduler::ts()->async_now( std::move(tg) ) ;

            auto const res = so.wait() ;
            if( so_imex::success(res) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res ) ;
            }
        }
    }) ;

    return so_thread::task_graph_t(tptr) ;
}

//*************************************************************************************
so_thread::task_graph_t module_registry::export_scene( 
    so_imex::iscene_module::export_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=] ( void_t )
    {
        // 1. get all modules for the passed path
        this_t::scene_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            if( so_core::is_nullptr( params_copy.module_reg_ptr ) )
                params_copy.module_reg_ptr = this;


            auto tg = mod_ptr->export_scene( params_copy );
            so_thread::scheduler::ts()->async_now( std::move(tg) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}

//*************************************************************************************
so_thread::task_graph_t module_registry::import_mesh( 
    so_imex::imesh_module::import_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=] ( void_t )
    {
        // 1. get all modules for the passed path
        this_t::mesh_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            auto tg = mod_ptr->import_mesh( params_copy );
            so_thread::scheduler::ts()->async_now( std::move(tg) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}

//*************************************************************************************
so_thread::task_graph_t module_registry::export_mesh( 
    so_imex::imesh_module::export_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=] ( void_t )
    {
        // 1. get all modules for the passed path
        this_t::mesh_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            auto tg = mod_ptr->export_mesh( params_copy );
            so_thread::scheduler::ts()->async_now( std::move(tg) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}

//*************************************************************************************
so_thread::task_graph_t module_registry::import_image( 
    so_imex::iimage_module::import_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=] ( void_t )
    {
        // 1. get all modules for the passed path
        this_t::image_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            auto tg = mod_ptr->import_image( params_copy );
            so_thread::scheduler::ts()->async_now( std::move(tg) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}

//*************************************************************************************
so_thread::task_graph_t module_registry::export_image( 
    so_imex::iimage_module::export_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=] ( void_t )
    {
        // 1. get all modules for the passed path
        this_t::image_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            auto tg = mod_ptr->export_image( params_copy );
            so_thread::scheduler::ts()->async_now( std::move(tg) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}

//*************************************************************************************
so_thread::task_graph_t module_registry::import_audio( so_imex::iaudio_module::import_params_cref_t params_in )
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        // 1. get all modules for the passed path
        this_t::audio_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            auto tg = mod_ptr->import_audio( params_copy );
            so_thread::scheduler::ts()->async_now( std::move( tg ) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}

//*************************************************************************************
so_thread::task_graph_t module_registry::export_audio( so_imex::iaudio_module::export_params_cref_t params_in)
{
    so_thread::itask_ptr_t tptr = so_thread::void_funk_task_t::create( [=]( void_t )
    {
        // 1. get all modules for the passed path
        this_t::audio_modules_t all_mods;
        if( so_core::is_not( this->find_modules_for_path( params_in.path_to_file, all_mods ) ) )
        {
            return so_imex::sync_object::set_and_signal(
                params_in.sync_ptr, so_imex::extension_not_supported );
        }

        // 2. store the passed sync object for later
        so_imex::sync_object_ptr_t other_so_ptr = params_in.sync_ptr;

        // 3. try through all registered modules for the given path
        // until there are no more modules available or until the operation succeeds
        for( auto * mod_ptr : all_mods )
        {
            so_imex::sync_object so;
            auto params_copy = params_in;
            params_copy.sync_ptr = &so;

            auto tg = mod_ptr->export_audio( params_copy );
            so_thread::scheduler::ts()->async_now( std::move( tg ) );

            auto const res = so.wait();
            if( so_imex::success( res ) )
            {
                return so_imex::sync_object::set_and_signal( other_so_ptr, res );
            }
        }
    } );

    return so_thread::task_graph_t( tptr );
}