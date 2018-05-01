//
// Created by Panagiotis Zestanakis on 08/04/18.
//

#include "linear_least_quares.hpp"
#include <memory>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_vector.h>

GSL_Wrappers::WorkspaceUniquePtr GSL_Wrappers::makeWorkspaceUniquePtr (const GSL_Wrappers::MatrixSize &ms)
{
  return GSL_Wrappers::WorkspaceUniquePtr(gsl_multifit_linear_alloc(ms.noOfRows,ms.noOfColumns),gsl_multifit_linear_free);
}


GSL_Wrappers::MultifitLinear::MultifitLinear (const GSL_Wrappers::Matrix& predictorMatrix, const GSL_Wrappers::Vector& observations)
:workspace_{makeWorkspaceUniquePtr(predictorMatrix.size())},
 covariance_(predictorMatrix.size().noOfColumns,predictorMatrix.size().noOfColumns),
 params_(predictorMatrix.size().noOfColumns)

{

  auto numberOfParameters= predictorMatrix.size().noOfColumns;
  auto numberOfObservations = predictorMatrix.size().noOfRows;

  if (observations.size()!=numberOfObservations)
    throw std::length_error("GSL_Wrappers::MultifitLinear: Incompatible sizes of predictorMatrix and observations");

  if (numberOfObservations<numberOfParameters)
    throw std::length_error("GSL_Wrappers::MultifitLinear: Not enough observations to fit the model parameters");

  gsl_multifit_linear(*predictorMatrix,*observations,*params_,*covariance_,&chisq_,workspace_.get());

  rhosq_= 1-chisq_/gsl_stats_tss((*observations)->data,(*observations)->stride,observations.size());

}
const GSL_Wrappers::Matrix& GSL_Wrappers::MultifitLinear::covariance () const
{
  return covariance_;
}
const GSL_Wrappers::Vector& GSL_Wrappers::MultifitLinear::params () const
{
  return params_;
}
double GSL_Wrappers::MultifitLinear::chisq () const
{
  return chisq_;
}
double GSL_Wrappers::MultifitLinear::rhosq () const
{
  return rhosq_;
}
