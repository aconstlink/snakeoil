//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "shade_manager.h"

#include <snakeoil/shade/code/icode.h>
#include <snakeoil/shade/material/imaterial.h>
#include <snakeoil/shade/shader/vertex_shader.h>
#include <snakeoil/shade/shader/pixel_shader.h>

using namespace so_imex ;

//*************************************************************************************
shade_manager::shade_manager( void_t ) 
{
}

//*************************************************************************************
shade_manager::shade_manager( this_rref_t rhv )
{
    _code_res_mgr = std::move( rhv._code_res_mgr ) ;
    _counter = rhv._counter ;
}

//*************************************************************************************
shade_manager::~shade_manager( void_t )
{
    _code_res_mgr.call_for_each( [&]( code_manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->code_ptr ) )
            item_ptr->code_ptr->destroy() ;
    } ) ;

    _material_mgr.call_for_each( [&]( material_manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->mat_ptr ) )
            item_ptr->mat_ptr->destroy() ;
    } ) ;

    _vs_mgr.call_for_each( [&]( vertex_shader_manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->shd_ptr ) )
            item_ptr->shd_ptr->destroy() ;
    } ) ;

    _ps_mgr.call_for_each( [&]( pixel_shader_manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->shd_ptr ) )
            item_ptr->shd_ptr->destroy() ;
    } ) ;
}

