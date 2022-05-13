//
// File: ifft.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
namespace coder
{
  void b_ifft(const emxArray_creal_T *x, emxArray_creal_T *y)
  {
    emxArray_creal_T b_x;
    emxArray_creal_T *yCol;
    emxArray_real_T *costab;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    int c_x[1];
    int d_x[1];
    int N2blue;
    int nRows;
    if (x->size[1] == 0) {
      N2blue = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = x->size[1];
      emxEnsureCapacity_creal_T(y, N2blue);
      nRows = x->size[1];
      for (N2blue = 0; N2blue < nRows; N2blue++) {
        y->data[N2blue].re = 0.0;
        y->data[N2blue].im = 0.0;
      }
    } else {
      boolean_T useRadix2;
      emxInit_real_T(&costab, 2);
      emxInit_real_T(&sintab, 2);
      emxInit_real_T(&sintabinv, 2);
      useRadix2 = ((x->size[1] & (x->size[1] - 1)) == 0);
      internal::FFTImplementationCallback::get_algo_sizes((x->size[1]),
        (useRadix2), (&N2blue), (&nRows));
      internal::FFTImplementationCallback::c_generate_twiddle_tables((nRows),
        (useRadix2), (costab), (sintab), (sintabinv));
      emxInit_creal_T(&yCol, 1);
      if (useRadix2) {
        nRows = x->size[1];
        b_x = *x;
        d_x[0] = nRows;
        b_x.size = &d_x[0];
        b_x.numDimensions = 1;
        internal::FFTImplementationCallback::r2br_r2dit_trig_impl((&b_x),
          (x->size[1]), (costab), (sintab), (yCol));
        if (yCol->size[0] > 1) {
          double b;
          b = 1.0 / static_cast<double>(yCol->size[0]);
          nRows = yCol->size[0];
          for (N2blue = 0; N2blue < nRows; N2blue++) {
            yCol->data[N2blue].re *= b;
            yCol->data[N2blue].im *= b;
          }
        }
      } else {
        nRows = x->size[1];
        b_x = *x;
        c_x[0] = nRows;
        b_x.size = &c_x[0];
        b_x.numDimensions = 1;
        internal::FFTImplementationCallback::dobluesteinfft((&b_x), (N2blue),
          (x->size[1]), (costab), (sintab), (sintabinv), (yCol));
      }

      emxFree_real_T(&sintabinv);
      emxFree_real_T(&sintab);
      emxFree_real_T(&costab);
      N2blue = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = x->size[1];
      emxEnsureCapacity_creal_T(y, N2blue);
      nRows = x->size[1];
      for (N2blue = 0; N2blue < nRows; N2blue++) {
        y->data[N2blue] = yCol->data[N2blue];
      }

      emxFree_creal_T(&yCol);
    }
  }

  //
  // Arguments    : const emxArray_creal_T *x
  //                emxArray_creal_T *y
  // Return Type  : void
  //
  void ifft(const emxArray_creal_T *x, emxArray_creal_T *y)
  {
    emxArray_real_T *costab;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    int N2blue;
    int nRows;
    if (x->size[0] == 0) {
      nRows = y->size[0];
      y->size[0] = x->size[0];
      emxEnsureCapacity_creal_T(y, nRows);
      N2blue = x->size[0];
      for (nRows = 0; nRows < N2blue; nRows++) {
        y->data[nRows].re = 0.0;
        y->data[nRows].im = 0.0;
      }
    } else {
      boolean_T useRadix2;
      emxInit_real_T(&costab, 2);
      emxInit_real_T(&sintab, 2);
      emxInit_real_T(&sintabinv, 2);
      useRadix2 = ((x->size[0] & (x->size[0] - 1)) == 0);
      internal::FFTImplementationCallback::get_algo_sizes((x->size[0]),
        (useRadix2), (&N2blue), (&nRows));
      internal::FFTImplementationCallback::c_generate_twiddle_tables((nRows),
        (useRadix2), (costab), (sintab), (sintabinv));
      if (useRadix2) {
        internal::FFTImplementationCallback::r2br_r2dit_trig_impl((x), (x->size
          [0]), (costab), (sintab), (y));
        if (y->size[0] > 1) {
          double b;
          b = 1.0 / static_cast<double>(y->size[0]);
          N2blue = y->size[0];
          for (nRows = 0; nRows < N2blue; nRows++) {
            y->data[nRows].re *= b;
            y->data[nRows].im *= b;
          }
        }
      } else {
        internal::FFTImplementationCallback::dobluesteinfft((x), (N2blue),
          (x->size[0]), (costab), (sintab), (sintabinv), (y));
      }

      emxFree_real_T(&sintabinv);
      emxFree_real_T(&sintab);
      emxFree_real_T(&costab);
    }
  }
}

//
// File trailer for ifft.cpp
//
// [EOF]
//
