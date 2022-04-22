//
// File: fft.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
//

// Include Files
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "HS_EWL_RECEIVE_types.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
//                double varargin_1
//                emxArray_creal_T *y
// Return Type  : void
//
namespace coder
{
  void fft(const emxArray_real_T *x, double varargin_1, emxArray_creal_T *y)
  {
    emxArray_real_T *costab;
    emxArray_real_T *costab1q;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    double twid_im;
    double twid_re;
    int i;
    int iDelta2;
    int ihi;
    int ix;
    int j;
    int ju;
    int k;
    int nd2;
    emxInit_real_T(&costab1q, 2);
    emxInit_real_T(&costab, 2);
    emxInit_real_T(&sintab, 2);
    emxInit_real_T(&sintabinv, 2);
    if ((x->size[0] == 0) || (0 == static_cast<int>(varargin_1))) {
      ix = y->size[0];
      y->size[0] = static_cast<int>(varargin_1);
      emxEnsureCapacity_creal_T(y, ix);
      nd2 = static_cast<int>(varargin_1);
      for (ix = 0; ix < nd2; ix++) {
        y->data[ix].re = 0.0;
        y->data[ix].im = 0.0;
      }
    } else {
      boolean_T useRadix2;
      useRadix2 = ((static_cast<int>(varargin_1) > 0) && ((static_cast<int>
        (varargin_1) & (static_cast<int>(varargin_1) - 1)) == 0));
      internal::FFTImplementationCallback::get_algo_sizes((static_cast<int>
        (varargin_1)), (useRadix2), (&iDelta2), (&nd2));
      twid_re = 6.2831853071795862 / static_cast<double>(nd2);
      ihi = nd2 / 2 / 2;
      ix = costab1q->size[0] * costab1q->size[1];
      costab1q->size[0] = 1;
      costab1q->size[1] = ihi + 1;
      emxEnsureCapacity_real_T(costab1q, ix);
      costab1q->data[0] = 1.0;
      nd2 = ihi / 2 - 1;
      for (k = 0; k <= nd2; k++) {
        costab1q->data[k + 1] = std::cos(twid_re * (static_cast<double>(k) + 1.0));
      }

      ix = nd2 + 2;
      nd2 = ihi - 1;
      for (k = ix; k <= nd2; k++) {
        costab1q->data[k] = std::sin(twid_re * static_cast<double>(ihi - k));
      }

      costab1q->data[ihi] = 0.0;
      if (!useRadix2) {
        ihi = costab1q->size[1] - 1;
        nd2 = (costab1q->size[1] - 1) << 1;
        ix = costab->size[0] * costab->size[1];
        costab->size[0] = 1;
        costab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(costab, ix);
        ix = sintab->size[0] * sintab->size[1];
        sintab->size[0] = 1;
        sintab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintab, ix);
        costab->data[0] = 1.0;
        sintab->data[0] = 0.0;
        ix = sintabinv->size[0] * sintabinv->size[1];
        sintabinv->size[0] = 1;
        sintabinv->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintabinv, ix);
        for (k = 0; k < ihi; k++) {
          sintabinv->data[k + 1] = costab1q->data[(ihi - k) - 1];
        }

        ix = costab1q->size[1];
        for (k = ix; k <= nd2; k++) {
          sintabinv->data[k] = costab1q->data[k - ihi];
        }

        for (k = 0; k < ihi; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = -costab1q->data[(ihi - k) - 1];
        }

        ix = costab1q->size[1];
        for (k = ix; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = -costab1q->data[k - ihi];
        }
      } else {
        ihi = costab1q->size[1] - 1;
        nd2 = (costab1q->size[1] - 1) << 1;
        ix = costab->size[0] * costab->size[1];
        costab->size[0] = 1;
        costab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(costab, ix);
        ix = sintab->size[0] * sintab->size[1];
        sintab->size[0] = 1;
        sintab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintab, ix);
        costab->data[0] = 1.0;
        sintab->data[0] = 0.0;
        for (k = 0; k < ihi; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = -costab1q->data[(ihi - k) - 1];
        }

        ix = costab1q->size[1];
        for (k = ix; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = -costab1q->data[k - ihi];
        }

