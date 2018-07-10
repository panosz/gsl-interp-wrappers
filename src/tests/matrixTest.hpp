//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIXTEST_HPP
#define GSL_WRAPPERS_MATRIXTEST_HPP

#include <gsl/gsl_vector_double.h>
#include "gmock/gmock.h"
#include "GSL_wrappers/matrix.hpp"

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

TEST(aMatrix,copiesToSelf)
{
  auto m1=Matrix{10,10};
  m1(7,8)=5;
  m1=m1;
  ASSERT_EQ(m1(7,8),5);

}

TEST(aMatrix,givesAccessToRows)
{
  auto m=Matrix{4,4};
  {
    auto r0 = *(m.rows_begin());
    for (auto & e:r0)
      e=5;                         //range assignment
    auto r1=m.row(1);
    r1<<std::vector<int>{1,2,3,4}; //lvalue stream assignment
    m.row(2)<<std::initializer_list<double>{-1,-2,-3,-4}; //rvalue stream assignment
    m.row(3)={10,11,12,13};               //rvalue initializer list assignment

    std::cout<<m<<std::endl;
  }

  ASSERT_ANY_THROW((m.row(1)={M_PI,M_EULER}));
  ASSERT_EQ(m(0,2),5);//range assignment
  ASSERT_EQ(m(1,2),3);//lvalue stream assignment
  ASSERT_EQ(m(2,0),-1);//rvalue stream assignment
  ASSERT_EQ(m(3,3),13);//rvalue initializer list assignment
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
  auto v2=Vector(10);
  v2(8)=5;
  v1=std::move(v2);
  ASSERT_EQ(v1(8),5);
}

TEST(aVector,CopiesToSelf)
{
  auto v1=Vector(10);
  v1(8)=5;
  v1=v1;
  ASSERT_EQ(v1(8),5);
}

TEST(aVector,supportsRangeForAccess)
{
  Vector v(10);
  for(auto & i:v)
    i=M_PI_2;
  ASSERT_DOUBLE_EQ(v[5],M_PI_2);
}

TEST(aVector, supportsInitializerListConstruction)
{
  Vector v{0,1,2,3,4,5,6,7,8};
  ASSERT_EQ(v.size(),9);
  ASSERT_EQ(v[3],3);


  v={M_PI,M_PI_2,M_1_PI,M_2_SQRTPI};
  ASSERT_EQ(v.size(),4);
  ASSERT_DOUBLE_EQ(v[3],M_2_SQRTPI);
}

TEST(aVector, initializerListsConstructorDoesNotBreakCurlyBracketCopyConstructor)
{
  //For copy construction, prefer round bracket copy constructor;

  Vector v1{0,1,2,3,4,5,6,7,8};
  Vector v2{v1};
  ASSERT_EQ(v2.size(),9);
  ASSERT_EQ(v2[3],3);
}

TEST(aVector,streamAssignment)
{
  auto v = Vector(4);
  v << std::vector<int>{1, 2, 3, 4};
  ASSERT_THROW((v<<std::array<double ,2>{M_PI, M_EULER}), std::length_error); //stream assignment exception safety
  ASSERT_EQ(v[2], 3);

}



#endif //GSL_WRAPPERS_MATRIXTEST_HPP
