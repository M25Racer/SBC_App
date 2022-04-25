//
// File: HS_EWL_RECEIVE.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//
#ifndef HS_EWL_RECEIVE_H
#define HS_EWL_RECEIVE_H

// Include Files
#include "HS_EWL_RECEIVE_types.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_RECEIVE(emxArray_real_T *data, double len, double Fs, double
  f_opt, double sps, double mode, double Pl, double msg_len, double M, double
  pre_qam, creal_T qam_symbols_data[], int qam_symbols_size[1], double
  byte_data_data[], int byte_data_size[1], double f_est_data[], int f_est_size[1],
  double *warning_satus);

#endif

//
// File trailer for HS_EWL_RECEIVE.h
//
// [EOF]
//
