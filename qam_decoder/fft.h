//
// File: fft.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 19-Apr-2022 11:19:13
//
#ifndef FFT_H
#define FFT_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;
struct emxArray_creal_T;

// Function Declarations
namespace coder
{
  void fft(const emxArray_real_T *x, double varargin_1, emxArray_creal_T *y);
}

#endif

//
// File trailer for fft.h
//
// [EOF]
//
