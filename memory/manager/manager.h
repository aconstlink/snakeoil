//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_MEMORY_MEMORY_DEFAULT_MANAGER_H_
#define _SNAKEOIL_MEMORY_MEMORY_DEFAULT_MANAGER_H_

#include "imanager.h"

#include <mutex>
#include <map>

namespace so_memory
{
    class SNAKEOIL_MEMORY_API manager : public so_memory::imanager
    {
        so_this_typedefs( manager ) ;

        struct memory_info
        {
            size_t sib ;
            so_memory::purpose_t purpose ;
        } ;
        typedef std::map< void_ptr_t, memory_info > ptr_to_info_t ;

        typedef std::mutex mutex_t ;
        typedef std::unique_lock< mutex_t > lock_t ;

    private:

        mutable mutex_t _mtx ;
        size_t _allocated_sib = 0 ;
        ptr_to_info_t _ptr_to_info ;
       
    public:

        manager( void_t ) ;
        virtual ~manager( void_t ) ;

    public:

        static this_ptr_t create( void_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:
        
        virtual void_ptr_t alloc( size_t sib, so_memory::purpose_cref_t purpose ) ;
        virtual void_ptr_t alloc( size_t sib ) ;
        virtual void_t dealloc( void_ptr_t ) ;
        virtual size_t get_sib( void_t ) const ;
        virtual bool_t get_purpose( void_ptr_t, so_memory::purpose_ref_t ) const ;

        virtual void_t dump_to_std( void_t ) const ;

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( manager ) ;
}

#endif

