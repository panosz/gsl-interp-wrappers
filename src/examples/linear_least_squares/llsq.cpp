//
// Created by Panagiotis Zestanakis on 11/04/18.
//

#include <iostream>
#include "gsl_wrappers.hpp"
#include <algorithm>
#include <numeric>
#include <gnuplot_i.hpp>
#include <GSL_wrappers/linear_least_quares.hpp>

int main ()
{
  using  GSL_Wrappers::MultifitLinear;
  using GSL_Wrappers::Matrix;
  using GSL_Wrappers::Vector;

  std::array<double,10> xa{0,1,2,3,4,5,6,7,8,9};
  Vector ya(10);

  auto myfun =[](double x){return x+cos(x);};
  std::transform(xa.begin(),xa.end(),ya.begin(),myfun);

  Matrix m(ya.size(),4);

  for (auto i=0;i<ya.size();++i)
    m.row(i)={1,xa[i],xa[i]*xa[i],xa[i]*xa[i]*xa[i]};

std::cout<<m<<std::endl;
  MultifitLinear fit(m,ya);

  auto params = fit.params();
  std::cout<<"parameters:\n"<<params<<'\n';
  std::cout<<"chi_sq = "<< fit.chisq()<<", rho_sq = "<<fit.rhosq()<<'\n';



  std::vector<double > x(100);
  std::generate(x.begin(),x.end(),[n=0]()mutable{return(9.0*(++n)/100);});

  std::vector<double> y;
  std::transform(x.begin(),x.end(),std::back_inserter(y),[&params](double xit)
  {return params(0)+params(1)*xit+params(2)*xit*xit+params(3)*std::pow(xit,3);});


  Gnuplot g1;
  for (auto i=0;i<xa.size();++i)
    std::cout<<ya[i]<<'\n';

  g1.set_style("points").plot_xy(xa,ya,"samples");
  g1.set_style("lines").plot_xy(x,y,"fit");

  int a;
  std::cin>>a;
//
  return 0;




}