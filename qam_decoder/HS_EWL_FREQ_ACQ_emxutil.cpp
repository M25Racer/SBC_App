//
// File: HS_EWL_FREQ_ACQ_emxutil.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "HS_EWL_FREQ_ACQ_emxutil.h"
#include "FIRDecimator.h"
#include "HS_EWL_FREQ_ACQ_types.h"
#include "RaisedCosineReceiveFilter.h"
#include "rt_nonfinite.h"
#include <algorithm>
#include <cstdlib>

// Function Definitions
//
// Arguments    : emxArray_boolean_T *emxArray
//                int oldNumel
// Return Type  : void
//
void emxEnsureCapacity_boolean_T(emxArray_boolean_T *emxArray, int oldNumel)
{
  int i;
  int newNumel;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }
  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }
    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }
    newData = std::calloc(static_cast<unsigned int>(i), sizeof(boolean_T));
    if (emxArray->data != nullptr) {
      std::copy(emxArray->data,
                emxArray->data + static_cast<unsigned int>(oldNumel),
                (boolean_T *)newData);
      if (emxArray->canFreeData) {
        std::free(emxArray->data);
      }
    }
    emxArray->data = (boolean_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

//
// Arguments    : emxArray_creal_T *emxArray
//                int oldNumel
// Return Type  : void
//
void emxEnsureCapacity_creal_T(emxArray_creal_T *emxArray, int oldNumel)
{
  int i;
  int newNumel;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }
  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }
    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }
    newData = std::calloc(static_cast<unsigned int>(i), sizeof(creal_T));
    if (emxArray->data != nullptr) {
      std::copy(emxArray->data,
                emxArray->data + static_cast<unsigned int>(oldNumel),
                (creal_T *)newData);
      if (emxArray->canFreeData) {
        std::free(emxArray->data);
      }
    }
    emxArray->data = (creal_T *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

//
// Arguments    : emxArray_int32_T *emxArray
//                int oldNumel
// Return Type  : void
//
void emxEnsureCapacity_int32_T(emxArray_int32_T *emxArray, int oldNumel)
{
  int i;
  int newNumel;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }
  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }
    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }
    newData = std::calloc(static_cast<unsigned int>(i), sizeof(int));
    if (emxArray->data != nullptr) {
      std::copy(emxArray->data,
                emxArray->data + static_cast<unsigned int>(oldNumel),
                (int *)newData);
      if (emxArray->canFreeData) {
        std::free(emxArray->data);
      }
    }
    emxArray->data = (int *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

//
// Arguments    : emxArray_real_T *emxArray
//                int oldNumel
// Return Type  : void
//
void emxEnsureCapacity_real_T(emxArray_real_T *emxArray, int oldNumel)
{
  int i;
  int newNumel;
  void *newData;
  if (oldNumel < 0) {
    oldNumel = 0;
  }
  newNumel = 1;
  for (i = 0; i < emxArray->numDimensions; i++) {
    newNumel *= emxArray->size[i];
  }
  if (newNumel > emxArray->allocatedSize) {
    i = emxArray->allocatedSize;
    if (i < 16) {
      i = 16;
    }
    while (i < newNumel) {
      if (i > 1073741823) {
        i = MAX_int32_T;
      } else {
        i *= 2;
      }
    }
    newData = std::calloc(static_cast<unsigned int>(i), sizeof(double));
    if (emxArray->data != nullptr) {
      std::copy(emxArray->data,
                emxArray->data + static_cast<unsigned int>(oldNumel),
                (double *)newData);
      if (emxArray->canFreeData) {
        std::free(emxArray->data);
      }
    }
    emxArray->data = (double *)newData;
    emxArray->allocatedSize = i;
    emxArray->canFreeData = true;
  }
}

//
// Arguments    : coder::dspcodegen::FIRDecimator *pStruct
// Return Type  : void
//
void emxFreeStruct_FIRDecimator(coder::dspcodegen::FIRDecimator *pStruct)
{
  emxFreeStruct_dsp_FIRDecimator_0(&pStruct->cSFunObject);
}

//
// Arguments    : coder::comm::RaisedCosineReceiveFilter *pStruct
// Return Type  : void
//
void emxFreeStruct_RaisedCosineReceiveFilter(
    coder::comm::RaisedCosineReceiveFilter *pStruct)
{
  emxFreeStruct_FIRDecimator(&pStruct->_pobj0);
}

//
// Arguments    : dsp_FIRDecimator_0 *pStruct
// Return Type  : void
//
void emxFreeStruct_dsp_FIRDecimator_0(dsp_FIRDecimator_0 *pStruct)
{
  emxFree_creal_T(&pStruct->O0_Y0);
}

//
// Arguments    : emxArray_boolean_T **pEmxArray
// Return Type  : void
//
void emxFree_boolean_T(emxArray_boolean_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_boolean_T *)nullptr) {
    if (((*pEmxArray)->data != (boolean_T *)nullptr) &&
        (*pEmxArray)->canFreeData) {
      std::free((*pEmxArray)->data);
    }
    std::free((*pEmxArray)->size);
    std::free(*pEmxArray);
    *pEmxArray = (emxArray_boolean_T *)nullptr;
  }
}

