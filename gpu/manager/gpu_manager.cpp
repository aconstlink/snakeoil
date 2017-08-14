//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "gpu_manager.h"

using namespace so_gpu ;

//***************************************************************
gpu_manager::gpu_manager( void_t ) 
{
    _fb2d_mgr = so_gpu::memory::alloc( this_t::fb2d_manager_t(), 
        "[gpu_manager::gpu_manager] : fb2d manager" ) ;

    _tx2d_mgr = so_gpu::memory::alloc( this_t::tx2d_manager_t(),
        "[gpu_manager::gpu_manager] : tx2d manager" ) ;

    _img2d_mgr = so_gpu::memory::alloc( this_t::img2d_manager_t(),
        "[gpu_manager::gpu_manager] : image_2d manager" ) ;

    _vb_mgr = so_gpu::memory::alloc( this_t::vb_manager_t(), 
        "[gpu_manager::gpu_manager] : vb manager" ) ;

    _ib_mgr = so_gpu::memory::alloc( this_t::ib_manager_t(),
        "[gpu_manager::gpu_manager] : ib manager" ) ;    
}

//***************************************************************
gpu_manager::gpu_manager( this_rref_t rhv ) 
{
    so_move_member_ptr( _fb2d_mgr, rhv ) ;
    so_move_member_ptr( _tx2d_mgr, rhv ) ;
    so_move_member_ptr( _img2d_mgr, rhv ) ;
    so_move_member_ptr( _vb_mgr, rhv ) ;
    so_move_member_ptr( _ib_mgr, rhv ) ;
}

//***************************************************************
gpu_manager::~gpu_manager( void_t )
{
    so_gpu::memory::dealloc( _fb2d_mgr ) ;
    so_gpu::memory::dealloc( _tx2d_mgr ) ;
    so_gpu::memory::dealloc( _img2d_mgr ) ;
    so_gpu::memory::dealloc( _vb_mgr ) ;
    so_gpu::memory::dealloc( _ib_mgr ) ;
}

//***************************************************************
gpu_manager::this_ptr_t gpu_manager::create( so_memory::purpose_cref_t p )
{
    return so_gpu::memory::alloc( this_t(), p ) ;
}

//***************************************************************
void_t gpu_manager::destroy( this_ptr_t ptr )
{
    so_gpu::memory::dealloc( ptr ) ;
}

//***************************************************************
gpu_manager::fb2d_manager_ptr_t gpu_manager::get_fb2d_mgr( void_t )
{
    return _fb2d_mgr ;
}

//***************************************************************
gpu_manager::tx2d_manager_ptr_t gpu_manager::get_tx2d_mgr( void_t )
{
    return _tx2d_mgr ;
}

//***************************************************************
gpu_manager::img2d_manager_ptr_t gpu_manager::get_img2d_mgr( void_t )
{
    return _img2d_mgr ;
}

//***************************************************************
gpu_manager::vb_manager_ptr_t gpu_manager::get_vb_mgr( void_t )
{
    return _vb_mgr ;
}

//***************************************************************
gpu_manager::ib_manager_ptr_t gpu_manager::get_ib_mgr( void_t )
{
    return _ib_mgr ;
}

//***************************************************************