//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#ifndef GSL_WRAPPERS_LINEAR_LEAST_QUARES_HPP
#define GSL_WRAPPERS_LINEAR_LEAST_QUARES_HPP
#include "matrix.hpp"
#include <gsl/gsl_multifit.h>
#include <memory>
#include <mutex>

namespace GSL_Wrappers
{


    using WorkspaceUniquePtr = std::unique_ptr<gsl_multifit_linear_workspace, void(*)(gsl_multifit_linear_workspace*)>;

    WorkspaceUniquePtr makeWorkspaceUniquePtr(const MatrixSize &);

    class MultifitLinearBase
    {};

    class MultifitLinear: public MultifitLinearBase
    {
     private:
      WorkspaceUniquePtr workspace_;
      Matrix covariance_;
      Vector params_;
      double chisq_;

      mutable std::mutex m_;
      mutable double rhosqCached_;
      mutable bool rhosqValid_;

     public:
      MultifitLinear(const Matrix & predictorMatrix, const Vector& observations);
      const Matrix & covariance() const;
      const Vector & params() const;


    };
};
#endif //GSL_WRAPPERS_LINEAR_LEAST_QUARES_HPP
