//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_MANAGER_RESOURCE_MANAGER_HPP_
#define _SNAKEOIL_IMEX_MANAGER_RESOURCE_MANAGER_HPP_

#include "base_resource_manager.hpp"

namespace so_imex
{
    template< typename T >
    class resource_manager : public base_resource_manager< T >
    {
        typedef base_resource_manager< T > base_t ;
        so_this_typedefs( so_imex::resource_manager<T> ) ;

    public:

        so_typedefs( typename base_t::manage_item_t, manage_item ) ;

    public:

        resource_manager( void_t ) {}
        resource_manager( this_rref_t rhv ) : base_t( std::move(rhv) ) {}
        virtual ~resource_manager( void_t ) {}

    public:

        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
        {
            return so_imex::memory::alloc( std::move(rhv), p ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_imex::memory::dealloc( ptr ) ;
        }

    public:

        void_t do_destruction( typename base_t::resource_manager_t::for_each_funk_t funk )
        {
            base_t::do_destruction( funk ) ;
        }

    public:

        virtual void_t destroy( void_t ) 
        {
            this_t::destroy( this ) ;
        }

    };
}

#endif
