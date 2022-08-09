//
// File: movSumProdOrMean.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "movSumProdOrMean.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
//                int nx
//                emxArray_real_T *y
// Return Type  : void
//
namespace coder
{
  void vmovfun(const emxArray_real_T *x, int nx, emxArray_real_T *y)
  {
    double b_y;
    int b_k;
    int i;
    int iLeft;
    int loop_ub;
    int vlen;
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = x->size[1];
    emxEnsureCapacity_real_T(y, i);
    loop_ub = x->size[1];
    for (i = 0; i < loop_ub; i++) {
      y->data[i] = 0.0;
    }

    loop_ub = nx - 1;


    for (int k = 0; k <= loop_ub; k++) {
      if (k + 1 <= 25) {
        iLeft = 0;
      } else {
        iLeft = k - 25;
      }

      if (k + 25 > x->size[1]) {
        b_k = x->size[1];
      } else {
        b_k = k + 25;
      }

      vlen = b_k - iLeft;
      if ((x->size[1] == 0) || (vlen == 0)) {
        b_y = 0.0;
      } else {
        b_y = x->data[iLeft];
        for (b_k = 2; b_k <= vlen; b_k++) {
          b_y += x->data[(iLeft + b_k) - 1];
        }
      }

      y->data[k] = b_y / static_cast<double>(vlen);
    }
  }
}

//
// File trailer for movSumProdOrMean.cpp
//
// [EOF]
//
