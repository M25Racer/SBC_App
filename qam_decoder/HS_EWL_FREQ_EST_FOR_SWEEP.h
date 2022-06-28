//
// File: HS_EWL_FREQ_EST_FOR_SWEEP.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef HS_EWL_FREQ_EST_FOR_SWEEP_H
#define HS_EWL_FREQ_EST_FOR_SWEEP_H

// Include Files
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>
#include "RaisedCosineReceiveFilter.h"

// Function Declarations
extern void HS_EWL_FREQ_EST_FOR_SWEEP(coder::comm::RaisedCosineReceiveFilter *b_rxFilter1, boolean_T *b_rxFilter1_not_empty,
                                      emxArray_real_T *s2, double len, double Fs,
  double f0, double per_numb, double sps, double *f_opt, double *ph_opt, double *
  sweep_freq_warning_status);

#endif

//
// File trailer for HS_EWL_FREQ_EST_FOR_SWEEP.h
//
// [EOF]
//
