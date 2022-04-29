//
// File: HS_EWL_FREQ_EST_FOR_SWEEP.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//
#ifndef HS_EWL_FREQ_EST_FOR_SWEEP_H
#define HS_EWL_FREQ_EST_FOR_SWEEP_H

// Include Files
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_FREQ_EST_FOR_SWEEP(emxArray_real_T *s2, double len, double Fs,
  double f0, double per_numb, double sps, double *f_opt, double *ph_opt, double *
  sweep_freq_warning_status);

#endif

//
// File trailer for HS_EWL_FREQ_EST_FOR_SWEEP.h
//
// [EOF]
//
