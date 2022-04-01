//
// File: HS_EWL_FREQ_ACQ_types.h
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

#ifndef HS_EWL_FREQ_ACQ_TYPES_H
#define HS_EWL_FREQ_ACQ_TYPES_H

// Include Files
#include "rtwtypes.h"

// Type Definitions
struct emxArray_creal_T {
  creal_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct dsp_FIRDecimator_0 {
  int S0_isInitialized;
  int W0_PhaseIdx;
  creal_T W1_Sums;
  int W2_CoeffIdx;
  creal_T W3_StatesBuff[80];
  int W4_TapDelayIndex;
  int W5_PrevNumChan;
  creal_T P0_IC;
  double P1_FILT[88];
  emxArray_creal_T *O0_Y0;
};

struct emxArray_real_T {
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct emxArray_boolean_T {
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct emxArray_int32_T {
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif
//
// File trailer for HS_EWL_FREQ_ACQ_types.h
//
// [EOF]
//
