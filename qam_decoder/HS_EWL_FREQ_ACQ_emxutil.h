//
// File: HS_EWL_FREQ_ACQ_emxutil.h
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

#ifndef HS_EWL_FREQ_ACQ_EMXUTIL_H
#define HS_EWL_FREQ_ACQ_EMXUTIL_H

// Include Files
#include "FIRDecimator.h"
#include "HS_EWL_FREQ_ACQ_types.h"
#include "RaisedCosineReceiveFilter.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
extern void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray,
                                        int oldNumel);

extern void emxEnsureCapacity_creal_T(emxArray_creal_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel);

extern void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel);

extern void
emxFreeStruct_FIRDecimator(coder::dspcodegen::FIRDecimator *pStruct);

extern void emxFreeStruct_RaisedCosineReceiveFilter(
    coder::comm::RaisedCosineReceiveFilter *pStruct);

extern void emxFreeStruct_dsp_FIRDecimator_0(dsp_FIRDecimator_0 *pStruct);

extern void emxFree_boolean_T(emxArray_boolean_T **pEmxArray);

extern void emxFree_creal_T(emxArray_creal_T **pEmxArray);

extern void emxFree_int32_T(emxArray_int32_T **pEmxArray);

extern void emxFree_real_T(emxArray_real_T **pEmxArray);

extern void
emxInitStruct_FIRDecimator(coder::dspcodegen::FIRDecimator *pStruct);

extern void emxInitStruct_RaisedCosineReceiveFilter(
    coder::comm::RaisedCosineReceiveFilter *pStruct);

extern void emxInitStruct_dsp_FIRDecimator_0(dsp_FIRDecimator_0 *pStruct);

extern void emxInit_boolean_T(emxArray_boolean_T **pEmxArray);

extern void emxInit_creal_T(emxArray_creal_T **pEmxArray, int numDimensions);

extern void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions);

extern void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions);

#endif
//
// File trailer for HS_EWL_FREQ_ACQ_emxutil.h
//
// [EOF]
//
