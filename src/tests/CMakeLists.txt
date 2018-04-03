

  add_library(gmock ${GOOGLETEST_DIR}/src/gtest-all.cc ${GOOGLEMOCK_DIR}/src/gmock-all.cc)

  target_include_directories(gmock PUBLIC ${GOOGLETEST_DIR} ${GOOGLEMOCK_DIR}
          ${GOOGLETEST_DIR}/include ${GOOGLEMOCK_DIR}/include)

  add_executable(Interpolant1DTest Interpolant1DTest.cpp)

  target_link_libraries(Interpolant1DTest PUBLIC gmock ${PROJECT_NAME})

  add_test(Interpolant1DTest Interpolant1DTest)