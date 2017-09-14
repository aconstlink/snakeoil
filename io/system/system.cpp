//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "system.h"

#include <snakeoil/thread/typedefs.h>
#include <snakeoil/thread/helper/helper_os.h>

#include <snakeoil/log/global.h>

#include <iostream>
#include <fstream>

using namespace so_io ;

//************************************************************************************
system::this_ptr_t system::create( void_t ) 
{
    return so_io::memory::alloc( this_t(), "[so_io::system::create]" ) ;
}

//************************************************************************************
system::this_ptr_t system::create( this_rref_t rhv, so_memory::purpose_cref_t purpose ) 
{
    return so_io::memory::alloc( std::move(rhv), purpose ) ;
}

//************************************************************************************
void_t system::destroy( this_ptr_t ptr ) 
{
    so_io::memory::dealloc( ptr ) ;
}

//************************************************************************************
system::system( void_t ) 
{}

//************************************************************************************
system::~system( void_t )
{
    while( _load_stack.has_item() )
    {
        this_t::load_item_ptr_t ptr = _load_stack.pop() ;
        so_memory::global::dealloc( ptr ) ;
    }

    while( _store_stack.has_item() )
    {
        this_t::store_item_ptr_t ptr = _store_stack.pop() ;
        so_memory::global::dealloc( ptr ) ;
    }
}

//************************************************************************************
system::system( this_rref_t rhv )
{
    _load_stack = std::move( rhv._load_stack ) ;
    _store_stack = std::move( rhv._store_stack ) ;
}

//************************************************************************************
so_io::load_handle_t system::load( so_io::path_cref_t file_path )
{
    this_t::load_item_ptr_t li = this_t::get_load_item() ;

    so_thread::thread_t( [=]( void_t )
    {
        std::ifstream is( file_path, std::ifstream::binary ) ;

        if( is.is_open() )
        {
            // process content
            {
                is.seekg( 0, is.end ) ;
                size_t const length = size_t( is.tellg() ) ;
                is.seekg( 0, is.beg ) ;
                
                char_ptr_t data = so_memory::global::alloc_raw<char_t>( length ) ;

                is.read( data, length ) ;
                
                li->sib = is.gcount() ;
                li->data = data ;
            }

            // signal shared data
            {
                so_thread::lock_guard_t lk( li->mtx ) ;
                li->ready = true ;
                li->status = so_io::ok ;
            }
            li->cv.notify_all() ;
        }
        else
        {
            // signal shared data
            {
                so_thread::lock_guard_t lk( li->mtx ) ;
                li->ready = true ;
                li->status = so_io::file_does_not_exist ;
            }
            li->cv.notify_all() ;
        }
    } ).detach() ;

    return so_io::load_handle_t( so_io::internal_item_ptr_t(li), this ) ;
}

//************************************************************************************
so_io::store_handle_t system::store( so_io::path_cref_t file_path, char_cptr_t data_ptr, size_t sib )
{
    this_t::store_item_ptr_t si = this_t::get_store_item() ;

    so_thread::thread_t( [=]( void_t )
    {
        so_std::filesystem::path p( file_path ) ;

        // 1. check if the directory exists. if not, create it
        if( so_core::is_not( so_std::filesystem::exists( p.parent_path() ) ) )
        {
            auto const ec = so_std::filesystem::create_directories( p.parent_path() ) ;

            if( so_core::is_not( ec ) )
            {
                so_log::global::error( "[so_io::system::store] : create directories : " ) ;

                // signal shared data
                {
                    so_thread::lock_guard_t lk( si->mtx ) ;
                    si->ready = true ;
                    si->status = so_io::failed ;
                }
                si->cv.notify_all() ;
                return ;
            }
        }

        std::ofstream os( file_path, std::ifstream::binary ) ;

        if( os.is_open() )
        {
            os.write( data_ptr, sib ) ;

            // signal shared data
            {
                so_thread::lock_guard_t lk( si->mtx ) ;
                si->ready= true ;
                si->status = so_io::ok ;
            }
            si->cv.notify_all() ;
        }
        else
        {
            // signal shared data
            {
                so_thread::lock_guard_t lk( si->mtx ) ;
                si->ready = true ;
                si->status = so_io::file_does_not_exist ;
            }
            si->cv.notify_all() ;
        }
    } ).detach() ;

    return so_io::store_handle_t( so_io::internal_item_ptr_t( si ), this ) ;
}

//************************************************************************************
so_io::result system::wait_for_operation( so_io::load_handle_rref_t hnd, 
    so_io::load_completion_funk_t funk )
{
    this_t::load_item_ptr_t li = this_t::load_item_ptr_t(hnd._data_ptr) ;
    if( so_core::is_nullptr( li ) )
        return so_io::invalid_argument ;
    
    hnd._data_ptr = nullptr ;

    // 1. wait for the operation
    {
        so_thread::lock_t lk( li->mtx ) ;
        while( so_core::is_not( li->ready ) ) li->cv.wait( lk ) ;
    }

    auto const res = li->status ;

    // 2. call the user funk
    {
        funk( li->data, li->sib, li->status ) ;
    }

    // 3. reclaim the item
    {
        so_memory::global::dealloc( li->data ) ;
        *li = this_t::load_item_t() ;
    }
    
    // 4. store item for later use
    {
        so_thread::lock_guard_t lk( _load_mtx ) ;
        _load_stack.push( li ) ;
    }

    return res ;
}

//************************************************************************************
so_io::result system::wait_for_operation( so_io::store_handle_rref_t hnd, 
    so_io::store_completion_funk_t funk )
{
    this_t::store_item_ptr_t si = this_t::store_item_ptr_t( hnd._data_ptr ) ;
    if( so_core::is_nullptr( si ) )
        return so_io::invalid_argument ;

    hnd._data_ptr = nullptr ;

    // 1. wait for the operation
    {
        so_thread::lock_t lk( si->mtx ) ;
        while( so_core::is_not( si->ready ) ) si->cv.wait( lk ) ;
    }

    auto const res = si->status ;

    // 2. call the user funk
    {
        funk( si->status ) ;
    }

    // 3. reclaim the item
    {
        *si = this_t::store_item_t() ;
    }

    // 4. store item for later use
    {
        so_thread::lock_guard_t lk( _store_mtx ) ;
        _store_stack.push( si ) ;
    }

    return res ;
}

//************************************************************************************
system::load_item_ptr_t system::get_load_item( void_t )
{
    so_thread::lock_guard_t lk( _load_mtx ) ;
    return _load_stack.has_item() ? _load_stack.pop() : 
        so_memory::global::alloc( load_item_t(), "[system::get_load_item] : item" ) ;
}

//************************************************************************************
system::store_item_ptr_t system::get_store_item( void_t )
{
    return _store_stack.has_item() ? _store_stack.pop() :
        so_memory::global::alloc( store_item_t(), "[system::get_store_item] : item" ) ;
}