//
// Arguments    : emxArray_creal_T **pEmxArray
// Return Type  : void
//
void emxFree_creal_T(emxArray_creal_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_creal_T *)nullptr) {
    if (((*pEmxArray)->data != (creal_T *)nullptr) &&
        (*pEmxArray)->canFreeData) {
      std::free((*pEmxArray)->data);
    }
    std::free((*pEmxArray)->size);
    std::free(*pEmxArray);
    *pEmxArray = (emxArray_creal_T *)nullptr;
  }
}

//
// Arguments    : emxArray_int32_T **pEmxArray
// Return Type  : void
//
void emxFree_int32_T(emxArray_int32_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_int32_T *)nullptr) {
    if (((*pEmxArray)->data != (int *)nullptr) && (*pEmxArray)->canFreeData) {
      std::free((*pEmxArray)->data);
    }
    std::free((*pEmxArray)->size);
    std::free(*pEmxArray);
    *pEmxArray = (emxArray_int32_T *)nullptr;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
// Return Type  : void
//
void emxFree_real_T(emxArray_real_T **pEmxArray)
{
  if (*pEmxArray != (emxArray_real_T *)nullptr) {
    if (((*pEmxArray)->data != (double *)nullptr) &&
        (*pEmxArray)->canFreeData) {
      std::free((*pEmxArray)->data);
    }
    std::free((*pEmxArray)->size);
    std::free(*pEmxArray);
    *pEmxArray = (emxArray_real_T *)nullptr;
  }
}

//
// Arguments    : coder::dspcodegen::FIRDecimator *pStruct
// Return Type  : void
//
void emxInitStruct_FIRDecimator(coder::dspcodegen::FIRDecimator *pStruct)
{
  emxInitStruct_dsp_FIRDecimator_0(&pStruct->cSFunObject);
}

//
// Arguments    : coder::comm::RaisedCosineReceiveFilter *pStruct
// Return Type  : void
//
void emxInitStruct_RaisedCosineReceiveFilter(
    coder::comm::RaisedCosineReceiveFilter *pStruct)
{
  emxInitStruct_FIRDecimator(&pStruct->_pobj0);
}

//
// Arguments    : dsp_FIRDecimator_0 *pStruct
// Return Type  : void
//
void emxInitStruct_dsp_FIRDecimator_0(dsp_FIRDecimator_0 *pStruct)
{
  emxInit_creal_T(&pStruct->O0_Y0, 2);
}

//
// Arguments    : emxArray_boolean_T **pEmxArray
// Return Type  : void
//
void emxInit_boolean_T(emxArray_boolean_T **pEmxArray)
{
  emxArray_boolean_T *emxArray;
  *pEmxArray = (emxArray_boolean_T *)std::malloc(sizeof(emxArray_boolean_T));
  emxArray = *pEmxArray;
  emxArray->data = (boolean_T *)nullptr;
  emxArray->numDimensions = 2;
  emxArray->size = (int *)std::malloc(sizeof(int) * 2U);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (int i{0}; i < 2; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_creal_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_creal_T(emxArray_creal_T **pEmxArray, int numDimensions)
{
  emxArray_creal_T *emxArray;
  *pEmxArray = (emxArray_creal_T *)std::malloc(sizeof(emxArray_creal_T));
  emxArray = *pEmxArray;
  emxArray->data = (creal_T *)nullptr;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)std::malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (int i{0}; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_int32_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_int32_T(emxArray_int32_T **pEmxArray, int numDimensions)
{
  emxArray_int32_T *emxArray;
  *pEmxArray = (emxArray_int32_T *)std::malloc(sizeof(emxArray_int32_T));
  emxArray = *pEmxArray;
  emxArray->data = (int *)nullptr;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)std::malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (int i{0}; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// Arguments    : emxArray_real_T **pEmxArray
//                int numDimensions
// Return Type  : void
//
void emxInit_real_T(emxArray_real_T **pEmxArray, int numDimensions)
{
  emxArray_real_T *emxArray;
  *pEmxArray = (emxArray_real_T *)std::malloc(sizeof(emxArray_real_T));
  emxArray = *pEmxArray;
  emxArray->data = (double *)nullptr;
  emxArray->numDimensions = numDimensions;
  emxArray->size = (int *)std::malloc(sizeof(int) * numDimensions);
  emxArray->allocatedSize = 0;
  emxArray->canFreeData = true;
  for (int i{0}; i < numDimensions; i++) {
    emxArray->size[i] = 0;
  }
}

//
// File trailer for HS_EWL_FREQ_ACQ_emxutil.cpp
//
// [EOF]
//
