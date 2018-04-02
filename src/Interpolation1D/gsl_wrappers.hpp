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


namespace GSL_Interp
{


    enum class DomainLocate{
      OUTSIDE_DOMAIN=GSL_EDOM,
      INSIDE_DOMAIN=0
    };
    struct ReturnType {
        double result=0;
        DomainLocate error=DomainLocate::INSIDE_DOMAIN;
    };

    class SplineBase {
     private:
      gsl_spline *spline_;
      gsl_interp_accel *acc_;

      ReturnType general_spline_eval(double, int (*)(const gsl_spline *, double, gsl_interp_accel* , double *));

     protected:
      SplineBase (const gsl_interp_type *T, const double xa[], const double ya[], size_t size);

     public:



      ReturnType eval(double);
      ReturnType eval_deriv(double);
      ReturnType eval_deriv2(double);
      ReturnType eval_integ(double,double);

      unsigned int min_size();

      std::string name() const;

      virtual ~SplineBase ();

    };

//    class InterpolationMethodBase
//    {
//     public:
//      const gsl_interp_type *method;
//      InterpolationMethodBase (const gsl_interp_type *method);
//    };


    //Linear interpolation.
    //==========================================
    // This interpolation method does not require
    // any additional memory.
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
    Interpolator : public SplineBase
      //Linear interpolation. This interpolation method does not require any additional memory.

    {
      //const static unsigned min_size_ = gsl
     public:


      Interpolator(std::array<double,N>xa,std::array<double,N>ya):
          SplineBase(Method::method(),xa.data(),ya.data(),N)
      // xa is assumed to be sorted. If otherwise, the behaviour is undefined.
      {}
    };
}



#endif //INTERPOLATION1D_GSL_WRAPPERS_HPP
