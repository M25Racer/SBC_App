//
// File: HS_EWL_FREQ_ACQ.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//
#ifndef HS_EWL_FREQ_ACQ_H
#define HS_EWL_FREQ_ACQ_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;

// Function Declarations
void HS_EWL_FREQ_ACQ(const emxArray_real_T *data, double len, double Fs, double
                     f_opt, double sps, double mode, double Pl, double msg_len,
                     double M, double pre_qam, double warningStatusLocal, double
                     *index_data, double *len_data, double f_est_data[], int
                     f_est_size[1], double *warningStatus);
void HS_EWL_FREQ_ACQ_free();
void HS_EWL_FREQ_ACQ_init();
void lagrange_resamp(emxArray_real_T *s, double p, double q, emxArray_real_T
                     *resamp_data);

#endif

//
// File trailer for HS_EWL_FREQ_ACQ.h
//
// [EOF]
//
