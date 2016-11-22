/*
 * Interpolant1D_test.cpp
 *
 *  Created on: Oct 4, 2016
 *      Author: panosz
 */

#include "gnuplot_i.hpp"
#include "Interpolant1D.h"
#include <vector>
#include <iostream>
#include <exception>
#include <cmath>
#include <memory>

using namespace Panos_1DInterp;



int main()
{
	std::vector<double> x{};
	std::vector<double> y{};


	auto h = [](double x){return cos(.5*x);};
	auto hd =[](double x){return -.5*sin(.5*x);};
	auto hdd = [](double x){return -.25*cos(.5*x);};


	for (auto i=0; i<100;++i)
	{
		x.push_back(i/10.0);
		y.push_back(h(i/10.0));
	}

	

	std::unique_ptr<Interpolant1D> y_calc{};
	try{
	y_calc=make_Interpolant1D(x,y,"cspline");
	} catch(std::exception & e)
	  {
	    std::cerr<<e.what()<<std::endl;
	    return -1;
	  }
	std::cout<<"Interpolation method is "<<y_calc->method_name()<<"\n";

	std::vector<double> xi{};	
	for (auto i=0;i<1010;++i)
	  xi.push_back(i/100.0);
	
	std::vector<double> yi{};
	std::vector<double> yi_d{};
	std::vector<double> yi_dd{};
	std::vector<double> yi_integral{};

	for (auto u:xi)
	{
	  yi.push_back(y_calc->eval(u));
	  yi_d.push_back(y_calc->eval_deriv(u));
	  yi_dd.push_back(y_calc->eval_deriv2(u));
	  yi_integral.push_back(y_calc->eval_integral(0,u));
	  
	  //  std::cout<< "xi " << u<< " y " <<yi.back()<<" vs "<< h(u)<<"\n";
	}
        Gnuplot g1;

	g1.set_title(y_calc->method_name().c_str());

        g1.set_style("points").plot_xy(x,y,"user-defined doubles");

        //g1.reset_plot();
        //cout << endl << endl << "*** user-defined lists of points (x,y)" << endl;
        g1.set_grid();
        g1.set_style("lines").plot_xy(xi,yi,"interpolating line 2d");
	g1.plot_xy(xi,yi_d,"interpolating derivative 2d");
	g1.plot_xy(xi,yi_dd,"interpolating second derivative 2d");
	g1.plot_xy(xi,yi_integral, "interpolating integral 2d");

	return 0;

}


