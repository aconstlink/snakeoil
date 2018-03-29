#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Find the stb include headers
#

#
# NOTE: Stb image requires warnings to be disabled in order to compile
# with "warnings as errors"
# stb_image: STBI_DISABLE_WARNINGS_MSC
#
set( STB_FOUND False )

if( NOT STB_PATH_HINT )
    set( STB_PATH_HINT "${CMAKE_CURRENT_LIST_DIR}/../../externals/stb-2.31/include" )
endif()

find_path( STB_INCLUDE_DIRECTORY 
    NAMES "stb/stb.h" 
    HINTS 
    ${STB_PATH_HINT}
    $ENV{STB_INCLUDEDIR}
    )

if( NOT STB_INCLUDE_DIRECTORY )
    message( FATAL_ERROR "Stb include directory required" ) 
endif()


set( STB_INCLUDE_DIRS 
    ${STB_INCLUDE_DIRECTORY} 
    )

# unfortunately required by user of stb image
set( STBI_DISABLE_WARNINGS_MSC "/wd4706 /wd4220 /wd4244 /wd4996" )

set( STB_FOUND True )
