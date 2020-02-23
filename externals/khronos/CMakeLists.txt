

set( SOURCES 
  include/GL/glcorearb.h
  include/GL/glext.h
  include/GL/glxext.h
  include/GL/wglext.h )

set( INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/include" )
add_library( khronos_glheader INTERFACE )
target_include_directories( khronos_glheader INTERFACE $<BUILD_INTERFACE:${INCLUDE_DIR}> )

###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "khronos")
set( BASE_INSTALL_DIR "${KHRONOS_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )
install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )


