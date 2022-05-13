//
// File: fft.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
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
  void b_fft(const emxArray_real_T *x, double varargin_1, emxArray_creal_T *y)
  {
    emxArray_creal_T *yCol;
    emxArray_real_T b_x;
    emxArray_real_T *costab;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    int c_x[1];
    int d_x[1];
    int N2blue;
    int nRows;
    emxInit_creal_T(&yCol, 1);
    emxInit_real_T(&costab, 2);
    emxInit_real_T(&sintab, 2);
    emxInit_real_T(&sintabinv, 2);
    if ((x->size[1] == 0) || (0 == static_cast<int>(varargin_1))) {
      N2blue = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = static_cast<int>(varargin_1);
      emxEnsureCapacity_creal_T(y, N2blue);
      nRows = static_cast<int>(varargin_1);
      for (N2blue = 0; N2blue < nRows; N2blue++) {
        y->data[N2blue].re = 0.0;
        y->data[N2blue].im = 0.0;
      }
    } else {
      boolean_T useRadix2;
      useRadix2 = ((static_cast<int>(varargin_1) > 0) && ((static_cast<int>
        (varargin_1) & (static_cast<int>(varargin_1) - 1)) == 0));
      internal::FFTImplementationCallback::get_algo_sizes((static_cast<int>
        (varargin_1)), (useRadix2), (&N2blue), (&nRows));
      internal::FFTImplementationCallback::generate_twiddle_tables((nRows),
        (useRadix2), (costab), (sintab), (sintabinv));
      if (useRadix2) {
        nRows = x->size[1];
        b_x = *x;
        d_x[0] = nRows;
        b_x.size = &d_x[0];
        b_x.numDimensions = 1;
        internal::FFTImplementationCallback::r2br_r2dit_trig((&b_x), (
          static_cast<int>(varargin_1)), (costab), (sintab), (yCol));
      } else {
        nRows = x->size[1];
        b_x = *x;
        c_x[0] = nRows;
        b_x.size = &c_x[0];
        b_x.numDimensions = 1;
        internal::FFTImplementationCallback::dobluesteinfft((&b_x), (N2blue), (
          static_cast<int>(varargin_1)), (costab), (sintab), (sintabinv), (yCol));
      }

      N2blue = y->size[0] * y->size[1];
      y->size[0] = 1;
      y->size[1] = static_cast<int>(varargin_1);
      emxEnsureCapacity_creal_T(y, N2blue);
      nRows = static_cast<int>(varargin_1);
      for (N2blue = 0; N2blue < nRows; N2blue++) {
        y->data[N2blue] = yCol->data[N2blue];
      }
    }

    emxFree_real_T(&sintabinv);
    emxFree_real_T(&sintab);
    emxFree_real_T(&costab);
    emxFree_creal_T(&yCol);
  }

  //
  // Arguments    : const emxArray_real_T *x
  //                double varargin_1
  //                emxArray_creal_T *y
  // Return Type  : void
  //
  void fft(const emxArray_real_T *x, double varargin_1, emxArray_creal_T *y)
  {
    emxArray_real_T *costab;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    int N2blue;
    int nRows;
    emxInit_real_T(&costab, 2);
    emxInit_real_T(&sintab, 2);
    emxInit_real_T(&sintabinv, 2);
    if ((x->size[0] == 0) || (0 == static_cast<int>(varargin_1))) {
      nRows = y->size[0];
      y->size[0] = static_cast<int>(varargin_1);
      emxEnsureCapacity_creal_T(y, nRows);
      N2blue = static_cast<int>(varargin_1);
      for (nRows = 0; nRows < N2blue; nRows++) {
        y->data[nRows].re = 0.0;
        y->data[nRows].im = 0.0;
      }
    } else {
      boolean_T useRadix2;
      useRadix2 = ((static_cast<int>(varargin_1) > 0) && ((static_cast<int>
        (varargin_1) & (static_cast<int>(varargin_1) - 1)) == 0));
      internal::FFTImplementationCallback::get_algo_sizes((static_cast<int>
        (varargin_1)), (useRadix2), (&N2blue), (&nRows));
      internal::FFTImplementationCallback::generate_twiddle_tables((nRows),
        (useRadix2), (costab), (sintab), (sintabinv));
      if (useRadix2) {
        internal::FFTImplementationCallback::r2br_r2dit_trig((x), (static_cast<
          int>(varargin_1)), (costab), (sintab), (y));
      } else {
        internal::FFTImplementationCallback::dobluesteinfft((x), (N2blue), (
          static_cast<int>(varargin_1)), (costab), (sintab), (sintabinv), (y));
      }
    }

    emxFree_real_T(&sintabinv);
    emxFree_real_T(&sintab);
    emxFree_real_T(&costab);
  }

  //
  // Arguments    : const double x[1820]
  //                double varargin_1
  //                emxArray_creal_T *y
  // Return Type  : void
  //
  void fft(const double x[1820], double varargin_1, emxArray_creal_T *y)
  {
    emxArray_creal_T *b_fv;
    emxArray_creal_T *fv;
    emxArray_creal_T *wwc;
    emxArray_real_T *costab;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    double twid_im;
    double twid_re;
    int N2blue;
    int i;
    int idx;
    int ihi;
    int k;
    int minNrowsNx;
    int nInt2;
    int nInt2m1;
    int rt;
    ihi = static_cast<int>(varargin_1);
    if (0 == static_cast<int>(varargin_1)) {
      i = y->size[0];
      y->size[0] = static_cast<int>(varargin_1);
      emxEnsureCapacity_creal_T(y, i);
      minNrowsNx = static_cast<int>(varargin_1);
      for (i = 0; i < minNrowsNx; i++) {
        y->data[i].re = 0.0;
        y->data[i].im = 0.0;
      }
    } else {
      boolean_T useRadix2;
      emxInit_real_T(&costab, 2);
      emxInit_real_T(&sintab, 2);
      emxInit_real_T(&sintabinv, 2);
      useRadix2 = ((static_cast<int>(varargin_1) > 0) && ((static_cast<int>
        (varargin_1) & (static_cast<int>(varargin_1) - 1)) == 0));
      internal::FFTImplementationCallback::get_algo_sizes((static_cast<int>
        (varargin_1)), (useRadix2), (&N2blue), (&minNrowsNx));
      internal::FFTImplementationCallback::generate_twiddle_tables((minNrowsNx),
        (useRadix2), (costab), (sintab), (sintabinv));
      if (useRadix2) {
        i = y->size[0];
        y->size[0] = static_cast<int>(varargin_1);
        emxEnsureCapacity_creal_T(y, i);
        if (static_cast<int>(varargin_1) > 1820) {
          minNrowsNx = static_cast<int>(varargin_1);
          i = y->size[0];
          y->size[0] = static_cast<int>(varargin_1);
          emxEnsureCapacity_creal_T(y, i);
          for (i = 0; i < minNrowsNx; i++) {
            y->data[i].re = 0.0;
            y->data[i].im = 0.0;
          }
        }

        if (static_cast<int>(varargin_1) != 1) {
          internal::FFTImplementationCallback::doHalfLengthRadix2((x), (y), (
            static_cast<int>(varargin_1)), (costab), (sintab));
        } else {
          double nt_im;
          double nt_re;
          int b_y;
          if (1820 < static_cast<int>(varargin_1)) {
            b_y = 1818;
          } else {
            b_y = static_cast<int>(varargin_1) - 2;
          }

          nInt2m1 = static_cast<int>(varargin_1) - 2;
          N2blue = static_cast<int>(varargin_1) / 2;
          k = N2blue / 2;
          rt = 0;
          minNrowsNx = 0;
          nInt2 = 0;
          for (i = 0; i <= b_y; i++) {
            y->data[minNrowsNx].re = x[rt];
            y->data[minNrowsNx].im = 0.0;
            minNrowsNx = static_cast<int>(varargin_1);
            useRadix2 = true;
            while (useRadix2) {
              minNrowsNx >>= 1;
              nInt2 ^= minNrowsNx;
              useRadix2 = ((nInt2 & minNrowsNx) == 0);
            }

            minNrowsNx = nInt2;
            rt++;
          }

          y->data[minNrowsNx].re = x[rt];
          y->data[minNrowsNx].im = 0.0;
          if (static_cast<int>(varargin_1) > 1) {
            for (i = 0; i <= nInt2m1; i += 2) {
              nt_re = y->data[i + 1].re;
              nt_im = y->data[i + 1].im;
              twid_im = y->data[i].re;
              twid_re = y->data[i].im;
              y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
              y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
              twid_im += nt_re;
              twid_re += nt_im;
              y->data[i].re = twid_im;
              y->data[i].im = twid_re;
            }
          }

          minNrowsNx = 2;
          nInt2m1 = 4;
          rt = ((k - 1) << 2) + 1;
          while (k > 0) {
            for (i = 0; i < rt; i += nInt2m1) {
              b_y = i + minNrowsNx;
              nt_re = y->data[b_y].re;
              nt_im = y->data[b_y].im;
              y->data[b_y].re = y->data[i].re - nt_re;
              y->data[b_y].im = y->data[i].im - nt_im;
              y->data[i].re += nt_re;
              y->data[i].im += nt_im;
            }

            nInt2 = 1;
            for (idx = k; idx < N2blue; idx += k) {
              twid_re = costab->data[idx];
              twid_im = sintab->data[idx];
              i = nInt2;
              ihi = nInt2 + rt;
              while (i < ihi) {
                b_y = i + minNrowsNx;
                nt_re = twid_re * y->data[b_y].re - twid_im * y->data[b_y].im;
                nt_im = twid_re * y->data[b_y].im + twid_im * y->data[b_y].re;
                y->data[b_y].re = y->data[i].re - nt_re;
                y->data[b_y].im = y->data[i].im - nt_im;
                y->data[i].re += nt_re;
                y->data[i].im += nt_im;
                i += nInt2m1;
              }

              nInt2++;
            }

            k /= 2;
            minNrowsNx = nInt2m1;
            nInt2m1 += nInt2m1;
            rt -= minNrowsNx;
          }
        }
      } else {
        int b_y;
        if ((static_cast<int>(varargin_1) != 1) && ((static_cast<int>(varargin_1)
              & 1) == 0)) {
          useRadix2 = true;
        } else {
          useRadix2 = false;
        }

        emxInit_creal_T(&wwc, 1);
        if (useRadix2) {
          minNrowsNx = static_cast<int>(varargin_1) / 2;
          nInt2m1 = (minNrowsNx + minNrowsNx) - 1;
          i = wwc->size[0];
          wwc->size[0] = nInt2m1;
          emxEnsureCapacity_creal_T(wwc, i);
          idx = minNrowsNx;
          rt = 0;
          wwc->data[minNrowsNx - 1].re = 1.0;
          wwc->data[minNrowsNx - 1].im = 0.0;
          nInt2 = minNrowsNx << 1;
          for (k = 0; k <= minNrowsNx - 2; k++) {
            double nt_im;
            double nt_re;
            b_y = ((k + 1) << 1) - 1;
            if (nInt2 - rt <= b_y) {
              rt += b_y - nInt2;
            } else {
              rt += b_y;
            }

            nt_im = -3.1415926535897931 * static_cast<double>(rt) / static_cast<
              double>(minNrowsNx);
            if (nt_im == 0.0) {
              nt_re = 1.0;
              nt_im = 0.0;
            } else {
              nt_re = std::cos(nt_im);
              nt_im = std::sin(nt_im);
            }

            wwc->data[idx - 2].re = nt_re;
            wwc->data[idx - 2].im = -nt_im;
            idx--;
          }

          idx = 0;
          i = nInt2m1 - 1;
          for (k = i; k >= minNrowsNx; k--) {
            wwc->data[k] = wwc->data[idx];
            idx++;
          }
        } else {
          nInt2m1 = (static_cast<int>(varargin_1) + static_cast<int>(varargin_1))
            - 1;
          i = wwc->size[0];
          wwc->size[0] = nInt2m1;
          emxEnsureCapacity_creal_T(wwc, i);
          idx = static_cast<int>(varargin_1);
          rt = 0;
          wwc->data[static_cast<int>(varargin_1) - 1].re = 1.0;
          wwc->data[static_cast<int>(varargin_1) - 1].im = 0.0;
          nInt2 = static_cast<int>(varargin_1) << 1;
          i = static_cast<int>(varargin_1);
          for (k = 0; k <= i - 2; k++) {
            double nt_im;
            double nt_re;
            b_y = ((k + 1) << 1) - 1;
            if (nInt2 - rt <= b_y) {
              rt += b_y - nInt2;
            } else {
              rt += b_y;
            }

            nt_im = -3.1415926535897931 * static_cast<double>(rt) / static_cast<
              double>(static_cast<int>(varargin_1));
            if (nt_im == 0.0) {
              nt_re = 1.0;
              nt_im = 0.0;
            } else {
              nt_re = std::cos(nt_im);
              nt_im = std::sin(nt_im);
            }

            wwc->data[idx - 2].re = nt_re;
            wwc->data[idx - 2].im = -nt_im;
            idx--;
          }

          idx = 0;
          i = nInt2m1 - 1;
          for (k = i; k >= ihi; k--) {
            wwc->data[k] = wwc->data[idx];
            idx++;
          }
        }

        i = y->size[0];
        y->size[0] = static_cast<int>(varargin_1);
        emxEnsureCapacity_creal_T(y, i);
        if (static_cast<int>(varargin_1) > 1820) {
          minNrowsNx = static_cast<int>(varargin_1);
          i = y->size[0];
          y->size[0] = static_cast<int>(varargin_1);
          emxEnsureCapacity_creal_T(y, i);
          for (i = 0; i < minNrowsNx; i++) {
            y->data[i].re = 0.0;
            y->data[i].im = 0.0;
          }
        }

        if ((N2blue != 1) && ((static_cast<int>(varargin_1) & 1) == 0)) {
          internal::FFTImplementationCallback::doHalfLengthBluestein((x), (y), (
            static_cast<int>(varargin_1)), (N2blue), (wwc), (costab), (sintab),
            (costab), (sintabinv));
        } else {
          if (static_cast<int>(varargin_1) < 1820) {
            minNrowsNx = static_cast<int>(varargin_1) - 1;
          } else {
            minNrowsNx = 1819;
          }

          nInt2m1 = 0;
          for (k = 0; k <= minNrowsNx; k++) {
            b_y = (static_cast<int>(varargin_1) + k) - 1;
            y->data[k].re = wwc->data[b_y].re * x[nInt2m1];
            y->data[k].im = wwc->data[b_y].im * -x[nInt2m1];
            nInt2m1++;
          }

          i = minNrowsNx + 2;
          for (k = i; k <= ihi; k++) {
            y->data[k - 1].re = 0.0;
            y->data[k - 1].im = 0.0;
          }

          emxInit_creal_T(&fv, 1);
          emxInit_creal_T(&b_fv, 1);
          internal::FFTImplementationCallback::r2br_r2dit_trig_impl((y), (N2blue),
            (costab), (sintab), (fv));
          internal::FFTImplementationCallback::r2br_r2dit_trig_impl((wwc),
            (N2blue), (costab), (sintab), (b_fv));
          i = b_fv->size[0];
          b_fv->size[0] = fv->size[0];
          emxEnsureCapacity_creal_T(b_fv, i);
          minNrowsNx = fv->size[0];
          for (i = 0; i < minNrowsNx; i++) {
            twid_re = fv->data[i].re * b_fv->data[i].im + fv->data[i].im *
              b_fv->data[i].re;
            b_fv->data[i].re = fv->data[i].re * b_fv->data[i].re - fv->data[i].
              im * b_fv->data[i].im;
            b_fv->data[i].im = twid_re;
          }

          internal::FFTImplementationCallback::r2br_r2dit_trig_impl((b_fv),
            (N2blue), (costab), (sintabinv), (fv));
          emxFree_creal_T(&b_fv);
          if (fv->size[0] > 1) {
            twid_re = 1.0 / static_cast<double>(fv->size[0]);
            minNrowsNx = fv->size[0];
            for (i = 0; i < minNrowsNx; i++) {
              fv->data[i].re *= twid_re;
              fv->data[i].im *= twid_re;
            }
          }

          idx = 0;
          i = static_cast<int>(varargin_1);
          minNrowsNx = wwc->size[0];
          for (k = i; k <= minNrowsNx; k++) {
            y->data[idx].re = wwc->data[k - 1].re * fv->data[k - 1].re +
              wwc->data[k - 1].im * fv->data[k - 1].im;
            y->data[idx].im = wwc->data[k - 1].re * fv->data[k - 1].im -
              wwc->data[k - 1].im * fv->data[k - 1].re;
            idx++;
          }

          emxFree_creal_T(&fv);
        }

        emxFree_creal_T(&wwc);
      }

      emxFree_real_T(&sintabinv);
      emxFree_real_T(&sintab);
      emxFree_real_T(&costab);
    }
  }

  //
  // Arguments    : const emxArray_real_T *x
  //                emxArray_creal_T *y
  // Return Type  : void
  //
  void fft(const emxArray_real_T *x, emxArray_creal_T *y)
  {
    emxArray_creal_T *yCol;
    emxArray_real_T b_x;
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
      internal::FFTImplementationCallback::generate_twiddle_tables((nRows),
        (useRadix2), (costab), (sintab), (sintabinv));
      emxInit_creal_T(&yCol, 1);
      if (useRadix2) {
        nRows = x->size[1];
        b_x = *x;
        d_x[0] = nRows;
        b_x.size = &d_x[0];
        b_x.numDimensions = 1;
        internal::FFTImplementationCallback::r2br_r2dit_trig((&b_x), (x->size[1]),
          (costab), (sintab), (yCol));
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
}

//
// File trailer for fft.cpp
//
// [EOF]
//
