//
// File: qamdemod.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef QAMDEMOD_H
#define QAMDEMOD_H

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
  void qamdemod(const creal_T y_data[], const int y_size[1], double M,
                emxArray_real_T *x);
}

#endif

//
// File trailer for qamdemod.h
//
// [EOF]
//