//
// File: xcorr.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "xcorr.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_rtwutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "fft.h"
#include "ifft.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <math.h>

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
//                const emxArray_real_T *varargin_1
//                emxArray_real_T *c
//                emxArray_real_T *lags
// Return Type  : void
//
namespace coder
{
  void xcorr(const emxArray_real_T *x, const emxArray_real_T *varargin_1,
             emxArray_real_T *c)//, emxArray_real_T *lags)
  {
    emxArray_creal_T *X;
    emxArray_creal_T *Y;
    emxArray_real_T *b_c1;
    emxArray_real_T *c1;
    emxArray_real_T *r;
    double Sn;
    double c0;
    double m2;
    int b_mxl;
    int ceilLog2;
    int i;
    int m;
    int maxval;
    int mxl;
    int n;
    if (x->size[0] > varargin_1->size[0]) {
      maxval = x->size[0] - 1;
    } else {
      maxval = varargin_1->size[0] - 1;
    }

    mxl = x->size[0];
    m = varargin_1->size[0];
    if (mxl > m) {
      m = mxl;
    }

    b_mxl = m - 1;
    if (maxval < b_mxl) {
      b_mxl = maxval;
    }

    c0 = frexp(static_cast<double>(static_cast<unsigned int>(std::abs(2.0 *
      static_cast<double>(m) - 1.0))), &ceilLog2);
    if (c0 == 0.5) {
      ceilLog2--;
    }

    m2 = rt_powd_snf(2.0, static_cast<double>(ceilLog2));
    mxl = x->size[0];
    m = varargin_1->size[0];
    if (mxl > m) {
      m = mxl;
    }

    mxl = x->size[0];
    n = varargin_1->size[0];
    if (mxl < n) {
      n = mxl;
    }

    c0 = 2.0 * static_cast<double>(n) - 1.0;
    if (b_mxl <= n - 1) {
      Sn = static_cast<double>(b_mxl) * ((c0 - static_cast<double>(b_mxl)) - 1.0);
      if (b_mxl <= m - n) {
        c0 = (c0 + static_cast<double>(b_mxl) * c0) + Sn;
      } else {
        c0 = ((c0 + static_cast<double>(m - n) * c0) + (static_cast<double>
               (b_mxl) - static_cast<double>(m - n)) * (((static_cast<double>(m)
                 - static_cast<double>(b_mxl)) + static_cast<double>(n)) - 2.0))
          + Sn;
      }
    } else if (b_mxl <= m - 1) {
      Sn = (static_cast<double>(n) - 1.0) * (static_cast<double>(n) - 1.0);
      if (b_mxl <= m - n) {
        c0 = (c0 + static_cast<double>(b_mxl) * c0) + Sn;
      } else {
        c0 = ((c0 + static_cast<double>(m - n) * c0) + (static_cast<double>
               (b_mxl) - static_cast<double>(m - n)) * ((static_cast<double>(m -
                 b_mxl) + static_cast<double>(n)) - 2.0)) + Sn;
      }
    } else {
      c0 = 2.0 * static_cast<double>(m) * static_cast<double>(n) - (static_cast<
        double>(static_cast<unsigned int>(m) + n) - 1.0);
    }

    emxInit_real_T(&c1, 1);
    if (c0 < m2 * (15.0 * static_cast<double>(ceilLog2) + 6.0)) {
      m = x->size[0];
      n = varargin_1->size[0];
      mxl = x->size[0];
      ceilLog2 = varargin_1->size[0];
      if (mxl > ceilLog2) {
        ceilLog2 = mxl;
      }

      mxl = ceilLog2 - 1;
      if (b_mxl < mxl) {
        mxl = b_mxl;
      }

      c0 = 2.0 * static_cast<double>(mxl) + 1.0;
      i = c1->size[0];
      c1->size[0] = static_cast<int>(c0);
      emxEnsureCapacity_real_T(c1, i);
      ceilLog2 = static_cast<int>(c0);
      for (i = 0; i < ceilLog2; i++) {
        c1->data[i] = 0.0;
      }

      for (b_mxl = 0; b_mxl <= mxl; b_mxl++) {
        ceilLog2 = m - b_mxl;
        if (ceilLog2 >= n) {
          ceilLog2 = n;
        }

        c0 = 0.0;
        for (i = 0; i < ceilLog2; i++) {
          c0 += varargin_1->data[i] * x->data[b_mxl + i];
        }

        c1->data[mxl + b_mxl] = c0;
      }

      for (b_mxl = 0; b_mxl < mxl; b_mxl++) {
        ceilLog2 = (n - b_mxl) - 1;
        if (m < ceilLog2) {
          ceilLog2 = m;
        }

        c0 = 0.0;
        for (i = 0; i < ceilLog2; i++) {
          c0 += varargin_1->data[(b_mxl + i) + 1] * x->data[i];
        }

        c1->data[(mxl - b_mxl) - 1] = c0;
      }
    } else {
      emxInit_creal_T(&X, 1);
      emxInit_creal_T(&Y, 1);
      fft(x, m2, X);
      fft(varargin_1, m2, Y);
      i = Y->size[0];
      Y->size[0] = X->size[0];
      emxEnsureCapacity_creal_T(Y, i);
      ceilLog2 = X->size[0];
      for (i = 0; i < ceilLog2; i++) {
        c0 = Y->data[i].re;
        Sn = -Y->data[i].im;
        Y->data[i].re = X->data[i].re * c0 - X->data[i].im * Sn;
        Y->data[i].im = X->data[i].re * Sn + X->data[i].im * c0;
      }

      emxInit_real_T(&b_c1, 1);
      ifft(Y, X);
      i = b_c1->size[0];
      b_c1->size[0] = X->size[0];
      emxEnsureCapacity_real_T(b_c1, i);
      ceilLog2 = X->size[0];
      emxFree_creal_T(&Y);
      for (i = 0; i < ceilLog2; i++) {
        b_c1->data[i] = X->data[i].re;
      }

      emxFree_creal_T(&X);
      emxInit_real_T(&r, 2);
      if (b_mxl < 1) {
        r->size[0] = 1;
        r->size[1] = 0;
      } else {
        i = r->size[0] * r->size[1];
        r->size[0] = 1;
        ceilLog2 = b_mxl - 1;
        r->size[1] = b_mxl;
        emxEnsureCapacity_real_T(r, i);
        for (i = 0; i <= ceilLog2; i++) {
          r->data[i] = static_cast<double>(i) + 1.0;
        }
      }

      m2 -= static_cast<double>(b_mxl);
      i = c1->size[0];
      c1->size[0] = (r->size[1] + b_mxl) + 1;
      emxEnsureCapacity_real_T(c1, i);
      ceilLog2 = r->size[1];
      for (i = 0; i < ceilLog2; i++) {
        c1->data[i] = b_c1->data[static_cast<int>(m2 + r->data[i]) - 1];
      }

      for (i = 0; i <= b_mxl; i++) {
        c1->data[i + r->size[1]] = b_c1->data[i];
      }

      emxFree_real_T(&r);
      emxFree_real_T(&b_c1);
    }

    i = c->size[0];
    c->size[0] = (maxval << 1) + 1;
    emxEnsureCapacity_real_T(c, i);
    ceilLog2 = maxval << 1;
    for (i = 0; i <= ceilLog2; i++) {
      c->data[i] = 0.0;
    }

    ceilLog2 = c1->size[0];
    for (i = 0; i < ceilLog2; i++) {
      c->data[i] = c1->data[i];
    }

//    emxFree_real_T(&c1);
//    i = lags->size[0] * lags->size[1];
//    lags->size[0] = 1;
//    lags->size[1] = (maxval + maxval) + 1;
//    emxEnsureCapacity_real_T(lags, i);
//    ceilLog2 = maxval + maxval;
//    for (i = 0; i <= ceilLog2; i++) {
//      lags->data[i] = i - maxval;
//    }
  }