//*************************************************************************************
shade_manager::this_ptr_t shade_manager::create( so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
shade_manager::this_ptr_t shade_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t shade_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_code_by_path( so_io::path_cref_t path_in, 
    so_resource::purpose_cref_t p, code_handle_ref_t hnd_out ) 
{
    return _code_res_mgr.acquire_if( [&]( code_manage_item_ptr_t item_ptr )
    {
        return item_ptr->file_path == path_in ;
    }, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_code( so_imex::key_cref_t key_in, 
    so_resource::purpose_cref_t p, code_handle_ref_t hnd_out )
{
    return _code_res_mgr.acquire( key_in, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::release_code( code_handle_rref_t hnd )
{
    return _code_res_mgr.release( std::move( hnd ) ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_material_by_path( so_io::path_cref_t path_in,
    so_resource::purpose_cref_t p, material_handle_out_t hnd_out ) 
{
    return _material_mgr.acquire_if( [&]( material_manage_item_ptr_t item_ptr )
    {
        return item_ptr->file_path == path_in ;
    }, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_material( so_imex::key_cref_t key_in,
    so_resource::purpose_cref_t p, material_handle_out_t hnd_out )
{
    return _material_mgr.acquire( key_in, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::release_material( material_handle_rref_t hnd )
{
    return _material_mgr.release( std::move(hnd) ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_shader_by_path( so_io::path_cref_t path_in,
    so_resource::purpose_cref_t p, vertex_shader_handle_out_t hnd_out ) 
{
    return _vs_mgr.acquire_if( [&]( vertex_shader_manage_item_ptr_t item_ptr )
    {
        return item_ptr->file_path == path_in ;
    }, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_shader( so_imex::key_cref_t key_in,
    so_resource::purpose_cref_t p, vertex_shader_handle_ref_t hnd_out )
{
    return _vs_mgr.acquire( key_in, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::release_shader( vertex_shader_handle_rref_t hnd )
{
    return _vs_mgr.release( std::move(hnd) ) ;
}

//*************************************************************************************    
bool_t shade_manager::acquire_shader_by_path( so_io::path_cref_t path_in,
    so_resource::purpose_cref_t p, pixel_shader_handle_out_t hnd_out )
{
    return _ps_mgr.acquire_if( [&]( pixel_shader_manage_item_ptr_t item_ptr )
    {
        return item_ptr->file_path == path_in ;
    }, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::acquire_shader( so_imex::key_cref_t key_in,
    so_resource::purpose_cref_t p, pixel_shader_handle_ref_t hnd_out )
{
    return _ps_mgr.acquire( key_in, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shade_manager::release_shader( pixel_shader_handle_rref_t hnd )
{
     return _ps_mgr.release( std::move(hnd) ) ;
}

//*************************************************************************************
so_imex::result shade_manager::insert_code( so_imex::key_cref_t key_in, code_params_cref_t cp )
{
    code_manage_item mi;
    mi.code_ptr = cp.code_ptr ;
    mi.file_path = cp.file_path ;

    code_manage_item_ptr_t mi_ptr = so_imex::memory::alloc( std::move( mi ),
        "[so_imex::image_manager::manage] : manage item" ) ;

    auto const res = _code_res_mgr.insert( key_in, mi_ptr ) ;
    if(so_log::log::error( so_resource::no_success( res ),
        "[so_imex::image_manager::manage] : insert_and_acquire" ))
    {
        so_imex::memory::dealloc( mi_ptr ) ;
        return so_imex::already_managed ;
    }

    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result shade_manager::reserve_code_by_key( so_imex::key_cref_t key_in )
{
    auto const res = _code_res_mgr.reserve( key_in ) ;

    if(res == so_resource::reserved)
    {
        code_manage_item mi;
        mi.code_ptr = nullptr ;
        mi.file_path = "" ;

        code_manage_item_ptr_t mi_ptr = so_imex::memory::alloc( std::move( mi ),
        "[so_imex::image_manager::manage] : manage item" ) ;

        code_manage_item_ptr_t mi_old ;
        auto const ires = _code_res_mgr.exchange( key_in, mi_ptr, mi_old ) ;
        so_log::log::error_and_exit( so_core::not( ires ),
            "[so_imex::image_manager::reserve_by_path] : exchange must be successful" ) ;

        return so_imex::reserved ;
    }
    else if(res == so_resource::found)
        return so_imex::found ;

    return so_imex::failed ;
}

//*************************************************************************************
so_imex::result shade_manager::reserve_code_by_path( so_io::path_cref_t by_path, 
    so_imex::key_ref_t key_inout)
{
    auto const res = _code_res_mgr.reserve_or_find_if( [&]( code_manage_item_ptr_t item_ptr )
    {
        return item_ptr->file_path == by_path ;
    }, key_inout ) ;

    // if the resource could be reserved, an empty 
    // data carrier is placed.
    if(res == so_resource::reserved)
    {
        code_manage_item mi;
        mi.code_ptr = nullptr ;
        mi.file_path = "" ;

        code_manage_item_ptr_t mi_ptr = so_imex::memory::alloc( std::move( mi ),
        "[so_imex::image_manager::manage] : manage item" ) ;

        code_manage_item_ptr_t mi_old ;
        auto const ires = _code_res_mgr.exchange( key_inout, mi_ptr, mi_old ) ;
        so_log::log::error_and_exit( so_core::not( ires ),
            "[so_imex::image_manager::reserve_by_path] : exchange must be successful" ) ;

        return so_imex::reserved ;
    }
    else if(res == so_resource::found) return so_imex::found ;

    so_log::log::error( "[so_imex::image_manager::reserve_by_path] : reserve_or_find_if" ) ;
    return so_imex::invalid ;
}

//*************************************************************************************
so_imex::result shade_manager::exchange_code( so_imex::key_cref_t key_in, code_params_cref_t cp )
{
    auto const res = _code_res_mgr.exchange( key_in, [&]( code_manage_item_ptr_t mi )
    {
        mi->file_path = cp.file_path ;
        mi->code_ptr = cp.code_ptr ;
        return true ;
    } ) ;

    if( so_log::log::error( so_core::not( res ), "[so_imex::image_manager::exchange]" ) )
        return so_imex::failed ;

    return so_imex::ok ;
}

//*************************************************************************************
so_imex::result shade_manager::take_and_clear( this_ref_t other )
{
    _code_res_mgr.take_all_items_from( other._code_res_mgr ) ;
    return so_imex::ok ;
}

//*************************************************************************************
size_t shade_manager::unique_number( void_t ) 
{
    so_thread::lock_guard_t lk(_mtx_counter) ;
    return _counter++ ;
}

//*************************************************************************************
so_std::string_t shade_manager::unique_number_as_string( void_t ) 
{
    size_t const number = this_t::unique_number() ;
    return so_std::string_t( std::to_string(number) ) ;
}

//*************************************************************************************
void_t shade_manager::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}
