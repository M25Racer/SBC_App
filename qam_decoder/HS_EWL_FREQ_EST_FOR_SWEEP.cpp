//
// File: HS_EWL_FREQ_EST_FOR_SWEEP.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "HS_EWL_FREQ_EST_FOR_SWEEP.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_DEMOD_QAM_rtwutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// sweep_freq_warning_status = 0;% OK input array
// sweep_freq_warning_status = 1;% all or more than 33% input array equal 0
// sweep_freq_warning_status = 2;% path of input array equal 0, less than 33%
// Arguments    : emxArray_real_T *s2
//                double len
//                double Fs
//                double f0
//                double per_numb
//                double sps
//                double *f_opt
//                double *ph_opt
//                double *sweep_freq_warning_status
// Return Type  : void
//
void HS_EWL_FREQ_EST_FOR_SWEEP(emxArray_real_T *s2, double len, double Fs,
  double f0, double per_numb, double sps, double *f_opt, double *ph_opt, double *
  sweep_freq_warning_status)
{
  emxArray_real_T *a;
  emxArray_real_T *idx;
  emxArray_real_T *t;
  emxArray_real_T *x;
  double FFnu[50];
  double b_sine_to;
  double err2;
  double sine_from;
  double sine_to;
  int i;
  int i1;
  int loop_ub;
  int nx;
  boolean_T guard1 = false;
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  sps = rt_roundd_snf(sps);
  sine_to = coder::internal::maximum(s2);
  i = s2->size[0] * s2->size[1];
  i1 = s2->size[0] * s2->size[1];
  s2->size[0] = 1;
  emxEnsureCapacity_real_T(s2, i1);
  loop_ub = i - 1;
  for (i = 0; i <= loop_ub; i++) {
    s2->data[i] /= sine_to;
  }

  sine_from = preambule_from(s2, len);
  emxInit_real_T(&t, 2);
  emxInit_real_T(&idx, 2);
  emxInit_real_T(&x, 2);
  emxInit_real_T(&a, 2);
  guard1 = false;
  if (sine_from < 0.0) {
    guard1 = true;
  } else {
    sine_to = (len - sine_from) / sps;
    if ((sine_to < per_numb / 3.0 - 5.0) || (Fs == 0.0)) {
      guard1 = true;
    } else {
      int b_loop_ub;
      int c_loop_ub;
      int loop_ub_tmp;
      if (sine_to < per_numb - 5.0) {
        sine_to = sine_from + sps * ((sine_to - 30.0) - 5.0);
        *sweep_freq_warning_status = 2.0;

        //  path of input array equal 0, less than 33%
      } else {
        sine_to = sine_from + sps * (per_numb - 5.0);
        *sweep_freq_warning_status = 0.0;

        //  OK input array
      }

      if (sine_from > sine_to) {
        i = -1;
        i1 = -1;
      } else {
        i = static_cast<int>(sine_from) - 2;
        i1 = static_cast<int>(sine_to) - 1;
      }

      loop_ub_tmp = i1 - i;
      if (loop_ub_tmp < 1) {
        t->size[0] = 1;
        t->size[1] = 0;
      } else {
        i1 = t->size[0] * t->size[1];
        t->size[0] = 1;
        loop_ub = loop_ub_tmp - 1;
        t->size[1] = loop_ub_tmp;
        emxEnsureCapacity_real_T(t, i1);
        for (i1 = 0; i1 <= loop_ub; i1++) {
          t->data[i1] = static_cast<double>(i1) + 1.0;
        }
      }

      i1 = t->size[0] * t->size[1];
      nx = t->size[0] * t->size[1];
      t->size[0] = 1;
      emxEnsureCapacity_real_T(t, nx);
      loop_ub = i1 - 1;
      for (i1 = 0; i1 <= loop_ub; i1++) {
        t->data[i1] /= Fs;
      }

      sine_from = 1.0;
      err2 = 0.0;
      *f_opt = f0;
      sine_to = f0 * t->data[0];
      if (sine_to < 0.0) {
        b_sine_to = std::ceil(sine_to);
      } else {
        b_sine_to = std::floor(sine_to);
      }

      *ph_opt = 0.2013579207903308 - 6.2831853071795862 * (sine_to - b_sine_to);
      if (*ph_opt < 0.0) {
        *ph_opt += 6.2831853071795862;
      }

      loop_ub = t->size[0] * t->size[1];
      b_loop_ub = t->size[0] * t->size[1];
      c_loop_ub = t->size[0] * t->size[1];
      while (sine_from > 1.0E-6) {
        int b_i;
        int k;
        sine_from = 6.2831853071795862 * (*f_opt - 50.0);
        sine_to = 6.2831853071795862 * ((*f_opt + 50.0) - (*f_opt - 50.0)) /
          50.0;
        for (b_i = 0; b_i < 50; b_i++) {
          b_sine_to = sine_from + sine_to * ((static_cast<double>(b_i) + 1.0) -
            1.0);
          i1 = x->size[0] * x->size[1];
          x->size[0] = 1;
          x->size[1] = t->size[1];
          emxEnsureCapacity_real_T(x, i1);
          for (i1 = 0; i1 < loop_ub; i1++) {
            x->data[i1] = b_sine_to * t->data[i1] + *ph_opt;
          }

          nx = x->size[1];
          for (k = 0; k < nx; k++) {
            x->data[k] = std::sin(x->data[k]);
          }

          i1 = a->size[0] * a->size[1];
          a->size[0] = 1;
          a->size[1] = loop_ub_tmp;
          emxEnsureCapacity_real_T(a, i1);
          for (i1 = 0; i1 < loop_ub_tmp; i1++) {
            a->data[i1] = s2->data[(i + i1) + 1] - 0.8 * x->data[i1];
          }

          i1 = x->size[0] * x->size[1];
          x->size[0] = 1;
          x->size[1] = a->size[1];
          emxEnsureCapacity_real_T(x, i1);
          nx = a->size[1];
          for (k = 0; k < nx; k++) {
            x->data[k] = rt_powd_snf(a->data[k], 2.0);
          }

          nx = x->size[1];
          if (x->size[1] == 0) {
            b_sine_to = 0.0;
          } else {
            b_sine_to = x->data[0];
            for (k = 2; k <= nx; k++) {
              b_sine_to += x->data[k - 1];
            }
          }

          FFnu[b_i] = b_sine_to;

          // FFnu(i) = FFnu(i) + sum((y(bounds(2):end)-y2(bounds(2):end)).^2);
        }

        coder::internal::minimum(FFnu, &b_sine_to, &nx);
        *f_opt = (sine_from + sine_to * (absolute_min(static_cast<double>(nx),
                    FFnu) - 1.0)) / 2.0 / 3.1415926535897931;
        sine_to = ((*ph_opt + 3.1415926535897931) - (*ph_opt -
                    3.1415926535897931)) / 50.0;
        sine_from = 6.2831853071795862 * *f_opt;
        for (b_i = 0; b_i < 50; b_i++) {
          b_sine_to = (*ph_opt - 3.1415926535897931) + sine_to * ((static_cast<
            double>(b_i) + 1.0) - 1.0);
          i1 = x->size[0] * x->size[1];
          x->size[0] = 1;
          x->size[1] = t->size[1];
          emxEnsureCapacity_real_T(x, i1);
          for (i1 = 0; i1 < b_loop_ub; i1++) {
            x->data[i1] = sine_from * t->data[i1] + b_sine_to;
          }

          nx = x->size[1];
          for (k = 0; k < nx; k++) {
            x->data[k] = std::sin(x->data[k]);
          }

          i1 = a->size[0] * a->size[1];
          a->size[0] = 1;
          a->size[1] = loop_ub_tmp;
          emxEnsureCapacity_real_T(a, i1);
          for (i1 = 0; i1 < loop_ub_tmp; i1++) {
            a->data[i1] = s2->data[(i + i1) + 1] - 0.8 * x->data[i1];
          }

          i1 = x->size[0] * x->size[1];
          x->size[0] = 1;
          x->size[1] = a->size[1];
          emxEnsureCapacity_real_T(x, i1);
          nx = a->size[1];
          for (k = 0; k < nx; k++) {
            x->data[k] = rt_powd_snf(a->data[k], 2.0);
          }

          nx = x->size[1];
          if (x->size[1] == 0) {
            b_sine_to = 0.0;
          } else {
            b_sine_to = x->data[0];
            for (k = 2; k <= nx; k++) {
              b_sine_to += x->data[k - 1];
            }
          }

          FFnu[b_i] = b_sine_to;

          // FFfi(i) = FFfi(i) + sum((y(bounds(2):end)-y2(bounds(2):end)).^2);
        }

        coder::internal::minimum(FFnu, &b_sine_to, &nx);
        *ph_opt = (*ph_opt - 3.1415926535897931) + sine_to * (absolute_min(
          static_cast<double>(nx), FFnu) - 1.0);
        i1 = x->size[0] * x->size[1];
        x->size[0] = 1;
        x->size[1] = t->size[1];
        emxEnsureCapacity_real_T(x, i1);
        for (i1 = 0; i1 < c_loop_ub; i1++) {
          x->data[i1] = sine_from * t->data[i1] + *ph_opt;
        }

        nx = x->size[1];
        for (k = 0; k < nx; k++) {
          x->data[k] = std::sin(x->data[k]);
        }

        i1 = x->size[0] * x->size[1];
        nx = x->size[0] * x->size[1];
        x->size[0] = 1;
        emxEnsureCapacity_real_T(x, nx);
        nx = i1 - 1;
        for (i1 = 0; i1 <= nx; i1++) {
          x->data[i1] *= 0.8;
        }

        sine_to = err2;
        if (loop_ub_tmp < 1) {
          idx->size[0] = 1;
          idx->size[1] = 0;
        } else {
          i1 = idx->size[0] * idx->size[1];
          idx->size[0] = 1;
          nx = static_cast<int>(static_cast<double>(static_cast<unsigned int>
            (loop_ub_tmp)) - 1.0);
          idx->size[1] = static_cast<int>(static_cast<double>(static_cast<
            unsigned int>(loop_ub_tmp)) - 1.0) + 1;
          emxEnsureCapacity_real_T(idx, i1);
          for (i1 = 0; i1 <= nx; i1++) {
            idx->data[i1] = static_cast<double>(i1) + 1.0;
          }
        }

        // idx = idx<=bounds(1) | idx>=bounds(2);
        i1 = a->size[0] * a->size[1];
        a->size[0] = 1;
        a->size[1] = idx->size[1];
        emxEnsureCapacity_real_T(a, i1);
        nx = idx->size[1];
        for (i1 = 0; i1 < nx; i1++) {
          k = static_cast<int>(idx->data[i1]);
          a->data[i1] = x->data[k - 1] - s2->data[i + k];
        }

        i1 = x->size[0] * x->size[1];
        x->size[0] = 1;
        x->size[1] = a->size[1];
        emxEnsureCapacity_real_T(x, i1);
        nx = a->size[1];
        for (k = 0; k < nx; k++) {
          x->data[k] = rt_powd_snf(a->data[k], 2.0);
        }

        nx = x->size[1];
        if (x->size[1] == 0) {
          err2 = 0.0;
        } else {
          err2 = x->data[0];
          for (k = 2; k <= nx; k++) {
            err2 += x->data[k - 1];
          }
        }

        sine_from = std::abs((sine_to - err2) / (sine_to + err2));
      }
    }
  }

  if (guard1) {
    *sweep_freq_warning_status = 1.0;

    //  all or more than 33% input array equal 0
    *f_opt = 0.0;
    *ph_opt = 0.0;
  }

  emxFree_real_T(&a);
  emxFree_real_T(&x);
  emxFree_real_T(&idx);
  emxFree_real_T(&t);
}

//
// File trailer for HS_EWL_FREQ_EST_FOR_SWEEP.cpp
//
// [EOF]
//
