//
// File: mean.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "mean.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
namespace coder
{
  double mean(const emxArray_real_T *x)
  {
    double y;
    int vlen;
    vlen = x->size[1];
    if (x->size[1] == 0) {
      y = 0.0;
    } else {
      y = x->data[0];
      for (int k = 2; k <= vlen; k++) {
        y += x->data[k - 1];
      }
    }

    y /= static_cast<double>(x->size[1]);
    return y;
  }
}

//
// File trailer for mean.cpp
//
// [EOF]
//
