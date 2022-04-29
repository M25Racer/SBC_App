//
// File: fftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//

// Include Files
#include "fftshift.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : emxArray_creal_T *x
// Return Type  : void
//
namespace coder
{
  void fftshift(emxArray_creal_T *x)
  {
    double xtmp_im;
    double xtmp_re;
    int a;
    int i;
    int i1;
    int i2;
    int ia;
    int ib;
    int j;
    int k;
    int npages;
    int vlend2;
    a = x->size[0] - 1;
    if (x->size[0] > 1) {
      vlend2 = x->size[0] / 2 - 1;
      npages = 1;
      for (k = 2; k < 3; k++) {
        npages *= x->size[1];
      }

      if ((vlend2 + 1) << 1 == x->size[0]) {
        i2 = 0;
        for (i = 0; i < npages; i++) {
          i1 = i2;
          i2 += a;
          for (j = 0; j < 1; j++) {
            i1++;
            i2++;
            ia = i1 - 1;
            ib = i1 + vlend2;
            for (k = 0; k <= vlend2; k++) {
              xtmp_re = x->data[ia].re;
              xtmp_im = x->data[ia].im;
              x->data[ia] = x->data[ib];
              x->data[ib].re = xtmp_re;
              x->data[ib].im = xtmp_im;
              ia++;
              ib++;
            }
          }
        }
      } else {
        i2 = 0;
        for (i = 0; i < npages; i++) {
          i1 = i2;
          i2 += a;
          for (j = 0; j < 1; j++) {
            i1++;
            i2++;
            ia = i1 - 1;
            ib = i1 + vlend2;
            xtmp_re = x->data[ib].re;
            xtmp_im = x->data[ib].im;
            for (k = 0; k <= vlend2; k++) {
              x->data[ib] = x->data[ia];
              x->data[ia] = x->data[ib + 1];
              ia++;
              ib++;
            }

            x->data[ib].re = xtmp_re;
            x->data[ib].im = xtmp_im;
          }
        }
      }
    }

    a = x->size[1] - 1;
    if (x->size[1] > 1) {
      vlend2 = x->size[1] / 2 - 1;
      if ((vlend2 + 1) << 1 == x->size[1]) {
        i2 = 0;
        for (i = 0; i < 1; i++) {
          i1 = i2;
          i2 += a;
          for (j = 0; j < 1; j++) {
            i1++;
            i2++;
            ia = i1 - 1;
            ib = i1 + vlend2;
            for (k = 0; k <= vlend2; k++) {
              xtmp_re = x->data[ia].re;
              xtmp_im = x->data[ia].im;
              x->data[ia] = x->data[ib];
              x->data[ib].re = xtmp_re;
              x->data[ib].im = xtmp_im;
              ia++;
              ib++;
            }
          }
        }
      } else {
        i2 = 0;
        for (i = 0; i < 1; i++) {
          i1 = i2;
          i2 += a;
          for (j = 0; j < 1; j++) {
            i1++;
            i2++;
            ia = i1 - 1;
            ib = i1 + vlend2;
            xtmp_re = x->data[ib].re;
            xtmp_im = x->data[ib].im;
            for (k = 0; k <= vlend2; k++) {
              x->data[ib] = x->data[ia];
              x->data[ia] = x->data[ib + 1];
              ia++;
              ib++;
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
// File trailer for fftshift.cpp
//
// [EOF]
//
