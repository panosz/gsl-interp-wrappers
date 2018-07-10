//
// Created by Panagiotis Zestanakis on 04/07/18.
//

#include <iostream>
#include <fstream>
#include <cmath>
#include "polynomials.hpp"
#include "myUtilities.hpp"
#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

int main ()
{

  const size_t no_of_samples = 10;
  const double xmax =18;
  std::ofstream foutput("output.dat");

  std::vector<double> xa_no_derivs;
  boost::push_back(xa_no_derivs, PanosUtilities::linspace(0, xmax, no_of_samples));

  std::vector<double> ya_no_derivs;
  boost::push_back(ya_no_derivs, xa_no_derivs | boost::adaptors::transformed(sin));

  foutput << "# Input data for simple interpolation (no derivs)\n"
          << "X\tY\n";
  for (int j = 0; j < no_of_samples; ++j)
    {
      foutput << xa_no_derivs[j] << '\t' << ya_no_derivs[j] << '\n';
    }
  foutput << "\n\n";

  std::vector<double> xa_derivs;
  boost::push_back(xa_derivs, PanosUtilities::linspace(0, xmax, no_of_samples / 2));

  std::vector<double> ya_derivs;
  boost::push_back(ya_derivs, xa_derivs | boost::adaptors::transformed(sin));

  std::vector<double> dya_derivs;
  boost::push_back(dya_derivs, xa_derivs | boost::adaptors::transformed(cos));

  foutput << "# Input data for hermite interpolation (with derivatives)\n"
          << "X\tY\tDERIVS\n";
  for (int j = 0; j < no_of_samples / 2; ++j)
    {
      foutput << xa_derivs[j] << '\t' << ya_derivs[j] << '\t' << dya_derivs[j] << '\n';
    }
  foutput << "\n\n";

  GSL_Wrappers::DividedDifferencePolynomial sin_poly(xa_no_derivs, ya_no_derivs);
  GSL_Wrappers::DividedDifferencePolynomial hermite_sin_poly(xa_derivs, ya_derivs, dya_derivs);

  const size_t no_of_enquiries = 1000;
  std::vector<double> xi;
  boost::push_back(xi, PanosUtilities::linspace(0, xmax, no_of_enquiries));

  std::vector<double> yi_simple;
  auto sin_poly_eval_fn = [&sin_poly] (double x)
  { return sin_poly.eval(x); };

  boost::push_back(yi_simple, xi | boost::adaptors::transformed(sin_poly_eval_fn));

  std::vector<double> yi_hermite;
  auto hermite_sin_poly_eval_fn = [&hermite_sin_poly] (double x)
  { return hermite_sin_poly.eval(x); };

  boost::push_back(yi_hermite, xi | boost::adaptors::transformed(hermite_sin_poly_eval_fn));

  foutput << "# Interpolation output (index 2)\n"
          << "XI\tY_NO_DERIVS\tERROR_NO_DERIVS\tY_DERIVS\tERROR_DERIVS\n";
  for (int j = 0; j < no_of_enquiries; ++j)
    {
      foutput << xi[j] << '\t'
              << yi_simple[j] << '\t'
              << yi_simple[j] - sin(xi[j]) << '\t'
              << yi_hermite[j] << '\t'
              << yi_hermite[j] - sin(xi[j]) << '\n';
    }
  foutput << "\n\n";

  foutput.close();

}
