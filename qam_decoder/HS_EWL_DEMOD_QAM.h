//
// File: HS_EWL_DEMOD_QAM.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//
#ifndef HS_EWL_DEMOD_QAM_H
#define HS_EWL_DEMOD_QAM_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_DEMOD_QAM(const double data[14000], double len, double
  index_data, double len_data, double f_est, double Fs, double sps, double
  pre_qam, double M, double *qam_symbols_real, double *qam_symbols_imag, double *
  byte_data);
void HS_EWL_DEMOD_QAM_free();
void HS_EWL_DEMOD_QAM_init();

#endif

//
// File trailer for HS_EWL_DEMOD_QAM.h
//
// [EOF]
//
