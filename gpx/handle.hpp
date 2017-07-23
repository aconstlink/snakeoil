//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_GPX_HANDLE_H_
#define _SNAKEOIL_GPX_HANDLE_H_

#include "log.h"
#include "typedefs.h"

#include "system/system.h"

#include <snakeoil/gpu/result.h>
#include <snakeoil/gpu/protos.h>

namespace so_gpx
{
    /// references the user accessable object
    /// access only allowed on specific conditions.
    template< typename type_t >
    class handle
    {
        so_this_typedefs( handle<type_t> ) ;

        typedef type_t * type_ptr_t ;
        typedef type_t const * type_cptr_t ;
        
        typedef std::function< so_gpu::result ( so_gpu::driver_ptr_t, type_ptr_t ) > exec_t ;

    private:

        so_gpx::system_ptr_t _system_ptr = nullptr ;
        type_ptr_t _object_ptr = nullptr ;

    private:

        // no copy
        handle( this_cref_t rhv ) {}

    public:

        handle( void_t )
        {
        
        }

        handle( type_ptr_t obj_ptr, so_gpx::system_ptr_t sys_ptr )
        {
            so_assert( sys_ptr != nullptr ) ;
            so_assert( obj_ptr != nullptr ) ;
            _system_ptr = sys_ptr ;
            _object_ptr = obj_ptr ;
        }

        handle( this_rref_t rhv ) 
        {
            *this = rhv ;
        }

        ~handle( void_t )
        {
            // @todo release res
            // send object_ptr back to system 
            // for proper  destruction
        }

    public:

        this_ref_t operator = ( this_rref_t rhv )
        {
            _system_ptr = rhv._system_ptr ;
            rhv._system_ptr = nullptr ;
            _object_ptr = rhv._object_ptr ;
            rhv._object_ptr = nullptr ;
            return *this ;
        }

    public:

        /// write access level
        type_ptr_t operator -> ( void_t )
        {
            check_accessable() ;
            return _object_ptr ;
        }

    public:

        /// write access level
        /// allows to write to an engine object.
        type_ptr_t write( void_t ) 
        {
            check_accessable() ;
            return _object_ptr ;
        }

        /// read access level
        /// writing to the object is permitted. Only reading 
        /// the engine object is possible. That is, all
        /// const declared functions.
        type_cptr_t read( void_t ) const 
        {
            return _object_ptr ;
        }

    private:

        // do now allow access to the graphics system 
        // while the graphics system is preparing the
        // shadow command queue.
        void_t check_accessable( void_t ) const 
        {
            so_assert( _system_ptr != nullptr ) ;
            so_assert( _object_ptr != nullptr ) ;
            //so_assert( _system_ptr->is_accessable() ) ;
        }
    };
}

#endif

