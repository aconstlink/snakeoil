//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "sync_object.h"


using namespace so_imex ;

//*************************************************************************************
sync_object::sync_object( void_t ) 
{}

//*************************************************************************************
sync_object::sync_object( this_rref_t rhv )
{
    _so = std::move( rhv._so ) ;
    _res = rhv._res ;
}

//*************************************************************************************
sync_object::~sync_object( void_t )
{
    this_t::set_and_signal( so_imex::pre_exit ) ;           
}
//*************************************************************************************
sync_object::this_ptr_t sync_object::create( so_memory::purpose_cref_t purp ) 
{
    return so_imex::memory::alloc( this_t(), purp ) ;
}

//*************************************************************************************
void_t sync_object::destroy( this_ptr_t ptr ) 
{
    so_imex::memory::dealloc( ptr ) ;
}

//*************************************************************************************
void_t sync_object::set_and_signal( this_ptr_t ptr, so_imex::result res) 
{
    if( ptr != nullptr ) ptr->set_and_signal( res ) ;
}

//*************************************************************************************
void_t sync_object::set_not_signal( this_ptr_t ptr, so_imex::result res ) 
{
    if( ptr != nullptr ) ptr->set( res ) ;
}

//*************************************************************************************
void_t sync_object::set( so_imex::result res ) 
{
    if( _so.is_not_signaled() )
    {
        _res = res ;
    }
}

//*************************************************************************************
void_t sync_object::set_and_signal( so_imex::result res )
{
    if( _so.is_not_signaled() )
    {
        _res = res ;
        _so.set_and_signal() ;
    }            
}

//*************************************************************************************
so_imex::result sync_object::wait( void_t )
{
    _so.wait() ;
    return _res ;
}

//*************************************************************************************
