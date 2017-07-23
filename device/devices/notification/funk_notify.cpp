//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "funk_notify.h"

using namespace so_device ;

//****************************************************************************************
funk_notify::funk_notify( user_funk_t funk ) : _funk( funk )
{}

//****************************************************************************************
funk_notify::this_ptr_t funk_notify::create( user_funk_t funk, so_memory::purpose_cref_t p ) 
{
    return this_t::create( this_t(funk), p ) ;
}

//****************************************************************************************
funk_notify::this_ptr_t funk_notify::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_device::memory::alloc( std::move( rhv ), p ) ;
}

//****************************************************************************************
void_t funk_notify::destroy( this_ptr_t ptr )
{
    so_device::memory::dealloc( ptr ) ;
}

//****************************************************************************************
void_t funk_notify::on_change( so_device::idevice_ptr_t dev_ptr, so_std::string_cref_t name,
    so_device::so_component::iinput_component_ptr_t comp_ptr )
{
    _funk( dev_ptr, name, comp_ptr ) ;
}

//****************************************************************************************
void_t funk_notify::destroy( void_t )
{
    this_t::destroy( this ) ;
}
