# - Config file for the PolynomialFit package
# It defines the following variables
#  POLYNOMIALFIT_INCLUDE_DIRS - include directories for PolynomialFit
#  POLYNOMIALFIT_LIBRARIES    - libraries to link against
#  POLYNOMIALFIT_EXECUTABLE   - the bar executable

# Compute paths
get_filename_component(POLYNOMIALFIT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
set(POLYNOMIALFIT_INCLUDE_DIRS "${TUTORIAL_CMAKE_DIR}/../../../include")

# Our library dependencies (contains definitions for IMPORTED targets)
if(NOT TARGET PolynomialFit AND NOT PolynomialFit_BINARY_DIR)
  include("${POLYNOMIALFIT_CMAKE_DIR}/PolynomialFitTargets.cmake")
endif()

# These are IMPORTED targets created by PolynomialFitTargets.cmake
set(POLYNOMIALFIT_LIBRARIES PolynomialFit)
set(POLYNOMIALFIT_EXECUTABLE polyTest)
