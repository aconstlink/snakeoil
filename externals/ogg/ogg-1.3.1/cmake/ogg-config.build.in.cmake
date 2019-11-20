#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# libogg
# Automatically generated via CMake's configure_file for the build tree.
#

####################################################################
# Import targets
####################################################################
include( "@CMAKE_BINARY_DIR@/ogg-targets.cmake" )

set( OGG_MAJOR_VERSION @OGG_MAJOR_VERSION@ )
set( OGG_MINOR_VERSION @OGG_MINOR_VERSION@ )
set( OGG_PATCH_VERSION @OGG_PATCH_VERSION@ )
set( OGG_VERSION @OGG_VERSION@ )

set( OGG_LIBRARIES ogg )

####################################################################
# Definitions
####################################################################
set( OGG_DEFINITIONS )

####################################################################
# Directories
####################################################################
set( OGG_INCLUDE_BASE "@CMAKE_SOURCE_DIR@" )

set( OGG_INCLUDE_DIRS "${OGG_INCLUDE_BASE}/externals/ogg-@OGG_VERSION@/include" )

# not perfect but currently required for dlls under windows
set( OGG_BINDIR "@CMAKE_RUNTIME_OUTPUT_DIRECTORY@" )