# Created by the script cgal_create_CMakeLists
# This is the CMake script for compiling a set of CGAL applications.

project( InterpolationTest CXX )


cmake_minimum_required(VERSION 2.8.11)



# Boost and its components
find_package( Boost REQUIRED )

if ( NOT Boost_FOUND )

  message(STATUS "This project requires the Boost library, and will not be compiled.")

  return()  

endif()

find_package(Boost COMPONENTS program_options REQUIRED)

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -std=c++14 -frounding-math" )


# GSL and its components

SET(GSL_LINK_FLAGS "-L$ENV{GSL_LIBDIR} -lgsl -lgslcblas") #use the GSL cblas library
SET(GSL_INCLUDE_FLAGS "-I$ENV{GSL_HOME}/include") 

SET( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GSL_INCLUDE_FLAGS} ${GSL_LINK_FLAGS}" )

include_directories(${CMAKE_SOURCE_DIR} )
#include for boost
include_directories(${Boost_INCLUDE_DIRS})
SET(CMAKE_POSITION_INDEPENDENT_CODE 1)
            
 
# Creating entries for all C++ files with "main" routine
# ##########################################################
#( CGAL_CreateSingleSourceCGALProgram )

add_executable(Interpolant1D_test "Interpolant1D_test.cpp")
target_link_libraries(Interpolant1D_test ${Boost_LIBRARIES} )



