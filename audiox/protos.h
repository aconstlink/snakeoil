//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_AUDIOX_PROTOS_H_
#define _SNAKEOIL_AUDIOX_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_audiox
{
    so_class_proto_typedefs( audio_system ) ;
    so_class_proto_typedefs( iaudio_engine ) ;
    so_class_proto_typedefs( iloopback_handler ) ;

    so_class_proto_typedefs( audio_technique ) ;
    so_class_proto_typedefs( iplug ) ;
    so_class_proto_typedefs( iplug_factory ) ;


    so_class_proto_typedefs( iglobal_service ) ;
    so_class_proto_typedefs( ilocal_service ) ;
    so_class_proto_typedefs( iuser_data ) ;

}

#endif

