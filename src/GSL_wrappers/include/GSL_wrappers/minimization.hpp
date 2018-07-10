//
// Created by Panagiotis Zestanakis on 16/05/18.
//

#ifndef GSL_WRAPPERS_MINIMIZATION_HPP
#define GSL_WRAPPERS_MINIMIZATION_HPP

#include <gsl/gsl_errno.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_multimin.h>
#include "matrix.hpp"

namespace GSL_Wrappers
{
    using GslMultFdfMinUniquePtr = std::unique_ptr<gsl_multimin_fdfminimizer, void (*)(gsl_multimin_fdfminimizer *) >;
    GslMultFdfMinUniquePtr makeGslMultFdfMinUniquePtr (const gsl_multimin_fdfminimizer_type * T, size_t n);

}


#endif //GSL_WRAPPERS_MINIMIZATION_HPP
