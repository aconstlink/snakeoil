//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_SLOT_OUTPUT_OUTPUT_SLOT_HPP_
#define _SNAKEOIL_FLOW_SLOT_OUTPUT_OUTPUT_SLOT_HPP_

#include "output_slot.h"
#include "../../data/data.hpp"

#include "../../data/compatibility_funk.h"

namespace so_flow
{
    template< typename T >
    class generic_output_slot : public output_slot
    {
        so_this_typedefs( generic_output_slot<T> ) ;
        
        so_typedefs( T, type ) ;

    private:

        so_flow::data< T > _data ;

    public:

        generic_output_slot( void_t ) 
        {}

        generic_output_slot( type_ptr_t d ) : _data( d )
        {}

        generic_output_slot( this_rref_t rhv )
        {
            _data = std::move( rhv._data ) ;
        }

        virtual ~generic_output_slot( void_t ) 
        {}

    public:

        static this_ptr_t create( so_memory::purpose_cref_t p )
        {
            return so_flow::memory::alloc( this_t(), p ) ;
        }

        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
        {
            return so_flow::memory::alloc( std::move(rhv), p ) ;
        }

        static void_t destroy( this_ptr_t ptr ) 
        {
            so_flow::memory::dealloc( ptr ) ;
        }

    public:

        virtual bool_t is_compatible( so_flow::iinput_slot_ptr_t is_ptr ) const 
        {
            if( so_core::is_nullptr(is_ptr) )
                return false ;

            auto comp_funk = so_flow::generic_funk::compatibility_funk<T>() ;

            return comp_funk( &_data, is_ptr->get_data_ptr() ) ;
        }

        virtual idata_cptr_t get_data_ptr( void_t ) const
        {
            return &_data ;
        }

    protected:

        virtual so_flow::generic_funk::exchange_funk_t exchange_funk( void_t ) const
        {
            return so_flow::generic_funk::exchange_value_funk_no_check<T>() ;
        }

        bool_t get( type_ref_t d ) const
        {
            return _data.get( d ) ;
        }

        void_t set_ptr( type_ptr_t ptr )
        {
            _data.set_ptr( ptr ) ;
        }

    private:

        bool_t set( type_cref_t d )
        {
            return _data.set( d ) ;
        }

    public: // interface
        
        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }
    };
}

#endif
