//
// File: ifft.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef IFFT_H
#define IFFT_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_creal_T;

// Function Declarations
namespace coder
{
  void b_ifft(const emxArray_creal_T *x, emxArray_creal_T *y);
  void ifft(const emxArray_creal_T *x, emxArray_creal_T *y);
}

#endif

//
// File trailer for ifft.h
//
// [EOF]
//
