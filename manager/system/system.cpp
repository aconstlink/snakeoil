//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"
#include "../registry/manager_registry.h"

using namespace so_manager ;

//*************************************************************************************
system::system( void_t ) 
{}

//*************************************************************************************
system::system( this_rref_t rhv )
{
    _regs = std::move( rhv._regs ) ;
}

//*************************************************************************************
system::~system( void_t )
{
    for( auto & item : _regs )
    {
        so_manager::manager_registry_t::destroy(item.second) ;
    }
}

//*************************************************************************************
system::this_ptr_t system::create( so_memory::purpose_cref_t p )
{
    return so_manager::memory::alloc( this_t(), p ) ;
}

//*************************************************************************************
void_t system::destroy( this_ptr_t ptr )
{
    so_manager::memory::dealloc( ptr ) ;
}

//*************************************************************************************
so_manager::result system::register_registry( so_std::string_cref_t key, 
    so_manager::manager_registry_ptr_t reg_ptr )
{
    if( so_core::is_nullptr(reg_ptr) ) 
        return so_manager::invalid_argument ;

    {
        so_thread::lock_guard_t lk( _mtx ) ;

        auto iter = _regs.find( key ) ;
        if( iter != _regs.end() )
            return so_manager::invalid_argument ;

        _regs[key] = reg_ptr ;
    }

    return so_manager::ok ;
}

//*************************************************************************************
so_manager::manager_registry_ptr_t system::find_registry( so_std::string_cref_t key ) 
{
    so_thread::lock_guard_t lk(_mtx) ;

    auto iter = _regs.find( key ) ;
    return iter == _regs.end() ? nullptr : iter->second ;
}

//*************************************************************************************
so_manager::result system::shutdown( void_t ) 
{
    size_t count = 0 ;
    for( auto & item : _regs )
    {
        auto const res = item.second->shutdown() ;
        if( res == so_manager::not_ready ) count++ ;
    }

    return count > 0 ? so_manager::not_ready : so_manager::ok ;
}