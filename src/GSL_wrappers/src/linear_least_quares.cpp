//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#include "linear_least_quares.hpp"
#include <memory>


GSL_Wrappers::WorkspaceUniquePtr GSL_Wrappers::makeWorkspaceUniquePtr (const GSL_Wrappers::MatrixSize &ms)
{
  return GSL_Wrappers::WorkspaceUniquePtr(gsl_multifit_linear_alloc(ms.noOfRows,ms.noOfColumns),gsl_multifit_linear_free);
}


GSL_Wrappers::MultifitLinear::MultifitLinear (const GSL_Wrappers::Matrix& predictorMatrix, const GSL_Wrappers::Vector& observations)
:workspace_{makeWorkspaceUniquePtr(predictorMatrix.size())},
 covariance_{predictorMatrix.size().noOfColumns,predictorMatrix.size().noOfColumns},
 params_{predictorMatrix.size().noOfColumns}

{

  gsl_multifit_linear(*predictorMatrix,*observations,*params_,*covariance_,&chisq_,workspace_.get());
}
const GSL_Wrappers::Matrix& GSL_Wrappers::MultifitLinear::covariance () const
{
  return covariance_;
}
const GSL_Wrappers::Vector& GSL_Wrappers::MultifitLinear::params () const
{
  return params_;
}
