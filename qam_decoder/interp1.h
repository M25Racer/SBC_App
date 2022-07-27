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
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  void interp1(const double varargin_1[14001], const double varargin_2[14001],
               const double varargin_3[513], double Vq[513]);
  void interp1SplineOrPCHIP(const double y[1024], double yi[2048]);
  void interp1SplineOrPCHIP(const double y[2048], const double xi[57820], double
    yi[57820]);
}

#endif

//
// File trailer for interp1.h
//
// [EOF]
//
