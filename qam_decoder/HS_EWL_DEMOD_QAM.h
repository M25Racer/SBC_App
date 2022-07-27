//
// File: HS_EWL_DEMOD_QAM.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//
#ifndef HS_EWL_DEMOD_QAM_H
#define HS_EWL_DEMOD_QAM_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern int HS_EWL_DEMOD_QAM(const double data[14040], double len_data, double
  f_est, double Fs, double qam_symbols_real[275], double qam_symbols_imag[275],
  double byte_data[212], double *start_inf_data);
void HS_EWL_DEMOD_QAM_free();
void HS_EWL_DEMOD_QAM_init();

#endif

//
// File trailer for HS_EWL_DEMOD_QAM.h
//
// [EOF]
//
