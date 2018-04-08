//
// Created by Panagiotis Zestanakis on 30/03/18.
//

#ifndef INTERPOLATION1D_GSL_WRAPPERS_HPP
#define INTERPOLATION1D_GSL_WRAPPERS_HPP


#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <array>
#include <cassert>



namespace GSL_Wrappers
{


    enum class DomainLocate{
      OUTSIDE_DOMAIN=GSL_EDOM,
      INSIDE_DOMAIN=0
    };
    struct ReturnType {
        double result=0;
        DomainLocate error=DomainLocate::INSIDE_DOMAIN;
    };


    class InterpolationBase
    {

     public:


      virtual ReturnType eval(double) const noexcept =0;
      virtual ReturnType eval_deriv(double) const noexcept =0;
      virtual ReturnType eval_deriv2(double) const noexcept =0;
      virtual ReturnType eval_integ(double,double) const noexcept =0;
      virtual unsigned int min_size() const  noexcept =0 ;

      virtual std::string name() const =0;

      virtual ~InterpolationBase ();

    };

    class SplineBase: public InterpolationBase

    {
     private:
      gsl_spline *spline_;
      mutable gsl_interp_accel *acc_;



      ReturnType general_spline_eval(double, int (*)(const gsl_spline *, double, gsl_interp_accel* , double *)) const noexcept ;

     protected:
      SplineBase (const gsl_interp_type *T, const double xa[], const double ya[], size_t size);

     public:



      ReturnType eval(double) const noexcept override final ;
      ReturnType eval_deriv(double) const noexcept override final ;
      ReturnType eval_deriv2(double) const noexcept override final ;
      ReturnType eval_integ(double,double) const noexcept override final ;

      unsigned int min_size() const noexcept override final;

      std::string name() const override;

      virtual ~SplineBase ();

    };



    class LowLevelInterpBase : public InterpolationBase
      /// Unlike SplineBase, this class does not store the interpolation data. It only stores the state computed from the data
      /// and references to it
    {
     private:
      mutable gsl_interp_accel *acc_;
      gsl_interp *interp_;
      const double *xa_; //not owning
      const double *ya_; //not owning

      ReturnType general_interp_eval(double, int (*)
          (const gsl_interp *, const double[], const double[], double, gsl_interp_accel *, double *)) const noexcept ;

     protected:
      LowLevelInterpBase (const gsl_interp_type *T, const double xa[], const double ya[], size_t size);

     public:

      ReturnType eval(double) const noexcept  final ;
      ReturnType eval_deriv(double) const noexcept  final;
      ReturnType eval_deriv2(double) const noexcept  final ;
      ReturnType eval_integ(double,double) const noexcept  final;

      std::string name() const override;
      unsigned int min_size() const noexcept final;

      virtual ~LowLevelInterpBase();

    };






    ///Linear interpolation.
    ///==========================================
    /// This interpolation method does not require
    /// any additional memory.
    class LinearInterpolation
    {
     public:
      const static gsl_interp_type *method()
      {return gsl_interp_linear;}
    };

    //Cubic spline with natural boundary conditions.
    //=================================================
    // The resulting curve
    // is piecewise cubic on each interval, with matching first and second
    // derivatives at the supplied data-points. The second derivative is
    // chosen to be zero at the first point and last point.
    class CSplineInterpolation
    {
     public:
      const static gsl_interp_type *method()
      {return gsl_interp_cspline;}

    };

    //Cubic spline with periodic boundary conditions.
    //==========================================
    // The resulting curve is
    // piecewise cubic on each interval, with matching first and second
    // derivatives at the supplied data-points. The derivatives at the first
    // and last points are also matched. Note that the last point in the data
    // must have the same y-value as the first point, otherwise the resulting
    // periodic interpolation will have a discontinuity at the boundary.
    class CSplinePeriodicInterpolation
    {
     public:
      const static gsl_interp_type *method()
      {return gsl_interp_cspline_periodic;}

    } ;



    //Non-rounded Akima spline with natural boundary conditions.
    //==========================================
    // This method uses the non-rounded corner algorithm of Wodicka.

    class AkimaInterpolation
    {
     public:
      const static gsl_interp_type *method()
      {return gsl_interp_akima;}

    };

    //Non-rounded Akima spline with periodic boundary conditions.
    //==========================================
    // This method uses the non-rounded corner algorithm of Wodicka.

    class AkimaPeriodicInterpolation
    {
     public:
      const static gsl_interp_type *method()
      {return gsl_interp_akima_periodic;}
    };

    //Steffen's method
    //==========================================
    // Steffen’s method guarantees the monotonicity of the interpolating
    // function between the given data points. Therefore, minima
    // and maxima can only occur exactly at the data points, and there
    // can never be spurious oscillations between data points. The
    // interpolated function is piecewise cubic in each interval. The
    // resulting curve and its first derivative are guaranteed to be continuous,
    // but the second derivative may be discontinuous.

    class SteffenInterpolation
    {
     public:
      const static gsl_interp_type *method()
      {return gsl_interp_steffen;}

    };

    template <typename Method>
    unsigned min_samples()
    {
      return gsl_interp_type_min_size(Method::method());
    }


    template <int N,typename Method=LinearInterpolation>
    class
    SplineInterpolator : public SplineBase
    {
     public:


      SplineInterpolator(const std::array<double,N> &xa,const std::array<double,N>& ya):
          SplineBase(Method::method(),xa.data(),ya.data(),N)
      // xa is assumed to be sorted. If otherwise, the behaviour is undefined.
      {}
    };

    template <int N,typename Method=LinearInterpolation>
    class
    LowLevelInterpolator : public LowLevelInterpBase
      /// Unlike SplineInterpolator, the input data are not stored. The user is required to keep valid references to it.
    {
     public:


      LowLevelInterpolator(const std::array<double,N> &xa,const std::array<double,N>& ya):
          LowLevelInterpBase(Method::method(),xa.data(),ya.data(),N)
      // xa is assumed to be sorted. If otherwise, the behaviour is undefined.
      {}
    };
}
#endif //INTERPOLATION1D_GSL_WRAPPERS_HPP
