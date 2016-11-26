
add_library(Interpolation1D INTERFACE)

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};$ENV{HOME};/opt/intel/")
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};$ENV{HOME}/lib/CMake/Modules")

#set(BLA_VENDOR "Intel10_64lp")
find_package(BLAS REQUIRED)

#set(GSL_ROOT_DIR $ENV{GSL_HOME})
set(GSL_CBLAS_LIB  "${BLAS_LIBRARIES}" CACHE FILEPATH "jhg" )

message(STATUS "${CMAKE_MODULE_PATH}")
find_package(GSL REQUIRED)
#find_package(MKL REQUIRED)
message(STATUS "BLAS_LIBRARIES ${BLAS_LIBRARIES}")
list(REMOVE_AT GSL_LIBRARIES 1)

message(STATUS "GSL LIBRARIES ${GSL_LIBRARIES}")

target_include_directories(
  Interpolation1D INTERFACE ${GSL_INCLUDES})


target_link_libraries(Interpolation1D INTERFACE ${GSL_LIBRARIES} ${BLAS_LIBRARIES})



target_sources(
  Interpolation1D INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Interpolant1D.hpp>
  $<INSTALL_INTERFACE:include/Interpolant1D.hpp>
  )

install (
  TARGETS Interpolation1D EXPORT Interpolation1DTargets)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/Interpolant1D.hpp
  DESTINATION "${INSTALL_INCLUDE_DIR}/Interpolation1D")
