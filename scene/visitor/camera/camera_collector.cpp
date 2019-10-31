
#include "camera_collector.h"

#include "../../node/camera/camera.h"

using namespace so_scene ;
using namespace so_scene::so_visitor ;

//*****************************************************
camera_collector::camera_collector( void_t ) noexcept 
{
}

//*****************************************************
camera_collector::camera_collector( this_rref_t rhv ) noexcept : visitor( std::move( rhv ) )
{
    _cams = std::move( rhv._cams ) ;
}

//*****************************************************
camera_collector::~camera_collector( void_t ) noexcept 
{
}

//*****************************************************
so_scene::result camera_collector::visit( so_scene::so_node::camera_ptr_t cptr )
{
    _cams.emplace_back( cptr->get_camera() ) ;
    return so_scene::result::ok ;
}

//*****************************************************
camera_collector::this_ptr_t camera_collector::create( so_memory::purpose_cref_t p ) 
{
    return so_memory::global_t::alloc( this_t(), p ) ;
}

//*****************************************************
void_t camera_collector::destroy( this_ptr_t ptr ) 
{
    so_memory::global_t::dealloc( ptr ) ;
}

//*****************************************************
void_t camera_collector::destroy( void_t ) 
{
    this_t::destroy( this ) ;
}

//*****************************************************
so_gfx::icamera_cptr_t camera_collector::get_camera( size_t const n ) const noexcept 
{
    if( n >= _cams.size() ) return nullptr ;
    return _cams[ n ] ;
}