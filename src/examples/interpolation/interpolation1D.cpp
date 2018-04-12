//
// Created by Panagiotis Zestanakis on 11/04/18.
//

#include <iostream>
#include "gsl_wrappers.hpp"
#include <algorithm>
#include <numeric>
#include <gnuplot_i.hpp>



int main ()
{
  using  GSL_Wrappers::SplineInterpolator;

  std::array<double,10> xa{0,1,2,3,4,5,6,7,8,9};
  std::array<double,10> ya{};
  std::transform(xa.begin(),xa.end(),ya.begin(),[](double x){return x+cos(x);});


  auto interpolator = SplineInterpolator<xa.size(),GSL_Wrappers::CSplineInterpolation>(xa,ya);


  std::vector<double > x(100);
  std::generate(x.begin(),x.end(),[n=0]()mutable{return(9.0*(++n)/100);});

  std::vector<double> y;
  std::transform(x.begin(),x.end(),std::back_inserter(y),[&interpolator](double xit){return interpolator.eval(xit).result;});


  Gnuplot g1;
  g1.set_style("points").plot_xy(xa,ya,"samples");
  g1.set_style("lines").plot_xy(x,y,interpolator.name());

  int a;
  std::cin>>a;
//
  return 0;


  return 0;


}