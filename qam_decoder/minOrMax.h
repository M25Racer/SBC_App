//
// File: minOrMax.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//
#ifndef MINORMAX_H
#define MINORMAX_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;

// Function Declarations
namespace coder
{
  namespace internal
  {
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
