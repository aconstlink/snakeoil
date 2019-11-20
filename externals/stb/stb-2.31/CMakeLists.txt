
set( SOURCES 
  include/stb/stb.h
  include/stb/stb_image.h
  include/stb/stb_image_write.h
  include/stb/stb_truetype.h
  )

###########################################################
# SECTION: Install
###########################################################

set( BASE_INSTALL_DIR "${STB_INSTALL_HEADER_BASE}" )

install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )
