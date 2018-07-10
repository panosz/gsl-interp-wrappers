//
// Created by Panagiotis Zestanakis on 18/04/18.
//

#ifndef GSL_WRAPPERS_MATRIX_AND_VECTOR_ITERATORS_HPP
#define GSL_WRAPPERS_MATRIX_AND_VECTOR_ITERATORS_HPP

#include <boost/iterator/iterator_adaptor.hpp>

# ifndef BOOST_NO_SFINAE
#  include <boost/type_traits/is_convertible.hpp>
#  include <boost/utility/enable_if.hpp>
# endif

namespace GSL_Wrappers
{
    template<typename Value=double>
    class stride_iter
        : public boost::iterator_adaptor<
            stride_iter<Value>               // Derived
            , Value *                          // Base
            , Value              // Value
            , boost::random_access_traversal_tag    // CategoryOrTraversal
            , Value &           //Reference
            , typename std::iterator_traits<Value*>::difference_type
        > {
     private:
      struct enabler {
      };  // a private type avoids misuse
      const size_t stride_ = 1;

     public:
      size_t stride() const
      {return stride_;}
      stride_iter ()
          : stride_iter::iterator_adaptor_(0)
      { }

      explicit stride_iter (Value *p)
          : stride_iter::iterator_adaptor_(p)
      { }

      stride_iter (Value *p, size_t stride)
          : stride_iter::iterator_adaptor_(p), stride_{stride}
      { }
      template<typename OtherValue>
      stride_iter (
          stride_iter<OtherValue> const& other
# ifndef BOOST_NO_SFINAE
          , typename boost::enable_if<
          boost::is_convertible<OtherValue *, Value *>, enabler
      >::type = enabler()
#endif
      )
          : stride_iter::iterator_adaptor_(other.base()),
            stride_{other.stride()}
      { }

     private:
      friend class boost::iterator_core_access;
      void increment ()
      { this->base_reference() = this->base() + stride_; }

    };
}

#endif //GSL_WRAPPERS_MATRIX_AND_VECTOR_ITERATORS_HPP
