#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# run-time linkage check
#

#if( NOT SNAKEOIL_FOUND )
#    message( FATAL_ERROR "Please use FindVerzerrt module first." )
#endif()

##
## Run-Time Linkage
##

set( SNAKEOIL_RUNTIME_LINKAGE "Shared" CACHE STRING "How to link the C++ Runtime?" )
set_property(CACHE SNAKEOIL_RUNTIME_LINKAGE PROPERTY STRINGS "Static" "Shared")

if( ${SNAKEOIL_RUNTIME_LINKAGE} STREQUAL "Static" )
    message( "Runtime static: Be sure to link to the same static run-time in all used modules." )
    set( SNAKEOIL_RUNTIME_STATIC TRUE )
    add_definitions( -DSNAKEOIL_RUNTIME_STATIC )
elseif( ${SNAKEOIL_RUNTIME_LINKAGE} STREQUAL "Shared" )
    message( "Runtime dynamic: Be sure to link to the same run-time dynamic library in all used modules." )
    message( "Runtime dynamic: Be sure to not mix with static run-time." )
    set( SNAKEOIL_RUNTIME_DYNAMIC TRUE )
    add_definitions( -DSNAKEOIL_RUNTIME_DYNAMIC )
endif()

message( STATUS "Run-Time Linkage: " ${SNAKEOIL_RUNTIME_LINKAGE} )


