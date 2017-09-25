//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "global.h"

#include "manager/manager.h"

#include <snakeoil/log/global.h>

using namespace so_memory ;

so_thread::mutex_t global::_mtx ;
global::this_ptr_t global::_ptr = nullptr ;

//*************************************************************************************
global::global( void_t )
{
    _manager = so_memory::manager_t::create() ;
}

//*************************************************************************************
global::global( this_rref_t rhv )
{
    so_move_member_ptr( _manager, rhv ) ;
}

//*************************************************************************************
global::~global( void_t )
{
    if( so_core::is_not_nullptr( _manager ) )
        _manager->destroy() ;
}

//*************************************************************************************
global::this_ptr_t global::init( void_t )
{
    if( so_core::is_not_nullptr( this_t::_ptr ) )
        return this_t::_ptr ;

    {
        so_thread::lock_guard_t lk( this_t::_mtx ) ;

        if( so_core::is_not_nullptr( this_t::_ptr ) )
            return this_t::_ptr ;

        // again, do not use snakeoil memory
        // memory is above log
        this_t::_ptr = new this_t() ;

        so_log::global::status( "[online] : snakeoil memory" ) ;
    }

    return this_t::_ptr ;
}

//*************************************************************************************
void_t global::deinit( void_t )
{
    if( so_core::is_nullptr( global_t::_ptr ) )
        return ;

    delete global_t::_ptr ;
    this_t::_ptr = nullptr ;
}

//*************************************************************************************
global::this_ptr_t global::get( void_t )
{
    return this_t::init() ;
}

//*************************************************************************************
void global::dump_to_std( void_t )
{
    this_t::get()->_manager->dump_to_std() ;
}

//*************************************************************************************
void_ptr_t global::alloc( size_t sib, so_memory::purpose_cref_t purpose )
{
    return this_t::get()->_manager->alloc( sib, purpose ) ;
}

//*************************************************************************************
void_ptr_t global::alloc( size_t sib )
{
    return this_t::get()->_manager->alloc( sib ) ;
}

//*************************************************************************************
void_t global::dealloc( void_ptr_t ptr )
{
    if( ptr == nullptr ) return ;

    this_t::get()->_manager->dealloc( ptr ) ;
}

//*************************************************************************************
size_t global::get_sib( void_t )
{
    return this_t::get()->_manager->get_sib() ;
}

//*************************************************************************************
bool_t global::get_purpose( void_ptr_t ptr, so_memory::purpose_ref_t pout )
{
    return this_t::get()->_manager->get_purpose( ptr, pout ) ;
}
