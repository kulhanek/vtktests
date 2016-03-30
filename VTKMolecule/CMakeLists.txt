# ==============================================================================
# Test CMake File
# ==============================================================================

CMAKE_MINIMUM_REQUIRED(VERSION 2.8.9 FATAL_ERROR)
#SET(CMAKE_VERBOSE_MAKEFILE ON)

# what languages we use --------------------------------------------------------
PROJECT(myprog) 
ENABLE_LANGUAGE(C)
ENABLE_LANGUAGE(CXX)

# external dependencies --------------------------------------------------------
IF(NOT DEFINED COMPOSITE_PROJECT)
    SET(DEVELOPMENT_ROOT "$ENV{DEVELOPMENT_ROOT}")
ENDIF(NOT DEFINED COMPOSITE_PROJECT)

IF(NOT DEVELOPMENT_ROOT)
    MESSAGE(FATAL_ERROR "DEVELOPMENT_ROOT variable is not set!")
ENDIF(NOT DEVELOPMENT_ROOT)

# ==============================================================================
# external libraries -----------------------------------------------------------
# ==============================================================================
# QT5 --------------------------------------------
SET(CMAKE_AUTOMOC ON)
FIND_PACKAGE(Qt5Core REQUIRED)
FIND_PACKAGE(Qt5Widgets REQUIRED)
FIND_PACKAGE(Qt5OpenGL REQUIRED)

# OpenBabel --------------------------------------
SET(OPEN_BABEL_ROOT ${DEVELOPMENT_ROOT}/thirdparty/obcore/2.3.1)
INCLUDE_DIRECTORIES(${OPEN_BABEL_ROOT}/include)
LINK_DIRECTORIES(${OPEN_BABEL_ROOT}/lib)
SET(OPEN_BABEL_LIB obcore)

# VTK --------------------------------------------
SET(VTK_DIR "${DEVELOPMENT_ROOT}/thirdparty/vtk/6.3.0/build/lib/cmake/vtk-6.3")
MESSAGE(${VTK_DIR})
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})

SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl -lpthread")
# ==============================================================================
# output directories -----------------------------------------------------------
# ==============================================================================

SET(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# rpaths -----------------------------------------------------------------------
# ==============================================================================

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)

# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)

# the RPATH to be used when installing
SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)

# ==============================================================================
# compiler setups --------------------------------------------------------------
# ==============================================================================

IF(NOT DEFINED COMPOSITE_PROJECT)
    SET(CMAKE_CXX_FLAGS_RELEASE "-O2")
    SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wundef -Wno-long-long")

    #SET(CMAKE_BUILD_TYPE Release)
    SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT DEFINED COMPOSITE_PROJECT)

# ==============================================================================
# project subdirectories  ------------------------------------------------------
# ==============================================================================

ADD_SUBDIRECTORY(src)



