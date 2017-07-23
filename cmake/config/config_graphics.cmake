#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Can be used to configure the rendering system 
# for the target platform.
#

set( SNAKEOIL_TARGET_GRAPHICS_CONFIGURED FALSE )


set( SNAKEOIL_TARGET_GRAPHICS_NULL ON )
set( SNAKEOIL_TARGET_GRAPHICS_VULKAN OFF )
set( SNAKEOIL_TARGET_GRAPHICS_OPENGL OFF )
set( SNAKEOIL_TARGET_GRAPHICS_OPENGLES OFF )
set( SNAKEOIL_TARGET_GRAPHICS_DIRECT3D OFF )

set( SNAKEOIL_TARGET_GRAPHICS_API "null" CACHE STRING 
    "Select the graphics api to be compiled into the package." )

set_property(CACHE SNAKEOIL_TARGET_GRAPHICS_API PROPERTY STRINGS 
    "null" "Vulkan" "OpenGL" "OpenGL ES" "Direct3D" )

if( SNAKEOIL_TARGET_GRAPHICS_API STREQUAL "null" )
    message( "No Graphics API compiled" )
elseif( SNAKEOIL_TARGET_GRAPHICS_API STREQUAL "OpenGL" )
    find_package( OpenGL REQUIRED )
    set( SNAKEOIL_TARGET_GRAPHICS_OPENGL ON )
elseif( SNAKEOIL_TARGET_GRAPHICS_API STREQUAL "OpenGL ES" )
    message( FATAL_ERROR "Graphcis API currently not supported" )
elseif( SNAKEOIL_TARGET_GRAPHICS_API STREQUAL "Vulkan" )
    message( FATAL_ERROR "Graphcis API currently not supported" )
elseif( SNAKEOIL_TARGET_GRAPHICS_API STREQUAL "Direct3D" )
    message( FATAL_ERROR "Graphcis API currently not supported" )
endif()

set( SNAKEOIL_TARGET_GRAPHICS_CONFIGURED TRUE )

