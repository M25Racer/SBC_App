//
// File: HS_EWL_RECEIVE_emxutil.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
//
#ifndef HS_EWL_RECEIVE_EMXUTIL_H
#define HS_EWL_RECEIVE_EMXUTIL_H

// Include Files
#include "FIRDecimator.h"
#include "HS_EWL_RECEIVE_types.h"
#include "RaisedCosineReceiveFilter.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int
  oldNumel);
extern void emxEnsureCapacity_creal_T(emxArray_creal_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);
extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);
extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);
extern void emxFree_creal_T(emxArray_creal_T **pEmxArray);
extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);
extern void emxFree_real_T(emxArray_real_T **pEmxArray);
extern void emxInitStruct_FIRDecimator(coder::dspcodegen::FIRDecimator *pStruct);
extern void emxInitStruct_RaisedCosineReceiveFilter(coder::comm::
  RaisedCosineReceiveFilter *pStruct);
extern void emxInitStruct_dsp_FIRDecimator_0(dsp_FIRDecimator_0 *pStruct);
extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray, int numDimensions);
extern void emxInit_creal_T(emxArray_creal_T **pEmxArray, int numDimensions);
extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);
extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif

//
// File trailer for HS_EWL_RECEIVE_emxutil.h
//
// [EOF]
//
