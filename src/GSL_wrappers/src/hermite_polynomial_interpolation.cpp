//
// Created by Panagiotis Zestanakis on 04/07/18.
//

#include "hermite_polynomial_interpolation.hpp"

namespace GSL_Wrappers
{

    DividedDifferencePolynomial::DividedDifferencePolynomial (std::vector<double> xa, const std::vector<double>& ya)
        : xa_{std::move(xa)}, dd_(xa_.size())
    {
      const auto size = xa_.size();

      if (size == 0)
        throw std::length_error("xa cannot be empty");

      if (ya.size() != size)
        throw std::length_error("Incompatible sizes of xa and ya");

      gsl_poly_dd_init(dd_.data(), xa_.data(), ya.data(), size);

    }
    DividedDifferencePolynomial::DividedDifferencePolynomial (const std::vector<double>& xa, const std::vector<double>& ya, const std::vector<double>& dya)
        :xa_(2*xa.size()),dd_(2*xa.size())
    {
      const auto size = xa.size();

      if (size == 0)
        throw std::length_error("xa cannot be empty");

      if ((ya.size() != size) || dya.size() != size)
        throw std::length_error("Incompatible sizes of xa, ya and dya");


      gsl_poly_dd_hermite_init(dd_.data(),xa_.data(),xa.data(),ya.data(),dya.data(),size);

    }
}