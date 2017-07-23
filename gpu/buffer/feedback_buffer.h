//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_BUFFER_FEEDBACK_BUFFER_H_
#define _SNAKEOIL_GPU_BUFFER_FEEDBACK_BUFFER_H_

#include "../enums/primitive_types.h"

#include "../object.h"
#include <snakeoil/std/container/vector.hpp>

namespace so_gpu
{
    class feedback_buffer : public so_gpu::object
    {
        typedef so_gpu::object base_t ;

        so_this_typedefs( feedback_buffer ) ;

        typedef so_std::vector< so_gpu::buffer_ptr_t > buffers_t ;
        typedef buffers_t & buffers_ref_t ;

    private:
        
        size_t _num_prims_written = 0 ;
        buffers_t _buffers ;
        bool_t _has_changed = true ;

    public:

        feedback_buffer( void_t ) ;
        feedback_buffer( this_rref_t rhv ) ;
        virtual ~feedback_buffer( void_t ) ;


    public:

        static SNAKEOIL_GPU_API this_ptr_t create( std::string const & purpose ) ;
        static SNAKEOIL_GPU_API this_ptr_t create( this_rref_t, std::string const & purpose ); 
        static SNAKEOIL_GPU_API void_t destroy( this_ptr_t ) ;

    public:

        size_t get_num_primitives_written( void_t ) const ;
        void_t set_num_primitives_written( size_t ) ;

    public:

        this_ref_t add_buffer( so_gpu::buffer_ptr_t ) ;

        size_t get_num_buffers( void_t ) ;
        so_gpu::buffer_ptr_t get_buffer( size_t ) ;
        so_gpu::buffer_cptr_t get_buffer( size_t ) const ;
        buffers_ref_t get_buffers( void_t ) ;

        bool_t has_changed( void_t ) const ;
        void_t set_changed( bool_t ) ;

    public:

        virtual void_t destroy( void_t ) ;
    };
    so_typedef( feedback_buffer ) ;
}

#endif

