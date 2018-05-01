#include "gmock/gmock.h"
#include "gsl_wrappers.hpp"
#include "matrixTest.hpp"
#include "Interp1DTest.hpp"




int main (int argc, char **argv)
{

  // The following line must be executed to initialize Google Mock
  // (and Google Test) before running the tests.
  ::testing::InitGoogleMock (&argc, argv);

  return RUN_ALL_TESTS ();
}
