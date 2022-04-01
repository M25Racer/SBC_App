//
// File: HS_EWL_FREQ_ACQ.h
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

#ifndef HS_EWL_FREQ_ACQ_H
#define HS_EWL_FREQ_ACQ_H

// Include Files
#include "HS_EWL_FREQ_ACQ_types.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void HS_EWL_FREQ_ACQ(const emxArray_real_T *data, double len, double Fs,
                            double f_opt, double mode, double Pl,
                            double msg_len, double M, double pre_qam,
                            double f_est_data[], int f_est_size[1]);

void HS_EWL_FREQ_ACQ_free();

void HS_EWL_FREQ_ACQ_init();

#endif
//
// File trailer for HS_EWL_FREQ_ACQ.h
//
// [EOF]
//
