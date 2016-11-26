  # configure a header file to pass some of the CMake settings
# to the source code
# configure_file (
#   PolyTestConfig.h.in
#   "${CMAKE_CURRENT_BINARY_DIR}/PolyTestConfig.h" @ONLY)




add_subdirectory(
  ${GOOGLEMOCK_DIR}
  "${GOOGLEMOCK_DIR}/googlemock_Build_dir"  EXCLUDE_FROM_ALL)

include_directories(${GOOGLEMOCK_DIR}/googlemock/include
${GOOGLEMOCK_DIR}/googletest/include)

add_executable(Interpolant1DTest Interpolant1DTest.cpp)


target_link_libraries(Interpolant1DTest gmock Interpolation1D)
