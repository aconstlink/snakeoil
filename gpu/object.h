//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPU_OBJECT_H_
#define _SNAKEOIL_GPU_OBJECT_H_

#include "api.h"
#include "protos.h"
#include "typedefs.h"
#include "result.h"

namespace so_gpu
{
    /// non-copyable due to uniqueness of driver objects.
    class SNAKEOIL_GPU_API object
    {
    private:

        friend class so_gpu::api_object_helper ;

        so_gpu::api_object_ptr_t _driver_object_ptr = nullptr ;
        
    private:

        so_gpu::api_object_ptr_t get_driver_object( void_t )
        {
            return _driver_object_ptr ;
        }

        so_gpu::api_object_cptr_t get_driver_object( void_t ) const
        {
            return _driver_object_ptr ;
        }

        void_t set_driver_object( so_gpu::api_object_ptr_t ptr )
        {
            _driver_object_ptr = ptr ;
        }

    private:

        object( object const & ){}

    public:

        object( void_t ){}

        object( object && rhv )
        {
            _driver_object_ptr = rhv._driver_object_ptr ;
            rhv._driver_object_ptr = nullptr ;
        }

        virtual ~object( void_t )
        {
            so_assert( has_no_dirver_object() ) ;
        }

    public:

        bool_t has_driver_object( void_t ) const 
        {
            return _driver_object_ptr != nullptr ;
        }

        bool_t has_no_dirver_object( void_t ) const
        {
            return _driver_object_ptr == nullptr ;
        }

    public:

        virtual void_t destroy( void_t ) = 0 ;
    };
}

#endif

