//
// File: HS_EWL_TR_FUN_EST.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef HS_EWL_TR_FUN_EST_H
#define HS_EWL_TR_FUN_EST_H

// Include Files
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>
#include "RaisedCosineReceiveFilter.h"

// Function Declarations
extern void HS_EWL_TR_FUN_EST(coder::comm::RaisedCosineReceiveFilter *b_rxFilter1,
                              boolean_T *b_rxFilter1_not_empty,
                              const emxArray_real_T *sweep_data, const
  emxArray_real_T *sweep_math, double Fs, double f_opt, double f_sine, double
  PreSPP, double gain_data[], int gain_size[2], double phase_data[], int
  phase_size[2], double *shift_for_qam_data, double *sweep_warning_status);

#endif

//
// File trailer for HS_EWL_TR_FUN_EST.h
//
// [EOF]
//
