//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_INTERPOLANT1DTEST_HPP
#define GSL_WRAPPERS_INTERPOLANT1DTEST_HPP
#include "gmock/gmock.h"
#include "GSL_wrappers/Interpolant1D.hpp"

using namespace GSL_Wrappers;
using namespace testing;


TEST(InterpolationInstantiation, throwsWhenInitializedWithTooFewSamples)
{
  std::array<double, 2> xa{}, ya{};
  for (auto i = 0; i < xa.size(); ++i)
    {
      xa[i] = i + 0.5 * sin(i);
      ya[i] = i + cos(i * i);
    }

  std::cout << "minimum sample size for Linear " << min_samples<LinearInterpolation>() << '\n';
  std::cout << "minimum sample size for cspline " << min_samples<CSplineInterpolation>() << '\n';
  std::cout << "minimum sample size for cspline periodic " << min_samples<CSplinePeriodicInterpolation>() << '\n';
  std::cout << "minimum sample size for Akima " << min_samples<AkimaInterpolation>() << '\n';

  std::cout << "minimum sample size for Akima periodic " << min_samples<AkimaPeriodicInterpolation>() << '\n';

  std::cout << "minimum sample size for Steffen's " << min_samples<SteffenInterpolation>() << '\n';
  ASSERT_ANY_THROW((SplineInterpolator<SteffenInterpolation>{xa, ya}));
  ASSERT_ANY_THROW((LowLevelInterpolator<AkimaInterpolation>{xa,ya}));
}

TEST(InterpolationEvaluation, OutputIsNaNWhenOutsideTheDomain)
{
  std::array<double, 10> xa{}, ya{};
  for (auto i = 0; i < xa.size(); ++i)
    {
      xa[i] = i + 0.5 * sin(i);
      ya[i] = i + cos(i * i);
    }

  LowLevelInterpolator<LinearInterpolation>linInterp(xa, ya);
  SplineInterpolator<AkimaPeriodicInterpolation> akimaPeriodicInterp(xa, ya);

  auto x = -10;


  std::cout << '\n' << linInterp.name() <<" "<< linInterp.eval(3).result<< '\n';
  std::cout << akimaPeriodicInterp.name() <<" "<< akimaPeriodicInterp.eval(3).result<< '\n';

  //assert linear
  {
    auto reslin = linInterp.eval_deriv(x);

    ASSERT_TRUE(std::isnan(reslin.result));
    ASSERT_EQ(reslin.error, DomainLocate::OUTSIDE_DOMAIN);
  }

  //assert akima periodic
  {
    auto resak = akimaPeriodicInterp.eval(x);
    ASSERT_TRUE(std::isnan(resak.result));
    ASSERT_EQ(resak.error, DomainLocate::OUTSIDE_DOMAIN);
  }

}

TEST (Interpolators, CoppyAndMoveOperations)
{

  std::array<double, 10> xa{}, ya{};
  for (auto i = 0; i < xa.size(); ++i)
    {
      xa[i] = i + 0.5 * sin(i);
      ya[i] = i + cos(i * i);
    }

  LowLevelInterpolator<LinearInterpolation>linInterp(xa, ya);
  SplineInterpolator<CSplineInterpolation> cSplineInterp1(xa, ya);
  auto cSpline2(cSplineInterp1);

  std::cout << '\n' << cSpline2.name() <<" "<< cSpline2.eval(3).result<< '\n';
}

#endif //GSL_WRAPPERS_INTERPOLANT1DTEST_HPP
