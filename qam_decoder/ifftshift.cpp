//
// File: ifftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "ifftshift.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "eml_fftshift.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : emxArray_creal_T *x
// Return Type  : void
//
namespace coder
{
  void ifftshift(emxArray_creal_T *x)
  {
    double xtmp_im;
    double xtmp_re;
    int a;
    int i;
    int i1;
    int i2;
    int ia;
    int ib;
    int ic;
    int j;
    int k;
    int npages;
    int vlend2;
    a = x->size[0];
    if (x->size[0] > 1) {
      vlend2 = x->size[0] / 2 - 1;
      if ((vlend2 + 1) << 1 == x->size[0]) {
        eml_fftshift(x, 1);
      } else {
        npages = 1;
        for (k = 2; k < 3; k++) {
          npages *= x->size[1];
        }

        i2 = -1;
        for (i = 0; i < npages; i++) {
          i1 = i2 + 1;
          i2 = (i2 + a) - 1;
          for (j = 0; j < 1; j++) {
            i1++;
            i2++;
            ia = i1 + vlend2;
            ib = i2;
            xtmp_re = x->data[i2].re;
            xtmp_im = x->data[i2].im;
            for (k = 0; k <= vlend2; k++) {
              ia--;
              ic = ib;
              ib--;
              x->data[ic] = x->data[ia];
              x->data[ia] = x->data[ib];
            }

            x->data[ib].re = xtmp_re;
            x->data[ib].im = xtmp_im;
          }
        }
      }
    }

    a = x->size[1];
    if (x->size[1] > 1) {
      vlend2 = x->size[1] / 2 - 1;
      if ((vlend2 + 1) << 1 == x->size[1]) {
        eml_fftshift(x, 2);
      } else {
        i2 = -1;
        for (i = 0; i < 1; i++) {
          i1 = i2 + 1;
          i2 = (i2 + a) - 1;
          for (j = 0; j < 1; j++) {
            i1++;
            i2++;
            ia = i1 + vlend2;
            ib = i2;
            xtmp_re = x->data[i2].re;
            xtmp_im = x->data[i2].im;
            for (k = 0; k <= vlend2; k++) {
              ia--;
              ic = ib;
              ib--;
              x->data[ic] = x->data[ia];
              x->data[ia] = x->data[ib];
            }

            x->data[ib].re = xtmp_re;
            x->data[ib].im = xtmp_im;
          }
        }
      }
    }
  }
}

//
// File trailer for ifftshift.cpp
//
// [EOF]
//
