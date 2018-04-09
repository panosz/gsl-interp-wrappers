//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIX_HPP
#define GSL_WRAPPERS_MATRIX_HPP
#include <gsl/gsl_matrix.h>
#include <iostream>
#include <memory>

namespace GSL_Wrappers
{


    struct MatrixSize
    {
        size_t noOfRows;
        size_t noOfColumns;
    };


    class Matrix
    {

      using Deleter = void(*)(gsl_matrix *);
      std::unique_ptr<gsl_matrix,Deleter> mat_{nullptr,gsl_matrix_free};
     public:
      Matrix()=default;
      Matrix (size_t rows, size_t columns);
      Matrix ( const Matrix &);
      Matrix (Matrix&&)= default;
      Matrix& operator=(const Matrix&);
      Matrix& operator =(Matrix&&);
      double & operator()(size_t i, size_t j);
      double get(size_t i, size_t j) const;
      gsl_matrix * operator*() const noexcept ;
      MatrixSize size() const noexcept ;
      void swap(Matrix &) noexcept;


    };

    class Vector
    {
      using Deleter = void(*)(gsl_vector *);
      std::unique_ptr<gsl_vector,Deleter> vec_{nullptr,gsl_vector_free};
     public:
      Vector()= default;
      explicit Vector (size_t);
      Vector ( const Vector &);
      Vector (Vector&&)= default;
      Vector& operator=(const Vector&);
      Vector& operator=(Vector&&);
      double & operator()(size_t);
      double get(size_t) const;
      gsl_vector * operator*() const noexcept ;
      size_t size() const noexcept ;
      void swap(Vector& other) noexcept ;


    };

    std::ostream& operator<<(std::ostream& os, const Matrix&);
    std::ostream& operator<<(std::ostream& os, const Vector&);

}

namespace std
{
    template<>                                                      // revised specialization of
    void swap<GSL_Wrappers::Matrix>                                 // std::swap
        (GSL_Wrappers::Matrix& a,
         GSL_Wrappers::Matrix& b);


    template<>                                                      // revised specialization of
    void swap<GSL_Wrappers::Vector>                                 // std::swap
        (GSL_Wrappers::Vector& a,
         GSL_Wrappers::Vector& b);

}


#endif //GSL_WRAPPERS_MATRIX_HPP
