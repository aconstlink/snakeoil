
set( SOURCES 
  include/stb/stb.h
  include/stb/stb_image.h
  include/stb/stb_image_write.h
  include/stb/stb_truetype.h
  )

###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "stb-2.29")
set( BASE_INSTALL_DIR "${STB_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )

install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )
