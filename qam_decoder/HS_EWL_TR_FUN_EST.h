//
// File: HS_EWL_TR_FUN_EST.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//
#ifndef HS_EWL_TR_FUN_EST_H
#define HS_EWL_TR_FUN_EST_H

// Include Files
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_TR_FUN_EST(const emxArray_real_T *sweep_data, const
  emxArray_real_T *sweep_math, double Fs, double f_opt, double f_sine, double
  PreSPP, double gain_data[], int gain_size[2], double phase_data[], int
  phase_size[2], double *sweep_warning_status);

#endif

//
// File trailer for HS_EWL_TR_FUN_EST.h
//
// [EOF]
//
