//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IO_SYSTEM_SYSTEM_H_
#define _SNAKEOIL_IO_SYSTEM_SYSTEM_H_

#include "../handle/handle.h"

#include <snakeoil/thread/typedefs.h>
#include <snakeoil/thread/mutex.h>
#include <snakeoil/std/container/stack.hpp>

#include <snakeoil/core/macros/move.h>

namespace so_io
{
    class SNAKEOIL_IO_API system
    {
        so_this_typedefs( system ) ;
        
        friend class load_handle ;
        friend class store_handle ;

    private:

        struct load_item
        {
            so_this_typedefs( load_item ) ;

            char_ptr_t data = nullptr ;
            size_t sib = 0 ;

            std::condition_variable cv ;
            so_thread::mutex_t mtx ;
            bool_t ready = false ;
            so_io::result status = so_io::invalid ;

            load_item( void_t ){}
            load_item( this_rref_t rhv )
            {
                so_move_member_ptr( data, rhv ) ;
                sib = rhv.sib ;
                ready = rhv.ready ;
                status = rhv.status ;
            }
            load_item( this_cref_t ) = delete ;
            ~load_item( void_t ){}

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t rhv )
            {
                so_move_member_ptr( data, rhv ) ;
                sib = rhv.sib ;
                ready = rhv.ready ;
                status = rhv.status ;
                return *this ;
            }
        };
        so_typedef( load_item ) ;

        typedef so_std::stack< load_item_ptr_t, 20 > __load_stack_t ;
        so_typedefs( __load_stack_t, load_stack ) ;
        so_typedefs( so_std::vector< load_item_ptr_t >, load_items ) ;

        struct store_item
        {
            so_this_typedefs( store_item ) ;

            char_cptr_t data = nullptr ;
            size_t sib = 0 ;

            std::condition_variable cv ;
            so_thread::mutex_t mtx ;
            bool_t ready = false ;
            so_io::result status = so_io::invalid ;

            store_item( void_t ){}
            store_item( this_rref_t rhv )
            {
                so_move_member_ptr( data, rhv ) ;
                sib = rhv.sib ;
                ready = rhv.ready ;
                status = rhv.status ;
            }
            store_item( this_cref_t ) = delete ;
            ~store_item( void_t ){}

            this_ref_t operator = ( this_cref_t ) = delete ;
            this_ref_t operator = ( this_rref_t rhv )
            {
                so_move_member_ptr( data, rhv ) ;
                sib = rhv.sib ;
                ready = rhv.ready ;
                status = rhv.status ;
                return *this ;
            }
        };
        so_typedef( store_item ) ;

        typedef so_std::stack< store_item_ptr_t, 20 > __store_stack_t ;
        so_typedefs( __store_stack_t, store_stack ) ;
        so_typedefs( so_std::vector< store_item_ptr_t >, store_items ) ;
        

    private:

        so_thread::mutex_t _load_mtx ;
        load_stack_t _load_stack ;

        so_thread::mutex_t _store_mtx ;
        store_stack_t _store_stack ;

    public:

        system( void_t ) ;
        ~system( void_t ) ;

        system( this_cref_t ) = delete ;
        system( this_rref_t ) ;

    public:

        static this_ptr_t create( void_t ) ;
        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_io::load_handle_t load( so_io::path_cref_t ) ;
        so_io::store_handle_t store( so_io::path_cref_t, char_cptr_t, size_t ) ;

    private:

        so_io::result wait_for_operation( so_io::load_handle_rref_t, so_io::load_completion_funk_t ) ;
        so_io::result wait_for_operation( so_io::store_handle_rref_t, so_io::store_completion_funk_t ) ;

    private:

        load_item_ptr_t get_load_item( void_t ) ;
        store_item_ptr_t get_store_item( void_t ) ;


    };
    so_typedef( system ) ;
}

#endif

