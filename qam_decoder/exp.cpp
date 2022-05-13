//
// File: exp.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "exp.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : emxArray_creal_T *x
// Return Type  : void
//
namespace coder
{
  void b_exp(emxArray_creal_T *x)
  {
    int nx;
    nx = x->size[1];
    for (int k = 0; k < nx; k++) {
      if (x->data[k].im == 0.0) {
        x->data[k].re = std::exp(x->data[k].re);
        x->data[k].im = 0.0;
      } else if (rtIsInf(x->data[k].im) && rtIsInf(x->data[k].re) && (x->data[k]
                  .re < 0.0)) {
        x->data[k].re = 0.0;
        x->data[k].im = 0.0;
      } else {
        double d;
        double r;
        r = std::exp(x->data[k].re / 2.0);
        d = x->data[k].im;
        x->data[k].re = r * (r * std::cos(x->data[k].im));
        x->data[k].im = r * (r * std::sin(d));
      }
    }
  }
}

//
// File trailer for exp.cpp
//
// [EOF]
//
