//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "global.h"

#include "system/system.h" 

#include <snakeoil/log/global.h>

using namespace so_imex ;

so_thread::mutex_t global::_mtx ;
global::this_ptr_t global::_ptr = nullptr ;


//************************************************************************************
global::global( void_t )
{
    _system_ptr = so_imex::system_t::create_with_default_modules( 
        "[so_imex::global::global] : system" ) ;
}

//************************************************************************************
global::global( this_rref_t rhv )
{
    so_move_member_ptr( _system_ptr, rhv ) ;
}

//************************************************************************************
global::~global( void_t )
{
    so_imex::system_t::destroy( _system_ptr ) ;
}

//************************************************************************************
global::this_ptr_t global::create( this_rref_t rhv, so_memory::purpose_cref_t p )
{
    return so_imex::memory::alloc( std::move( rhv ), p ) ;
}

//************************************************************************************
void_t global::destroy( this_ptr_t ptr )
{
    so_imex::memory::dealloc( ptr ) ;
}

//************************************************************************************
global::this_ptr_t global::init( void_t )
{
    // 1. do unguarded check first
    if( so_core::is_not_nullptr( this_t::_ptr ) )
        return this_t::_ptr ;

    // 2. if unguarded failed, do guarded allocation
    {
        so_thread::lock_guard_t lk( this_t::_mtx ) ;

        if( so_core::is_not_nullptr( this_t::_ptr ) )
            return this_t::_ptr ;

        this_t::_ptr = this_t::create( this_t(), "[so_imex::global::init]" ) ;

        so_log::global::status( "[online] : snakeoil imex" ) ;
    }

    return this_t::_ptr ;
}

//************************************************************************************
void_t global::deinit( void_t )
{
    if( so_core::is_nullptr( _ptr ) )
        return ;

    this_t::destroy( _ptr ) ;
}

//************************************************************************************
global::this_ptr_t global::get( void_t )
{
    return this_t::init() ;
}

//************************************************************************************
so_imex::isystem_ptr_t global::system( void_t )
{
    return this_t::get()->_system_ptr ;
}

//************************************************************************************
so_imex::module_registry_ptr_t global::module_registry( void_t )
{
    return this_t::get()->_system_ptr->get_module_registry() ;
}

//************************************************************************************
so_imex::manager_registry_ptr_t global::manager_registry( void_t )
{
    return this_t::get()->_system_ptr->get_manager_registry() ;
}