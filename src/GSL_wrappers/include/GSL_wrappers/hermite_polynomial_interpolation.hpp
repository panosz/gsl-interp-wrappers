//
// Created by Panagiotis Zestanakis on 04/07/18.
//

#ifndef GSL_WRAPPERS_POLYNOMIALS_HPP
#define GSL_WRAPPERS_POLYNOMIALS_HPP

#include <vector>
#include <gsl/gsl_poly.h>
#include <stdexcept>
#include <array>

namespace GSL_Wrappers
{

    class Polynomial;

    class DividedDifferencePolynomial {
      friend Polynomial;
     private:
      std::vector<double> xa_;
      std::vector<double> dd_;

     public:
      DividedDifferencePolynomial (std::vector<double> xa, const std::vector<double>& ya);

      DividedDifferencePolynomial (const std::vector<double>& xa, const std::vector<double>& ya, const std::vector<double>& dya);

      inline double eval (double x) const
      {
        return gsl_poly_dd_eval(dd_.data(), xa_.data(), xa_.size(), x);
      }

    };

    class Polynomial {
     private:
      std::vector<double> coefs{};

      const double xp = 0; //Polynomial about point xp.
      ///TODO: Maybe choose a different value for xp, according to expected input, for better accuracy

      std::vector<double> toPolynomial (const DividedDifferencePolynomial& ddpoly) const
      {
        const size_t size = ddpoly.xa_.size();

        std::vector<double> c(size);
        std::vector<double> workspace(size);

        gsl_poly_dd_taylor(c.data(), xp, ddpoly.dd_.data(), ddpoly.xa_.data(), size, workspace.data());
        return c;
      }

     public:
      Polynomial (const DividedDifferencePolynomial& ddp)
          : coefs{toPolynomial(ddp)}
      { };
      Polynomial (std::vector<double> xa, const std::vector<double>& ya)
          : coefs{toPolynomial(DividedDifferencePolynomial(xa, ya))}
      { };
      Polynomial (const std::vector<double>& xa, const std::vector<double>& ya, const std::vector<double>& dya)
          : coefs{toPolynomial(DividedDifferencePolynomial(xa, ya, dya))}
      { };

      inline double eval (double x) const
      {
        return gsl_poly_eval(coefs.data(), static_cast<int>(coefs.size()), x);
      }

      inline double deriv (double x) const
      {
        double deriv_res = 0;
        gsl_poly_eval_derivs(coefs.data(), coefs.size(), x, &deriv_res, 1);
        return deriv_res;
      }

      template<size_t N>
      inline std::array<double, N> eval_derivs (double x) const
      {
        std::array<double,N> deriv_res{};
        gsl_poly_eval_derivs(coefs.data(),coefs.size(),x,deriv_res.data(),deriv_res.size());
        return deriv_res;
      }

    };

}

#endif //GSL_WRAPPERS_POLYNOMIALS_HPP
