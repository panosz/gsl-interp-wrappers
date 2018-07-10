find_package(myUtilities REQUIRED)


add_executable(polynomials_example polynomials_example.cpp)

set_target_properties(polynomials_example PROPERTIES EXCLUDE_FROM_ALL TRUE)

target_link_libraries(polynomials_example PUBLIC ${PROJECT_NAME} myUtilities::myUtilities)