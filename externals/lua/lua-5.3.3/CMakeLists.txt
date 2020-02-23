
#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.0 )
project( lua C )

if( NOT LUA_INSTALL_HEADER_BASE )
  set( INSTALL_HEADER_BASE "" )
endif()

if( MSVC )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4324 /wd4310" )
endif()

set( LUA_MAJOR_VERSION 5 )
set( LUA_MINOR_VERSION 3 )
set( LUA_PATCH_VERSION 3 )
set( LUA_VERSION "${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}" )

set( LUA_BUILD_TYPE STATIC )
option( LUA_BUILD_AS_DLL "Build lua as dynamically linked library" OFF )



set( CMAKE_DEBUG_POSTFIX d )

set( INCLUDES 
    "include/lapi.h"
    "include/lauxlib.h"
    "include/lcode.h"
    "include/lctype.h"
    "include/ldebug.h"
    "include/ldo.h"
    "include/lfunc.h"
    "include/lgc.h"
    "include/llex.h"
    "include/llimits.h"
    "include/lmem.h"
    "include/lobject.h"
    "include/lopcodes.h"
    "include/lparser.h"
    "include/lprefix.h"
    "include/lstate.h"
    "include/lstring.h"
    "include/ltable.h"
    "include/ltm.h"
    "include/lua.h"
    "include/lua.hpp"
    "include/luaconf.h"
    "include/lualib.h"
    "include/lundump.h"
    "include/lvm.h"
    "include/lzio.h"    
)

set( LIB_SOURCES 
    "src/lauxlib.c"
    "src/lbaselib.c"
    "src/lbitlib.c"
    "src/lcorolib.c"
    "src/ldblib.c"
    "src/liolib.c"
    "src/lmathlib.c"
    "src/loadlib.c"
    "src/loslib.c"
    "src/lstrlib.c"
    "src/ltablib.c"
    "src/lutf8lib.c"
    "src/linit.c"
)

set( CORE_SOURCES 
    "src/lapi.c"    
    "src/lcode.c"    
    "src/lctype.c"    
    "src/ldebug.c"
    "src/ldo.c"
    "src/ldump.c"
    "src/lfunc.c"
    "src/lgc.c"        
    "src/llex.c"    
    "src/lmem.c"    
    "src/lobject.c"
    "src/lopcodes.c"    
    "src/lparser.c"
    "src/lstate.c"
    "src/lstring.c"    
    "src/ltable.c"    
    "src/ltm.c"        
    "src/lundump.c"    
    "src/lvm.c"
    "src/lzio.c"
)

set( COMPILER_SOURCES 
    "src/luac.c"    
)

set( LUA_SOURCES 
    "src/lua.c"      
)

set( SOURCES 
    ${CORE_SOURCES} 
    ${LIB_SOURCES} 
    ${INCLUDES}
)

set( INCLUDES "${CMAKE_CURRENT_LIST_DIR}/include" )

if( LUA_BUILD_AS_DLL )    
    set( LUA_BUILD_TYPE SHARED )
endif()

add_library( lua ${LUA_BUILD_TYPE} ${SOURCES} )
set_target_properties( lua PROPERTIES LINKER_LANGUAGE C )
target_include_directories( lua PUBLIC $<BUILD_INTERFACE:${INCLUDES}> )

if( LUA_BUILD_AS_DLL )
    target_compile_options( lua PUBLIC -LUA_BUILD_AS_DLL -LUA_USE_LONGJMP) 
endif()



###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "${PROJECT_NAME}-${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}")
set( BASE_INSTALL_DIR "${LUA_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )
install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )
install_library( lua )


