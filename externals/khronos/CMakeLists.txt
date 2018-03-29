

set( SOURCES 
  include/GL/glcorearb.h
  include/GL/glext.h
  include/GL/glxext.h
  include/GL/wglext.h )


###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "khronos")
set( BASE_INSTALL_DIR "${KHRONOS_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )
install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )
