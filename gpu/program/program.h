//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_PROGRAM_PROGRAM_H_
#define _SNAKEOIL_GPU_PROGRAM_PROGRAM_H_

#include "../object.h"

#include "../enums/feedback_buffer_types.h"
#include "../enums/shader_variable_types.h"
#include "../shader/vertex_shader.h"
#include "../shader/geometry_shader.h"
#include "../shader/pixel_shader.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_gpu
{
    class SNAKEOIL_GPU_API program : public so_gpu::object
    {
        typedef so_gpu::object base_t ;
        so_this_typedefs( program ) ;

        typedef so_std::vector<shader_variable_ptr_t> shader_variables_t ;
        typedef so_std::vector<config_ptr_t> configs_t ;

        friend class config ;

    private:

        vertex_shader_ptr_t _vertex_shader = nullptr ;
        geometry_shader_ptr_t _geometry_shader = nullptr ;
        pixel_shader_ptr_t _pixel_shader = nullptr ;

        shader_variables_t _input_variables ;
        shader_variables_t _shader_variables ;

        /// output variable names for feedback rendering
        std::vector<std::string> _feedback_names ;
        so_gpu::feedback_buffer_type _fbt = so_gpu::feedback_buffer_type::interleaved ;

        ///  all connected program configurations
        /// if the program is linked after a configs or multiple
        /// configs are set to a program, the varialbes need to
        /// rechecked.
        configs_t _configs ;

    public:

        static this_ptr_t create( this_rref_t, so_memory::purpose_cref_t ) ;
        static this_ptr_t create( so_memory::purpose_cref_t ) ;
        static void_t destroy( this_ptr_t ) ;

    public:

        program( void_t ) ;
        program( this_rref_t rhv ) ;
        virtual ~program( void_t ) ;

    public:

        this_ref_t set_shader( shader_ptr_t shd_ptr ) ;
        this_ref_t set_shader( vertex_shader_ptr_t shd_ptr ) ;
        this_ref_t set_shader( geometry_shader_ptr_t shd_ptr ) ;
        this_ref_t set_shader( pixel_shader_ptr_t shd_ptr ) ;

    public: // variable stuff

        void_t add_input_variable( shader_variable_ptr_t var_ptr ) ;
        void_t add_shader_variable( shader_variable_ptr_t var_ptr ) ;

        shader_variables_t move_out_input_variables( void_t ) { return std::move(_input_variables) ; }
        shader_variables_t move_out_shader_variables( void_t ) { return std::move(_shader_variables) ; }

        shader_variable_ptr_t find_shader_input_variable( std::string const & name ) ;
        shader_variable_ptr_t find_shader_variable( std::string const & name ) ;
        shader_variable_ptr_t has_shader_variable( std::string const & name, shader_variable_type svt ) ;

    public: // shader stuff

        vertex_shader_ptr_t get_vertex_shader( void_t) { return _vertex_shader ; }
        vertex_shader_cptr_t get_vertex_shader( void_t) const { return _vertex_shader ; }

        geometry_shader_ptr_t get_geometry_shader( void_t ) { return _geometry_shader ; }
        geometry_shader_cptr_t get_geometry_shader( void_t ) const { return _geometry_shader ; }
        
        pixel_shader_ptr_t get_pixel_shader( void_t ) { return _pixel_shader ; }
        pixel_shader_cptr_t get_pixel_shader( void_t ) const { return _pixel_shader ; }

        bool_t has_vertex_shader( void_t ) const ;
        bool_t has_no_vertex_shader( void_t ) const ;
        bool_t has_geometry_shader( void_t ) const ;
        bool_t has_no_geometry_shader( void_t ) const ;
        bool_t has_pixel_shader( void_t ) const ;
        bool_t has_no_pixel_shader( void_t ) const ;

    public: // feedback stuff

        void_t set_feedback_type( so_gpu::feedback_buffer_type t ) { _fbt = t ; }
        so_gpu::feedback_buffer_type get_feedback_type( void_t ) const { return _fbt ; }
        
        this_ref_t add_feedback_name( std::string const & name ) ;
        size_t get_num_feedback_names( void_t ) const ;
        std::string const & get_feedback_name( size_t index ) const ;
        bool_t has_feedback_names( void_t ) const ;
        bool_t has_no_feedback_names( void_t ) const ;

    public:

        /// required to be called after the program has been 
        /// linked. So all variables are checked if configs
        /// are set to this program before the link.
        /// @note this is done by the system. No user 
        /// intervention required.
        void_t validate_connected_configs( void_t ) ;

    private: // program config

        void_t add_config( so_gpu::config_ptr_t ptr ) ;
        void_t detach_config( so_gpu::config_ptr_t ptr ) ;
        void_t detach_all_configs( void_t ) ;

    private:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( program ) ;
}


#endif

