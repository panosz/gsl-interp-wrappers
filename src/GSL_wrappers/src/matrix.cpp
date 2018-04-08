//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector_double.h>
#include "matrix.hpp"

using namespace GSL_Wrappers;

    GSL_Wrappers::Matrix::Matrix (size_t rows, size_t columns)
    {
      mat_ = gsl_matrix_calloc(rows, columns);
    }

    GSL_Wrappers::Matrix::~Matrix ()
    {
      gsl_matrix_free(mat_);
    }
    double& GSL_Wrappers::Matrix::operator() (size_t i, size_t j)
    {
      return *gsl_matrix_ptr(mat_, i, j);
    }
     const gsl_matrix *GSL_Wrappers::Matrix::operator* () const
    {
      return mat_;
    }
    double Matrix::get (size_t i, size_t j) const
    {
      return gsl_matrix_get(mat_,i,j);
    }

    GSL_Wrappers::MatrixSize GSL_Wrappers::Matrix::size () const
    {
      MatrixSize ms;
      ms.noOfRows = mat_->size1;
      ms.noOfColumns = mat_->size2;
      return ms;
    }

    GSL_Wrappers::Vector::Vector (size_t n)
    {
      vec_ = gsl_vector_calloc(n);
    }
    GSL_Wrappers::Vector::~Vector ()
    {
      gsl_vector_free(vec_);
    }
    double& GSL_Wrappers::Vector::operator() (size_t i)
    {
      return *gsl_vector_ptr(vec_, i);
    }
    const gsl_vector *GSL_Wrappers::Vector::operator* () const
    {
      return vec_;
    }
    size_t GSL_Wrappers::Vector::size () const
    {
      return vec_->size;
    }
    double Vector::get (size_t i) const
    {
      return gsl_vector_get(vec_,i);
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

