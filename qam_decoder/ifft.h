//
// File: ifft.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//
#ifndef IFFT_H
#define IFFT_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  void b_ifft(const creal_T x[57820], creal_T y[57820]);
  void ifft(const creal_T x[131072], creal_T y[131072]);
}

#endif

//
// File trailer for ifft.h
//
// [EOF]
//
