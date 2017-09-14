//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "manager_registry.h"

#include "audio_manager.h"
#include "image_manager.h"
#include "graph_manager.h"
#include "mesh_manager.h"
#include "camera_manager.h"
#include "shade_manager/material_manager.h"
#include "shade_manager/vertex_shader_manager.h"
#include "shade_manager/pixel_shader_manager.h"

using namespace so_imex ;


//*************************************************************************************
manager_registry::manager_registry( void_t ) 
{
    _audio_managers["default"] = so_imex::audio_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : audio_manager" ) ;
    _image_managers["default"] = so_imex::image_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : image_manager") ;
    _graph_managers["default"] = so_imex::graph_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : graph_manager" ) ;
    _mesh_managers["default"] = so_imex::mesh_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : mesh_manager" ) ;
    _camera_managers["default"] = so_imex::camera_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : camera_manager" ) ;
    _material_managers["default"] = so_imex::material_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : material_manager" ) ;
    _vs_managers["default"] = so_imex::vertex_shader_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : vertex_shader_manager" ) ; 
    _ps_managers["default"] = so_imex::pixel_shader_manager_t::create(
        "[so_imex::manager_registry::manager_registry] : pixel_shader_manager" ) ;
    
}

//*************************************************************************************
manager_registry::manager_registry( this_rref_t rhv )
{
    _audio_managers = std::move( rhv._audio_managers ) ;
    _image_managers = std::move( rhv._image_managers ) ;
    _graph_managers = std::move( rhv._graph_managers ) ;
    _mesh_managers = std::move( rhv._mesh_managers ) ;
    _camera_managers = std::move( rhv._camera_managers ) ;
    _material_managers = std::move( rhv._material_managers ) ;
    _vs_managers = std::move( rhv._vs_managers ) ;
    _ps_managers = std::move( rhv._ps_managers ) ;
}

//*************************************************************************************
manager_registry::~manager_registry( void_t )
{
    {
        auto * mgr_ptr = _audio_managers["default"] ;
        if( so_core::is_not_nullptr( mgr_ptr ) )
            mgr_ptr->destroy() ;
    }

    {
        auto * mgr_ptr = _graph_managers["default"] ;
        if(so_core::is_not_nullptr( mgr_ptr ))
            mgr_ptr->destroy() ;
    }

    {
        auto * mgr_ptr = _image_managers["default"] ;
        if( so_core::is_not_nullptr(mgr_ptr) )
            mgr_ptr->destroy() ;
    }   

    {
        auto * mgr_ptr = _mesh_managers["default"] ;
        if(so_core::is_not_nullptr( mgr_ptr ))
            mgr_ptr->destroy() ;
    }

    {
        auto * mgr_ptr = _camera_managers["default"] ;
        if(so_core::is_not_nullptr( mgr_ptr ))
            mgr_ptr->destroy() ;
    }

    {
        auto * mgr_ptr = _material_managers["default"] ;
        if( so_core::is_not_nullptr( mgr_ptr ) )
            mgr_ptr->destroy() ;
    }

    {
        auto * mgr_ptr = _vs_managers["default"] ;
        if( so_core::is_not_nullptr( mgr_ptr ) )
            mgr_ptr->destroy() ;
    }

    {
        auto * mgr_ptr = _ps_managers["default"] ;
        if( so_core::is_not_nullptr( mgr_ptr ) )
            mgr_ptr->destroy() ;
    }
}

//*************************************************************************************
manager_registry::this_ptr_t manager_registry::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
manager_registry::this_ptr_t manager_registry::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t manager_registry::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
audio_manager_ptr_t manager_registry::get_audio_manager( void_t )
{
    return _audio_managers["default"] ;
}

//*************************************************************************************
image_manager_ptr_t manager_registry::get_image_manager( void_t )
{
    return _image_managers["default"] ;
}

//*************************************************************************************
graph_manager_ptr_t manager_registry::get_graph_manager( void_t )
{
    return _graph_managers["default"] ;
}

//*************************************************************************************
mesh_manager_ptr_t manager_registry::get_mesh_manager( void_t )
{
    return _mesh_managers["default"] ;
}

//*************************************************************************************
camera_manager_ptr_t manager_registry::get_camera_manager( void_t ) 
{
    return _camera_managers["default"] ;
}

//*************************************************************************************
material_manager_ptr_t manager_registry::get_material_manager( void_t ) 
{
    return _material_managers["default"] ;
}

//*************************************************************************************
vertex_shader_manager_ptr_t manager_registry::get_vertex_shader_manager( void_t ) 
{
    return _vs_managers["default"] ;
}

//*************************************************************************************
pixel_shader_manager_ptr_t manager_registry::get_pixel_shader_manager( void_t ) 
{
    return _ps_managers["default"] ;
}

//*************************************************************************************
so_imex::result manager_registry::take_over( this_ptr_t other_ptr ) 
{
    if( so_core::is_nullptr(other_ptr) )
        return so_imex::invalid_argument ;

    {
        so_imex::result const res = this_t::get_audio_manager()->take_and_clear(
            *other_ptr->get_audio_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear audio_managers" ) ;
    }

    {
        so_imex::result const res = this_t::get_image_manager()->take_and_clear( 
            *other_ptr->get_image_manager() ) ;

        so_log::global::error( so_imex::no_success(res), 
            "[so_imex::manager_registry::take_over] : take_and_clear image_managers" ) ;
    }


    {
        so_imex::result const res = this_t::get_graph_manager()->take_and_clear(
            *other_ptr->get_graph_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear graph_managers" ) ;
    }

    {
        so_imex::result const res = this_t::get_mesh_manager()->take_and_clear(
            *other_ptr->get_mesh_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear mesh_managers" ) ;
    }

    {
        so_imex::result const res = this_t::get_camera_manager()->take_and_clear(
             *other_ptr->get_camera_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear camera_managers" ) ;
    }

    {
        so_imex::result const res = this_t::get_material_manager()->take_and_clear(
            *other_ptr->get_material_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear material_managers" ) ;
    }

    {
        so_imex::result const res = this_t::get_vertex_shader_manager()->take_and_clear(
            *other_ptr->get_vertex_shader_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear vertex_shader_managers" ) ;
    }

    {
        so_imex::result const res = this_t::get_pixel_shader_manager()->take_and_clear(
            *other_ptr->get_pixel_shader_manager() ) ;

        so_log::global::error( so_imex::no_success( res ),
            "[so_imex::manager_registry::take_over] : take_and_clear pixel_shader_managers" ) ;
    }

    return so_imex::ok ;

}
