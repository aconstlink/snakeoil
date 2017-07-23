#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
#
#

set( SNAKEOIL_BUILD_ENV_CONFIGURED FALSE )

set( SNAKEOIL_BUILD_ENV_MSVS OFF )
set( SNAKEOIL_BUILD_ENV_MINGW OFF )
set( SNAKEOIL_BUILD_ENV_CYGWIN OFF )
set( SNAKEOIL_BUILD_ENV_MAKE OFF )

set( SNAKEOIL_BUILD_ENV_CONFIGURED TRUE )

if( MSVC_IDE  )
    set( SNAKEOIL_BUILD_ENV_MSVS ON )
endif()