        sintabinv->size[0] = 1;
        sintabinv->size[1] = 0;
      }

      if (useRadix2) {
        ix = y->size[0];
        y->size[0] = static_cast<int>(varargin_1);
        emxEnsureCapacity_creal_T(y, ix);
        if (static_cast<int>(varargin_1) > x->size[0]) {
          nd2 = static_cast<int>(varargin_1);
          ix = y->size[0];
          y->size[0] = static_cast<int>(varargin_1);
          emxEnsureCapacity_creal_T(y, ix);
          for (ix = 0; ix < nd2; ix++) {
            y->data[ix].re = 0.0;
            y->data[ix].im = 0.0;
          }
        }

        if (static_cast<int>(varargin_1) != 1) {
          internal::FFTImplementationCallback::doHalfLengthRadix2((x), (y), (
            static_cast<int>(varargin_1)), (costab), (sintab));
        } else {
          double temp_im;
          double temp_re;
          int nRowsD2;
          nd2 = x->size[0];
          j = static_cast<int>(varargin_1);
          if (nd2 < j) {
            j = nd2;
          }

          iDelta2 = static_cast<int>(varargin_1) - 2;
          nRowsD2 = static_cast<int>(varargin_1) / 2;
          k = nRowsD2 / 2;
          ix = 0;
          nd2 = 0;
          ju = 0;
          for (i = 0; i <= j - 2; i++) {
            y->data[nd2].re = x->data[ix];
            y->data[nd2].im = 0.0;
            ihi = static_cast<int>(varargin_1);
            useRadix2 = true;
            while (useRadix2) {
              ihi >>= 1;
              ju ^= ihi;
              useRadix2 = ((ju & ihi) == 0);
            }

            nd2 = ju;
            ix++;
          }

          y->data[nd2].re = x->data[ix];
          y->data[nd2].im = 0.0;
          if (static_cast<int>(varargin_1) > 1) {
            for (i = 0; i <= iDelta2; i += 2) {
              temp_re = y->data[i + 1].re;
              temp_im = y->data[i + 1].im;
              twid_re = y->data[i].re;
              twid_im = y->data[i].im;
              y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
              y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
              twid_re += temp_re;
              twid_im += temp_im;
              y->data[i].re = twid_re;
              y->data[i].im = twid_im;
            }
          }

          nd2 = 2;
          iDelta2 = 4;
          ix = ((k - 1) << 2) + 1;
          while (k > 0) {
            int temp_re_tmp;
            for (i = 0; i < ix; i += iDelta2) {
              temp_re_tmp = i + nd2;
              temp_re = y->data[temp_re_tmp].re;
              temp_im = y->data[temp_re_tmp].im;
              y->data[temp_re_tmp].re = y->data[i].re - temp_re;
              y->data[temp_re_tmp].im = y->data[i].im - temp_im;
              y->data[i].re += temp_re;
              y->data[i].im += temp_im;
            }

            ju = 1;
            for (j = k; j < nRowsD2; j += k) {
              twid_re = costab->data[j];
              twid_im = sintab->data[j];
              i = ju;
              ihi = ju + ix;
              while (i < ihi) {
                temp_re_tmp = i + nd2;
                temp_re = twid_re * y->data[temp_re_tmp].re - twid_im * y->
                  data[temp_re_tmp].im;
                temp_im = twid_re * y->data[temp_re_tmp].im + twid_im * y->
                  data[temp_re_tmp].re;
                y->data[temp_re_tmp].re = y->data[i].re - temp_re;
                y->data[temp_re_tmp].im = y->data[i].im - temp_im;
                y->data[i].re += temp_re;
                y->data[i].im += temp_im;
                i += iDelta2;
              }

              ju++;
            }

            k /= 2;
            nd2 = iDelta2;
            iDelta2 += iDelta2;
            ix -= nd2;
          }
        }
      } else {
        internal::FFTImplementationCallback::dobluesteinfft((x), (iDelta2), (
          static_cast<int>(varargin_1)), (costab), (sintab), (sintabinv), (y));
      }
    }

    emxFree_real_T(&sintabinv);
    emxFree_real_T(&sintab);
    emxFree_real_T(&costab);
    emxFree_real_T(&costab1q);
  }
}

//
// File trailer for fft.cpp
//
// [EOF]
//
