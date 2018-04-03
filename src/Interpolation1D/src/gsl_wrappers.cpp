//
// Created by Panagiotis Zestanakis on 30/03/18.
//
#include "gsl_wrappers.hpp"
#include <exception>
using namespace GSL_Interp;

ReturnType SplineBase::general_spline_eval(double x, int (*eval_fn)(const gsl_spline *, double, gsl_interp_accel* , double *)) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(eval_fn(spline_,x,acc_,&ret.result));
  return ret;
}



ReturnType GSL_Interp::SplineBase::eval (double x) const noexcept
{
  return general_spline_eval(x,gsl_spline_eval_e);
}
ReturnType SplineBase::eval_deriv (double x) const noexcept
{
  return general_spline_eval(x,gsl_spline_eval_deriv_e);
}
ReturnType SplineBase::eval_deriv2 (double x) const noexcept
{
  return general_spline_eval(x,gsl_spline_eval_deriv2_e);
}
ReturnType SplineBase::eval_integ (double a, double b) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(gsl_spline_eval_integ_e(spline_,a,b,acc_,&ret.result));
  return ret;
}

SplineBase::SplineBase (const gsl_interp_type *T, const double *xa, const double *ya, size_t size)
{
  if (size>=gsl_interp_type_min_size(T))
    {
      acc_ = gsl_interp_accel_alloc();
      spline_ = gsl_spline_alloc(T, size);
      gsl_spline_init(spline_, xa, ya, size);
    }
  else
    throw std::length_error("Spline: Insufficient number of samples");
}

SplineBase::~SplineBase()
{
  gsl_spline_free(spline_);
  gsl_interp_accel_free(acc_);
}

unsigned int SplineBase::min_size() const noexcept
{
  return gsl_spline_min_size(spline_);
}
std::string SplineBase::name() const
{
  return  gsl_spline_name(spline_);
}





ReturnType
LowLevelInterpBase::general_interp_eval
    (double x,
     int(* eval_fn) (const gsl_interp *, const double[], const double[], double, gsl_interp_accel *, double *)) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(eval_fn(interp_,xa_,ya_,x,acc_,&ret.result));

  return ret;
}

LowLevelInterpBase::LowLevelInterpBase (const gsl_interp_type *T, const double *xa, const double *ya, size_t size)
    :xa_{xa},ya_{ya}
{

  if (size>=gsl_interp_type_min_size(T))
    {
      acc_ = gsl_interp_accel_alloc();
      interp_ = gsl_interp_alloc(T, size);
      gsl_interp_init(interp_, xa, ya, size);
    }
  else
    throw std::length_error("Interp: Insufficient number of samples");

}
LowLevelInterpBase::~LowLevelInterpBase ()
{
  gsl_interp_free(interp_);

  gsl_interp_accel_free(acc_);
}
ReturnType LowLevelInterpBase::eval (double x) const noexcept
{
  return general_interp_eval(x,gsl_interp_eval_e);
}

ReturnType LowLevelInterpBase::eval_deriv (double x) const noexcept
{
  return general_interp_eval(x,gsl_interp_eval_deriv_e);
}

ReturnType LowLevelInterpBase::eval_deriv2 (double x) const noexcept
{
  return general_interp_eval(x,gsl_interp_eval_deriv2_e);
}
ReturnType LowLevelInterpBase::eval_integ (double a, double b) const noexcept
{
  ReturnType ret;
  ret.error= static_cast<DomainLocate>
  (gsl_interp_eval_integ_e(interp_,xa_,ya_,a,b,acc_,&ret.result));
  return ret;
}
std::string LowLevelInterpBase::name () const
{
  return gsl_interp_name(interp_);
}
unsigned int LowLevelInterpBase::min_size () const noexcept
{
  return gsl_interp_min_size(interp_);
}

InterpolationBase::~InterpolationBase ()
{


}
