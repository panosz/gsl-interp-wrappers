  # configure a header file to pass some of the CMake settings
# to the source code
# configure_file (
#   PolyTestConfig.h.in
#   "${CMAKE_CURRENT_BINARY_DIR}/PolyTestConfig.h" @ONLY)


  add_library(gmock ${GOOGLETEST_DIR}/src/gtest-all.cc ${GOOGLEMOCK_DIR}/src/gmock-all.cc)

  target_include_directories(gmock PUBLIC ${GOOGLETEST_DIR} ${GOOGLEMOCK_DIR}
          ${GOOGLETEST_DIR}/include ${GOOGLEMOCK_DIR}/include)



  add_executable(Interpolant1DTest Interpolant1DTest.cpp)


target_link_libraries(Interpolant1DTest gmock Interpolation1D)
