//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_PROGRAM_CONFIG_H_
#define _SNAKEOIL_GPU_PROGRAM_CONFIG_H_

#include "program.h"
#include "../enums/primitive_types.h"
#include "../enums/vertex_attributes.h"

#include <snakeoil/std/container/vector.hpp>
#include <snakeoil/thread/typedefs.h>

namespace so_gpu
{
    class SNAKEOIL_GPU_API config : public so_gpu::object
    {
        so_this_typedefs( config ) ;
        typedef so_gpu::object base_t ;

        struct variable_set_info
        {
            variable_set_ptr_t ptr = nullptr ;
            bool_t changed = true ;
        };
        so_typedef( variable_set_info ) ;
        typedef so_std::vector< variable_set_info > variable_sets_t ;

        typedef std::pair< so_gpu::vertex_attribute, so_std::string > va_to_name_t ;
        typedef so_std::vector< va_to_name_t > va_mappings_t ;

    private:

        variable_sets_t _variable_sets ;

        so_gpu::program_ptr_t _program_ptr = nullptr ;

        so_gpu::primitive_type _pt = so_gpu::primitive_type::undefined ;
        so_gpu::buffer_ptr_t _vb_ptr = nullptr ;
        so_gpu::buffer_ptr_t _ib_ptr = nullptr ;

        /// for the run-time system
        bool_t _config_has_changed = true ;

        /// map vertex attributes to variable names.
        va_mappings_t _va_mappings ;

    public:

        config( void_t ) ;
        config( this_t && rhv ) ;
        virtual ~config( void_t ) ;

    public:

        static this_ptr_t create( void_t ) ;
        static this_ptr_t create( std::string const & purpose ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        void_t set_program( program_ptr_t ptr ) ;
        program_ptr_t get_program( void_t ) ;

    public: // variable sets 

        void_t add_variable_set( variable_set_ptr_t ptr ) ;
        variable_set_ptr_t get_variable_set( size_t index ) ;
        size_t get_num_varsets( void_t ) const ;
        bool_t remove_variable_set( variable_set_ptr_t ) ;

    public:

        /// bind a vertex attribute to a name
        so_gpu::result bind( so_gpu::vertex_attribute va, std::string const & name ) ;

        /// get the input variable from the program via the vertex attribute.
        shader_variable_ptr_t get_input_variable( so_gpu::vertex_attribute va ) ;

    public: // geometry stuff

        so_gpu::result bind( so_gpu::primitive_type pt, so_gpu::buffer_ptr_t vb_ptr, so_gpu::buffer_ptr_t ib_ptr ) ;
        
        so_gpu::primitive_type get_primitive_type( void_t ) const { return _pt ; }
        buffer_ptr_t get_vertex_buffer( void_t ) { return _vb_ptr ; }
        buffer_ptr_t get_index_buffer( void_t ) { return _ib_ptr ; }

    public: // non user functions

        /// required by the system to be called.
        /// validates all varialbes against the program's
        /// internal shader variables. Unused variables
        /// will be dropped.
        void_t validate_shader_variables( void_t ) ;
        
        /// for the run-time
        bool_t has_config_changed( void_t ) const { return _config_has_changed ; }
        void_t set_config_changed( bool_t b ) { _config_has_changed = b ; }


        bool_t has_var_set_changed( size_t const vs ) const 
        { 
            if( vs >= _variable_sets.size() )
                return false ;

            return _variable_sets[vs].changed ; 
        } 
        void_t set_var_set_changed( size_t const vs, bool_t const b ) 
        { 
            if( vs >= _variable_sets.size() ) return ;

            _variable_sets[vs].changed = b ;
        }

    private:

        /// if a program has changed, all variable sets need to be 
        /// checked if the variables match.
        void_t validate_change( void_t ) ;

        /// if a variable set has changed, it must be checked
        /// if all variables are present in the program.
        void_t validate_varset_change( so_gpu::variable_set_ptr_t ptr ) ;

    private:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( config ) ;
}

#endif

