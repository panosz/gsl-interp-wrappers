//
// Created by Panagiotis Zestanakis on 16/05/18.
//
#include "minimization.hpp"

using namespace GSL_Wrappers;

GslMultFdfMinUniquePtr makeGslMultFdfMinUniquePtr (const gsl_multimin_fdfminimizer_type *T, size_t n)
{

  return GSL_Wrappers::GslMultFdfMinUniquePtr(gsl_multimin_fdfminimizer_alloc(T,n),gsl_multimin_fdfminimizer_free);
}
