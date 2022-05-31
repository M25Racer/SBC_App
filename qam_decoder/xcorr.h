//
// File: xcorr.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef XCORR_H
#define XCORR_H

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
  void xcorr(const emxArray_real_T *x, const emxArray_real_T *varargin_1,
             emxArray_real_T *c);//, emxArray_real_T *lags);
  void xcorr(const emxArray_real_T *x, const double varargin_1[1820],
             emxArray_real_T *c);//, emxArray_real_T *lags);
}

#endif

//
// File trailer for xcorr.h
//
// [EOF]
//
