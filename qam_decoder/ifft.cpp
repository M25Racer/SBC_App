//
// File: ifft.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//

// Include Files
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "HS_EWL_RECEIVE_data.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "HS_EWL_RECEIVE_types.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const emxArray_creal_T *x
//                emxArray_creal_T *y
// Return Type  : void
//
namespace coder
{
  void ifft(const emxArray_creal_T *x, emxArray_creal_T *y)
  {
    emxArray_creal_T *b_fv;
    emxArray_creal_T *fv;
    emxArray_creal_T *wwc;
    emxArray_real_T *costab;
    emxArray_real_T *costab1q;
    emxArray_real_T *sintab;
    emxArray_real_T *sintabinv;
    int N2blue;
    int nd2;
    int nfft;
    int rt;
    nfft = x->size[0];
    if (x->size[0] == 0) {
      int i;
      i = y->size[0];
      y->size[0] = x->size[0];
      emxEnsureCapacity_creal_T(y, i);
      nd2 = x->size[0];
      for (i = 0; i < nd2; i++) {
        y->data[i].re = 0.0;
        y->data[i].im = 0.0;
      }
    } else {
      double nt_im;
      int i;
      int k;
      boolean_T useRadix2;
      emxInit_real_T(&costab1q, 2);
      useRadix2 = ((x->size[0] & (x->size[0] - 1)) == 0);
      internal::FFTImplementationCallback::get_algo_sizes((x->size[0]),
        (useRadix2), (&N2blue), (&nd2));
      nt_im = 6.2831853071795862 / static_cast<double>(nd2);
      rt = nd2 / 2 / 2;
      i = costab1q->size[0] * costab1q->size[1];
      costab1q->size[0] = 1;
      costab1q->size[1] = rt + 1;
      emxEnsureCapacity_real_T(costab1q, i);
      costab1q->data[0] = 1.0;
      nd2 = rt / 2 - 1;
      for (k = 0; k <= nd2; k++) {
        costab1q->data[k + 1] = std::cos(nt_im * (static_cast<double>(k) + 1.0));
      }

      i = nd2 + 2;
      nd2 = rt - 1;
      for (k = i; k <= nd2; k++) {
        costab1q->data[k] = std::sin(nt_im * static_cast<double>(rt - k));
      }

      costab1q->data[rt] = 0.0;
      emxInit_real_T(&costab, 2);
      emxInit_real_T(&sintab, 2);
      emxInit_real_T(&sintabinv, 2);
      if (!useRadix2) {
        rt = costab1q->size[1] - 1;
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
        for (k = 0; k < rt; k++) {
          sintabinv->data[k + 1] = costab1q->data[(rt - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          sintabinv->data[k] = costab1q->data[k - rt];
        }

        for (k = 0; k < rt; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = -costab1q->data[(rt - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = -costab1q->data[k - rt];
        }
      } else {
        rt = costab1q->size[1] - 1;
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
        for (k = 0; k < rt; k++) {
          costab->data[k + 1] = costab1q->data[k + 1];
          sintab->data[k + 1] = costab1q->data[(rt - k) - 1];
        }

        i = costab1q->size[1];
        for (k = i; k <= nd2; k++) {
          costab->data[k] = -costab1q->data[nd2 - k];
          sintab->data[k] = costab1q->data[k - rt];
        }

        sintabinv->size[0] = 1;
        sintabinv->size[1] = 0;
      }

      emxFree_real_T(&costab1q);
      if (useRadix2) {
        internal::FFTImplementationCallback::r2br_r2dit_trig_impl((x), (x->size
          [0]), (costab), (sintab), (y));
        if (y->size[0] > 1) {
          nt_im = 1.0 / static_cast<double>(y->size[0]);
          nd2 = y->size[0];
          for (i = 0; i < nd2; i++) {
            y->data[i].re *= nt_im;
            y->data[i].im *= nt_im;
          }
        }
      } else {
        double nt_re;
        int idx;
        int nInt2;
        emxInit_creal_T(&wwc, 1);
        nd2 = (x->size[0] + x->size[0]) - 1;
        i = wwc->size[0];
        wwc->size[0] = nd2;
        emxEnsureCapacity_creal_T(wwc, i);
        idx = x->size[0];
        rt = 0;
        wwc->data[x->size[0] - 1].re = 1.0;
        wwc->data[x->size[0] - 1].im = 0.0;
        nInt2 = x->size[0] << 1;
        i = x->size[0];
        for (k = 0; k <= i - 2; k++) {
          int b_y;
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
        i = nd2 - 1;
        for (k = i; k >= nfft; k--) {
          wwc->data[k] = wwc->data[idx];
          idx++;
        }

        i = y->size[0];
        y->size[0] = x->size[0];
        emxEnsureCapacity_creal_T(y, i);
        nd2 = x->size[0];
        rt = 0;
        for (k = 0; k < nd2; k++) {
          nInt2 = (nfft + k) - 1;
          nt_re = wwc->data[nInt2].re;
          nt_im = wwc->data[nInt2].im;
          y->data[k].re = nt_re * x->data[rt].re + nt_im * x->data[rt].im;
          y->data[k].im = nt_re * x->data[rt].im - nt_im * x->data[rt].re;
          rt++;
        }

        i = x->size[0] + 1;
        for (k = i; k <= nfft; k++) {
          y->data[k - 1].re = 0.0;
          y->data[k - 1].im = 0.0;
        }

        emxInit_creal_T(&fv, 1);
        emxInit_creal_T(&b_fv, 1);
        internal::FFTImplementationCallback::r2br_r2dit_trig_impl((y), (N2blue),
          (costab), (sintab), (fv));
        internal::FFTImplementationCallback::r2br_r2dit_trig_impl((wwc), (N2blue),
          (costab), (sintab), (b_fv));
        i = b_fv->size[0];
        b_fv->size[0] = fv->size[0];
        emxEnsureCapacity_creal_T(b_fv, i);
        nd2 = fv->size[0];
        for (i = 0; i < nd2; i++) {
          nt_im = fv->data[i].re * b_fv->data[i].im + fv->data[i].im *
            b_fv->data[i].re;
          b_fv->data[i].re = fv->data[i].re * b_fv->data[i].re - fv->data[i].im *
            b_fv->data[i].im;
          b_fv->data[i].im = nt_im;
        }

        internal::FFTImplementationCallback::r2br_r2dit_trig_impl((b_fv),
          (N2blue), (costab), (sintabinv), (fv));
        emxFree_creal_T(&b_fv);
        if (fv->size[0] > 1) {
          nt_im = 1.0 / static_cast<double>(fv->size[0]);
          nd2 = fv->size[0];
          for (i = 0; i < nd2; i++) {
            fv->data[i].re *= nt_im;
            fv->data[i].im *= nt_im;
          }
        }

        idx = 0;
        nt_re = x->size[0];
        i = x->size[0];
        nd2 = wwc->size[0];
        for (k = i; k <= nd2; k++) {
          double ai;
          double re;
          y->data[idx].re = wwc->data[k - 1].re * fv->data[k - 1].re + wwc->
            data[k - 1].im * fv->data[k - 1].im;
          y->data[idx].im = wwc->data[k - 1].re * fv->data[k - 1].im - wwc->
            data[k - 1].im * fv->data[k - 1].re;
          nt_im = y->data[idx].re;
          ai = y->data[idx].im;
          if (ai == 0.0) {
            re = nt_im / nt_re;
            nt_im = 0.0;
          } else if (nt_im == 0.0) {
            re = 0.0;
            nt_im = ai / nt_re;
          } else {
            re = nt_im / nt_re;
            nt_im = ai / nt_re;
          }

          y->data[idx].re = re;
          y->data[idx].im = nt_im;
          idx++;
        }

        emxFree_creal_T(&fv);
        emxFree_creal_T(&wwc);
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
