//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#include "../api.h"
#include "../protos.h"
#include "../typedefs.h"

#include <snakeoil/std/container/vector.hpp>

namespace so_audio
{
    struct SNAKEOIL_AUDIO_API api_creator
    {
        so_typedefs( so_std::vector< so_audio::iapi_ptr_t >, apis ) ;

        static bool_t create_defaults( apis_out_t ) ;
    };
    so_typedef( api_creator ) ;
}