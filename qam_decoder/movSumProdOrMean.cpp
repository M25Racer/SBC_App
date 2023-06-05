//
// File: movSumProdOrMean.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "movSumProdOrMean.h"
#include "rt_nonfinite.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const double x[14001]
//                int nx
//                double y[14001]
// Return Type  : void
//
namespace coder
{
  void vmovfun(const double x[14001], int nx, double y[14001])
  {
    double b_y;
    int b_k;
    int iLeft;
    int ub_loop;
    int vlen;
    std::memset(&y[0], 0, 14001U * sizeof(double));
    ub_loop = nx - 1;

    for (int k = 0; k <= ub_loop; k++) {
      if (k + 1 <= 25) {
        iLeft = -24;
      } else {
        iLeft = k - 49;
      }

      if (k + 25 > 14001) {
        b_k = 13977;
      } else {
        b_k = k + 1;
      }

      vlen = b_k - iLeft;
      if (vlen == 0) {
        b_y = 0.0;
      } else {
        b_y = x[iLeft + 24];
        for (b_k = 2; b_k <= vlen; b_k++) {
          b_y += x[(iLeft + b_k) + 23];
        }
      }

      y[k] = b_y / static_cast<double>(vlen);
    }
  }
}

//
// File trailer for movSumProdOrMean.cpp
//
// [EOF]
//
