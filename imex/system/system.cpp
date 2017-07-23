//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"

#include "../module/iscene_module.h"
#include "../module/imesh_module.h"
#include "../module/iimage_module.h"
#include "../module/module_registry.h"

#include "../manager/camera_manager.h"
#include "../manager/image_manager.h"
#include "../manager/graph_manager.h"
#include "../manager/mesh_manager.h"
#include "../manager/manager_registry.h"

#include "../image/deduce_image_format.h"

using namespace so_imex ;

//*************************************************************************************
system::system( void_t ) 
{
    _camera_mgr_ptr = so_imex::camera_manager::create("[so_imex::system::system] : camera manager") ;

    _mod_reg = so_imex::module_registry_t::create("[so_imex::system::system] : module registry") ;
    _mgr_reg = so_imex::manager_registry_t::create("[so_imex::system::system] : manager registry") ;
}

//*************************************************************************************
system::system( this_rref_t rhv ) 
{
    so_move_member_ptr( _mod_reg, rhv ) ;
    so_move_member_ptr( _mgr_reg, rhv ) ;
    so_move_member_ptr( _camera_mgr_ptr, rhv ) ;
}

//*************************************************************************************
system::~system( void_t ) 
{
    if( _camera_mgr_ptr != nullptr )
        _camera_mgr_ptr->destroy() ;

    so_imex::module_registry_t::destroy( _mod_reg ) ;
    so_imex::manager_registry_t::destroy( _mgr_reg ) ;
}

//*************************************************************************************
system::this_ptr_t system::create( so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
system::this_ptr_t system::create( this_rref_t rhv, so_memory::purpose_cref_t p ) 
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t system::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_imex::result system::register_module( so_imex::iscene_module_ptr_t mptr ) 
{
    return _mod_reg->register_module( mptr ) ;
}

//*************************************************************************************
so_imex::result system::register_module( so_imex::iimage_module_ptr_t mptr ) 
{
    return _mod_reg->register_module( mptr ) ;
}

//*************************************************************************************
so_imex::result system::register_module( so_imex::imesh_module_ptr_t mptr ) 
{
    return _mod_reg->register_module( mptr ) ;
}

//*************************************************************************************
so_imex::result system::register_module( so_imex::icode_module_ptr_t mptr ) 
{
    return so_imex::not_implemented ;
}

//*************************************************************************************
so_imex::result system::register_module( so_imex::iaudio_module_ptr_t mptr )
{
    return _mod_reg->register_module( mptr ) ;
}

//*************************************************************************************
so_imex::graph_manager_ptr_t system::get_graph_manager( void_t ) 
{
    return _mgr_reg->get_graph_manager() ;
}

//*************************************************************************************
so_imex::camera_manager_ptr_t system::get_camera_manager( void_t )
{
    return _camera_mgr_ptr ;
}

//*************************************************************************************
so_imex::mesh_manager_ptr_t system::get_mesh_manager( void_t )
{
    return _mgr_reg->get_mesh_manager() ;
}

//*************************************************************************************
so_imex::image_manager_ptr_t system::get_image_manager( void_t )
{
    return _mgr_reg->get_image_manager() ;
}

//*************************************************************************************
so_imex::audio_manager_ptr_t system::get_audio_manager( void_t )
{
    return _mgr_reg->get_audio_manager() ;
}

//*************************************************************************************
bool_t system::find_module_for_path( so_io::path_cref_t path_to_file, 
    so_imex::iscene_module_ptr_t & mod_ptr_out )
{
    return _mod_reg->find_module_for_path( path_to_file, mod_ptr_out ) ;
}

//*************************************************************************************
bool_t system::find_module_for_path( so_io::path_cref_t path_to_file, 
    so_imex::iimage_module_ptr_t & mod_ptr_out )
{
    return _mod_reg->find_module_for_path( path_to_file, mod_ptr_out ) ;
}

//*************************************************************************************
bool_t system::find_module_for_path( so_io::path_cref_t path_to_file, 
    so_imex::imesh_module_ptr_t & mod_ptr_out )
{
    return _mod_reg->find_module_for_path( path_to_file, mod_ptr_out ) ;
}

//*************************************************************************************
bool_t system::find_module_for_path( so_io::path_cref_t path_to_file, so_imex::iaudio_module_ptr_t & mod_ptr_out  )
{
    return _mod_reg->find_module_for_path( path_to_file, mod_ptr_out ) ;
}

//*************************************************************************************
so_imex::result system::destroy( void_t ) 
{
    this_t::destroy( this ) ;
    return so_imex::ok ;
}

//*************************************************************************************
module_registry_ptr_t system::get_module_registry( void_t ) 
{
    return _mod_reg ;
}

//*************************************************************************************
manager_registry_ptr_t system::get_manager_registry( void_t ) 
{
    return _mgr_reg ;
}
