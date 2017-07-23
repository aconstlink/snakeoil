//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "buffer_creator.h"

#include <snakeoil/gpu/buffer/ivertex_buffer.h>
#include <snakeoil/gpu/buffer/iindex_buffer.h>
#include <snakeoil/core/macros/move.h>

using namespace so_manager ;
using namespace so_manager::so_graphics ;

//*************************************************************************************
buffer_creator::buffer_creator( this_rref_t rhv ) 
{
    _pt = rhv._pt ;
    so_move_member_ptr( _vb, rhv ) ;
    so_move_member_ptr( _ib, rhv ) ;
}

//*************************************************************************************
buffer_creator::buffer_creator( so_geo::flat_tri_mesh_cref_t ptr ) 
{
    this_t::construct_from( ptr ) ;
}

//*************************************************************************************
buffer_creator::buffer_creator( so_geo::flat_line_mesh_cref_t ptr )
{
    this_t::construct_from( ptr ) ;
}

//*************************************************************************************
buffer_creator::~buffer_creator( void_t ) 
{
    if( _vb != nullptr )
        _vb->destroy() ;

    if( _ib != nullptr )
        _ib->destroy() ;
}

//*************************************************************************************
so_gpu::primitive_type buffer_creator::get_primitive_type( void_t ) const 
{
    return _pt ;
}

//*************************************************************************************
so_gpu::ivertex_buffer_ptr_t buffer_creator::move_vertex_buffer( void_t ) 
{
    auto * old_ptr = _vb ;
    _vb = nullptr ;
    return old_ptr ;
}

//*************************************************************************************
so_gpu::iindex_buffer_ptr_t buffer_creator::move_index_buffer( void_t ) 
{
    auto * old_ptr = _ib ;
    _ib = nullptr ;
    return old_ptr ;
}

//*************************************************************************************