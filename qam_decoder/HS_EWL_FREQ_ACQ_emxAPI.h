//
// File: HS_EWL_FREQ_ACQ_emxAPI.h
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

#ifndef HS_EWL_FREQ_ACQ_EMXAPI_H
#define HS_EWL_FREQ_ACQ_EMXAPI_H

// Include Files
#include "HS_EWL_FREQ_ACQ_types.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern emxArray_real_T *emxCreateND_real_T(int numDimensions, const int *size);

extern emxArray_real_T *
emxCreateWrapperND_real_T(double *data, int numDimensions, const int *size);

extern emxArray_real_T *emxCreateWrapper_real_T(double *data, int rows,
                                                int cols);

extern emxArray_real_T *emxCreate_real_T(int rows, int cols);

extern void emxDestroyArray_real_T(emxArray_real_T *emxArray);

extern void emxInitArray_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif
//
// File trailer for HS_EWL_FREQ_ACQ_emxAPI.h
//
// [EOF]
//
