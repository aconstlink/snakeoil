#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

message( STATUS "[config] : compiler flags")

if( NOT SNAKEOIL_COMPILER_CONFIGURED )
    message( FATAL_ERROR "Compiler not configured." )
endif()

if( SNAKEOIL_COMPILER_MSC )
    
    # cherry-picking
    # C4100 : unreferenced local variable. Got error within 3rd parties
    # C4127 : conditional expression is constant. silence too inconvenient warnings.
    # C4251 : exporting stl container when building dlls
    # C4505 : unreferenced local function, e.g. unused function warning
    # C4702 : unreachable code hit in 3rd parties.
    # C4706 : assignment within conditional expression
    set( SNAKEOIL_UNWANTED_COMPILER_OPTIONS 
	"/wd4100 /wd4127 /wd4251 /wd4505 /wd4702 /wd4706" )

    # Wall : all warnings!
    # W4 : Warning level 4, e.g. nearly all warnings
    # WX : all compiler warnings as errors
    # EH : exception handling
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4 /WX /EHsc" )
    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}" )

    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX /GR /EHsc" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}" )

    unset( SNAKEOIL_UNWANTED_COMPILER_OPTIONS ) 

    #message( STATUS "Configured Microsoft Compiler Flags")

elseif( SNAKEOIL_COMPILER_GNU )

    message( STATUS "[compiler] : GNU Compiler Flags") 

    set( SNAKEOIL_UNWANTED_COMPILER_OPTIONS "-Wno-unused-function" )

    set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${SNAKEOIL_UNWANTED_COMPILER_OPTIONS}")

    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

    set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
    set( CMAKE_CXX_FLAGS_RELEASE " ${CMAKE_CXX_FLAGS_RELEASE} -O2")


    set( CMAKE_CXX_FLAGS_DEBUG  "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g")



    unset( SNAKEOIL_UNWANTED_COMPILER_OPTIONS )

    #message( STATUS "GCC-C++ Flags: ${CMAKE_CXX_FLAGS}")

else()
    message( "No compiler flags configured due to unknown compiler.")
endif()



