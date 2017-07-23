//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_VARIABLE_VARIABLE_HPP_
#define _SNAKEOIL_VARIABLE_VARIABLE_HPP_

#include "ivariable.h"

namespace so_var
{
    template< typename T >
    class variable : public ivariable
    {
        so_this_typedefs( variable<T> ) ;

    public:

        so_typedefs( T, type ) ;

    private:

        type_t _data ;

    public:
        
        variable( type_cref_t data ) : _data(data)
        {}

        variable( this_cref_t rhv ) : variable( rhv.get_data() )
        {}

        variable( this_rref_t rhv )
        {
            _data = std::move(rhv._data) ;
        }

        virtual ~variable( void_t ) 
        {}

        virtual void_t destroy( void_t )
        {
            this_t::destroy( this ) ;
        }

    public:

        static this_ptr_t create( type_t val, so_memory::purpose_cref_t p )
        {
            return so_var::memory::alloc( this_t(val), p ) ;
        }

        static this_ptr_t create( this_rref_t rhv, so_memory::purpose_cref_t p )
        {
            return so_var::memory::alloc( std::move(rhv), p ) ;
        }

        static void_t destroy( this_ptr_t ptr )
        {
            so_var::memory::dealloc( ptr ) ;
        }

    public:

        type_ptr_t get_data_ptr( void_t )
        {
            return &_data ;
        }

        type_cref_t get_data( void_t ) const
        {
            return _data ;
        }

        void_t set_data( type_cref_t d )
        {
            _data = d ;
        }

    public:

        this_ref_t operator = ( this_cref_t rhv )
        {
            _data = rhv.get_data() ;
            return *this ;
        }

        this_ref_t operator = ( type_cref_t d )
        {
            _data = d ;
            return *this ;
        }

        operator type_ptr_t ( void_t )
        {
            return &_data ;
        }

        operator type_cptr_t ( void_t ) const
        {
            return &_data ;
        }

        operator type_cref_t ( void_t ) const
        {
            return _data ;
        }


    };
}

#endif
