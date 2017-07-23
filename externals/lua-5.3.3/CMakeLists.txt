
#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

cmake_minimum_required( VERSION 3.0 )
project( lua C )

if( MSVC )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /wd4244 /wd4324 /wd4310" )
endif()

set( LUA_MAJOR_VERSION 5 )
set( LUA_MINOR_VERSION 3 )
set( LUA_PATCH_VERSION 3 )
set( LUA_VERSION "${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}" )

set( LUA_BUILD_TYPE STATIC )
set( LUA_BUILD_AS_DLL OFF CACHE BOOL "Build lua as dynamically linked library" )
if( ${LUA_BUILD_AS_DLL} )
    add_definitions( -DLUA_BUILD_AS_DLL -DLUA_USE_LONGJMP )
    set( LUA_BUILD_TYPE SHARED )
endif()

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

include_directories( "./include" ) 

add_library( lua ${LUA_BUILD_TYPE} ${SOURCES} )
set_target_properties( lua PROPERTIES LINKER_LANGUAGE C )

if( ${LUA_BUILD_AS_DLL} )
    target_compile_definitions( lua PUBLIC -DLUA_BUILD_AS_DLL ) 
endif()



###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "${PROJECT_NAME}-${LUA_MAJOR_VERSION}.${LUA_MINOR_VERSION}.${LUA_PATCH_VERSION}")
set( BASE_INSTALL_DIR "externals/${FOLDER_NAME}" )
install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )

#message( ${LUA_BUILD_TYPE} )
if( ${LUA_BUILD_TYPE} STREQUAL "SHARED" )
    install( TARGETS lua 
        RUNTIME DESTINATION "${BASE_INSTALL_DIR}/bin"
        LIBRARY DESTINATION "${BASE_INSTALL_DIR}/lib" 
        ARCHIVE DESTINATION "${BASE_INSTALL_DIR}/lib" )
else()
    install( TARGETS lua 
        DESTINATION "${BASE_INSTALL_DIR}/lib" )
endif()

