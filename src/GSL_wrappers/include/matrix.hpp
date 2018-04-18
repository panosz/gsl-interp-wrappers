//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIX_HPP
#define GSL_WRAPPERS_MATRIX_HPP


#include <gsl/gsl_matrix.h>
#include <iostream>
#include <memory>
#include <iterator>
#include <functional>
#include <gsl/gsl_vector_double.h>

#include "matrix_and_vector_iterators.hpp"
#include <boost/iterator/transform_iterator.hpp>



namespace GSL_Wrappers
{



    struct MatrixSize
    {
        size_t noOfRows;
        size_t noOfColumns;
    };


    /* Row
     * ------------------------------------------------*/

    class Matrix;

    class Row
    {
      friend Matrix;

      double* begin_;
      double* end_;
      Row(double* begin,double* end); //only Matrix can construct Row
     public:

      double* begin();
      const double* begin() const;
      double* end();
      const double* end() const;
      size_t size () const;
      Row& operator =(const std::initializer_list<double> &);
    };
    

    template <class E>
    Row& operator<<(Row& r, const E& e)
    {
      if (r.size()==e.size())
        std::copy(std::begin(e),std::end(e),r.begin());
      else
        throw std::length_error("Operator << : incompatible input size for Row");
      return r;
    }

    template <class E>
    void operator<<(Row&& r, const E& e)
    {
      if (r.size()==e.size())
        std::copy(std::begin(e),std::end(e),r.begin());
      else
        throw std::length_error("Operator << : incompatible input size for Row");
    }

    std::ostream& operator<<(std::ostream& os, const Row&);




    /* Matrix
     * ------------------------------------------------*/

    class RowsRange
    {
      friend Matrix;
      Matrix& mat_;
      RowsRange(Matrix& mat):mat_{mat}{};
     public:
      auto begin();
      auto end();

    };

    class RowsRangeConst
    {
      friend Matrix;
      const Matrix& mat_;
      RowsRangeConst(const Matrix& mat):mat_{mat}{};
     public:
      auto begin() const;
      auto end() const;
    };

    class Matrix
    {

      using Deleter = void(*)(gsl_matrix *);
      std::unique_ptr<gsl_matrix,Deleter> mat_{nullptr,gsl_matrix_free};
      size_t unused_elems_in_a_row() const
      { return mat_->tda - size().noOfColumns;}
      Row make_row(double* row_begin)
      {return Row(row_begin,row_begin+size().noOfColumns);};

      const Row make_row(double* row_begin) const
      {return Row(row_begin,row_begin+size().noOfColumns);};

     public:
      Matrix()=default;
      Matrix (size_t rows, size_t columns);
      Matrix ( const Matrix &);
      Matrix (Matrix&&)= default;
      Matrix& operator=(const Matrix&);
      Matrix& operator =(Matrix&&);
      double & operator()(size_t i, size_t j);
      double & operator[](size_t s);
      double  operator[](size_t s) const;
      double get(size_t i, size_t j) const;
      gsl_matrix * operator*() const noexcept ;
      MatrixSize size() const noexcept ;
      void swap(Matrix &) noexcept;

      auto rows_begin()
      {return boost::make_transform_iterator(stride_iter<double>(mat_->data,mat_->tda),
        std::function<Row(double&)>([this](double& x){return make_row(&x);}));}

      auto rows_cbegin() const
      {return boost::make_transform_iterator(stride_iter<double>(mat_->data,mat_->tda),
                                             std::function<const Row(double&)>([this](double& x){return make_row(&x);}));}

      auto rows_end()
      {return boost::make_transform_iterator(stride_iter<double>(mat_->data+mat_->tda*size().noOfRows,mat_->tda),
          std::function<Row(double&)>([this](double& x){return make_row(&x);}));}

      auto rows_cend() const
      {return boost::make_transform_iterator(stride_iter<double>(mat_->data+mat_->tda*size().noOfRows,mat_->tda),
                                             std::function< const Row(double&)>([this](double& x){return make_row(&x);}));}
      Row row(size_t i)
      {
        double* row_begin = & operator()(i,0);
        return make_row(row_begin);
      };

      auto rows()
      {return RowsRange(*this);};

      const auto rows() const
      {return RowsRangeConst(*this);}
    };











    using vector_iterator= stride_iter<double>;
    using vector_const_iterator = stride_iter<double const>;

    class Vector
    // has an initializer_list constructor.
    // Prefer round bracket constructors, unless passing an initializer_list;
    {

     protected:
      using Deleter = void(*)(gsl_vector *);
      std::unique_ptr<gsl_vector,Deleter> vec_{nullptr,gsl_vector_free};
     public:
      Vector()= default;
      explicit Vector (size_t);
      explicit Vector(gsl_vector *);
      Vector ( const Vector &);
      Vector (Vector&&)= default;
      template<typename E>
      Vector (std::initializer_list<E>);
      Vector& operator=(const Vector&);
      Vector& operator=(Vector&&);
      double & operator()(size_t);
      double & operator[](size_t);
      double operator [] (size_t) const;
      double get(size_t) const;
      gsl_vector * operator*() const noexcept ;
      size_t size() const noexcept ;
      void swap(Vector& other) noexcept ;
      vector_iterator begin()
      {return vector_iterator(vec_->data,vec_->stride);}
      vector_const_iterator begin() const
      {return vector_iterator(vec_->data,vec_->stride);}

      vector_iterator end()
      {
          double* next_to_last = vec_->data+vec_->stride*size();
          return vector_iterator(next_to_last,vec_->stride);}

      vector_const_iterator end() const
      {
          double* next_to_last = vec_->data+vec_->stride*size();
          return vector_iterator(next_to_last,vec_->stride);}
    };

//
    template<typename E>
    Vector::Vector (std::initializer_list<E> s)
    :vec_(gsl_vector_alloc(s.size()),gsl_vector_free)
    {
      std::copy(s.begin(),s.end(),begin());
    }

    template<>
    Vector::Vector(std::initializer_list<Vector> v);
    //this is in case the user tries makes use of the copy constructor using curly braces.
    //The compiler will choose the specialized template initializer_list constructor.



    template <class E>
    Vector& operator<<(Vector& vector, const E& e)
    {
      if (vector.size()==e.size())
        std::copy(std::begin(e),std::end(e),vector.begin());
      else
        throw std::length_error("Operator << : incompatible input size for Vector");
      return vector;
    }

    

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
