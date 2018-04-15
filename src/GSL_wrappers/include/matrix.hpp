//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_MATRIX_HPP
#define GSL_WRAPPERS_MATRIX_HPP

#define HAVE_INLINE
#define GSL_RANGE_CHECK_OFF

#include <gsl/gsl_matrix.h>
#include <iostream>
#include <memory>
#include <iterator>

# include <boost/iterator/iterator_adaptor.hpp>

# ifndef BOOST_NO_SFINAE
#  include <boost/type_traits/is_convertible.hpp>
#  include <boost/utility/enable_if.hpp>
#include <gsl/gsl_vector_double.h>
# endif


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
        std::copy(e.begin(),e.end(),r.begin());
      else
        throw std::length_error("Operator << : incompatible input size for Row");
      return r;
    }

    template <class E>
    void operator<<(Row&& r, const E& e)
    {
      if (r.size()==e.size())
        std::copy(e.begin(),e.end(),r.begin());
      else
        throw std::length_error("Operator << : incompatible input size for Row");
    }

    /* Matrix
     * ------------------------------------------------*/

    class Matrix
    {

      using Deleter = void(*)(gsl_matrix *);
      std::unique_ptr<gsl_matrix,Deleter> mat_{nullptr,gsl_matrix_free};
      size_t unused_elems_in_a_row() const
      { return mat_->tda - size().noOfColumns;}
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



      Row row(size_t i)
      {
        double* first = & operator()(i,0);
        auto row_begin= first;
        double * next_to_last = first+size().noOfColumns;
        auto row_end = next_to_last;
        return Row(row_begin,row_end);
      };

    };









    template <typename Value=double>
    class vector_iter
        : public boost::iterator_adaptor<
            vector_iter<Value>               // Derived
            , Value*                          // Base
            , boost::use_default              // Value
            , boost::random_access_traversal_tag    // CategoryOrTraversal
        >
    {
     private:
      struct enabler {};  // a private type avoids misuse
      const size_t stride_=1;

     public:
      vector_iter()
          : vector_iter::iterator_adaptor_(0) {}

      explicit vector_iter(Value* p)
          : vector_iter::iterator_adaptor_(p) {}

      vector_iter(Value* p, size_t stride)
          : vector_iter::iterator_adaptor_(p),stride_{stride} {}
      template <typename OtherValue>
      vector_iter(
          vector_iter<OtherValue> const& other
# ifndef BOOST_NO_SFINAE
          , typename boost::enable_if<
          boost::is_convertible<OtherValue*,Value*>
          , enabler
      >::type = enabler()
#endif
      )
          : vector_iter::iterator_adaptor_(other.base()),
            stride_{other.stride_} {}

     private:
      friend class boost::iterator_core_access;
      void increment()
      { this->base_reference() = this->base() + stride_; }


    };

    using vector_iterator= vector_iter<double>;
    using vector_const_iterator = vector_iter<double const>;

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
      {return vector_const_iterator(vec_->data,vec_->stride);}

      vector_iterator end()
      {
          double* next_to_last = vec_->data+vec_->stride*size();
          return vector_iterator(next_to_last,vec_->stride);}

      vector_const_iterator end() const
      {
          double* next_to_last = vec_->data+vec_->stride*size();
          return vector_const_iterator(next_to_last,vec_->stride);}
    };

//
    template<typename E>
    Vector::Vector (std::initializer_list<E> s)
    :vec_(gsl_vector_alloc(s.size()),gsl_vector_free)
    {
      std::copy(s.begin(),s.end(),begin());
    };

    template<>
    Vector::Vector(std::initializer_list<Vector> v);
    //this is in case the user tries makes use of the copy constructor using curly braces.
    //The compiler will choose the specialized template initializer_list constructor.



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
