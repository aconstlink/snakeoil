#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# SECTION : Option
#

set( FREETYPE_FOUND False ) 

option( FREETYPE_ENABLE "Enable FreeType SDK" OFF )

if( FREETYPE_ENABLE )
    message( STATUS "[Enabled] : FreeType" )
else()
    message( STATUS "[Disabled] : FreeType" )
    return()
endif()

find_package( freetype CONFIG REQUIRED ) 


