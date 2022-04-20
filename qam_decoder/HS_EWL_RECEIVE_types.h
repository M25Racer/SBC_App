//
// File: HS_EWL_RECEIVE_types.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 19-Apr-2022 11:19:13
//
#ifndef HS_EWL_RECEIVE_TYPES_H
#define HS_EWL_RECEIVE_TYPES_H

// Include Files
#include "rtwtypes.h"

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

struct emxArray_boolean_T
{
  boolean_T *data;
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

#endif

//
// File trailer for HS_EWL_RECEIVE_types.h
//
// [EOF]
//
