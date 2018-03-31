//
// Created by Panagiotis Zestanakis on 30/03/18.
//
#include "gsl_wrappers.hpp"
using namespace GSL_Interp;

ReturnType SplineBase::general_spline_eval(double x, int (*eval_fn)(const gsl_spline *, double, gsl_interp_accel* , double *))
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(eval_fn(spline_,x,acc_,&ret.result));
  return ret;
}


ReturnType GSL_Interp::SplineBase::eval (double x)
{
  return general_spline_eval(x,gsl_spline_eval_e);
}
ReturnType SplineBase::eval_deriv (double x)
{
  return general_spline_eval(x,gsl_spline_eval_deriv_e);
}
ReturnType SplineBase::eval_deriv2 (double x)
{
  return general_spline_eval(x,gsl_spline_eval_deriv2_e);
}
ReturnType SplineBase::eval_integ (double a, double b)
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(gsl_spline_eval_integ_e(spline_,a,b,acc_,&ret.result));
  return ret;
}

SplineBase::SplineBase (const gsl_interp_type *T, const double *xa, const double *ya, size_t size)
{
  acc_ = gsl_interp_accel_alloc();
  spline_=gsl_spline_alloc(T, size);
  gsl_spline_init(spline_, xa, ya, size);

}

SplineBase::~SplineBase ()
{
  gsl_spline_free(spline_);
  gsl_interp_accel_free(acc_);
}


