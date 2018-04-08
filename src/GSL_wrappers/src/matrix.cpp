//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector_double.h>
#include <utility>

#include "matrix.hpp"


using namespace GSL_Wrappers;

GSL_Wrappers::Matrix::Matrix (size_t r, size_t c)
    :mat_{gsl_matrix_calloc(r, c),gsl_matrix_free}
{}


double& GSL_Wrappers::Matrix::operator() (size_t i, size_t j)
{
  return *gsl_matrix_ptr(mat_.get(), i, j);
}
const gsl_matrix *GSL_Wrappers::Matrix::operator* () const noexcept
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

GSL_Wrappers::Vector::Vector (size_t n):
    vec_{gsl_vector_calloc(n),gsl_vector_free}
{ }

double& GSL_Wrappers::Vector::operator() (size_t i)
{
  return *gsl_vector_ptr(vec_.get(), i);
}
const gsl_vector *GSL_Wrappers::Vector::operator* () const noexcept
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

std::ostream& GSL_Wrappers::operator<< (std::ostream& os, const GSL_Wrappers::Vector& v)
{
  for (unsigned i=0;i<v.size();++i)
    os << v.get(i) <<'\n';
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
