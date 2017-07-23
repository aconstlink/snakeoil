//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VARIABLE_SHADER_VARIABLE_H_
#define _SNAKEOIL_GPU_VARIABLE_SHADER_VARIABLE_H_

#include "../object.h"
#include "../enums/shader_variable_types.h"

#include <string>

namespace so_gpu
{
    class shader_variable : public so_gpu::object
    {
        typedef so_gpu::object base_t ;
        so_this_typedefs( shader_variable ) ;

    private:

        std::string _name ;
        shader_variable_type _type ;

    public:

        static SNAKEOIL_GPU_API this_ptr_t create( void_t ) ;
        static SNAKEOIL_GPU_API this_ptr_t create( this_rref_t rhv, std::string const & purpose ) ;
        static SNAKEOIL_GPU_API void_t destroy( this_ptr_t ) ;

    public:

        shader_variable( void_t ) ;
        SNAKEOIL_GPU_API shader_variable( std::string const & name, shader_variable_type type ) ;
        shader_variable( shader_variable && rhv ) ;
        virtual SNAKEOIL_GPU_API ~shader_variable( void_t ) ;

        bool_t is_name( std::string const & name ) const ;
        bool_t is_name( this_ptr_t rhv ) const ;
        std::string const & get_name( void_t ) const ;

        bool_t is_type( shader_variable_type type ) const ;
        shader_variable_type get_type( void_t ) const ;

    private:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( shader_variable ) ;
}

#endif

