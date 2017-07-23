//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_SYNC_SYNC_H_
#define _SNAKEOIL_GPU_SYNC_SYNC_H_

#include "../object.h"

namespace so_gpu
{
    class sync final : public so_gpu::object
    {
        typedef so_gpu::object base_t ;
        so_this_typedefs( sync ) ;

    private:

        bool_t _signaled = false ;

    public:

        static SNAKEOIL_GPU_API this_ptr_t create( void_t ) 
        {
            return this_t::create( "[so_gpu::sync::create]" ) ;
        }

        static SNAKEOIL_GPU_API this_ptr_t create( std::string const & purpose ) 
        {
            return so_gpu::memory::alloc( this_t(), purpose ) ;
        }

        static SNAKEOIL_GPU_API void_t destroy( this_ptr_t ptr ) 
        {
            so_gpu::memory::dealloc( ptr ) ;
        }

    public:

        sync( void_t ) {}
        sync( sync && rhv ) : base_t(std::move(rhv) ) {}
        virtual ~sync( void_t ) {}

    public:

        bool_t is_signaled( void_t )
        {
            return _signaled ;
        }

        void_t set_signaled( bool_t b ) 
        {
            _signaled = b ;
        }
       
    private:

        virtual void_t destroy( void_t ) final
        {
            this_t::destroy( this ) ;
        }

    };
}

#endif

