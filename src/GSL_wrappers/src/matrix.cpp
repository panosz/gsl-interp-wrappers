//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#include <gsl/gsl_matrix.h>
#include <utility>

#include "matrix.hpp"



using namespace GSL_Wrappers;

GSL_Wrappers::Matrix::Matrix (size_t r, size_t c)
    :mat_{gsl_matrix_calloc(r, c),gsl_matrix_free}
{}

Matrix::Matrix (const Matrix& m)
:mat_{gsl_matrix_calloc(m.size().noOfRows, m.size().noOfColumns),gsl_matrix_free}
{
  gsl_matrix_memcpy(mat_.get(),m.mat_.get());
}



double& GSL_Wrappers::Matrix::operator() (size_t i, size_t j)
{
  return *gsl_matrix_ptr(mat_.get(), i, j);
}

double& GSL_Wrappers::Matrix::operator[] (size_t s)
{
  auto numberOfCols=size().noOfColumns;
  size_t i = s/numberOfCols;
  size_t j = s%numberOfCols;
  return operator()(i,j);
}

double Matrix::operator[] (size_t s) const
{
  auto numberOfCols=size().noOfColumns;
  size_t i = s/numberOfCols;
  size_t j = s%numberOfCols;
  return *gsl_matrix_ptr(mat_.get(), i, j);
}

gsl_matrix *GSL_Wrappers::Matrix::operator* () const noexcept
{
  return mat_.get();
}
double Matrix::get (size_t i, size_t j) const
{
  return gsl_matrix_get(mat_.get(),i,j);
}

GSL_Wrappers::MatrixSize GSL_Wrappers::Matrix::size () const noexcept
{
  MatrixSize ms;
  ms.noOfRows = mat_->size1;
  ms.noOfColumns = mat_->size2;
  return ms;
}
void Matrix::swap (Matrix& other) noexcept
{
  using std::swap;
  swap(mat_,other.mat_);
}
Matrix& Matrix::operator= (const Matrix& m)
{
  Matrix tmp(m);
  swap(tmp);
  return *this;
}
Matrix& Matrix::operator= (Matrix&& m)
{
  swap(m);
  return *this;
}


GSL_Wrappers::Vector::Vector (size_t n):
    vec_{gsl_vector_calloc(n),gsl_vector_free}
{ }

double& GSL_Wrappers::Vector::operator() (size_t i)
{
  return operator[](i);
}
double& Vector::operator[] (size_t i)
{
  return *gsl_vector_ptr(vec_.get(),i);
}

double Vector::operator[] (size_t i) const
{
  return *gsl_vector_ptr(vec_.get(),i);
}

gsl_vector *GSL_Wrappers::Vector::operator* () const noexcept
{
  return vec_.get();
}
size_t GSL_Wrappers::Vector::size () const noexcept
{
  return vec_->size;
}
double Vector::get (size_t i) const
{
  return gsl_vector_get(vec_.get(),i);
}

void Vector::swap (Vector& other) noexcept
{
  using std::swap;
  swap(vec_,other.vec_);
}
Vector::Vector (const Vector&v):
vec_{gsl_vector_calloc(v.size()),gsl_vector_free}
{
 gsl_vector_memcpy(vec_.get(),v.vec_.get());
}
Vector& Vector::operator= (const Vector&v)
{
  Vector tmp(v);
  swap(tmp);
  return *this;
}

Vector& Vector::operator= (Vector&& v)
{
  swap(v);
  return *this;
}


std::ostream& GSL_Wrappers::operator<< (std::ostream& os, const GSL_Wrappers::Vector& v)
{
  for (const auto & i:v)
    os << i <<'\n';
  return os;
}

std::ostream& GSL_Wrappers::operator<< (std::ostream& os, const GSL_Wrappers::Matrix&m)
{
  auto[rows,columns] = m.size();
  for (unsigned i=0;i<rows;++i)
    {
      for (unsigned j=0;j<columns;++j)
        os<<m.get(i,j)<<' ';
      os<<'\n';
    }
  return os;
}

template<>
void std::swap<GSL_Wrappers::Matrix> (GSL_Wrappers::Matrix& a, GSL_Wrappers::Matrix& b)
{
  a.swap(b);
}

template<>
void std::swap<Vector> (GSL_Wrappers::Vector& a, GSL_Wrappers::Vector& b)
{
  a.swap(b);
}
