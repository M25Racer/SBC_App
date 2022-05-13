//
// File: HS_EWL_DEMOD_QAM.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef HS_EWL_DEMOD_QAM_H
#define HS_EWL_DEMOD_QAM_H

// Include Files
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;

// Function Declarations
extern void HS_EWL_DEMOD_QAM(const emxArray_real_T *data, double index_data,
  double len_data, double f_est, double Fs, double sps, double pre_qam, double M,
  creal_T qam_symbols_data[], int qam_symbols_size[1], double byte_data_data[],
  int byte_data_size[1]);
void HS_EWL_DEMOD_QAM_free();
void HS_EWL_DEMOD_QAM_init();
void lagrange_resamp(emxArray_real_T *s, double p, double q, emxArray_real_T
                     *resamp_data);

#endif

//
// File trailer for HS_EWL_DEMOD_QAM.h
//
// [EOF]
//
