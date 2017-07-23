//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIO_BUFFER_PCM_IPCM_BUFFER_H_
#define _SNAKEOIL_AUDIO_BUFFER_PCM_IPCM_BUFFER_H_

#include "../../typedefs.h"
#include "../../protos.h"
#include "../../api.h"

#include "pcm_types.h"

namespace so_audio
{
    class SNAKEOIL_AUDIO_API ipcm_buffer
    {
    public:

        virtual double_t get_time_in_sec( void_t ) const = 0 ;
        virtual buffer_type get_buffer_type( void_t ) const = 0 ;


        virtual range_t copy_from( ipcm_buffer_ptr_t, size_t const, size_t const ) = 0 ;
        virtual range_t copy_from( byte_cptr_t, size_t const ) = 0 ;
        virtual range_t copy_from( so_audio::buffer_type, byte_cptr_t, size_t const ) = 0 ;

        virtual void_t destroy( void_t ) = 0 ;

        virtual void_cptr_t get_buffer_ptr_at( size_t const ) const = 0 ;

        virtual void_cptr_t get_buffer_ptr( void_t ) const = 0 ;
        virtual size_t get_num_samples( void_t ) const = 0 ;
        virtual size_t get_num_samples_per_second( void_t ) const = 0 ;
        virtual size_t get_num_channels( void_t ) const = 0 ;
    };
    so_typedef( ipcm_buffer ) ;
}

#endif