//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"

#include <snakeoil/log/log.h>

#include "../module/so/so_module.h"

// stb ships with the engine, so it can be a default module
#include "../module/stb/stb_module.h"
#include "../module/dae/dae_module.h"
#include "../module/obj/obj_module.h"
#include "../module/wav/wav_module.h"

#if defined( MELANGESDK_ENABLE )
#include "../module/c4d/c4d_module.h"
#endif

#if defined( FBXSDK_ENABLE )
#include "../module/fbx/fbx_module.h"
#endif

#if defined( OPENCTM_ENABLE )
#include "../module/ctm/ctm_module.h"
#endif

using namespace so_imex ;

//*************************************************************************************
system::this_ptr_t system::create_with_default_modules( void_t ) 
{
    return this_t::create_with_default_modules( 
        "[so_imex::system::create_with_default_modules]" ) ;
}

//*************************************************************************************
system::this_ptr_t system::create_with_default_modules( so_memory::purpose_cref_t p ) 
{
    this_ptr_t sys_ptr = this_t::create( p ) ;

    {
        auto * stb_ptr = so_imex::stb_module::create( p + " : stb module") ;    
        auto res = sys_ptr->register_module( stb_ptr ) ;
        so_log::log::error( so_imex::no_success(res), 
            "[so_imex::system::create_with_default_modules] : register stb module" ) ;
    }

    {
        auto * so_ptr = so_imex::snakeoil_module_t::create( p + " : snakeoil module" ) ;
        auto res = sys_ptr->register_module( so_ptr ) ;
        so_log::log::error( so_imex::no_success( res ),
            "[so_imex::system::create_with_default_modules] : register snakeoil module" ) ;
    }

    {
        auto * so_ptr = so_imex::dae_module_t::create( p + " : collada module" ) ;
        auto res = sys_ptr->register_module( so_ptr ) ;
        so_log::log::error( so_imex::no_success( res ),
            "[so_imex::system::create_with_default_modules] : register collada module" ) ;
    }

    {
        auto * so_ptr = so_imex::obj_module_t::create( p + " : obj module" ) ;
        auto res = sys_ptr->register_module( so_ptr ) ;
        so_log::log::error( so_imex::no_success( res ),
            "[so_imex::system::create_with_default_modules] : register obj module" ) ;
    }

    {
        auto * so_ptr = so_imex::wav_module_t::create( p + " : wav module" ) ;
        auto res = sys_ptr->register_module( so_ptr ) ;
        so_log::log::error( so_imex::no_success( res ),
            "[so_imex::system::create_with_default_modules] : register wav module" ) ;
    }

#if defined( MELANGESDK_ENABLE )

    {
        auto * mptr = so_imex::c4d_module::create( p + " : c4d melange sdk module" ) ;
        auto res = sys_ptr->register_module( mptr ) ;
        so_log::log::error( so_imex::no_success(res), 
            "[so_imex::system::create_with_default_modules] : register c4d melange module" ) ;
    }

#endif

#if defined( FBXSDK_ENABLE )
    
    {
        auto * fbx_ptr = so_imex::fbx_module::create( p + " : fbx module" ) ;
        auto res = sys_ptr->register_module( fbx_ptr ) ;
        so_log::log::error( so_imex::no_success(res), 
            "[so_imex::system::create_with_default_modules] : register fbx" ) ;
    }

#endif

#if defined( OPENCTM_ENABLE )
    
    {
        auto * ctm_ptr = so_imex::ctm_module::create( p + " : ctm module" ) ;
        auto res = sys_ptr->register_module( ctm_ptr ) ;
        so_log::log::error( so_imex::no_success(res), 
            "[so_imex::system::create_with_default_modules] : register ctm" ) ;
    }

#endif
    
    // more register stuff here

    return sys_ptr ;
}
