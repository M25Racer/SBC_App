//
// File: interp1.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//
#ifndef INTERP1_H
#define INTERP1_H

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
  void interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
               *varargin_2, double Vq[513]);
  void interp1(const double varargin_1_data[], const double varargin_2[1024],
               const double varargin_3_data[], double Vq_data[], int Vq_size[2]);
}

#endif

//
// File trailer for interp1.h
//
// [EOF]
//
