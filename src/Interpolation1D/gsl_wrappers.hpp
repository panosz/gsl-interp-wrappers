//
// Created by Panagiotis Zestanakis on 30/03/18.
//

#ifndef INTERPOLATION1D_GSL_WRAPPERS_HPP
#define INTERPOLATION1D_GSL_WRAPPERS_HPP
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>


class InterpType
{
 private:
  gsl_interp_type * interp_type_;


};

class Spline
{
 private:
  gsl_spline * spline_;
 public:

  Spline()
  {
    gsl_spline_alloc();
  }
  ~Spline()
  {
    gsl_spline_free( spline_);
  }

};


#endif //INTERPOLATION1D_GSL_WRAPPERS_HPP
