//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "depth_texture_2d.h"

using namespace so_gpu ;

//*****************************************************************************************************
depth_texture_2d::depth_texture_2d( iimage_2d_ptr_t img_ptr ) : base_t( img_ptr )
{

}

//*****************************************************************************************************
depth_texture_2d::depth_texture_2d( this_rref_t rhv ) : base_t( std::move( rhv ) )
{

}

//*****************************************************************************************************
depth_texture_2d::~depth_texture_2d( void_t ) 
{

}

//*****************************************************************************************************
depth_texture_2d::this_ptr_t depth_texture_2d::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gpu::memory::alloc( std::move( rhv ), p ) ;
}

//*****************************************************************************************************
void_t depth_texture_2d::destroy( this_ptr_t ptr )
{
    so_gpu::memory::dealloc( ptr ) ;
}