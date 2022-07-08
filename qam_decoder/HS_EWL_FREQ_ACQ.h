//
// File: HS_EWL_FREQ_ACQ.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//
#ifndef HS_EWL_FREQ_ACQ_H
#define HS_EWL_FREQ_ACQ_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_FREQ_ACQ(const double data[14000], double len, double Fs,
  double f_opt, double sps, double mode, double Pl, double msg_len, double M,
  double pre_qam, double *index_data, double *len_data, double *f_est, double
  *warningStatus);
void HS_EWL_FREQ_ACQ_free();
void HS_EWL_FREQ_ACQ_init();
double absolute_min(double idx, const double FF[50]);

#endif

//
// File trailer for HS_EWL_FREQ_ACQ.h
//
// [EOF]
//
