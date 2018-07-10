
add_library(${PROJECT_NAME} SHARED
        include/Interpolant1D.hpp
        src/Interpolant1D.cpp
        include/gsl_wrappers.hpp
        src/gsl_wrappers.cpp include/matrix.hpp src/matrix.cpp src/linear_least_quares.cpp
        include/linear_least_quares.hpp include/polynomials.hpp
        src/polynomials.cpp include/minimization.hpp src/minimization.cpp)


find_package(BLAS REQUIRED)

find_package(GSL REQUIRED)
message(STATUS "BLAS_LIBRARIES ${BLAS_LIBRARIES}")


target_link_libraries(${PROJECT_NAME}  PUBLIC GSL::gsl PRIVATE ${BLAS_LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE
        -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic
        -Weffc++  -Wold-style-cast -Wcast-align -Wunused -Woverloaded-virtual
        -Wconversion -Wsign-conversion -Wmisleading-indentation
        -Wduplicated-cond -Wduplicated-branches -Wlogical-op
        -Wnull-dereference -Wuseless-cast -Wdouble-promotion
        -Wformat=2

        $<$<CONFIG:Release>:
        -O2
        -march=native
        -DHAVE_INLINE
        -DGSL_RANGE_CHECK_OFF >
        )
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)


target_include_directories(
        ${PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include/${PROJECT_NAME}>
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src

)



install (
  TARGETS ${PROJECT_NAME}
        EXPORT ${PROJECT_NAME}Targets
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME})




#
#install(
#  FILES ${CMAKE_CURRENT_SOURCE_DIR}/Interpolant1D.hpp
#  DESTINATION "${INSTALL_INCLUDE_DIR}/${PROJECT_NAME}")
