//
// File: abs.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 14-Apr-2022 16:12:53
//

// Include Files
#include "abs.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x_data[]
//                const int x_size[1]
//                double y_data[]
//                int y_size[1]
// Return Type  : void
//
namespace coder
{
  void b_abs(const double x_data[], const int x_size[1], double y_data[], int
             y_size[1])
  {
    int nx;
    nx = x_size[0];
    y_size[0] = x_size[0];
    for (int k = 0; k < nx; k++) {
      y_data[k] = std::abs(x_data[k]);
    }
  }
}

//
// File trailer for abs.cpp
//
// [EOF]
//
