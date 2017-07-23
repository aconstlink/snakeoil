//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "default_manager.h"

#include <snakeoil/log/log.h>

#include <cstdlib>

using namespace so_memory ;

//*************************************************************************************
default_memory_manager::default_memory_manager( void_t ) 
{

}

//*************************************************************************************
default_memory_manager::~default_memory_manager( void_t ) 
{

}

//*************************************************************************************
void_ptr_t default_memory_manager::alloc( size_t sib, so_memory::purpose_cref_t purpose ) 
{
    void_ptr_t ptr = malloc( sib ) ;
    {
        lock_t lk(_mtx) ;
        _ptr_to_info[ptr] = memory_info{sib, purpose} ;
        _allocated_sib += sib ;
    }
    return ptr ;
}

//*************************************************************************************
void_ptr_t default_memory_manager::alloc( size_t sib ) 
{
    return this_t::alloc( sib, "[default_memory_manager::alloc]" ) ;
}

//*************************************************************************************
void default_memory_manager::dealloc( void_ptr_t ptr ) 
{
    if( ptr == nullptr ) return ;

    {
        lock_t lk(_mtx) ;
        auto iter = _ptr_to_info.find( ptr ) ;

        if( so_log::log::error( iter==_ptr_to_info.end(),
            "[default_memory_manager::dealloc] : ptr location not found") ) 
            return ;        

        _allocated_sib -= iter->second.sib ;
        _ptr_to_info.erase( iter ) ;
    }
    ::free( ptr ) ;
}

//*************************************************************************************
size_t default_memory_manager::get_sib( void_t ) const
{
    return _allocated_sib ;
}

//*************************************************************************************
bool_t default_memory_manager::get_purpose( void_ptr_t ptr, so_memory::purpose_ref_t pout ) const 
{
    if( so_core::is_nullptr(ptr) ) 
        return false ;

    {
        lock_t lk( _mtx );
        auto const iter = _ptr_to_info.find( ptr );

        if( so_log::log::error( iter == _ptr_to_info.end(),
            "[default_memory_manager::get_purpose] : ptr location not found" ) )
            return false ;

        pout = iter->second.purpose ;        
    }

    return true ;
}

//*************************************************************************************
void_t default_memory_manager::dump_to_std( void_t ) const 
{
    lock_t lk(_mtx) ;

    std::cout << "***************************************************" << std::endl ;
    std::cout << "[default_memory_manager::dump_to_std] : Dump to std [" << _allocated_sib << "] sib" << std::endl ;

    for( auto iter : _ptr_to_info )
    {
        std::cout << iter.second.purpose << "; sib: " << std::to_string(iter.second.sib) << std::endl ;
    }

    if( _ptr_to_info.empty() )
    {
        std::cout << "* Memory manager has no entries." << std::endl ;
    }
}

