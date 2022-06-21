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
#include <qtconcurrentrun.h>

static void func_parallel(int start_k, int stop_k, const emxArray_real_T *x, emxArray_real_T *y)
{
    int iLeft;
    int b_k;
    int vlen;
    double b_y;

    for (int k = start_k; k <= stop_k; k++) {
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

      // todo do we need to protect 'y' with mutex?
      y->data[k] = b_y / static_cast<double>(vlen);
    }
}

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
    int i;
    int loop_ub;

    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = x->size[1];
    emxEnsureCapacity_real_T(y, i);
    loop_ub = x->size[1];
    for (i = 0; i < loop_ub; i++) {
      y->data[i] = 0.0;
    }

    loop_ub = nx - 1;

    QFuture<void> f1 = QtConcurrent::run(func_parallel, 0, loop_ub/2, x, y);
    QFuture<void> f2 = QtConcurrent::run(func_parallel, loop_ub/2, loop_ub, x, y);
    f1.waitForFinished();
    f2.waitForFinished();
  }
}

//
// File trailer for movSumProdOrMean.cpp
//
// [EOF]
//
