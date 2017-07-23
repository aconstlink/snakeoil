//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IO_HANDLE_HANDLE_H_
#define _SNAKEOIL_IO_HANDLE_HANDLE_H_

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"

namespace so_io
{
    class SNAKEOIL_IO_API load_handle
    {
        so_this_typedefs( load_handle ) ;

        friend class system ;

    private:

        so_io::internal_item_ptr_t _data_ptr = nullptr ;
        so_io::system_ptr_t _ios = nullptr ;

    private:

        load_handle( so_io::internal_item_ptr_t, so_io::system_ptr_t ) ;

    public:

        load_handle( void_t ) ;
        load_handle( this_rref_t ) ;
        ~load_handle( void_t ) ;

        load_handle( this_cref_t ) = delete ;

    public:

        this_ref_t operator = ( this_cref_t ) = delete ;
        this_ref_t operator = ( this_rref_t ) ;

    public:

        so_io::result wait_for_operation( so_io::load_completion_funk_t ) ;
    };
    so_typedef( load_handle ) ;

    class SNAKEOIL_IO_API store_handle
    {
        so_this_typedefs( store_handle ) ;

        friend class system ;

    private:

        so_io::internal_item_ptr_t _data_ptr = nullptr ;
        so_io::system_ptr_t _ios = nullptr ;

    private:

        store_handle( so_io::internal_item_ptr_t, so_io::system_ptr_t ) ;

    public:

        store_handle( void_t ) ;
        store_handle( this_rref_t ) ;
        ~store_handle( void_t ) ;

        store_handle( this_cref_t ) = delete ;

    public:

        this_ref_t operator = ( this_cref_t ) = delete ;
        this_ref_t operator = ( this_rref_t ) ;

    public:

        so_io::result wait_for_operation( void_t ) ;
        so_io::result wait_for_operation( so_io::store_completion_funk_t ) ;        
    };
    so_typedef( store_handle ) ;
}

#endif