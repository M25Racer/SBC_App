//
// File: fft.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//
#ifndef FFT_H
#define FFT_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Function Declarations
namespace coder
{
  void b_fft(const double x[57820], creal_T y[57820]);
  void fft(const double x[56001], creal_T y[56001]);
}

#endif

//
// File trailer for fft.h
//
// [EOF]
//