  //
  // Arguments    : const emxArray_real_T *x
  //                const double varargin_1[1820]
  //                emxArray_real_T *c
  //                emxArray_real_T *lags
  // Return Type  : void
  //
  void xcorr(const emxArray_real_T *x, const double varargin_1[1820],
             emxArray_real_T *c)//, emxArray_real_T *lags)
  {
    emxArray_creal_T *X;
    emxArray_creal_T *Y;
    emxArray_real_T *b_c1;
    emxArray_real_T *c1;
    emxArray_real_T *r;
    double m2;
    double tdops;
    int Sn;
    int c0;
    int ceilLog2;
    int m;
    int maxval;
    int mxl;
    int n;
    if (x->size[0] > 1820) {
      maxval = x->size[0] - 1;
    } else {
      maxval = 1819;
    }

    m = x->size[0];
    if (m <= 1820) {
      m = 1820;
    }

    mxl = m - 1;
    if (maxval < mxl) {
      mxl = maxval;
    }

    tdops = frexp(static_cast<double>((static_cast<unsigned int>(m) << 1) +
      MAX_uint32_T), &ceilLog2);
    if (tdops == 0.5) {
      ceilLog2--;
    }

    m2 = rt_powd_snf(2.0, static_cast<double>(ceilLog2));
    m = x->size[0];
    if (m <= 1820) {
      m = 1820;
    }

    n = x->size[0];
    if (n >= 1820) {
      n = 1820;
    }

    c0 = (n << 1) - 1;
    if (mxl <= n - 1) {
      Sn = 1819 * (c0 - 1820);
      if (1819 <= m - n) {
        tdops = (c0 + 1819 * c0) + Sn;
      } else {
        tdops = ((static_cast<double>(c0) + static_cast<double>(m - n) *
                  static_cast<double>(c0)) + (1819.0 - static_cast<double>(m - n))
                 * (static_cast<double>((static_cast<unsigned int>(m) + n) -
                   1819U) - 2.0)) + static_cast<double>(Sn);
      }
    } else if (mxl <= m - 1) {
      Sn = (n - 1) * (n - 1);
      if (mxl <= m - n) {
        tdops = (static_cast<double>(c0) + static_cast<double>(mxl) *
                 static_cast<double>(c0)) + static_cast<double>(Sn);
      } else {
        tdops = ((static_cast<double>(c0) + static_cast<double>(m - n) *
                  static_cast<double>(c0)) + static_cast<double>((mxl - m) + n) *
                 ((static_cast<double>(m - mxl) + static_cast<double>(n)) - 2.0))
          + static_cast<double>(Sn);
      }
    } else {
      tdops = 2.0 * static_cast<double>(m) * static_cast<double>(n) - (
        static_cast<double>(static_cast<unsigned int>(m) + n) - 1.0);
    }

    emxInit_real_T(&c1, 1);
    if (tdops < m2 * (15.0 * static_cast<double>(ceilLog2) + 6.0)) {
      unsigned int nc;
      m = x->size[0];
      if (x->size[0] > 1820) {
        n = x->size[0] - 1;
      } else {
        n = 1819;
      }

      if (mxl < n) {
        n = mxl;
      }

      nc = (static_cast<unsigned int>(n) << 1) + 1U;
      c0 = c1->size[0];
      c1->size[0] = static_cast<int>(nc);
      emxEnsureCapacity_real_T(c1, c0);
      ceilLog2 = static_cast<int>(nc);
      for (c0 = 0; c0 < ceilLog2; c0++) {
        c1->data[c0] = 0.0;
      }

      for (Sn = 0; Sn <= n; Sn++) {
        ceilLog2 = m - Sn;
        if (ceilLog2 < 1820) {
          ceilLog2--;
        } else {
          ceilLog2 = 1819;
        }

        tdops = 0.0;
        for (c0 = 0; c0 <= ceilLog2; c0++) {
          tdops += varargin_1[c0] * x->data[Sn + c0];
        }

        c1->data[n + Sn] = tdops;
      }

      for (Sn = 0; Sn < n; Sn++) {
        ceilLog2 = 1819 - Sn;
        if (m < ceilLog2) {
          ceilLog2 = m;
        }

        tdops = 0.0;
        for (c0 = 0; c0 < ceilLog2; c0++) {
          tdops += varargin_1[(Sn + c0) + 1] * x->data[c0];
        }

        c1->data[(n - Sn) - 1] = tdops;
      }
    } else {
      emxInit_creal_T(&X, 1);
      emxInit_creal_T(&Y, 1);
      fft(x, m2, X);
      fft(varargin_1, m2, Y);
      c0 = Y->size[0];
      Y->size[0] = X->size[0];
      emxEnsureCapacity_creal_T(Y, c0);
      ceilLog2 = X->size[0];
      for (c0 = 0; c0 < ceilLog2; c0++) {
        double Y_im;
        tdops = Y->data[c0].re;
        Y_im = -Y->data[c0].im;
        Y->data[c0].re = X->data[c0].re * tdops - X->data[c0].im * Y_im;
        Y->data[c0].im = X->data[c0].re * Y_im + X->data[c0].im * tdops;
      }

      emxInit_real_T(&b_c1, 1);
      ifft(Y, X);
      c0 = b_c1->size[0];
      b_c1->size[0] = X->size[0];
      emxEnsureCapacity_real_T(b_c1, c0);
      ceilLog2 = X->size[0];
      emxFree_creal_T(&Y);
      for (c0 = 0; c0 < ceilLog2; c0++) {
        b_c1->data[c0] = X->data[c0].re;
      }

      emxFree_creal_T(&X);
      emxInit_real_T(&r, 2);
      c0 = r->size[0] * r->size[1];
      r->size[0] = 1;
      ceilLog2 = mxl - 1;
      r->size[1] = mxl;
      emxEnsureCapacity_real_T(r, c0);
      for (c0 = 0; c0 <= ceilLog2; c0++) {
        r->data[c0] = static_cast<double>(c0) + 1.0;
      }

      m2 -= static_cast<double>(mxl);
      c0 = c1->size[0];
      c1->size[0] = (r->size[1] + mxl) + 1;
      emxEnsureCapacity_real_T(c1, c0);
      ceilLog2 = r->size[1];
      for (c0 = 0; c0 < ceilLog2; c0++) {
        c1->data[c0] = b_c1->data[static_cast<int>(m2 + r->data[c0]) - 1];
      }

      for (c0 = 0; c0 <= mxl; c0++) {
        c1->data[c0 + r->size[1]] = b_c1->data[c0];
      }

      emxFree_real_T(&r);
      emxFree_real_T(&b_c1);
    }

    c0 = c->size[0];
    c->size[0] = (maxval << 1) + 1;
    emxEnsureCapacity_real_T(c, c0);
    ceilLog2 = maxval << 1;
    for (c0 = 0; c0 <= ceilLog2; c0++) {
      c->data[c0] = 0.0;
    }

    ceilLog2 = c1->size[0];
    for (c0 = 0; c0 < ceilLog2; c0++) {
      c->data[c0] = c1->data[c0];
    }

//    emxFree_real_T(&c1);
//    c0 = lags->size[0] * lags->size[1];
//    lags->size[0] = 1;
//    lags->size[1] = (maxval + maxval) + 1;
//    emxEnsureCapacity_real_T(lags, c0);
//    ceilLog2 = maxval + maxval;
//    for (c0 = 0; c0 <= ceilLog2; c0++) {
//      lags->data[c0] = c0 - maxval;
//    }
  }
}

//
// File trailer for xcorr.cpp
//
// [EOF]
//
