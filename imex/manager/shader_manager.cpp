//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "shader_manager.h"

using namespace so_imex ;

//*************************************************************************************
shader_manager::shader_manager( void_t ) 
{

}

//*************************************************************************************
shader_manager::shader_manager( this_rref_t rhv ) : 
    _vs_manager( std::move(rhv._vs_manager) )
{}

//*************************************************************************************
shader_manager::~shader_manager( void_t )
{
    _vs_manager.do_destruction( [=]( vs_manager_t::manage_item_ptr_t item_ptr )
    {
        if( so_core::is_not_nullptr( item_ptr->data_ptr ) )
            so_imex::memory::dealloc( item_ptr->data_ptr ) ;
    } ) ;
}

//*************************************************************************************
shader_manager::this_ptr_t shader_manager::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move(rhv), p ) ;
}

//*************************************************************************************
void_t shader_manager::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
bool_t shader_manager::acquire_by_path( so_io::path_cref_t path_in,
    so_resource::purpose_cref_t p, handle_vs_ref_t hnd_out ) 
{
    return _vs_manager.acquire_by_path( path_in, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shader_manager::acquire( so_imex::key_cref_t key_in,
    so_resource::purpose_cref_t p, handle_vs_ref_t hnd_out )
{
    return _vs_manager.acquire( key_in, p, hnd_out ) ;
}

//*************************************************************************************
bool_t shader_manager::release( handle_vs_rref_t hnd )
{
    return _vs_manager.release( std::move(hnd) ) ;
}

//*************************************************************************************
void_t shader_manager::destroy( void_t )
{
    this_t::destroy( this ) ;
}

//*************************************************************************************
