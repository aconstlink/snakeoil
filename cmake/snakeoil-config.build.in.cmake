#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# snakeoil libraries
# Automatically generated via CMake's configure_file for the build tree.
#

####################################################################
# Import targets
####################################################################
include( "@SNAKEOIL_BINARY_DIR@/snakeoil-targets.cmake" )

####################################################################
# Directories
####################################################################
set( SNAKEOIL_INCLUDE_BASE "@CMAKE_SOURCE_DIR@" )

set( SNAKEOIL_CONFIGS_DIR 
    "${SNAKEOIL_INCLUDE_BASE}/cmake/config" )

set( SNAKEOIL_MODULES_DIR 
    "${SNAKEOIL_INCLUDE_BASE}/cmake/modules"
    "${SNAKEOIL_INCLUDE_BASE}/cmake/macros"
    "${SNAKEOIL_INCLUDE_BASE}/cmake/functions")

# not perfect but currently required for dlls under windows
set( SNAKEOIL_BINDIR "@CMAKE_RUNTIME_OUTPUT_DIRECTORY@" )

set( SNAKEOIL_CXX_STANDARD @SNAKEOIL_CXX_STANDARD@ )

list( APPEND CMAKE_MODULE_PATH ${SNAKEOIL_MODULES_DIR} )
include( so_visual_studio_src_dir )
include( so_visual_studio_generate_bat_dll )
