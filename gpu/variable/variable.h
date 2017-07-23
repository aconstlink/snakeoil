//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_VARIABLE_VARIABLE_H_
#define _SNAKEOIL_GPU_VARIABLE_VARIABLE_H_

#include "../object.h"
#include "../enums/shader_variable_types.h"

#include <string>

#include <snakeoil/core/macros/typedef.h>

namespace so_gpu
{
    class SNAKEOIL_GPU_API variable : public so_gpu::object
    {
        so_this_typedefs( variable ) ;

    private:

        bool_t _changed = true ;
        shader_variable_type _type = shader_variable_type::undefined ;

    protected:
        
        variable( void_t ) ;
        void_t set_shader_variable_type( shader_variable_type svt ) ;

    public:
        
        variable( shader_variable_type type ) ;
        variable( variable && rhv ) ;
        virtual ~variable( void_t ) ;

        bool_t is_type( shader_variable_type type ) const ;
        shader_variable_type get_type( void_t ) const ;   

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif

