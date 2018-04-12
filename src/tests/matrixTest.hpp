//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIXTEST_HPP
#define GSL_WRAPPERS_MATRIXTEST_HPP

#include "gmock/gmock.h"
#include "matrix.hpp"

using GSL_Wrappers::Matrix;
using GSL_Wrappers::Vector;

TEST(aMatrix,KnowsItsSize)
{
  Matrix m(2,3);

  auto s = m.size();
  ASSERT_EQ(s.noOfRows,2);
  ASSERT_EQ(s.noOfColumns,3);
}

TEST(aMatrix,IsDefaultInitializedToZero)
{
  Matrix m(10,15);
  for (unsigned i=0;i<10;++i)
    for (unsigned j=0;j<15;++j)
      ASSERT_EQ(m(i,j),0);
}

TEST(aMatrix,CanHoldValues)
{
  Matrix m(2,3);
  m(1,1)=7;
  ASSERT_EQ(m(1,1),7);

}

TEST(aMatrix,roundAndSquareBracketsAreEquivalent)
{
  Matrix m(2,3);
  m(1,1)=7;
  ASSERT_EQ(m(1,1),7);
  auto matrix_index=3*1+1;
  ASSERT_EQ(m[matrix_index],7);
};

TEST(aMatrix,isMoveConstructable)
{
  Matrix m1(2,3);
  m1(1,1)=7;
  Matrix m2(std::move(m1));
  ASSERT_EQ(m2(1,1),7);
}
TEST(aMatrix, isCopyConstructable)
{
  Matrix m1(2,3);
  m1(1,1)=7;
  Matrix m2(m1);
  ASSERT_EQ(m2(1,1),7);
  m2(1,1)=3;
  ASSERT_EQ(m1(1,1),7);
}

TEST(aMatrix, isCopyable)
{
  Matrix m1(2,2);
  Matrix m2(10,10);
  m2(8,8)=3;
  m1=m2;
  ASSERT_EQ(m1(8,8),3);
}

TEST(aMatrix,isMovable)
{
  Matrix m1{};
  auto m2=Matrix{10,10};
  m2(8,8)=5;
  m1=std::move(m2);
  ASSERT_EQ(m1(8,8),5);
}

TEST(aMatrix,CopiesToSelf)
{
  auto m1=Matrix{10,10};
  m1(7,8)=5;
  m1=m1;
  ASSERT_EQ(m1(7,8),5);
}

TEST(aVector,KnowsItsSize)
{
  Vector v(5);
  auto s=v.size();
  ASSERT_EQ(s,5);
}

TEST(aVector,IsDefaultInitializedToZero)
{
  Vector v(10);
  for (unsigned i=0;i<10;++i)
      ASSERT_EQ(v(i),0);
}

TEST(aVector,CanHoldValues)
{
  Vector v(10);
  v(1)=7;
  ASSERT_EQ(v(1),7);
}

TEST(aVector,roundAndSquareBracketsAreEquivalent)
{
  Vector v(10);
  v(5)=7;
  ASSERT_EQ(v[5],7);
};

TEST(aVector,isMoveConstructable)
{
  Vector v1(10);
  v1(1) = 7;
  Vector v2(std::move(v1));
  ASSERT_EQ(v2(1), 7);
}

TEST(aVector, isCopyConstructable)
{
  Vector v1(10);
  v1(8)=7;
  Vector v2(v1);
  ASSERT_EQ(v2(8),7);
  v2(8)=3;
  ASSERT_EQ(v1(8),7);
}

TEST(aVector, isCopyable)
{
  Vector v1(2);
  Vector v2(10);
  v2(8)=3;
  v1=v2;
  ASSERT_EQ(v1(8),3);
}

TEST(aVector,isMovable)
{
  Vector v1{};
  auto v2=Vector{10};
  v2(8)=5;
  v1=std::move(v2);
  ASSERT_EQ(v1(8),5);
}

TEST(aVector,CopiesToSelf)
{
  auto v1=Vector{10};
  v1(8)=5;
  v1=v1;
  ASSERT_EQ(v1(8),5);
}


#endif //GSL_WRAPPERS_MATRIXTEST_HPP
