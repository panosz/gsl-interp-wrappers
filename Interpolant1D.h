
/*
 * Interpolant1D.h
 *
 *  Created on: Oct 4, 2016
 *      Author: panosz
 */

#ifndef INTERPOLANT1D_H_
#define INTERPOLANT1D_H_

#include <map>
#include <vector>
#include <assert.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>
#include <algorithm>
#include <utility>
#include <cmath>
#include <exception>
#include <memory>
#include <string>
#include <algorithm>



namespace Panos_1DInterp {

  class Interpolant1D {
	std::unique_ptr<gsl_spline,void (*)(gsl_spline*)> m_interp;
	std::unique_ptr<gsl_interp_accel,void (*)(gsl_interp_accel*)> m_acc;
	const unsigned int m_min_size;
    //	const std::vector<double> m_x;
    //	const std::vector<double> m_y;
  public:
	Interpolant1D(const std::vector<double>& x, const std::vector<double>& y,
                  const gsl_interp_type *interp_type=gsl_interp_linear):
      m_interp(gsl_spline_alloc(interp_type,x.size()),gsl_spline_free),
      m_acc(gsl_interp_accel_alloc(), gsl_interp_accel_free),
      m_min_size{gsl_interp_type_min_size(interp_type)}
	{
      const auto length = x.size();
      if (length!=y.size())
        throw std::length_error("Interpolant1D::Interpolant1D : x and y length mismatch!\n");
      if (length<m_min_size)
        throw std::length_error
          ("Interpolant1D::Interpolant1D :at least " +std::to_string(m_min_size) +" samples are required!\n");

      gsl_spline_init(m_interp.get(),&x[0],&y[0],length);

	};


				
	const std::string method_name()
	{
	  const char* name =gsl_spline_name(m_interp.get());
	  if(name)
	    return name;

	  throw std::runtime_error("Interpolant1D::method_name(): could not allocate method name string\n");
	    
	};
	
	double eval(double xi)
	{
      double y=0;
      auto err =gsl_spline_eval_e(m_interp.get(),xi, m_acc.get(),&y);
      // if(err)
      //   throw std::runtime_error("Interpolant1D::eval(): could not evaluate interpolant at x=" + std::to_string(xi) );

      return (y==GSL_NAN)? std::nan("") : y;
	};

	double eval_deriv(double xi)
	{
      double y=0;
      auto err =gsl_spline_eval_deriv_e(m_interp.get(),xi, m_acc.get(),&y);
      // if(err)
      //   throw std::runtime_error("Interpolant1D::eval(): could not evaluate interpolant derivative at x=" + std::to_string(xi) );
      return (y==GSL_NAN)? std::nan("") : y;

	};


    double eval_deriv2(double xi)
    {
      double y=0;
      auto err =gsl_spline_eval_deriv2_e(m_interp.get(),xi, m_acc.get(),&y);
      // if(err)
      //   throw std::runtime_error("Interpolant1D::eval(): could not evaluate interpolant second derivative at x=" + std::to_string(xi) );
      return (y==GSL_NAN)? std::nan("") : y;
    };

    double eval_integral(double x0, double x1)
    {
      double y=0;
      auto err = gsl_spline_eval_integ_e(m_interp.get(),x0,x1,m_acc.get(),&y);
      // if(err)
      //   throw std::runtime_error("Interpolant1D::eval(): could not evaluate interpolant integral from x0 = " + std::to_string(x0)+" x1 = " + std::to_string(x1) );
      return (y==GSL_NAN)? std::nan("") : y;


    };

    ~Interpolant1D()=default;
  };


 std::unique_ptr<Interpolant1D> make_Interpolant1D(const std::vector<double>& x,
						   const std::vector<double>& y,
						   std::string method="linear")
   {
     std::transform(method.begin(), method.end(), method.begin(), ::tolower);

     if (method.compare("linear")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_linear);
     if (method.compare("polynomial")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_polynomial);
     if (method.compare("cspline")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_cspline);
     if (method.compare("cspline_periodic")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_cspline_periodic);
     if (method.compare("akima")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_akima);
     if (method.compare("akima_periodic")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_akima_periodic);
     if (method.compare("steffen")==0)
	       return std::make_unique<Interpolant1D>(x,y,gsl_interp_steffen);

     //if the specified method is unknown
     throw std::invalid_argument("make_Interpolant1D: unknown method\n");
     
   };
 
} /* namespace Panos_1DInterp */
#endif /* INTERPOLANT1D_H_ */
