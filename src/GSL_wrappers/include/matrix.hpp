//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIX_HPP
#define GSL_WRAPPERS_MATRIX_HPP
#include <gsl/gsl_matrix.h>
#include <iostream>

namespace GSL_Wrappers
{
    struct MatrixSize
    {
        size_t noOfRows;
        size_t noOfColumns;
    };

    class Matrix
    {
      gsl_matrix *mat_;
     public:
      Matrix (size_t rows, size_t columns);
      virtual ~Matrix ();
      double & operator()(size_t i, size_t j);
      double get(size_t i, size_t j) const;
      gsl_matrix * operator*();
      MatrixSize size() const;

      friend std::ostream& operator<<(std::ostream& os, const Matrix&);

    };

    class Vector
    {
      gsl_vector *vec_;
     public:
      Vector (size_t);
      virtual ~Vector ();
      double & operator()(size_t);
      double get(size_t) const;
      gsl_vector * operator*();
      size_t size() const;

      friend std::ostream& operator<<(std::ostream& os, const Vector&);

    };

    std::ostream& operator<<(std::ostream& os, const Matrix&);
    std::ostream& operator<<(std::ostream& os, const Vector&);

}



#endif //GSL_WRAPPERS_MATRIX_HPP
