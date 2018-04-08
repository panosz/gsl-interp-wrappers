//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIXTEST_HPP
#define GSL_WRAPPERS_MATRIXTEST_HPP

#include "gmock/gmock.h"
#include "matrix.hpp"

using GSL_Wrappers::Matrix;
using GSL_Wrappers::Vector;

TEST(matrixWrapper,KnowsItsSize)
{
  Matrix m(2,3);

  auto s = m.size();
  std::cout<< m;
  ASSERT_EQ(s.noOfRows,2);
  ASSERT_EQ(s.noOfColumns,3);
}

TEST(vectorWrapper,KnowsItsSize)
{
  Vector v(5);
  auto s=v.size();
  ASSERT_EQ(s,5);
}

#endif //GSL_WRAPPERS_MATRIXTEST_HPP
