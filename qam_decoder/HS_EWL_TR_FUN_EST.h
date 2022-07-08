//
// File: HS_EWL_TR_FUN_EST.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//
#ifndef HS_EWL_TR_FUN_EST_H
#define HS_EWL_TR_FUN_EST_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_TR_FUN_EST(const double sweep_data[450000], const double
  sweep_math[57820], double Fs, double f_opt, double f_sine, double PreSPP,
  double gain[2048], double phase[2048], double *shift_for_qam_data, double
  *sweep_warning_status);

#endif

//
// File trailer for HS_EWL_TR_FUN_EST.h
//
// [EOF]
//
