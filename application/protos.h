//------------------------------------------------------------
// snakeoil (c) Alexis Constantin Link
// Distributed under the MIT license
//------------------------------------------------------------
#ifndef _SNAKEOIL_APPLICATION_PROTOS_H_
#define _SNAKEOIL_APPLICATION_PROTOS_H_

#include <snakeoil/core/macros/typedef.h>

namespace so_app
{
    so_class_proto_typedefs( system ) ;  

    so_class_proto_typedefs( iapplication ) ;  
    so_class_proto_typedefs( window_application ) ;  
    so_class_proto_typedefs( iwindow ) ;
    so_class_proto_typedefs( iwindow_handle ) ;
    so_class_proto_typedefs( iwindow_listener ) ;
    so_class_proto_typedefs( irender_window_listener ) ;
    so_class_proto_typedefs( irender_context ) ;
    
    so_class_proto_typedefs( render_window ) ;
    so_class_proto_typedefs( decorator_window ) ;    


    so_class_proto_typedefs( iwindow_message_listener ) ;

    so_class_proto_typedefs( window_message_receiver ) ;
    
}

#endif

