//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_IMEX_AUDIO_IAUDIO_H_
#define _SNAKEOIL_IMEX_AUDIO_IAUDIO_H_

#include "../typedefs.h"
#include "../result.h"
#include "../api.h"
#include "../protos.h"

#include "pcm_buffer_types.h"

namespace so_imex
{
    class SNAKEOIL_IMEX_API iaudio
    {
    public:

        virtual double_t get_time_in_sec( void_t ) const = 0 ;
        virtual byte_cptr_t get_memory_at( size_t const sample ) const = 0 ;
        virtual size_t get_num_samples( void_t ) const = 0 ;
        virtual size_t get_num_samples_per_second( void_t ) const = 0 ;
        virtual size_t get_num_channels( void_t ) const = 0 ;
        virtual pcm_buffer_type get_type( void_t ) const = 0 ;
        virtual void_t destroy( void_t ) = 0 ;
    };
    so_typedef( iaudio )  ;
}

#endif