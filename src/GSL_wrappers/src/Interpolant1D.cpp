//
// Created by Panagiotis Zestanakis on 30/03/18.
//
#include <gsl/gsl_spline.h>
#include <gsl/gsl_interp.h>
#include "Interpolant1D.hpp"
#include <iostream>
using namespace GSL_Wrappers;

ReturnType
SplineBase::general_spline_eval (double x, int (*eval_fn) (const gsl_spline *, double, gsl_interp_accel *, double *)) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(eval_fn(spline_.get(), x, acc_.get(), &ret.result));
  return ret;
}

ReturnType GSL_Wrappers::SplineBase::eval (double x) const noexcept
{
  return general_spline_eval(x, gsl_spline_eval_e);
}
ReturnType SplineBase::eval_deriv (double x) const noexcept
{
  return general_spline_eval(x, gsl_spline_eval_deriv_e);
}
ReturnType SplineBase::eval_deriv2 (double x) const noexcept
{
  return general_spline_eval(x, gsl_spline_eval_deriv2_e);
}
ReturnType SplineBase::eval_integ (double a, double b) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(gsl_spline_eval_integ_e(spline_.get(), a, b, acc_.get(), &ret.result));
  return ret;
}

SplineBase::SplineBase (const gsl_interp_type *T, const double *xa, const double *ya, size_t size)
    : spline_{makeGslSplineUniquePtr(T, size)}, acc_{makeGslInterpAccelUniquePtr()}
{
  gsl_spline_init(spline_.get(), xa, ya, size);
}

unsigned int SplineBase::min_size () const noexcept
{
  return gsl_spline_min_size(spline_.get());
}
std::string SplineBase::name () const
{
  return gsl_spline_name(spline_.get());
}
SplineBase::SplineBase (const SplineBase& other)
    :
    spline_(makeGslSplineUniquePtr(other.spline_->interp->type, other.spline_->size)),
    acc_(makeGslInterpAccelUniquePtr())
{
  gsl_spline_init(spline_.get(), other.spline_->x, other.spline_->y, other.spline_->size);
}
void SplineBase::swap (SplineBase& other) noexcept
{

  using std::swap;
  swap(spline_, other.spline_);
  swap(acc_, other.acc_);

}
SplineBase& SplineBase::operator= (const SplineBase& other)
{
  SplineBase tmp(other);
  swap(tmp);
  return *this;
}
SplineBase& SplineBase::operator= (SplineBase&& other)
{
  swap(other);
  return *this;
}

ReturnType
LowLevelInterpBase::general_interp_eval
    (double x,
     int(*eval_fn) (const gsl_interp *, const double[], const double[], double, gsl_interp_accel *, double *)) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>(eval_fn(interp_.get(), xa_, ya_, x, acc_.get(), &ret.result));

  return ret;
}

LowLevelInterpBase::LowLevelInterpBase (const gsl_interp_type *T, const double *xa, const double *ya, size_t size)
    : interp_{makeGslInterpUniquePtr(T, size)}, acc_{makeGslInterpAccelUniquePtr()}, xa_{xa}, ya_{ya}
{
  gsl_interp_init(interp_.get(), xa, ya, size);
}

ReturnType LowLevelInterpBase::eval (double x) const noexcept
{
  return general_interp_eval(x, gsl_interp_eval_e);
}

ReturnType LowLevelInterpBase::eval_deriv (double x) const noexcept
{
  return general_interp_eval(x, gsl_interp_eval_deriv_e);
}

ReturnType LowLevelInterpBase::eval_deriv2 (double x) const noexcept
{
  return general_interp_eval(x, gsl_interp_eval_deriv2_e);
}
ReturnType LowLevelInterpBase::eval_integ (double a, double b) const noexcept
{
  ReturnType ret;
  ret.error = static_cast<DomainLocate>
  (gsl_interp_eval_integ_e(interp_.get(), xa_, ya_, a, b, acc_.get(), &ret.result));
  return ret;
}
std::string LowLevelInterpBase::name () const
{
  return gsl_interp_name(interp_.get());
}
unsigned int LowLevelInterpBase::min_size () const noexcept
{
  return gsl_interp_min_size(interp_.get());
}
LowLevelInterpBase::LowLevelInterpBase (const LowLevelInterpBase& other)
    :
    interp_{makeGslInterpUniquePtr(other.interp_->type, other.interp_->size)},
    acc_{makeGslInterpAccelUniquePtr()},
    xa_{other.xa_},
    ya_{other.ya_}
{
  gsl_interp_init(interp_.get(), xa_, ya_, other.interp_->size);
}
void LowLevelInterpBase::swap (LowLevelInterpBase& other) noexcept
{
  using std::swap;
  swap(interp_, other.interp_);
  swap(acc_, other.acc_);
  swap(xa_, other.xa_);
  swap(ya_, other.ya_);
}
LowLevelInterpBase& LowLevelInterpBase::operator= (const LowLevelInterpBase& other)
{
  LowLevelInterpBase tmp(other);
  swap(tmp);
  return *this;
}
LowLevelInterpBase& LowLevelInterpBase::operator= (LowLevelInterpBase&& other)
{
  swap(other);
  return *this;
}

GslSplineUniquePtr GSL_Wrappers::makeGslSplineUniquePtr (const gsl_interp_type *T, size_t size)
{
  if (size >= gsl_interp_type_min_size(T))
    return GslSplineUniquePtr(gsl_spline_alloc(T, size), gsl_spline_free);

  throw std::length_error("Spline: Insufficient number of samples");
}
GslInterpAccelUniquePtr GSL_Wrappers::makeGslInterpAccelUniquePtr ()
{
  return GSL_Wrappers::GslInterpAccelUniquePtr(gsl_interp_accel_alloc(), gsl_interp_accel_free);
}
GslInterpUniquePtr GSL_Wrappers::makeGslInterpUniquePtr (const gsl_interp_type *T, size_t size)
{
  if (size >= gsl_interp_type_min_size(T))
    return GslInterpUniquePtr(gsl_interp_alloc(T, size), gsl_interp_free);

  throw std::length_error("Spline: Insufficient number of samples");
}

template<>
const gsl_interp_type *GSL_Wrappers::interpType<LinearInterpolation> ()
{ return gsl_interp_linear; }

template<>
const gsl_interp_type *GSL_Wrappers::interpType<CSplineInterpolation> ()
{ return gsl_interp_cspline; }

template<>
const gsl_interp_type *GSL_Wrappers::interpType<CSplinePeriodicInterpolation> ()
{ return gsl_interp_cspline_periodic; }

template<>
const gsl_interp_type *GSL_Wrappers::interpType<AkimaInterpolation> ()
{ return gsl_interp_akima; }

template<>
const gsl_interp_type *GSL_Wrappers::interpType<AkimaPeriodicInterpolation> ()
{ return gsl_interp_akima_periodic; }

template<>
const gsl_interp_type *GSL_Wrappers::interpType<SteffenInterpolation> ()
{ return gsl_interp_steffen; }
