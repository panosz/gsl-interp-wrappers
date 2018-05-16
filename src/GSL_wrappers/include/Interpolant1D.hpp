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
#include <memory>

namespace GSL_Wrappers
{

    enum class DomainLocate {
      OUTSIDE_DOMAIN = GSL_EDOM,
      INSIDE_DOMAIN = 0
    };
    struct ReturnType {
        double result = 0;
        DomainLocate error = DomainLocate::INSIDE_DOMAIN;
    };

    class InterpolationBase {

     public:

      virtual ReturnType eval (double) const noexcept =0;
      virtual ReturnType eval_deriv (double) const noexcept =0;
      virtual ReturnType eval_deriv2 (double) const noexcept =0;
      virtual ReturnType eval_integ (double, double) const noexcept =0;
      virtual unsigned int min_size () const noexcept =0;

      virtual std::string name () const =0;

      virtual ~InterpolationBase () = default;

    };

    using GslSplineUniquePtr = std::unique_ptr<gsl_spline, void (*) (gsl_spline *)>;
    GslSplineUniquePtr makeGslSplineUniquePtr (const gsl_interp_type *T, size_t size);

    using GslInterpAccelUniquePtr = std::unique_ptr<gsl_interp_accel, void (*) (gsl_interp_accel *)>;
    GslInterpAccelUniquePtr makeGslInterpAccelUniquePtr ();

    class SplineBase : public InterpolationBase {
     private:
      GslSplineUniquePtr spline_;
      GslInterpAccelUniquePtr acc_;

      ReturnType
      general_spline_eval (double, int (*) (const gsl_spline *, double, gsl_interp_accel *, double *)) const noexcept;




     public:

      SplineBase (const gsl_interp_type *T, const double xa[], const double ya[], size_t size);
      SplineBase( const SplineBase &);
      SplineBase(SplineBase&&) =default;
      void swap(SplineBase &) noexcept;
      SplineBase& operator=(const SplineBase&);
      SplineBase& operator =(SplineBase&&);

      ReturnType eval (double) const noexcept override final;
      ReturnType eval_deriv (double) const noexcept override final;
      ReturnType eval_deriv2 (double) const noexcept override final;
      ReturnType eval_integ (double, double) const noexcept override final;

      unsigned int min_size () const noexcept override final;

      std::string name () const override;

      virtual ~SplineBase () = default;

    };

    using GslInterpUniquePtr = std::unique_ptr<gsl_interp, void (*) (gsl_interp *)>;
    GslInterpUniquePtr makeGslInterpUniquePtr (const gsl_interp_type *T, size_t size);

    class LowLevelInterpBase : public InterpolationBase
      /// Unlike SplineBase, this class does not store the interpolation data. It only stores the state computed from the data
      /// and references to it
    {
     private:
      GslInterpUniquePtr interp_;
      GslInterpAccelUniquePtr acc_;

      const double *xa_; //not owning
      const double *ya_; //not owning



      ReturnType general_interp_eval (double, int (*)
          (const gsl_interp *, const double[], const double[], double, gsl_interp_accel *, double *)) const noexcept;



     public:

      LowLevelInterpBase (const gsl_interp_type *T, const double xa[], const double ya[], size_t size);
      LowLevelInterpBase( const LowLevelInterpBase &);
      LowLevelInterpBase(LowLevelInterpBase&&) =default;
      void swap(LowLevelInterpBase &) noexcept;
      LowLevelInterpBase& operator=(const LowLevelInterpBase&);
      LowLevelInterpBase& operator =(LowLevelInterpBase&&);

      ReturnType eval (double) const noexcept final;
      ReturnType eval_deriv (double) const noexcept final;
      ReturnType eval_deriv2 (double) const noexcept final;
      ReturnType eval_integ (double, double) const noexcept final;

      std::string name () const override;
      unsigned int min_size () const noexcept final;

      virtual ~LowLevelInterpBase () = default;

    };

    template<class InterpolationAlgorithmTag>
    const gsl_interp_type *interpType ();

    ///Linear interpolation.
    ///==========================================
    /// This interpolation method does not require
    /// any additional memory.
    class LinearInterpolation //a tag
    {
    };


    //Cubic spline with natural boundary conditions.
    //=================================================
    // The resulting curve
    // is piecewise cubic on each interval, with matching first and second
    // derivatives at the supplied data-points. The second derivative is
    // chosen to be zero at the first point and last point.
    class CSplineInterpolation//a tag
    {
    };



    //Cubic spline with periodic boundary conditions.
    //==========================================
    // The resulting curve is
    // piecewise cubic on each interval, with matching first and second
    // derivatives at the supplied data-points. The derivatives at the first
    // and last points are also matched. Note that the last point in the data
    // must have the same y-value as the first point, otherwise the resulting
    // periodic interpolation will have a discontinuity at the boundary.
    class CSplinePeriodicInterpolation//a tag
    {
    };





    //Non-rounded Akima spline with natural boundary conditions.
    //==========================================
    // This method uses the non-rounded corner algorithm of Wodicka.

    class AkimaInterpolation//a tag
    {
    };




    //Non-rounded Akima spline with periodic boundary conditions.
    //==========================================
    // This method uses the non-rounded corner algorithm of Wodicka.

    class AkimaPeriodicInterpolation //a tag
    {
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

    class SteffenInterpolation //a tag
    {
    };



    template<typename Method>
    unsigned min_samples ()
    {
      return gsl_interp_type_min_size(interpType<Method>());
    }

    template<typename CT, typename Method>
    SplineBase makeSplineBase(const CT& xa, const CT& ya)
    {
        if (xa.size()==ya.size())
            return SplineBase(interpType<Method>(),xa.data(),ya.data(),xa.size());
        throw std::length_error("Interpolation: xa and ya must have the same size");
    }

    template<typename CT, typename Method>
    LowLevelInterpBase makeLowLevelInterpBase(const CT& xa, const CT& ya)
    {

        if (xa.size()==ya.size())
            return  LowLevelInterpBase(interpType<Method>(), xa.data(), ya.data(), xa.size());
        throw std::length_error("Interpolation: xa and ya must have the same size");
    }

    template<typename Method=LinearInterpolation>
    class
    SplineInterpolator : public SplineBase {
     public:

      template<typename CT>
      SplineInterpolator (const CT& xa, const CT& ya)
          :
          SplineBase(makeSplineBase<CT,Method>(xa, ya))
      // xa is assumed to be sorted. If otherwise, the behaviour is undefined.
      { }
    };

    template<typename Method=LinearInterpolation>
    class
    LowLevelInterpolator : public LowLevelInterpBase
      /// Unlike SplineInterpolator, the input data are not stored. The user is required to keep valid references to it.
    {
     public:

      template<typename CT>
      LowLevelInterpolator (const CT& xa, const CT& ya)
          :
          LowLevelInterpBase(makeLowLevelInterpBase<CT,Method>(xa, ya))
      // xa is assumed to be sorted. If otherwise, the behaviour is undefined.
      { }
    };

    template <typename T>
    void swap(SplineInterpolator<T>& a, SplineInterpolator<T>& b)
    {
      a.swap(b);
    }

    template <typename T>
    void swap(LowLevelInterpolator<T>& a, LowLevelInterpolator<T>& b)
    {
      a.swap(b);
    }



}
#endif //INTERPOLATION1D_GSL_WRAPPERS_HPP
