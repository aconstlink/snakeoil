//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "memory.h"
#include "manager/default_manager.h"

#include <snakeoil/core/assert.h>

using namespace so_memory ;

so_memory::manager_ptr_t memory::_manager = nullptr ;

//*************************************************************************************
void memory::init( void_t ) 
{
    _manager = new default_memory_manager() ;
}

//*************************************************************************************
void memory::init( manager_ptr_t ptr )
{
    so_assert( ptr != nullptr ) ;
    so_assert( _manager == nullptr ) ;
    _manager = ptr ;
}

//*************************************************************************************
void memory::dump_to_std( void_t ) 
{
    so_assert( _manager != nullptr ) ;
    _manager->dump_to_std() ;
}

//*************************************************************************************
void_ptr_t memory::alloc( size_t sib, so_memory::purpose_cref_t purpose ) 
{
    so_assert( _manager != nullptr ) ;
    return _manager->alloc( sib, purpose ) ;
}

//*************************************************************************************
void_ptr_t memory::alloc( size_t sib ) 
{
    so_assert( _manager != nullptr ) ;
    return _manager->alloc( sib ) ;
}

//*************************************************************************************
void_t memory::dealloc( void_ptr_t ptr ) 
{
    if( ptr == nullptr ) return ;
    so_assert( _manager != nullptr ) ;

    _manager->dealloc( ptr ) ;
}

//*************************************************************************************
size_t memory::get_sib( void_t ) 
{
    so_assert( _manager != nullptr ) ;
    return _manager->get_sib() ;
}

//*************************************************************************************
bool_t memory::get_purpose( void_ptr_t ptr, so_memory::purpose_ref_t pout ) 
{
    so_assert( _manager != nullptr ) ;
    return _manager->get_purpose( ptr, pout ) ;
}
