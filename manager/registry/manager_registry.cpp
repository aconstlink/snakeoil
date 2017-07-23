//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "manager_registry.h"

#include "../code/code_manager.h"
#include "../camera/camera_manager.h"
#include "../graphics/geometry/geometry_manager.h"
#include "../graphics/imagery/framebuffer_manager.h"
#include "../graphics/imagery/image_manager.h"
#include "../graphics/imagery/texture_manager.h"
#include "../graphics/shader/shader_manager.h"
#include "../graphics/shader/program_manager.h"
#include "../graphics/variable/variable_manager.h"

#include <snakeoil/log/log.h>

using namespace so_manager ;

//*************************************************************************************
manager_registry::manager_registry( so_gpx::iuser_system_ptr_t gpx_ptr ) 
{
    so_log::log::error_and_exit( so_core::is_nullptr(gpx_ptr), 
        "[so_manager::manager_registry::manager_registry] : invalid gpx system" ) ;

    _gpx_ptr = gpx_ptr ;

    _code_manager_ptr = so_manager::code_manager_t::create(
        "[manager_registry::manager_registry] : code_manager") ;

    _camera_manager_ptr = so_manager::camera_manager_t::create( 
        "[manager_registry::manager_registry] : camera_manager" ) ;

    _gfx_shader_managers.push_back( so_manager::so_graphics::shader_manager_t::create(
        so_manager::so_graphics::shader_manager_t(_code_manager_ptr, gpx_ptr), 
        "[manager_registry::manager_registry] : shader_manager" ) ) ;

    _gfx_program_managers.push_back( so_manager::so_graphics::program_manager_t::create(
        so_manager::so_graphics::program_manager_t( _gfx_shader_managers[0], _gpx_ptr ), 
        "[manager_registry::manager_registry] : program_manager") ) ;

    _gfx_geometry_managers.push_back( so_manager::so_graphics::geometry_manager_t::create(
        so_manager::so_graphics::geometry_manager_t( gpx_ptr ), 
        "[manager_registry::manager_registry] : geometry_manager" ) ) ;

    _gfx_variable_managers.push_back( so_manager::so_graphics::variable_manager_t::create(
        so_manager::so_graphics::variable_manager_t( gpx_ptr ), 
        "[manager_registry::manager_registry] : variable_manager" ) ) ;

    _gfx_image_managers.push_back( so_manager::so_graphics::image_manager_t::create(
        so_manager::so_graphics::image_manager_t( gpx_ptr ), 
        "[manager_registry::manager_registry] : image_manager" ) ) ;

    _gfx_texture_managers.push_back( so_manager::so_graphics::texture_manager_t::create(
        so_manager::so_graphics::texture_manager_t( gpx_ptr ),
        "[manager_registry::manager_registry] : texture_manager" ) ) ;
    
    _gfx_framebuffer_managers.push_back( so_manager::so_graphics::framebuffer_manager_t::create(
        so_manager::so_graphics::framebuffer_manager_t( gpx_ptr ),
        "[manager_registry::manager_registry] : framebuffer_manager" ) ) ;
}

//*************************************************************************************
manager_registry::manager_registry( this_rref_t rhv )
{
    so_move_member_ptr( _gpx_ptr, rhv ) ;
    so_move_member_ptr( _code_manager_ptr, rhv ) ;
    so_move_member_ptr( _camera_manager_ptr, rhv ) ;

    _gfx_shader_managers = std::move( rhv._gfx_shader_managers ) ;
    _gfx_program_managers = std::move( rhv._gfx_program_managers ) ;
    _gfx_geometry_managers = std::move( rhv._gfx_geometry_managers ) ;
    _gfx_variable_managers = std::move( rhv._gfx_variable_managers ) ;
    _gfx_image_managers = std::move( rhv._gfx_image_managers ) ;
    _gfx_texture_managers = std::move( rhv._gfx_texture_managers ) ;
    _gfx_framebuffer_managers = std::move( rhv._gfx_framebuffer_managers ) ;

}

//*************************************************************************************
manager_registry::~manager_registry( void_t )
{
    for( auto ptr : _gfx_framebuffer_managers )
        so_manager::so_graphics::framebuffer_manager_t::destroy( ptr ) ;

    for( auto ptr : _gfx_texture_managers )
        so_manager::so_graphics::texture_manager_t::destroy( ptr ) ;

    for( auto ptr : _gfx_image_managers )
        so_manager::so_graphics::image_manager_t::destroy( ptr ) ;

    for( auto ptr : _gfx_variable_managers )
        so_manager::so_graphics::variable_manager_t::destroy( ptr ) ;

    for( auto ptr : _gfx_geometry_managers )
        so_manager::so_graphics::geometry_manager_t::destroy( ptr ) ;

    for( auto ptr : _gfx_program_managers )
        so_manager::so_graphics::program_manager_t::destroy( ptr ) ;

    for( auto ptr : _gfx_shader_managers )
        so_manager::so_graphics::shader_manager_t::destroy( ptr ) ;


    so_manager::code_manager_t::destroy( _code_manager_ptr ) ;
    so_manager::camera_manager_t::destroy( _camera_manager_ptr ) ;
}

