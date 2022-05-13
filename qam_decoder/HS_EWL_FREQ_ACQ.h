//
// File: HS_EWL_FREQ_ACQ.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef HS_EWL_FREQ_ACQ_H
#define HS_EWL_FREQ_ACQ_H

// Include Files
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;

// Function Declarations
extern void HS_EWL_FREQ_ACQ(emxArray_real_T *data, double len, double Fs, double
  f_opt, double sps, double mode, double Pl, double msg_len, double M, double
  pre_qam, double *index_data, double *len_data, double f_est_data[], int
  f_est_size[1], double *warningStatus);
void HS_EWL_FREQ_ACQ_free();
void HS_EWL_FREQ_ACQ_init();
double absolute_min(double idx, const double FF[50]);
double preambule_from(const emxArray_real_T *s, double len);

#endif

//
// File trailer for HS_EWL_FREQ_ACQ.h
//
// [EOF]
//
