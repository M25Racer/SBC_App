//
// File: interp1.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//
#ifndef INTERP1_H
#define INTERP1_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include "stdint.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  void interp1(const double varargin_1[14001], const double varargin_2[14001],
               const double *varargin_3, double *Vq, uint16_t size);
  void interp1SplineOrPCHIP(const double *y,double *freq, double *abs_freq, double *yi,uint16_t size);
  void interp1SplineOrPCHIP_1(const double *y, const double xi[57820], double
    yi[57820]);
}

#endif

//
// File trailer for interp1.h
//
// [EOF]
//
