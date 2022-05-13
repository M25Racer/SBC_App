//
// File: minOrMax.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef MINORMAX_H
#define MINORMAX_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;

// Function Declarations
namespace coder
{
  namespace internal
  {
    double b_maximum(const emxArray_real_T *x);
    void b_maximum(const emxArray_real_T *x, double *ex, int *idx);
    void maximum(const emxArray_real_T *x, double *ex, int *idx);
    double maximum(const emxArray_real_T *x);
    void minimum(const double x[50], double *ex, int *idx);
  }
}

#endif

//
// File trailer for minOrMax.h
//
// [EOF]
//
