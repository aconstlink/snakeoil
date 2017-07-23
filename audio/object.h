//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_OBJECT_H_
#define _SNAKEOIL_AUDIO_OBJECT_H_

#include "api.h"
#include "protos.h"
#include "typedefs.h"
#include "result.h"

#include <snakeoil/core/assert.h>

namespace so_audio
{
    /// non-copyable due to uniqueness of driver objects.
    class SNAKEOIL_AUDIO_API object
    {
        so_this_typedefs( object ) ;

    private:

        so_audio::api_object_ptr_t _driver_object_ptr = nullptr ;
        
    public:

        so_audio::api_object_ptr_t get_driver_object( void_t )
        {
            return _driver_object_ptr ;
        }

        so_audio::api_object_cptr_t get_driver_object( void_t ) const
        {
            return _driver_object_ptr ;
        }

        void_t set_driver_object( so_audio::api_object_ptr_t ptr )
        {
            _driver_object_ptr = ptr ;
        }

    public:

        object( void_t ){}
        object( so_audio::api_object_ptr_t ptr ) : _driver_object_ptr(ptr) {}
        object( this_cref_t ) = delete ;
        object( this_rref_t rhv )
        {
            so_move_member_ptr( _driver_object_ptr, rhv ) ;
        }

        virtual ~object( void_t )
        {
            so_assert( has_no_dirver_object() ) ;
        }

        this_ref_t operator = ( this_rref_t rhv )
        {
            so_move_member_ptr( _driver_object_ptr, rhv ) ;
            return *this ;
        }

        this_ref_t operator = ( this_cref_t ) = delete ;

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

