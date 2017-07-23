//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_FLOW_SLOT_INPUT_SLOT_INPUT_SLOT_HPP_
#define _SNAKEOIL_FLOW_SLOT_INPUT_SLOT_INPUT_SLOT_HPP_

#include "input_slot.h"

#include "../../data/data.hpp"
#include "../../data/generic_funk.h"

namespace so_flow
{
    template< typename T >
    class generic_input_slot : public input_slot
    {
        so_this_typedefs( generic_input_slot ) ;
        
        so_typedefs( T, type ) ;

    private:

        so_flow::data< T > _data ;
        
    public:

        generic_input_slot( void_t )
        {}

        generic_input_slot( this_rref_t rhv ) : _data(std::move(rhv._data))
        {}

        generic_input_slot( type_ptr_t d ) : _data(d)
        {}

        virtual ~generic_input_slot( void_t ) 
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
        
        virtual bool_t is_compatible( so_flow::ioutput_slot_ptr_t os_ptr ) const 
        {
            if( so_core::is_nullptr(os_ptr) )
                return false ;

            auto comp_funk = so_flow::generic_funk::compatibility_funk<T>() ;
            return comp_funk( &_data, os_ptr->get_data_ptr() ) ;
        }

        virtual idata_ptr_t get_data_ptr( void_t )
        {
            return &_data ;
        }

        virtual idata_cptr_t get_data_ptr( void_t ) const
        {
            return &_data ;
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
