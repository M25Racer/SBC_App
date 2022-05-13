//
// File: HS_EWL_DEMOD_QAM_types.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef HS_EWL_DEMOD_QAM_TYPES_H
#define HS_EWL_DEMOD_QAM_TYPES_H

// Include Files
#include "rtwtypes.h"
#define MAX_THREADS                    omp_get_max_threads()

// Type Definitions
struct emxArray_real_T
{
  double *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct emxArray_creal_T
{
  creal_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct emxArray_int32_T
{
  int *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

struct emxArray_boolean_T
{
  boolean_T *data;
  int *size;
  int allocatedSize;
  int numDimensions;
  boolean_T canFreeData;
};

#endif

//
// File trailer for HS_EWL_DEMOD_QAM_types.h
//
// [EOF]
//
