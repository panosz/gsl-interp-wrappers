

  add_library(gmock ${GOOGLETEST_DIR}/src/gtest-all.cc ${GOOGLEMOCK_DIR}/src/gmock-all.cc)

  target_include_directories(gmock PUBLIC ${GOOGLETEST_DIR} ${GOOGLEMOCK_DIR}
          ${GOOGLETEST_DIR}/include ${GOOGLEMOCK_DIR}/include)

  add_executable(wrappersTest gsl_wrappersTest.cpp matrixTest.hpp interp1DTest.hpp linear_least_squaresTest.hpp)

  target_link_libraries(wrappersTest PUBLIC gmock ${PROJECT_NAME})

  add_test(wrappersTest wrappersTest)