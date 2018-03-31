cmake_minimum_required (VERSION 3.2)
project (Interpolation1D)


# The version number.
set (Interpolation1D_VERSION_MAJOR 1)
set (Interpolation1D_VERSION_MINOR 0)
set (Interpolation1D_PATCH_VERSION 0)



#configure version number
set(Interpolation1D_VERSION
  ${Interpolation1D_MAJOR_VERSION}.${Interpolation1D_MINOR_VERSION}.${Interpolation1D_PATCH_VERSION})

#set rpath for MacOs
if (APPLE)
  set(CMAKE_MACOSX_RPATH 1)
  link_directories(/opt/local/lib)
endif(APPLE)

if(APPLE)
  include_directories(/opt/local/include)
endif(APPLE)


set(CMAKE_CXX_STANDARD 14) # C++11...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_FLAGS "-O3 -march=native")

# Offer the user the choice of overriding the googlemock directory
set(GOOGLEMOCK_DIR $ENV{GMOCK_DIR} CACHE PATH "googlemock directory")

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR include CACHE PATH
  "Installation directory for header files")

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/Interpolation1D)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH
  "Installation directory for CMake files")

# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()


# add the binary tree to the search path for include files
# so that we will find Interpolation1DConfig.h
include_directories(
  "${PROJECT_BINARY_DIR}"
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_SOURCE_DIR}/Interpolation1D")




# The interesting stuff goes here
# ===============================

# add the project subdirectories
#
message(STATUS "INSTAL INCLUDE DIR ${INSTALL_INCLUDE_DIR}")
message(STATUS "Project Source Dir ${PROJECT_SOURCE_DIR}")

add_subdirectory(${PROJECT_SOURCE_DIR}/src/${PROJECT_NAME})
add_subdirectory(${PROJECT_SOURCE_DIR}/src/tests)

export(TARGETS ${PROJECT_TARGETS}
        FILE "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake")

# Export the package for use from the build-tree
# (this registers the build-tree with a global CMake-registry)
export(PACKAGE ${PROJECT_NAME})

# Create the ${PROJECT_NAME}Config.cmake and ${PROJECT_NAME}ConfigVersion files
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
        "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
configure_file(${PROJECT_NAME}Config.cmake.in
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "${INSTALL_INCLUDE_DIR}")
configure_file(${PROJECT_NAME}Config.cmake.in
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake" @ONLY)
# ... for both
configure_file(${PROJECT_NAME}ConfigVersion.cmake.in
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake" @ONLY)


# Install the ${PROJECT_NAME}Config.cmake and ${PROJECT_NAME}ConfigVersion.cmake
install(FILES
        "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/${PROJECT_NAME}Config.cmake"
        "${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
        DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)

if(UNIX AND NOT APPLE)
  link_directories($ENV{HOME}/lib)
endif()

# Install the export set for use with the install-tree
install(EXPORT ${PROJECT_NAME}Targets DESTINATION
        "${INSTALL_CMAKE_DIR}" COMPONENT dev)
