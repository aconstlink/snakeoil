#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

#
# Configures the build configuration
#
set( SNAKEOIL_BUILD_CONFIG_CONFIGURED FALSE )

#
# Required other config modules
#
if( NOT SNAKEOIL_COMPILER_CONFIGURED )
    message( FATAL_ERROR "Config the build configuration requires the compiler to be configured." )
elseif( NOT SNAKEOIL_BUILD_ENV_CONFIGURED )
    message( FATAL_ERROR "Config the build configuration requires the build environment to be configured." )
endif()

message( STATUS "[config] : build configuration" )

# Do we have microsoft visual studio, the build config requires all flags
# to be set in the compiler option variable because visual studio allows 
# multiple build configurations.
if( SNAKEOIL_COMPILER_MSC )

    set( CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} /DSNAKEOIL_DEBUG" )
    set( CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DSNAKEOIL_DEBUG" )

    set( CMAKE_C_FLAGS_MINSIZEREL "${CMAKE_C_FLAGS_MINSIZEREL} /DSNAKEOIL_RELEASE")	
    set( CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} /DSNAKEOIL_RELEASE")	

    set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DSNAKEOIL_RELEASE")	

    set( CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} /DSNAKEOIL_RELEASE")	
    set( CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /DSNAKEOIL_RELEASE")	

    set( CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO} /DSNAKEOIL_RELEASE")
    set( CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} /DSNAKEOIL_RELEASE")

# with a make system, we can only have one configuration at a time.
elseif( SNAKEOIL_COMPILER_GNU )

    #
    #
    #
    set( SNAKEOIL_BUILD_CONFIG_DEBUG OFF )
    set( SNAKEOIL_BUILD_CONFIG_RELEASE OFF ) 

    #
    # Section: Preset based on CMake
    #
    if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
        set( SNAKEOIL_BUILD_CONFIG_NAME "Debug" )
    elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
        set( SNAKEOIL_BUILD_CONFIG_NAME "Release" )
    else()
        set( SNAKEOIL_BUILD_CONFIG_NAME "Debug" )
    endif()

    #
    # Section: User Option
    #
    set( SNAKEOIL_BUILD_CONFIG_NAMES "Debug" "Release" )
    set( SNAKEOIL_BUILD_CONFIG_NAME ${SNAKEOIL_BUILD_CONFIG_NAME} CACHE STRING "Select your build configuration." )
    set_property( CACHE SNAKEOIL_BUILD_CONFIG_NAME PROPERTY STRINGS ${SNAKEOIL_BUILD_CONFIG_NAMES} )

    #
    # Reset CMake variable
    #
    set( CMAKE_BUILD_TYPE ${SNAKEOIL_BUILD_CONFIG_NAME} CACHE STRING ${SNAKEOIL_BUILD_CONFIG_NAME} FORCE )

    #
    # Section: Definitions
    #
    if( SNAKEOIL_BUILD_CONFIG_NAME STREQUAL "Debug" )
        
        set( SNAKEOIL_BUILD_CONFIG_DEBUG ON )
        add_definitions( -DSNAKEOIL_DEBUG )
        
    elseif( SNAKEOIL_BUILD_CONFIG_NAME STREQUAL "Release" )

        set( SNAKEOIL_BUILD_CONFIG_RELEASE ON )
        add_definitions( -DSNAKEOIL_RELEASE )
        
    endif()

endif()

message( STATUS "[done] : Build Comfiguration" )

set( SNAKEOIL_BUILD_CONFIG_CONFIGURED TRUE )

