//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "framebuffer_begin.h"
#include "framebuffer_begin_plug_factory.h"

using namespace so_gfx ;

//*************************************************************************************
framebuffer_begin::framebuffer_begin( so_std::string_cref_t name, so_gpx::render_system_ptr_t rs )
{
    _sd = so_gfx::memory::alloc( this_t::shared_data_t(), 
        "[framebuffer_begin::framebuffer_begin] : shared_data" ) ;
    _sd->name = name ;
    _rs = rs ;
    
    _tid = _rs->register_technique( framebuffer_begin_plug_factory_t::create(
        framebuffer_begin_plug_factory_t(_sd), 
        "[framebuffer_begin::framebuffer_begin] : framebuffer_begin_plug_factory_t" ) ) ;
}

//*************************************************************************************
framebuffer_begin::framebuffer_begin( this_rref_t rhv )
{
    so_move_member_ptr( _sd, rhv ) ;
    so_move_member_ptr( _rs, rhv ) ;
    _tid = std::move( rhv._tid ) ;
}

//*************************************************************************************
framebuffer_begin::~framebuffer_begin( void_t )
{

}

//*************************************************************************************
framebuffer_begin::this_ptr_t framebuffer_begin::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_gfx::memory::alloc( std::move( rhv ), p ) ;
}

//*************************************************************************************
void_t framebuffer_begin::destroy( this_ptr_t ptr )
{
    so_gfx::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t framebuffer_begin::schedule( void_t )
{
    
}

//*************************************************************************************