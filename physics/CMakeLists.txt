#.............................................................
#. snakeoil (c) Alexis Constantin Link
#. Distributed under the MIT license
#.............................................................

set( SOURCES 
    typedefs.h

    2d/math.h
    2d/body.hpp
    2d/solve/euler.hpp
    2d/inertia.hpp
    2d/motion/linear.hpp
    2d/motion/angular.hpp

    3d/body.hpp
    3d/solve.hpp
    )

so_vs_src_dir( SOURCES )

add_library( ${CUR_LIB_NAME} STATIC ${SOURCES} )
add_library( ${CUR_ALIAS_NAME} ALIAS ${CUR_LIB_NAME} )

###########################################################
# SECTION: Build Tree
###########################################################

so_export( ${CUR_LIB_NAME} )

###########################################################
# SECTION: Install
###########################################################

install_headers( "${SOURCES}" "include/${PROJECT_NAME}/${CUR_LIB_NAME_SHORT}" )