//*************************************************************************************
manager_registry::this_ptr_t manager_registry::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_manager::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t manager_registry::destroy( this_ptr_t ptr )
{
    so_manager::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_manager::code_manager_ptr_t manager_registry::code_manager( void_t )
{
    return _code_manager_ptr ;
}

//*************************************************************************************
so_manager::camera_manager_ptr_t manager_registry::camera_manager( void_t )
{
    return _camera_manager_ptr ;
}

//*************************************************************************************
so_manager::so_graphics::shader_manager_ptr_t manager_registry::gpu_shader_manager( size_t i )
{
    if( i >= _gfx_shader_managers.size() )
        return nullptr ;

    return _gfx_shader_managers[i] ;
}

//*************************************************************************************
so_manager::so_graphics::program_manager_ptr_t manager_registry::gpu_program_manager( size_t i )
{
    if( i >= _gfx_program_managers.size() )
        return nullptr ;

    return _gfx_program_managers[i] ;
}

//*************************************************************************************
so_manager::so_graphics::geometry_manager_ptr_t manager_registry::gpu_geometry_manager( size_t i )
{
    if( i >= _gfx_geometry_managers.size() )
        return nullptr ;

    return _gfx_geometry_managers[i] ;
}

//*************************************************************************************
so_manager::so_graphics::variable_manager_ptr_t manager_registry::gpu_variable_manager( size_t i )
{
    if( i >= _gfx_variable_managers.size() )
        return nullptr ;

    return _gfx_variable_managers[i] ;
}

//*************************************************************************************
so_manager::so_graphics::texture_manager_ptr_t manager_registry::gpu_texture_manager( size_t i )
{
    if( i >= _gfx_texture_managers.size() )
        return nullptr ;

    return _gfx_texture_managers[i] ;
}

//*************************************************************************************
so_manager::so_graphics::image_manager_ptr_t manager_registry::gpu_image_manager( size_t i )
{
    if( i >= _gfx_image_managers.size() )
        return nullptr ;

    return _gfx_image_managers[i] ;
}

//*************************************************************************************
so_manager::so_graphics::framebuffer_manager_ptr_t manager_registry::gpu_framebuffer_manager( size_t i ) 
{
    if(i >= _gfx_framebuffer_managers.size())
        return nullptr ;

    return _gfx_framebuffer_managers[i] ;
}

//*************************************************************************************
so_manager::result manager_registry::shutdown( void_t ) 
{
    so_manager::result ready = so_manager::ok ;
    
    if( so_core::is_not(_shutdown_called) )
    {
        for( auto * ptr : _gfx_shader_managers )
        {
            ready = ptr->shutdown() ;
        }

        for( auto * ptr : _gfx_program_managers )
        {
            ready = ptr->shutdown() ;
        }

        for( auto * ptr : _gfx_geometry_managers )
        {
            ready = ptr->shutdown() ;
        }

        for( auto * ptr : _gfx_variable_managers )
        {
            ready = ptr->shutdown() ;
        }

        for( auto * ptr : _gfx_texture_managers )
        {
            ready = ptr->shutdown() ;
        }

        for( auto * ptr : _gfx_image_managers )
        {
            ready = ptr->shutdown() ;
        }

        for( auto * ptr : _gfx_framebuffer_managers )
        {
            ready = ptr->shutdown() ;
        }
        _shutdown_called = true ;
    } 

    size_t count = 0 ;
    for( auto * ptr : _gfx_shader_managers )
    {
        if( ptr->has_shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    for( auto * ptr : _gfx_program_managers )
    {
        if( ptr->has_shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    for( auto * ptr : _gfx_geometry_managers )
    {
        if( ptr->shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    for( auto * ptr : _gfx_variable_managers )
    {
        if( ptr->has_shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    for( auto * ptr : _gfx_texture_managers )
    {
        if( ptr->shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    for( auto * ptr : _gfx_image_managers )
    {
        if( ptr->shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    for( auto * ptr : _gfx_framebuffer_managers )
    {
        if( ptr->has_shutdown() == so_manager::not_ready )
            ++count ;
    }
    if( count > 0 )
        return so_manager::not_ready ;

    return ready ;
}