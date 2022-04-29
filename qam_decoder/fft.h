//
// File: fft.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//
#ifndef FFT_H
#define FFT_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;
struct emxArray_creal_T;

// Function Declarations
namespace coder
{
  void fft(const emxArray_real_T *x, double varargin_1, emxArray_creal_T *y);
  void fft(const double x[1820], double varargin_1, emxArray_creal_T *y);
  void fft(const emxArray_real_T *x, emxArray_creal_T *y);
}

#endif

//
// File trailer for fft.h
//
// [EOF]
//
