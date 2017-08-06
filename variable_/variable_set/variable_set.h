//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_VARIABLE_VARIABLE_SET_VARIABLE_SET_H_
#define _SNAKEOIL_VARIABLE_VARIABLE_SET_VARIABLE_SET_H_

#include "../api.h"
#include "../typedefs.h"
#include "../protos.h"
#include "../result.h"

#include "../variable.hpp"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/std/string/string.hpp>

namespace so_var
{
    class SNAKEOIL_VARIABLE_API variable_set
    {
        so_this_typedefs( variable_set ) ;
        
        struct store_data
        {
            so_std::string_t name ;
            so_var::ivariable_ptr_t var_ptr ;
        };
        so_typedef( store_data ) ;

        so_typedefs( so_std::vector<store_data_t>, variables ) ;

    public:

        typedef std::function< void_t ( so_var::key_in_t, so_var::ivariable_ptr_t ) >  
            for_each_funk_t ;

    private:

        variables_t _variables ;

    private:

        variable_set( this_cref_t ) {}

    public:

        variable_set( void_t ) ;
        variable_set( this_rref_t ) ;
        ~variable_set( void_t ) ;

    public:

        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        so_var::result add( so_var::key_in_t, so_var::ivariable_ptr_t ) ;
        so_var::result remove( so_var::key_in_t ) ;

        bool_t get_variable( so_var::key_in_t, so_var::ivariable_ptr_t & ) ;

        template< typename T >
        bool_t get_variable( so_var::key_in_t key, so_var::variable<T> * & ptr_out )
        {
            so_var::ivariable_ptr_t ptr ;
            if( so_core::is_not( this->get_variable(key, ptr) ) )
                return false ;

            if( so_core::is_not( so_core::can_cast<so_var::variable<T> *>(ptr) ) )
                return false ;

            ptr_out = dynamic_cast< so_var::variable<T> * >( ptr ) ;

            return true ;
        }

        void_t for_each( for_each_funk_t ) const ;
        
    };
    so_typedef( variable_set ) ;
}

#endif
