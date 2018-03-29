
set( SOURCES 
  include/rapidxml/rapidxml.hpp
  include/rapidxml/rapidxml_iterators.hpp
  include/rapidxml/rapidxml_print.hpp
  include/rapidxml/rapidxml_utils.hpp )


###########################################################
# SECTION: Install
###########################################################

set( FOLDER_NAME "rapidxml-1.13")
set( BASE_INSTALL_DIR "${RAPIDXML_INSTALL_HEADER_BASE}/${FOLDER_NAME}" )

install_headers( "${SOURCES}" "${BASE_INSTALL_DIR}" )
