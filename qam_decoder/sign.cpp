//
// File: sign.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "sign.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : emxArray_real_T *x
// Return Type  : void
//
namespace coder
{
  void b_sign(emxArray_real_T *x)
  {
    int nx;
    nx = x->size[1];
    for (int k = 0; k < nx; k++) {
      double b_x;
      b_x = x->data[k];
      if (x->data[k] < 0.0) {
        b_x = -1.0;
      } else if (x->data[k] > 0.0) {
        b_x = 1.0;
      } else {
        if (x->data[k] == 0.0) {
          b_x = 0.0;
        }
      }

      x->data[k] = b_x;
    }
  }
}

//
// File trailer for sign.cpp
//
// [EOF]
//
