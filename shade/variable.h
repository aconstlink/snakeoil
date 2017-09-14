//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_SHADE_VARIABLE_H_
#define _SNAKEOIL_SHADE_VARIABLE_H_

#include "typedefs.h"
#include "result.h"
#include "type.h"
#include "texture_type.h"
#include "buffer_type.h"

namespace so_shade
{
    ///
    struct variable
    {
        so_this_typedefs( variable ) ;

        so_std::string_t name ;
        so_shade::type of_type ;

    private:

        /// sib of default value
        size_t _sib = 0 ;
        /// default value
        void_ptr_t _data_ptr = nullptr ;

    public:

        variable( void_t ){}
        variable( so_std::string_cref_t n, so_shade::type_cref_t t ) : name(n), of_type(t){}

        variable( this_cref_t rhv ) : name(rhv.name), of_type(rhv.of_type)
        {
            if( so_core::is_not_nullptr(rhv._data_ptr) )
            {
                _sib = rhv._sib ;
                _data_ptr = so_memory::global::alloc( _sib ) ;
                std::memcpy( _data_ptr, rhv._data_ptr, _sib ) ;
            }
        }

        variable( this_rref_t rhv ) : name( std::move(rhv.name) ), 
            of_type( std::move( rhv.of_type) ), _sib(rhv._sib) 
        {
            so_move_member_ptr( _data_ptr, rhv ) ;
        }

        ~variable( void_t )
        {
            so_memory::global::dealloc( _data_ptr ) ;
        }

    public:

        this_ref_t operator = ( this_cref_t rhv )
        {
            name = rhv.name ;
            of_type = rhv.of_type ;

            if( so_core::is_not_nullptr(rhv._data_ptr) )
            {
                _sib = rhv._sib ;
                _data_ptr = so_memory::global::alloc( _sib ) ;
                std::memcpy( _data_ptr, rhv._data_ptr, _sib ) ;
            }

            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            name = std::move( rhv.name ) ;
            of_type = std::move(rhv.of_type ) ;
            _sib = rhv._sib ;
            so_move_member_ptr( _data_ptr, rhv ) ;

            return *this ;
        }
        
        template< typename T >
        void_t create_default_value( T const & v )
        {
            _sib = sizeof( T ) ;
            _data_ptr = void_ptr_t(so_memory::global::alloc<T>( v )) ;
        }

        template< typename T >
        bool_t default_value_as( T & vout ) const
        {
            if( so_core::is_nullptr(_data_ptr) )
                return false ;

            vout = *reinterpret_cast<T*>(_data_ptr) ;

            return true ;
        }

        bool_t has_default_value( void_t ) const
        {
            return so_core::is_not_nullptr(_data_ptr) ;
        }


    };
    so_typedef( variable ) ;

    ///
    struct texture_variable
    {
        so_this_typedefs( texture_variable ) ;

        so_std::string_t name ;
        so_shade::texture_type of_type ;

    public:

        texture_variable( void_t ) {}
        texture_variable( this_cref_t rhv ) 
        {
            *this = rhv ;
        }

        texture_variable( this_rref_t rhv )
        {
            *this = std::move(rhv) ;
        }

    public:

        this_ref_t operator = ( this_cref_t rhv )
        {
            name = rhv.name ;
            of_type = rhv.of_type ;
            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            name = std::move( rhv.name ) ;
            of_type = std::move( rhv.of_type ) ;
            return *this ;
        }
    };
    so_typedef( texture_variable ) ;

    ///
    struct buffer_variable
    {
        so_this_typedefs( buffer_variable ) ;

        so_std::string_t name ;
        so_shade::buffer_type of_type ;

    public:

        buffer_variable( void_t ){}
        buffer_variable( this_cref_t rhv ) : 
            name(rhv.name), of_type(rhv.of_type){}
        buffer_variable( this_rref_t rhv ) : 
            name(std::move(rhv.name)), of_type(std::move(rhv.of_type)){}
        ~buffer_variable( void_t ){}
        
        this_ref_t operator = ( this_cref_t rhv )
        {
            name = rhv.name ;
            of_type = rhv.of_type ;
            return *this ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            name = std::move( rhv.name ) ;
            of_type = std::move( rhv.of_type ) ;
            return *this ;
        }
    };
    so_typedef( buffer_variable ) ;
}

#endif
