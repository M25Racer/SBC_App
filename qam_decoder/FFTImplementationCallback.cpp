//
// File: FFTImplementationCallback.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "FFTImplementationCallback.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : int nRows
//                emxArray_real_T *costab
//                emxArray_real_T *sintab
//                emxArray_real_T *sintabinv
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    void FFTImplementationCallback::b_generate_twiddle_tables(int nRows,
      emxArray_real_T *costab, emxArray_real_T *sintab, emxArray_real_T
      *sintabinv)
    {
      emxArray_real_T *costab1q;
      double e;
      int i;
      int k;
      int n;
      int nd2;
      emxInit_real_T(&costab1q, 2);
      e = 6.2831853071795862 / static_cast<double>(nRows);
      n = nRows / 2 / 2;
      i = costab1q->size[0] * costab1q->size[1];
      costab1q->size[0] = 1;
      costab1q->size[1] = n + 1;
      emxEnsureCapacity_real_T(costab1q, i);
      costab1q->data[0] = 1.0;
      nd2 = n / 2 - 1;
      for (k = 0; k <= nd2; k++) {
        costab1q->data[k + 1] = std::cos(e * (static_cast<double>(k) + 1.0));
      }

      i = nd2 + 2;
      nd2 = n - 1;
      for (k = i; k <= nd2; k++) {
        costab1q->data[k] = std::sin(e * static_cast<double>(n - k));
      }

      costab1q->data[n] = 0.0;
      n = costab1q->size[1] - 1;
      nd2 = (costab1q->size[1] - 1) << 1;
      i = costab->size[0] * costab->size[1];
      costab->size[0] = 1;
      costab->size[1] = nd2 + 1;
      emxEnsureCapacity_real_T(costab, i);
      i = sintab->size[0] * sintab->size[1];
      sintab->size[0] = 1;
      sintab->size[1] = nd2 + 1;
      emxEnsureCapacity_real_T(sintab, i);
      costab->data[0] = 1.0;
      sintab->data[0] = 0.0;
      i = sintabinv->size[0] * sintabinv->size[1];
      sintabinv->size[0] = 1;
      sintabinv->size[1] = nd2 + 1;
      emxEnsureCapacity_real_T(sintabinv, i);
      for (k = 0; k < n; k++) {
        sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
      }

      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        sintabinv->data[k] = costab1q->data[k - n];
      }

      for (k = 0; k < n; k++) {
        costab->data[k + 1] = costab1q->data[k + 1];
        sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
      }

      i = costab1q->size[1];
      for (k = i; k <= nd2; k++) {
        costab->data[k] = -costab1q->data[nd2 - k];
        sintab->data[k] = -costab1q->data[k - n];
      }

      emxFree_real_T(&costab1q);
    }

    //
    // Arguments    : const emxArray_real_T *x
    //                emxArray_creal_T *y
    //                int nrowsx
    //                int nRows
    //                int nfft
    //                const emxArray_creal_T *wwc
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                const emxArray_real_T *costabinv
    //                const emxArray_real_T *sintabinv
    // Return Type  : void
    //
    void FFTImplementationCallback::doHalfLengthBluestein(const emxArray_real_T *
      x, emxArray_creal_T *y, int nrowsx, int nRows, int nfft, const
      emxArray_creal_T *wwc, const emxArray_real_T *costab, const
      emxArray_real_T *sintab, const emxArray_real_T *costabinv, const
      emxArray_real_T *sintabinv)
    {
      emxArray_creal_T *b_fv;
      emxArray_creal_T *fv;
      emxArray_creal_T *reconVar1;
      emxArray_creal_T *reconVar2;
      emxArray_creal_T *ytmp;
      emxArray_int32_T *wrapIndex;
      emxArray_real_T *costable;
      emxArray_real_T *hcostabinv;
      emxArray_real_T *hsintab;
      emxArray_real_T *hsintabinv;
      emxArray_real_T *sintable;
      emxArray_real_T *unusedU0;
      double a_im;
      double a_re;
      double b_re;
      double z_tmp;
      int hnRows;
      int i;
      int idx;
      int u0;
      int xidx;
      boolean_T nxeven;
      emxInit_creal_T(&ytmp, 1);
      hnRows = nRows / 2;
      i = ytmp->size[0];
      ytmp->size[0] = hnRows;
      emxEnsureCapacity_creal_T(ytmp, i);
      if (hnRows > nrowsx) {
        i = ytmp->size[0];
        ytmp->size[0] = hnRows;
        emxEnsureCapacity_creal_T(ytmp, i);
        for (i = 0; i < hnRows; i++) {
          ytmp->data[i].re = 0.0;
          ytmp->data[i].im = 0.0;
        }
      }

      if ((x->size[0] & 1) == 0) {
        nxeven = true;
        u0 = x->size[0];
      } else if (x->size[0] >= nRows) {
        nxeven = true;
        u0 = nRows;
      } else {
        nxeven = false;
        u0 = x->size[0] - 1;
      }

      emxInit_real_T(&unusedU0, 2);
      emxInit_int32_T(&wrapIndex, 2);
      emxInit_real_T(&costable, 2);
      emxInit_real_T(&sintable, 2);
      emxInit_real_T(&hsintab, 2);
      emxInit_real_T(&hcostabinv, 2);
      emxInit_real_T(&hsintabinv, 2);
      emxInit_creal_T(&reconVar1, 1);
      emxInit_creal_T(&reconVar2, 1);
      if (u0 >= nRows) {
        u0 = nRows;
      }

      FFTImplementationCallback::b_generate_twiddle_tables((nRows << 1),
        (costable), (sintable), (unusedU0));
      FFTImplementationCallback::get_half_twiddle_tables((costab), (sintab),
        (costabinv), (sintabinv), (unusedU0), (hsintab), (hcostabinv),
        (hsintabinv));
      i = reconVar1->size[0];
      reconVar1->size[0] = hnRows;
      emxEnsureCapacity_creal_T(reconVar1, i);
      i = reconVar2->size[0];
      reconVar2->size[0] = hnRows;
      emxEnsureCapacity_creal_T(reconVar2, i);
      idx = 0;
      i = wrapIndex->size[0] * wrapIndex->size[1];
      wrapIndex->size[0] = 1;
      wrapIndex->size[1] = hnRows;
      emxEnsureCapacity_int32_T(wrapIndex, i);
      for (xidx = 0; xidx < hnRows; xidx++) {
        reconVar1->data[xidx].re = sintable->data[idx] + 1.0;
        reconVar1->data[xidx].im = -costable->data[idx];
        reconVar2->data[xidx].re = 1.0 - sintable->data[idx];
        reconVar2->data[xidx].im = costable->data[idx];
        idx += 2;
        if (xidx + 1 != 1) {
          wrapIndex->data[xidx] = (hnRows - xidx) + 1;
        } else {
          wrapIndex->data[0] = 1;
        }
      }

      emxFree_real_T(&sintable);
      emxFree_real_T(&costable);
      xidx = 0;
      z_tmp = static_cast<double>(u0) / 2.0;
      i = static_cast<int>(z_tmp);
      for (int k1 = 0; k1 < i; k1++) {
        double b_im;
        idx = (hnRows + k1) - 1;
        a_re = wwc->data[idx].re;
        a_im = wwc->data[idx].im;
        b_re = x->data[xidx];
        b_im = x->data[xidx + 1];
        ytmp->data[k1].re = a_re * b_re + a_im * b_im;
        ytmp->data[k1].im = a_re * b_im - a_im * b_re;
        xidx += 2;
      }

      if (!nxeven) {
        idx = (hnRows + static_cast<int>(z_tmp)) - 1;
        a_re = wwc->data[idx].re;
        a_im = wwc->data[idx].im;
        b_re = x->data[xidx];
        i = static_cast<int>(static_cast<double>(u0) / 2.0);
        ytmp->data[i].re = a_re * b_re + a_im * 0.0;
        ytmp->data[i].im = a_re * 0.0 - a_im * b_re;
        if (static_cast<int>(z_tmp) + 2 <= hnRows) {
          i = static_cast<int>(static_cast<double>(u0) / 2.0) + 2;
          for (xidx = i; xidx <= hnRows; xidx++) {
            ytmp->data[xidx - 1].re = 0.0;
            ytmp->data[xidx - 1].im = 0.0;
          }
        }
      } else {
        if (static_cast<int>(z_tmp) + 1 <= hnRows) {
          i = static_cast<int>(static_cast<double>(u0) / 2.0) + 1;
          for (xidx = i; xidx <= hnRows; xidx++) {
            ytmp->data[xidx - 1].re = 0.0;
            ytmp->data[xidx - 1].im = 0.0;
          }
        }
      }

      emxInit_creal_T(&fv, 1);
      emxInit_creal_T(&b_fv, 1);
      idx = static_cast<int>(static_cast<double>(nfft) / 2.0);
      FFTImplementationCallback::r2br_r2dit_trig_impl((ytmp), (idx), (unusedU0),
        (hsintab), (fv));
      FFTImplementationCallback::r2br_r2dit_trig_impl((wwc), (idx), (unusedU0),
        (hsintab), (b_fv));
      i = b_fv->size[0];
      b_fv->size[0] = fv->size[0];
      emxEnsureCapacity_creal_T(b_fv, i);
      xidx = fv->size[0];
      emxFree_real_T(&hsintab);
      emxFree_real_T(&unusedU0);
      for (i = 0; i < xidx; i++) {
        z_tmp = fv->data[i].re * b_fv->data[i].im + fv->data[i].im * b_fv->
          data[i].re;
        b_fv->data[i].re = fv->data[i].re * b_fv->data[i].re - fv->data[i].im *
          b_fv->data[i].im;
        b_fv->data[i].im = z_tmp;
      }

      FFTImplementationCallback::r2br_r2dit_trig_impl((b_fv), (idx), (hcostabinv),
        (hsintabinv), (fv));
      emxFree_creal_T(&b_fv);
      emxFree_real_T(&hsintabinv);
      emxFree_real_T(&hcostabinv);
      if (fv->size[0] > 1) {
        z_tmp = 1.0 / static_cast<double>(fv->size[0]);
        xidx = fv->size[0];
        for (i = 0; i < xidx; i++) {
          fv->data[i].re *= z_tmp;
          fv->data[i].im *= z_tmp;
        }
      }

      idx = 0;
      i = wwc->size[0];
      for (xidx = hnRows; xidx <= i; xidx++) {
        ytmp->data[idx].re = wwc->data[xidx - 1].re * fv->data[xidx - 1].re +
          wwc->data[xidx - 1].im * fv->data[xidx - 1].im;
        ytmp->data[idx].im = wwc->data[xidx - 1].re * fv->data[xidx - 1].im -
          wwc->data[xidx - 1].im * fv->data[xidx - 1].re;
        idx++;
      }

      emxFree_creal_T(&fv);
      for (xidx = 0; xidx < hnRows; xidx++) {
        i = wrapIndex->data[xidx];
        z_tmp = ytmp->data[i - 1].re;
        a_re = -ytmp->data[i - 1].im;
        y->data[xidx].re = 0.5 * ((ytmp->data[xidx].re * reconVar1->data[xidx].
          re - ytmp->data[xidx].im * reconVar1->data[xidx].im) + (z_tmp *
          reconVar2->data[xidx].re - a_re * reconVar2->data[xidx].im));
        y->data[xidx].im = 0.5 * ((ytmp->data[xidx].re * reconVar1->data[xidx].
          im + ytmp->data[xidx].im * reconVar1->data[xidx].re) + (z_tmp *
          reconVar2->data[xidx].im + a_re * reconVar2->data[xidx].re));
        i = hnRows + xidx;
        y->data[i].re = 0.5 * ((ytmp->data[xidx].re * reconVar2->data[xidx].re -
          ytmp->data[xidx].im * reconVar2->data[xidx].im) + (z_tmp *
          reconVar1->data[xidx].re - a_re * reconVar1->data[xidx].im));
        y->data[i].im = 0.5 * ((ytmp->data[xidx].re * reconVar2->data[xidx].im +
          ytmp->data[xidx].im * reconVar2->data[xidx].re) + (z_tmp *
          reconVar1->data[xidx].im + a_re * reconVar1->data[xidx].re));
      }

      emxFree_creal_T(&reconVar2);
      emxFree_creal_T(&reconVar1);
      emxFree_int32_T(&wrapIndex);
      emxFree_creal_T(&ytmp);
    }

    //
    // Arguments    : const emxArray_real_T *x
    //                emxArray_creal_T *y
    //                int unsigned_nRows
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    // Return Type  : void
    //
    void FFTImplementationCallback::doHalfLengthRadix2(const emxArray_real_T *x,
      emxArray_creal_T *y, int unsigned_nRows, const emxArray_real_T *costab,
      const emxArray_real_T *sintab)
    {
      emxArray_creal_T *reconVar1;
      emxArray_creal_T *reconVar2;
      emxArray_int32_T *bitrevIndex;
      emxArray_int32_T *wrapIndex;
      emxArray_real_T *hcostab;
      emxArray_real_T *hsintab;
      double temp_im;
      double temp_re;
      double z;
      int hszCostab;
      int i;
      int iDelta;
      int istart;
      int j;
      int k;
      int nRows;
      int nRowsD2;
      boolean_T nxeven;
      emxInit_real_T(&hcostab, 2);
      emxInit_real_T(&hsintab, 2);
      nRows = unsigned_nRows / 2;
      istart = nRows - 2;
      nRowsD2 = nRows / 2;
      k = nRowsD2 / 2;
      hszCostab = costab->size[1] / 2;
      iDelta = hcostab->size[0] * hcostab->size[1];
      hcostab->size[0] = 1;
      hcostab->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hcostab, iDelta);
      iDelta = hsintab->size[0] * hsintab->size[1];
      hsintab->size[0] = 1;
      hsintab->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hsintab, iDelta);
      for (i = 0; i < hszCostab; i++) {
        iDelta = ((i + 1) << 1) - 2;
        hcostab->data[i] = costab->data[iDelta];
        hsintab->data[i] = sintab->data[iDelta];
      }

      emxInit_int32_T(&wrapIndex, 2);
      emxInit_creal_T(&reconVar1, 1);
      emxInit_creal_T(&reconVar2, 1);
      iDelta = reconVar1->size[0];
      reconVar1->size[0] = nRows;
      emxEnsureCapacity_creal_T(reconVar1, iDelta);
      iDelta = reconVar2->size[0];
      reconVar2->size[0] = nRows;
      emxEnsureCapacity_creal_T(reconVar2, iDelta);
      iDelta = wrapIndex->size[0] * wrapIndex->size[1];
      wrapIndex->size[0] = 1;
      wrapIndex->size[1] = nRows;
      emxEnsureCapacity_int32_T(wrapIndex, iDelta);
      for (i = 0; i < nRows; i++) {
        temp_im = sintab->data[i];
        temp_re = costab->data[i];
        reconVar1->data[i].re = temp_im + 1.0;
        reconVar1->data[i].im = -temp_re;
        reconVar2->data[i].re = 1.0 - temp_im;
        reconVar2->data[i].im = temp_re;
        if (i + 1 != 1) {
          wrapIndex->data[i] = (nRows - i) + 1;
        } else {
          wrapIndex->data[0] = 1;
        }
      }

      emxInit_int32_T(&bitrevIndex, 1);
      z = static_cast<double>(unsigned_nRows) / 2.0;
      iDelta = y->size[0];
      if (iDelta >= nRows) {
        iDelta = nRows;
      }

      FFTImplementationCallback::get_bitrevIndex((iDelta - 1), (static_cast<int>
        (z)), (bitrevIndex));
      if ((x->size[0] & 1) == 0) {
        nxeven = true;
        iDelta = x->size[0];
      } else if (x->size[0] >= unsigned_nRows) {
        nxeven = true;
        iDelta = unsigned_nRows;
      } else {
        nxeven = false;
        iDelta = x->size[0] - 1;
      }

      hszCostab = 0;
      if (iDelta >= unsigned_nRows) {
        iDelta = unsigned_nRows;
      }

      temp_im = static_cast<double>(iDelta) / 2.0;
      iDelta = static_cast<int>(temp_im);
      for (i = 0; i < iDelta; i++) {
        y->data[bitrevIndex->data[i] - 1].re = x->data[hszCostab];
        y->data[bitrevIndex->data[i] - 1].im = x->data[hszCostab + 1];
        hszCostab += 2;
      }

      if (!nxeven) {
        iDelta = bitrevIndex->data[static_cast<int>(temp_im)] - 1;
        y->data[iDelta].re = x->data[hszCostab];
        y->data[iDelta].im = 0.0;
      }

      emxFree_int32_T(&bitrevIndex);
      if (nRows > 1) {
        for (i = 0; i <= istart; i += 2) {
          temp_re = y->data[i + 1].re;
          temp_im = y->data[i + 1].im;
          y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
          y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }
      }

      iDelta = 2;
      hszCostab = 4;
      nRows = ((k - 1) << 2) + 1;
      while (k > 0) {
        int temp_re_tmp;
        for (i = 0; i < nRows; i += hszCostab) {
          temp_re_tmp = i + iDelta;
          temp_re = y->data[temp_re_tmp].re;
          temp_im = y->data[temp_re_tmp].im;
          y->data[temp_re_tmp].re = y->data[i].re - temp_re;
          y->data[temp_re_tmp].im = y->data[i].im - temp_im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }

        istart = 1;
        for (j = k; j < nRowsD2; j += k) {
          double twid_im;
          double twid_re;
          int ihi;
          twid_re = hcostab->data[j];
          twid_im = hsintab->data[j];
          i = istart;
          ihi = istart + nRows;
          while (i < ihi) {
            temp_re_tmp = i + iDelta;
            temp_re = twid_re * y->data[temp_re_tmp].re - twid_im * y->
              data[temp_re_tmp].im;
            temp_im = twid_re * y->data[temp_re_tmp].im + twid_im * y->
              data[temp_re_tmp].re;
            y->data[temp_re_tmp].re = y->data[i].re - temp_re;
            y->data[temp_re_tmp].im = y->data[i].im - temp_im;
            y->data[i].re += temp_re;
            y->data[i].im += temp_im;
            i += hszCostab;
          }

          istart++;
        }

        k /= 2;
        iDelta = hszCostab;
        hszCostab += hszCostab;
        nRows -= iDelta;
      }

      emxFree_real_T(&hsintab);
      emxFree_real_T(&hcostab);
      FFTImplementationCallback::getback_radix2_fft((y), (reconVar1), (reconVar2),
        (wrapIndex), (static_cast<int>(z)));
      emxFree_creal_T(&reconVar2);
      emxFree_creal_T(&reconVar1);
      emxFree_int32_T(&wrapIndex);
    }

    //
    // Arguments    : int nRowsM1
    //                int nfftLen
    //                emxArray_int32_T *bitrevIndex
    // Return Type  : void
    //
    void FFTImplementationCallback::get_bitrevIndex(int nRowsM1, int nfftLen,
      emxArray_int32_T *bitrevIndex)
    {
      int b_j1;
      int iy;
      int ju;
      ju = 0;
      iy = 1;
      b_j1 = bitrevIndex->size[0];
      bitrevIndex->size[0] = nfftLen;
      emxEnsureCapacity_int32_T(bitrevIndex, b_j1);
      for (b_j1 = 0; b_j1 < nfftLen; b_j1++) {
        bitrevIndex->data[b_j1] = 0;
      }

      for (b_j1 = 0; b_j1 < nRowsM1; b_j1++) {
        boolean_T tst;
        bitrevIndex->data[b_j1] = iy;
        iy = nfftLen;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju + 1;
      }

      bitrevIndex->data[nRowsM1] = iy;
    }

    //
    // Arguments    : const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                const emxArray_real_T *costabinv
    //                const emxArray_real_T *sintabinv
    //                emxArray_real_T *hcostab
    //                emxArray_real_T *hsintab
    //                emxArray_real_T *hcostabinv
    //                emxArray_real_T *hsintabinv
    // Return Type  : void
    //
    void FFTImplementationCallback::get_half_twiddle_tables(const
      emxArray_real_T *costab, const emxArray_real_T *sintab, const
      emxArray_real_T *costabinv, const emxArray_real_T *sintabinv,
      emxArray_real_T *hcostab, emxArray_real_T *hsintab, emxArray_real_T
      *hcostabinv, emxArray_real_T *hsintabinv)
    {
      int hcostab_tmp;
      int hszCostab;
      hszCostab = costab->size[1] / 2;
      hcostab_tmp = hcostab->size[0] * hcostab->size[1];
      hcostab->size[0] = 1;
      hcostab->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hcostab, hcostab_tmp);
      hcostab_tmp = hsintab->size[0] * hsintab->size[1];
      hsintab->size[0] = 1;
      hsintab->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hsintab, hcostab_tmp);
      hcostab_tmp = hcostabinv->size[0] * hcostabinv->size[1];
      hcostabinv->size[0] = 1;
      hcostabinv->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hcostabinv, hcostab_tmp);
      hcostab_tmp = hsintabinv->size[0] * hsintabinv->size[1];
      hsintabinv->size[0] = 1;
      hsintabinv->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hsintabinv, hcostab_tmp);
      for (int i = 0; i < hszCostab; i++) {
        hcostab_tmp = ((i + 1) << 1) - 2;
        hcostab->data[i] = costab->data[hcostab_tmp];
        hsintab->data[i] = sintab->data[hcostab_tmp];
        hcostabinv->data[i] = costabinv->data[hcostab_tmp];
        hsintabinv->data[i] = sintabinv->data[hcostab_tmp];
      }
    }

    //
    // Arguments    : emxArray_creal_T *y
    //                const emxArray_creal_T *reconVar1
    //                const emxArray_creal_T *reconVar2
    //                const emxArray_int32_T *wrapIndex
    //                int hnRows
    // Return Type  : void
    //
    void FFTImplementationCallback::getback_radix2_fft(emxArray_creal_T *y,
      const emxArray_creal_T *reconVar1, const emxArray_creal_T *reconVar2,
      const emxArray_int32_T *wrapIndex, int hnRows)
    {
      double b_y_im;
      double temp1_im;
      double temp1_re;
      double temp2_im;
      double y_im;
      double y_im_tmp;
      int b_i;
      int iterVar;
      iterVar = hnRows / 2;
      temp1_re = y->data[0].re;
      temp1_im = y->data[0].im;
      y_im = y->data[0].re * reconVar1->data[0].im + y->data[0].im *
        reconVar1->data[0].re;
      temp2_im = y->data[0].re;
      b_y_im = -y->data[0].im;
      y->data[0].re = 0.5 * ((y->data[0].re * reconVar1->data[0].re - y->data[0]
        .im * reconVar1->data[0].im) + (temp2_im * reconVar2->data[0].re -
        b_y_im * reconVar2->data[0].im));
      y->data[0].im = 0.5 * (y_im + (temp2_im * reconVar2->data[0].im + b_y_im *
        reconVar2->data[0].re));
      y->data[hnRows].re = 0.5 * ((temp1_re * reconVar2->data[0].re - temp1_im *
        reconVar2->data[0].im) + (temp1_re * reconVar1->data[0].re - -temp1_im *
        reconVar1->data[0].im));
      y->data[hnRows].im = 0.5 * ((temp1_re * reconVar2->data[0].im + temp1_im *
        reconVar2->data[0].re) + (temp1_re * reconVar1->data[0].im + -temp1_im *
        reconVar1->data[0].re));
      for (int i = 2; i <= iterVar; i++) {
        int i1;
        temp1_re = y->data[i - 1].re;
        temp1_im = y->data[i - 1].im;
        b_i = wrapIndex->data[i - 1];
        b_y_im = y->data[b_i - 1].re;
        temp2_im = y->data[b_i - 1].im;
        y_im = y->data[i - 1].re * reconVar1->data[i - 1].im + y->data[i - 1].im
          * reconVar1->data[i - 1].re;
        y_im_tmp = -y->data[b_i - 1].im;
        y->data[i - 1].re = 0.5 * ((y->data[i - 1].re * reconVar1->data[i - 1].
          re - y->data[i - 1].im * reconVar1->data[i - 1].im) + (b_y_im *
          reconVar2->data[i - 1].re - y_im_tmp * reconVar2->data[i - 1].im));
        y->data[i - 1].im = 0.5 * (y_im + (b_y_im * reconVar2->data[i - 1].im +
          y_im_tmp * reconVar2->data[i - 1].re));
        i1 = (hnRows + i) - 1;
        y->data[i1].re = 0.5 * ((temp1_re * reconVar2->data[i - 1].re - temp1_im
          * reconVar2->data[i - 1].im) + (b_y_im * reconVar1->data[i - 1].re -
          -temp2_im * reconVar1->data[i - 1].im));
        y->data[i1].im = 0.5 * ((temp1_re * reconVar2->data[i - 1].im + temp1_im
          * reconVar2->data[i - 1].re) + (b_y_im * reconVar1->data[i - 1].im +
          -temp2_im * reconVar1->data[i - 1].re));
        y->data[b_i - 1].re = 0.5 * ((b_y_im * reconVar1->data[b_i - 1].re -
          temp2_im * reconVar1->data[b_i - 1].im) + (temp1_re * reconVar2->
          data[b_i - 1].re - -temp1_im * reconVar2->data[b_i - 1].im));
        y->data[b_i - 1].im = 0.5 * ((b_y_im * reconVar1->data[b_i - 1].im +
          temp2_im * reconVar1->data[b_i - 1].re) + (temp1_re * reconVar2->
          data[b_i - 1].im + -temp1_im * reconVar2->data[b_i - 1].re));
        i1 = (b_i + hnRows) - 1;
        y->data[i1].re = 0.5 * ((b_y_im * reconVar2->data[b_i - 1].re - temp2_im
          * reconVar2->data[b_i - 1].im) + (temp1_re * reconVar1->data[b_i - 1].
          re - -temp1_im * reconVar1->data[b_i - 1].im));
        y->data[i1].im = 0.5 * ((b_y_im * reconVar2->data[b_i - 1].im + temp2_im
          * reconVar2->data[b_i - 1].re) + (temp1_re * reconVar1->data[b_i - 1].
          im + -temp1_im * reconVar1->data[b_i - 1].re));
      }

      if (iterVar != 0) {
        temp2_im = y->data[iterVar].re;
        temp1_im = y->data[iterVar].im;
        y_im = y->data[iterVar].re * reconVar1->data[iterVar].im + y->
          data[iterVar].im * reconVar1->data[iterVar].re;
        y_im_tmp = -y->data[iterVar].im;
        y->data[iterVar].re = 0.5 * ((y->data[iterVar].re * reconVar1->
          data[iterVar].re - y->data[iterVar].im * reconVar1->data[iterVar].im)
          + (temp2_im * reconVar2->data[iterVar].re - y_im_tmp * reconVar2->
             data[iterVar].im));
        y->data[iterVar].im = 0.5 * (y_im + (temp2_im * reconVar2->data[iterVar]
          .im + y_im_tmp * reconVar2->data[iterVar].re));
        b_i = hnRows + iterVar;
        y->data[b_i].re = 0.5 * ((temp2_im * reconVar2->data[iterVar].re -
          temp1_im * reconVar2->data[iterVar].im) + (temp2_im * reconVar1->
          data[iterVar].re - -temp1_im * reconVar1->data[iterVar].im));
        y->data[b_i].im = 0.5 * ((temp2_im * reconVar2->data[iterVar].im +
          temp1_im * reconVar2->data[iterVar].re) + (temp2_im * reconVar1->
          data[iterVar].im + -temp1_im * reconVar1->data[iterVar].re));
      }
    }

    //
    // Arguments    : int nRows
    //                boolean_T useRadix2
    //                emxArray_real_T *costab
    //                emxArray_real_T *sintab
    //                emxArray_real_T *sintabinv
    // Return Type  : void
    //
    void FFTImplementationCallback::c_generate_twiddle_tables(int nRows,
      boolean_T useRadix2, emxArray_real_T *costab, emxArray_real_T *sintab,
      emxArray_real_T *sintabinv)
    {
      emxArray_real_T *costab1q;
      double e;
      int i;
      int k;
      int n;
      int nd2;
      emxInit_real_T(&costab1q, 2);
      e = 6.2831853071795862 / static_cast<double>(nRows);
      n = nRows / 2 / 2;
      i = costab1q->size[0] * costab1q->size[1];
      costab1q->size[0] = 1;
      costab1q->size[1] = n + 1;
      emxEnsureCapacity_real_T(costab1q, i);
      costab1q->data[0] = 1.0;
      nd2 = n / 2 - 1;
      for (k = 0; k <= nd2; k++) {
        costab1q->data[k + 1] = std::cos(e * (static_cast<double>(k) + 1.0));
      }

      i = nd2 + 2;
      nd2 = n - 1;
      for (k = i; k <= nd2; k++) {
        costab1q->data[k] = std::sin(e * static_cast<double>(n - k));
      }

      costab1q->data[n] = 0.0;
      if (!useRadix2) {
        n = costab1q->size[1] - 1;
        nd2 = (costab1q->size[1] - 1) << 1;
        i = costab->size[0] * costab->size[1];
        costab->size[0] = 1;
        costab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(costab, i);
        i = sintab->size[0] * sintab->size[1];
        sintab->size[0] = 1;
        sintab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintab, i);
        costab->data[0] = 1.0;
        sintab->data[0] = 0.0;
        i = sintabinv->size[0] * sintabinv->size[1];
        sintabinv->size[0] = 1;
        sintabinv->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintabinv, i);
        for (k = 0; k < n; k++) {
          sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          sintabinv->data[k] = costab1q->data[k - n];
        }

        for (k = 0; k < n; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = -costab1q->data[k - n];
        }
      } else {
        n = costab1q->size[1] - 1;
        nd2 = (costab1q->size[1] - 1) << 1;
        i = costab->size[0] * costab->size[1];
        costab->size[0] = 1;
        costab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(costab, i);
        i = sintab->size[0] * sintab->size[1];
        sintab->size[0] = 1;
        sintab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintab, i);
        costab->data[0] = 1.0;
        sintab->data[0] = 0.0;
        for (k = 0; k < n; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = costab1q->data[(n - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = costab1q->data[k - n];
        }

        sintabinv->size[0] = 1;
        sintabinv->size[1] = 0;
      }

      emxFree_real_T(&costab1q);
    }

    //
    // Arguments    : const double x[1820]
    //                emxArray_creal_T *y
    //                int nRows
    //                int nfft
    //                const emxArray_creal_T *wwc
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                const emxArray_real_T *costabinv
    //                const emxArray_real_T *sintabinv
    // Return Type  : void
    //
    void FFTImplementationCallback::doHalfLengthBluestein(const double x[1820],
      emxArray_creal_T *y, int nRows, int nfft, const emxArray_creal_T *wwc,
      const emxArray_real_T *costab, const emxArray_real_T *sintab, const
      emxArray_real_T *costabinv, const emxArray_real_T *sintabinv)
    {
      emxArray_creal_T *b_fv;
      emxArray_creal_T *fv;
      emxArray_creal_T *reconVar1;
      emxArray_creal_T *reconVar2;
      emxArray_creal_T *ytmp;
      emxArray_int32_T *wrapIndex;
      emxArray_real_T *costable;
      emxArray_real_T *hcostabinv;
      emxArray_real_T *hsintab;
      emxArray_real_T *hsintabinv;
      emxArray_real_T *sintable;
      emxArray_real_T *unusedU0;
      double a_im;
      double a_re;
      int hnRows;
      int i;
      int idx;
      int minHnrowsNx;
      int xidx;
      emxInit_creal_T(&ytmp, 1);
      hnRows = nRows / 2;
      i = ytmp->size[0];
      ytmp->size[0] = hnRows;
      emxEnsureCapacity_creal_T(ytmp, i);
      if (hnRows > 1820) {
        i = ytmp->size[0];
        ytmp->size[0] = hnRows;
        emxEnsureCapacity_creal_T(ytmp, i);
        for (i = 0; i < hnRows; i++) {
          ytmp->data[i].re = 0.0;
          ytmp->data[i].im = 0.0;
        }
      }

      emxInit_real_T(&unusedU0, 2);
      emxInit_int32_T(&wrapIndex, 2);
      emxInit_real_T(&costable, 2);
      emxInit_real_T(&sintable, 2);
      emxInit_real_T(&hsintab, 2);
      emxInit_real_T(&hcostabinv, 2);
      emxInit_real_T(&hsintabinv, 2);
      emxInit_creal_T(&reconVar1, 1);
      emxInit_creal_T(&reconVar2, 1);
      if (1820 < nRows) {
        minHnrowsNx = 1820;
      } else {
        minHnrowsNx = nRows;
      }

      FFTImplementationCallback::b_generate_twiddle_tables((nRows << 1),
        (costable), (sintable), (unusedU0));
      FFTImplementationCallback::get_half_twiddle_tables((costab), (sintab),
        (costabinv), (sintabinv), (unusedU0), (hsintab), (hcostabinv),
        (hsintabinv));
      i = reconVar1->size[0];
      reconVar1->size[0] = hnRows;
      emxEnsureCapacity_creal_T(reconVar1, i);
      i = reconVar2->size[0];
      reconVar2->size[0] = hnRows;
      emxEnsureCapacity_creal_T(reconVar2, i);
      idx = 0;
      i = wrapIndex->size[0] * wrapIndex->size[1];
      wrapIndex->size[0] = 1;
      wrapIndex->size[1] = hnRows;
      emxEnsureCapacity_int32_T(wrapIndex, i);
      for (xidx = 0; xidx < hnRows; xidx++) {
        reconVar1->data[xidx].re = sintable->data[idx] + 1.0;
        reconVar1->data[xidx].im = -costable->data[idx];
        reconVar2->data[xidx].re = 1.0 - sintable->data[idx];
        reconVar2->data[xidx].im = costable->data[idx];
        idx += 2;
        if (xidx + 1 != 1) {
          wrapIndex->data[xidx] = (hnRows - xidx) + 1;
        } else {
          wrapIndex->data[0] = 1;
        }
      }

      emxFree_real_T(&sintable);
      emxFree_real_T(&costable);
      xidx = 1;
      i = static_cast<int>(static_cast<double>(minHnrowsNx) / 2.0);
      for (int k1 = 0; k1 < i; k1++) {
        double b_im;
        double b_re;
        idx = (hnRows + k1) - 1;
        a_re = wwc->data[idx].re;
        a_im = wwc->data[idx].im;
        b_re = x[xidx - 1];
        b_im = x[xidx];
        ytmp->data[k1].re = a_re * b_re + a_im * b_im;
        ytmp->data[k1].im = a_re * b_im - a_im * b_re;
        xidx += 2;
      }

      if (i + 1 <= hnRows) {
        i = static_cast<int>(static_cast<double>(minHnrowsNx) / 2.0) + 1;
        for (xidx = i; xidx <= hnRows; xidx++) {
          ytmp->data[xidx - 1].re = 0.0;
          ytmp->data[xidx - 1].im = 0.0;
        }
      }

      emxInit_creal_T(&fv, 1);
      emxInit_creal_T(&b_fv, 1);
      idx = static_cast<int>(static_cast<double>(nfft) / 2.0);
      FFTImplementationCallback::r2br_r2dit_trig_impl((ytmp), (idx), (unusedU0),
        (hsintab), (fv));
      FFTImplementationCallback::r2br_r2dit_trig_impl((wwc), (idx), (unusedU0),
        (hsintab), (b_fv));
      i = b_fv->size[0];
      b_fv->size[0] = fv->size[0];
      emxEnsureCapacity_creal_T(b_fv, i);
      xidx = fv->size[0];
      emxFree_real_T(&hsintab);
      emxFree_real_T(&unusedU0);
      for (i = 0; i < xidx; i++) {
        a_re = fv->data[i].re * b_fv->data[i].im + fv->data[i].im * b_fv->data[i]
          .re;
        b_fv->data[i].re = fv->data[i].re * b_fv->data[i].re - fv->data[i].im *
          b_fv->data[i].im;
        b_fv->data[i].im = a_re;
      }

      FFTImplementationCallback::r2br_r2dit_trig_impl((b_fv), (idx), (hcostabinv),
        (hsintabinv), (fv));
      emxFree_creal_T(&b_fv);
      emxFree_real_T(&hsintabinv);
      emxFree_real_T(&hcostabinv);
      if (fv->size[0] > 1) {
        a_re = 1.0 / static_cast<double>(fv->size[0]);
        xidx = fv->size[0];
        for (i = 0; i < xidx; i++) {
          fv->data[i].re *= a_re;
          fv->data[i].im *= a_re;
        }
      }

      idx = 0;
      i = wwc->size[0];
      for (xidx = hnRows; xidx <= i; xidx++) {
        ytmp->data[idx].re = wwc->data[xidx - 1].re * fv->data[xidx - 1].re +
          wwc->data[xidx - 1].im * fv->data[xidx - 1].im;
        ytmp->data[idx].im = wwc->data[xidx - 1].re * fv->data[xidx - 1].im -
          wwc->data[xidx - 1].im * fv->data[xidx - 1].re;
        idx++;
      }

      emxFree_creal_T(&fv);
      for (xidx = 0; xidx < hnRows; xidx++) {
        i = wrapIndex->data[xidx];
        a_re = ytmp->data[i - 1].re;
        a_im = -ytmp->data[i - 1].im;
        y->data[xidx].re = 0.5 * ((ytmp->data[xidx].re * reconVar1->data[xidx].
          re - ytmp->data[xidx].im * reconVar1->data[xidx].im) + (a_re *
          reconVar2->data[xidx].re - a_im * reconVar2->data[xidx].im));
        y->data[xidx].im = 0.5 * ((ytmp->data[xidx].re * reconVar1->data[xidx].
          im + ytmp->data[xidx].im * reconVar1->data[xidx].re) + (a_re *
          reconVar2->data[xidx].im + a_im * reconVar2->data[xidx].re));
        i = hnRows + xidx;
        y->data[i].re = 0.5 * ((ytmp->data[xidx].re * reconVar2->data[xidx].re -
          ytmp->data[xidx].im * reconVar2->data[xidx].im) + (a_re *
          reconVar1->data[xidx].re - a_im * reconVar1->data[xidx].im));
        y->data[i].im = 0.5 * ((ytmp->data[xidx].re * reconVar2->data[xidx].im +
          ytmp->data[xidx].im * reconVar2->data[xidx].re) + (a_re *
          reconVar1->data[xidx].im + a_im * reconVar1->data[xidx].re));
      }

      emxFree_creal_T(&reconVar2);
      emxFree_creal_T(&reconVar1);
      emxFree_int32_T(&wrapIndex);
      emxFree_creal_T(&ytmp);
    }

    //
    // Arguments    : const double x[1820]
    //                emxArray_creal_T *y
    //                int unsigned_nRows
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    // Return Type  : void
    //
    void FFTImplementationCallback::doHalfLengthRadix2(const double x[1820],
      emxArray_creal_T *y, int unsigned_nRows, const emxArray_real_T *costab,
      const emxArray_real_T *sintab)
    {
      emxArray_creal_T *reconVar1;
      emxArray_creal_T *reconVar2;
      emxArray_int32_T *bitrevIndex;
      emxArray_int32_T *wrapIndex;
      emxArray_real_T *hcostab;
      emxArray_real_T *hsintab;
      double temp_im;
      double temp_re;
      double z;
      int hszCostab;
      int i;
      int iDelta;
      int istart;
      int j;
      int k;
      int nRows;
      int nRowsD2;
      emxInit_real_T(&hcostab, 2);
      emxInit_real_T(&hsintab, 2);
      nRows = unsigned_nRows / 2;
      istart = nRows - 2;
      nRowsD2 = nRows / 2;
      k = nRowsD2 / 2;
      hszCostab = costab->size[1] / 2;
      iDelta = hcostab->size[0] * hcostab->size[1];
      hcostab->size[0] = 1;
      hcostab->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hcostab, iDelta);
      iDelta = hsintab->size[0] * hsintab->size[1];
      hsintab->size[0] = 1;
      hsintab->size[1] = hszCostab;
      emxEnsureCapacity_real_T(hsintab, iDelta);
      for (i = 0; i < hszCostab; i++) {
        iDelta = ((i + 1) << 1) - 2;
        hcostab->data[i] = costab->data[iDelta];
        hsintab->data[i] = sintab->data[iDelta];
      }

      emxInit_int32_T(&wrapIndex, 2);
      emxInit_creal_T(&reconVar1, 1);
      emxInit_creal_T(&reconVar2, 1);
      iDelta = reconVar1->size[0];
      reconVar1->size[0] = nRows;
      emxEnsureCapacity_creal_T(reconVar1, iDelta);
      iDelta = reconVar2->size[0];
      reconVar2->size[0] = nRows;
      emxEnsureCapacity_creal_T(reconVar2, iDelta);
      iDelta = wrapIndex->size[0] * wrapIndex->size[1];
      wrapIndex->size[0] = 1;
      wrapIndex->size[1] = nRows;
      emxEnsureCapacity_int32_T(wrapIndex, iDelta);
      for (i = 0; i < nRows; i++) {
        z = sintab->data[i];
        temp_re = costab->data[i];
        reconVar1->data[i].re = z + 1.0;
        reconVar1->data[i].im = -temp_re;
        reconVar2->data[i].re = 1.0 - z;
        reconVar2->data[i].im = temp_re;
        if (i + 1 != 1) {
          wrapIndex->data[i] = (nRows - i) + 1;
        } else {
          wrapIndex->data[0] = 1;
        }
      }

      emxInit_int32_T(&bitrevIndex, 1);
      z = static_cast<double>(unsigned_nRows) / 2.0;
      iDelta = y->size[0];
      if (iDelta >= nRows) {
        iDelta = nRows;
      }

      FFTImplementationCallback::get_bitrevIndex((iDelta - 1), (static_cast<int>
        (z)), (bitrevIndex));
      hszCostab = 0;
      if (1820 < unsigned_nRows) {
        iDelta = 1820;
      } else {
        iDelta = unsigned_nRows;
      }

      iDelta = static_cast<int>(static_cast<double>(iDelta) / 2.0);
      for (i = 0; i < iDelta; i++) {
        y->data[bitrevIndex->data[i] - 1].re = x[hszCostab];
        y->data[bitrevIndex->data[i] - 1].im = x[hszCostab + 1];
        hszCostab += 2;
      }

      emxFree_int32_T(&bitrevIndex);
      if (nRows > 1) {
        for (i = 0; i <= istart; i += 2) {
          temp_re = y->data[i + 1].re;
          temp_im = y->data[i + 1].im;
          y->data[i + 1].re = y->data[i].re - y->data[i + 1].re;
          y->data[i + 1].im = y->data[i].im - y->data[i + 1].im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }
      }

      iDelta = 2;
      hszCostab = 4;
      nRows = ((k - 1) << 2) + 1;
      while (k > 0) {
        int temp_re_tmp;
        for (i = 0; i < nRows; i += hszCostab) {
          temp_re_tmp = i + iDelta;
          temp_re = y->data[temp_re_tmp].re;
          temp_im = y->data[temp_re_tmp].im;
          y->data[temp_re_tmp].re = y->data[i].re - temp_re;
          y->data[temp_re_tmp].im = y->data[i].im - temp_im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }

        istart = 1;
        for (j = k; j < nRowsD2; j += k) {
          double twid_im;
          double twid_re;
          int ihi;
          twid_re = hcostab->data[j];
          twid_im = hsintab->data[j];
          i = istart;
          ihi = istart + nRows;
          while (i < ihi) {
            temp_re_tmp = i + iDelta;
            temp_re = twid_re * y->data[temp_re_tmp].re - twid_im * y->
              data[temp_re_tmp].im;
            temp_im = twid_re * y->data[temp_re_tmp].im + twid_im * y->
              data[temp_re_tmp].re;
            y->data[temp_re_tmp].re = y->data[i].re - temp_re;
            y->data[temp_re_tmp].im = y->data[i].im - temp_im;
            y->data[i].re += temp_re;
            y->data[i].im += temp_im;
            i += hszCostab;
          }

          istart++;
        }

        k /= 2;
        iDelta = hszCostab;
        hszCostab += hszCostab;
        nRows -= iDelta;
      }

      emxFree_real_T(&hsintab);
      emxFree_real_T(&hcostab);
      FFTImplementationCallback::getback_radix2_fft((y), (reconVar1), (reconVar2),
        (wrapIndex), (static_cast<int>(z)));
      emxFree_creal_T(&reconVar2);
      emxFree_creal_T(&reconVar1);
      emxFree_int32_T(&wrapIndex);
    }

    //
    // Arguments    : const emxArray_creal_T *x
    //                int n2blue
    //                int nfft
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                const emxArray_real_T *sintabinv
    //                emxArray_creal_T *y
    // Return Type  : void
    //
    void FFTImplementationCallback::dobluesteinfft(const emxArray_creal_T *x,
      int n2blue, int nfft, const emxArray_real_T *costab, const emxArray_real_T
      *sintab, const emxArray_real_T *sintabinv, emxArray_creal_T *y)
    {
      emxArray_creal_T *b_fv;
      emxArray_creal_T *fv;
      emxArray_creal_T *wwc;
      double nt_im;
      double nt_re;
      int b_y;
      int idx;
      int k;
      int nInt2;
      int nInt2m1;
      int rt;
      emxInit_creal_T(&wwc, 1);
      nInt2m1 = (nfft + nfft) - 1;
      nInt2 = wwc->size[0];
      wwc->size[0] = nInt2m1;
      emxEnsureCapacity_creal_T(wwc, nInt2);
      idx = nfft;
      rt = 0;
      wwc->data[nfft - 1].re = 1.0;
      wwc->data[nfft - 1].im = 0.0;
      nInt2 = nfft << 1;
      for (k = 0; k <= nfft - 2; k++) {
        b_y = ((k + 1) << 1) - 1;
        if (nInt2 - rt <= b_y) {
          rt += b_y - nInt2;
        } else {
          rt += b_y;
        }

        nt_im = 3.1415926535897931 * static_cast<double>(rt) / static_cast<
          double>(nfft);
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
      nInt2 = nInt2m1 - 1;
      for (k = nInt2; k >= nfft; k--) {
        wwc->data[k] = wwc->data[idx];
        idx++;
      }

      nInt2 = y->size[0];
      y->size[0] = nfft;
      emxEnsureCapacity_creal_T(y, nInt2);
      if (nfft > x->size[0]) {
        nInt2 = y->size[0];
        y->size[0] = nfft;
        emxEnsureCapacity_creal_T(y, nInt2);
        for (nInt2 = 0; nInt2 < nfft; nInt2++) {
          y->data[nInt2].re = 0.0;
          y->data[nInt2].im = 0.0;
        }
      }

      rt = x->size[0];
      if (nfft < rt) {
        rt = nfft;
      }

      nInt2 = 0;
      for (k = 0; k < rt; k++) {
        b_y = (nfft + k) - 1;
        nt_re = wwc->data[b_y].re;
        nt_im = wwc->data[b_y].im;
        y->data[k].re = nt_re * x->data[nInt2].re + nt_im * x->data[nInt2].im;
        y->data[k].im = nt_re * x->data[nInt2].im - nt_im * x->data[nInt2].re;
        nInt2++;
      }

      nInt2 = rt + 1;
      for (k = nInt2; k <= nfft; k++) {
        y->data[k - 1].re = 0.0;
        y->data[k - 1].im = 0.0;
      }

      emxInit_creal_T(&fv, 1);
      emxInit_creal_T(&b_fv, 1);
      FFTImplementationCallback::r2br_r2dit_trig_impl((y), (n2blue), (costab),
        (sintab), (fv));
      FFTImplementationCallback::r2br_r2dit_trig_impl((wwc), (n2blue), (costab),
        (sintab), (b_fv));
      nInt2 = b_fv->size[0];
      b_fv->size[0] = fv->size[0];
      emxEnsureCapacity_creal_T(b_fv, nInt2);
      rt = fv->size[0];
      for (nInt2 = 0; nInt2 < rt; nInt2++) {
        nt_re = fv->data[nInt2].re * b_fv->data[nInt2].im + fv->data[nInt2].im *
          b_fv->data[nInt2].re;
        b_fv->data[nInt2].re = fv->data[nInt2].re * b_fv->data[nInt2].re -
          fv->data[nInt2].im * b_fv->data[nInt2].im;
        b_fv->data[nInt2].im = nt_re;
      }

      FFTImplementationCallback::r2br_r2dit_trig_impl((b_fv), (n2blue), (costab),
        (sintabinv), (fv));
      emxFree_creal_T(&b_fv);
      if (fv->size[0] > 1) {
        nt_re = 1.0 / static_cast<double>(fv->size[0]);
        rt = fv->size[0];
        for (nInt2 = 0; nInt2 < rt; nInt2++) {
          fv->data[nInt2].re *= nt_re;
          fv->data[nInt2].im *= nt_re;
        }
      }

      idx = 0;
      nInt2 = wwc->size[0];
      for (k = nfft; k <= nInt2; k++) {
        double ai;
        y->data[idx].re = wwc->data[k - 1].re * fv->data[k - 1].re + wwc->data[k
          - 1].im * fv->data[k - 1].im;
        y->data[idx].im = wwc->data[k - 1].re * fv->data[k - 1].im - wwc->data[k
          - 1].im * fv->data[k - 1].re;
        nt_re = y->data[idx].re;
        ai = y->data[idx].im;
        if (ai == 0.0) {
          nt_im = nt_re / static_cast<double>(nfft);
          nt_re = 0.0;
        } else if (nt_re == 0.0) {
          nt_im = 0.0;
          nt_re = ai / static_cast<double>(nfft);
        } else {
          nt_im = nt_re / static_cast<double>(nfft);
          nt_re = ai / static_cast<double>(nfft);
        }

        y->data[idx].re = nt_im;
        y->data[idx].im = nt_re;
        idx++;
      }

      emxFree_creal_T(&fv);
      emxFree_creal_T(&wwc);
    }

    //
    // Arguments    : const emxArray_real_T *x
    //                int n2blue
    //                int nfft
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                const emxArray_real_T *sintabinv
    //                emxArray_creal_T *y
    // Return Type  : void
    //
    void FFTImplementationCallback::dobluesteinfft(const emxArray_real_T *x, int
      n2blue, int nfft, const emxArray_real_T *costab, const emxArray_real_T
      *sintab, const emxArray_real_T *sintabinv, emxArray_creal_T *y)
    {
      emxArray_creal_T *b_fv;
      emxArray_creal_T *fv;
      emxArray_creal_T *wwc;
      double nt_re;
      int b_y;
      int idx;
      int k;
      int nInt2;
      int rt;
      boolean_T ishalflength;
      if ((nfft != 1) && ((nfft & 1) == 0)) {
        ishalflength = true;
      } else {
        ishalflength = false;
      }

      emxInit_creal_T(&wwc, 1);
      if (ishalflength) {
        int nInt2m1;
        int nRows;
        nRows = nfft / 2;
        nInt2m1 = (nRows + nRows) - 1;
        nInt2 = wwc->size[0];
        wwc->size[0] = nInt2m1;
        emxEnsureCapacity_creal_T(wwc, nInt2);
        idx = nRows;
        rt = 0;
        wwc->data[nRows - 1].re = 1.0;
        wwc->data[nRows - 1].im = 0.0;
        nInt2 = nRows << 1;
        for (k = 0; k <= nRows - 2; k++) {
          double nt_im;
          b_y = ((k + 1) << 1) - 1;
          if (nInt2 - rt <= b_y) {
            rt += b_y - nInt2;
          } else {
            rt += b_y;
          }

          nt_im = -3.1415926535897931 * static_cast<double>(rt) / static_cast<
            double>(nRows);
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
        nInt2 = nInt2m1 - 1;
        for (k = nInt2; k >= nRows; k--) {
          wwc->data[k] = wwc->data[idx];
          idx++;
        }
      } else {
        int nInt2m1;
        nInt2m1 = (nfft + nfft) - 1;
        nInt2 = wwc->size[0];
        wwc->size[0] = nInt2m1;
        emxEnsureCapacity_creal_T(wwc, nInt2);
        idx = nfft;
        rt = 0;
        wwc->data[nfft - 1].re = 1.0;
        wwc->data[nfft - 1].im = 0.0;
        nInt2 = nfft << 1;
        for (k = 0; k <= nfft - 2; k++) {
          double nt_im;
          b_y = ((k + 1) << 1) - 1;
          if (nInt2 - rt <= b_y) {
            rt += b_y - nInt2;
          } else {
            rt += b_y;
          }

          nt_im = -3.1415926535897931 * static_cast<double>(rt) / static_cast<
            double>(nfft);
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
        nInt2 = nInt2m1 - 1;
        for (k = nInt2; k >= nfft; k--) {
          wwc->data[k] = wwc->data[idx];
          idx++;
        }
      }

      nInt2 = y->size[0];
      y->size[0] = nfft;
      emxEnsureCapacity_creal_T(y, nInt2);
      if (nfft > x->size[0]) {
        nInt2 = y->size[0];
        y->size[0] = nfft;
        emxEnsureCapacity_creal_T(y, nInt2);
        for (nInt2 = 0; nInt2 < nfft; nInt2++) {
          y->data[nInt2].re = 0.0;
          y->data[nInt2].im = 0.0;
        }
      }

      if ((n2blue != 1) && ((nfft & 1) == 0)) {
        FFTImplementationCallback::doHalfLengthBluestein((x), (y), (x->size[0]),
          (nfft), (n2blue), (wwc), (costab), (sintab), (costab), (sintabinv));
      } else {
        rt = x->size[0];
        if (nfft < rt) {
          rt = nfft;
        }

        nInt2 = 0;
        for (k = 0; k < rt; k++) {
          b_y = (nfft + k) - 1;
          y->data[k].re = wwc->data[b_y].re * x->data[nInt2];
          y->data[k].im = wwc->data[b_y].im * -x->data[nInt2];
          nInt2++;
        }

        nInt2 = rt + 1;
        for (k = nInt2; k <= nfft; k++) {
          y->data[k - 1].re = 0.0;
          y->data[k - 1].im = 0.0;
        }

        emxInit_creal_T(&fv, 1);
        emxInit_creal_T(&b_fv, 1);
        FFTImplementationCallback::r2br_r2dit_trig_impl((y), (n2blue), (costab),
          (sintab), (fv));
        FFTImplementationCallback::r2br_r2dit_trig_impl((wwc), (n2blue), (costab),
          (sintab), (b_fv));
        nInt2 = b_fv->size[0];
        b_fv->size[0] = fv->size[0];
        emxEnsureCapacity_creal_T(b_fv, nInt2);
        rt = fv->size[0];
        for (nInt2 = 0; nInt2 < rt; nInt2++) {
          nt_re = fv->data[nInt2].re * b_fv->data[nInt2].im + fv->data[nInt2].im
            * b_fv->data[nInt2].re;
          b_fv->data[nInt2].re = fv->data[nInt2].re * b_fv->data[nInt2].re -
            fv->data[nInt2].im * b_fv->data[nInt2].im;
          b_fv->data[nInt2].im = nt_re;
        }

        FFTImplementationCallback::r2br_r2dit_trig_impl((b_fv), (n2blue),
          (costab), (sintabinv), (fv));
        emxFree_creal_T(&b_fv);
        if (fv->size[0] > 1) {
          nt_re = 1.0 / static_cast<double>(fv->size[0]);
          rt = fv->size[0];
          for (nInt2 = 0; nInt2 < rt; nInt2++) {
            fv->data[nInt2].re *= nt_re;
            fv->data[nInt2].im *= nt_re;
          }
        }

        idx = 0;
        nInt2 = wwc->size[0];
        for (k = nfft; k <= nInt2; k++) {
          y->data[idx].re = wwc->data[k - 1].re * fv->data[k - 1].re + wwc->
            data[k - 1].im * fv->data[k - 1].im;
          y->data[idx].im = wwc->data[k - 1].re * fv->data[k - 1].im - wwc->
            data[k - 1].im * fv->data[k - 1].re;
          idx++;
        }

        emxFree_creal_T(&fv);
      }

      emxFree_creal_T(&wwc);
    }

    //
    // Arguments    : int nRows
    //                boolean_T useRadix2
    //                emxArray_real_T *costab
    //                emxArray_real_T *sintab
    //                emxArray_real_T *sintabinv
    // Return Type  : void
    //
    void FFTImplementationCallback::generate_twiddle_tables(int nRows, boolean_T
      useRadix2, emxArray_real_T *costab, emxArray_real_T *sintab,
      emxArray_real_T *sintabinv)
    {
      emxArray_real_T *costab1q;
      double e;
      int i;
      int k;
      int n;
      int nd2;
      emxInit_real_T(&costab1q, 2);
      e = 6.2831853071795862 / static_cast<double>(nRows);
      n = nRows / 2 / 2;
      i = costab1q->size[0] * costab1q->size[1];
      costab1q->size[0] = 1;
      costab1q->size[1] = n + 1;
      emxEnsureCapacity_real_T(costab1q, i);
      costab1q->data[0] = 1.0;
      nd2 = n / 2 - 1;
      for (k = 0; k <= nd2; k++) {
        costab1q->data[k + 1] = std::cos(e * (static_cast<double>(k) + 1.0));
      }

      i = nd2 + 2;
      nd2 = n - 1;
      for (k = i; k <= nd2; k++) {
        costab1q->data[k] = std::sin(e * static_cast<double>(n - k));
      }

      costab1q->data[n] = 0.0;
      if (!useRadix2) {
        n = costab1q->size[1] - 1;
        nd2 = (costab1q->size[1] - 1) << 1;
        i = costab->size[0] * costab->size[1];
        costab->size[0] = 1;
        costab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(costab, i);
        i = sintab->size[0] * sintab->size[1];
        sintab->size[0] = 1;
        sintab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintab, i);
        costab->data[0] = 1.0;
        sintab->data[0] = 0.0;
        i = sintabinv->size[0] * sintabinv->size[1];
        sintabinv->size[0] = 1;
        sintabinv->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintabinv, i);
        for (k = 0; k < n; k++) {
          sintabinv->data[k + 1] = costab1q->data[(n - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          sintabinv->data[k] = costab1q->data[k - n];
        }

        for (k = 0; k < n; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = -costab1q->data[k - n];
        }
      } else {
        n = costab1q->size[1] - 1;
        nd2 = (costab1q->size[1] - 1) << 1;
        i = costab->size[0] * costab->size[1];
        costab->size[0] = 1;
        costab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(costab, i);
        i = sintab->size[0] * sintab->size[1];
        sintab->size[0] = 1;
        sintab->size[1] = nd2 + 1;
        emxEnsureCapacity_real_T(sintab, i);
        costab->data[0] = 1.0;
        sintab->data[0] = 0.0;
        for (k = 0; k < n; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = -costab1q->data[(n - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = -costab1q->data[k - n];
        }

        sintabinv->size[0] = 1;
        sintabinv->size[1] = 0;
      }

      emxFree_real_T(&costab1q);
    }

    //
    // Arguments    : int nfft
    //                boolean_T useRadix2
    //                int *n2blue
    //                int *nRows
    // Return Type  : void
    //
    void FFTImplementationCallback::get_algo_sizes(int nfft, boolean_T useRadix2,
      int *n2blue, int *nRows)
    {
      *n2blue = 1;
      if (useRadix2) {
        *nRows = nfft;
      } else {
        if (nfft > 0) {
          int n;
          int pmax;
          n = (nfft + nfft) - 1;
          pmax = 31;
          if (n <= 1) {
            pmax = 0;
          } else {
            int pmin;
            boolean_T exitg1;
            pmin = 0;
            exitg1 = false;
            while ((!exitg1) && (pmax - pmin > 1)) {
              int k;
              int pow2p;
              k = (pmin + pmax) >> 1;
              pow2p = 1 << k;
              if (pow2p == n) {
                pmax = k;
                exitg1 = true;
              } else if (pow2p > n) {
                pmax = k;
              } else {
                pmin = k;
              }
            }
          }

          *n2blue = 1 << pmax;
        }

        *nRows = *n2blue;
      }
    }

    //
    // Arguments    : const emxArray_real_T *x
    //                int n1_unsigned
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                emxArray_creal_T *y
    // Return Type  : void
    //
    void FFTImplementationCallback::r2br_r2dit_trig(const emxArray_real_T *x,
      int n1_unsigned, const emxArray_real_T *costab, const emxArray_real_T
      *sintab, emxArray_creal_T *y)
    {
      int u0;
      u0 = y->size[0];
      y->size[0] = n1_unsigned;
      emxEnsureCapacity_creal_T(y, u0);
      if (n1_unsigned > x->size[0]) {
        u0 = y->size[0];
        y->size[0] = n1_unsigned;
        emxEnsureCapacity_creal_T(y, u0);
        for (u0 = 0; u0 < n1_unsigned; u0++) {
          y->data[u0].re = 0.0;
          y->data[u0].im = 0.0;
        }
      }

      if (n1_unsigned != 1) {
        FFTImplementationCallback::doHalfLengthRadix2((x), (y), (n1_unsigned),
          (costab), (sintab));
      } else {
        int i;
        int loop_ub;
        u0 = x->size[0];
        if (u0 >= 1) {
          u0 = 1;
        }

        loop_ub = u0 - 2;
        i = 0;
        int exitg1;
        do {
          if (i <= loop_ub) {
            y->data[0].re = x->data[0];
            y->data[0].im = 0.0;
            exitg1 = 1;
          } else {
            y->data[0].re = x->data[0];
            y->data[0].im = 0.0;
            exitg1 = 1;
          }
        } while (exitg1 == 0);
      }
    }

    //
    // Arguments    : const emxArray_creal_T *x
    //                int unsigned_nRows
    //                const emxArray_real_T *costab
    //                const emxArray_real_T *sintab
    //                emxArray_creal_T *y
    // Return Type  : void
    //
    void FFTImplementationCallback::r2br_r2dit_trig_impl(const emxArray_creal_T *
      x, int unsigned_nRows, const emxArray_real_T *costab, const
      emxArray_real_T *sintab, emxArray_creal_T *y)
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iDelta2;
      int iheight;
      int ix;
      int iy;
      int ju;
      int k;
      int nRowsD2;
      iy = y->size[0];
      y->size[0] = unsigned_nRows;
      emxEnsureCapacity_creal_T(y, iy);
      if (unsigned_nRows > x->size[0]) {
        iy = y->size[0];
        y->size[0] = unsigned_nRows;
        emxEnsureCapacity_creal_T(y, iy);
        for (iy = 0; iy < unsigned_nRows; iy++) {
          y->data[iy].re = 0.0;
          y->data[iy].im = 0.0;
        }
      }

      iDelta2 = x->size[0];
      if (iDelta2 >= unsigned_nRows) {
        iDelta2 = unsigned_nRows;
      }

      iheight = unsigned_nRows - 2;
      nRowsD2 = unsigned_nRows / 2;
      k = nRowsD2 / 2;
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i <= iDelta2 - 2; i++) {
        boolean_T tst;
        y->data[iy] = x->data[ix];
        iy = unsigned_nRows;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y->data[iy] = x->data[ix];
      if (unsigned_nRows > 1) {
        for (i = 0; i <= iheight; i += 2) {
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

      iy = 2;
      iDelta2 = 4;
      iheight = ((k - 1) << 2) + 1;
      while (k > 0) {
        int temp_re_tmp;
        for (i = 0; i < iheight; i += iDelta2) {
          temp_re_tmp = i + iy;
          temp_re = y->data[temp_re_tmp].re;
          temp_im = y->data[temp_re_tmp].im;
          y->data[temp_re_tmp].re = y->data[i].re - temp_re;
          y->data[temp_re_tmp].im = y->data[i].im - temp_im;
          y->data[i].re += temp_re;
          y->data[i].im += temp_im;
        }

        ix = 1;
        for (ju = k; ju < nRowsD2; ju += k) {
          int ihi;
          twid_re = costab->data[ju];
          twid_im = sintab->data[ju];
          i = ix;
          ihi = ix + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
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

          ix++;
        }

        k /= 2;
        iy = iDelta2;
        iDelta2 += iDelta2;
        iheight -= iy;
      }
    }
  }
}

//
// File trailer for FFTImplementationCallback.cpp
//
// [EOF]
//
