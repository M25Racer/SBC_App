//
// File: HS_EWL_RECEIVE.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 19-Apr-2022 11:19:13
//

// Include Files
#include "HS_EWL_RECEIVE.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "HS_EWL_RECEIVE_data.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "HS_EWL_RECEIVE_initialize.h"
#include "HS_EWL_RECEIVE_rtwutil.h"
#include "HS_EWL_RECEIVE_types.h"
#include "RaisedCosineReceiveFilter.h"
#include "qamdemod.h"
#include "qammod.h"
#include "rat.h"
#include "rt_nonfinite.h"
#include "xcorr.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// %#codegen
//                % Length of a preambule in symbols
//               % Length of whole message in symbols
//               % QAM order
//  Preamble QAM symbol
// Arguments    : const emxArray_real_T *data
//                double len
//                double Fs
//                double f_opt
//                double sps
//                double mode
//                double Pl
//                double msg_len
//                double M
//                double pre_qam
//                creal_T qam_symbols_data[]
//                int qam_symbols_size[1]
//                double byte_data_data[]
//                int byte_data_size[1]
//                double f_est_data[]
//                int f_est_size[1]
// Return Type  : void
//
void HS_EWL_RECEIVE(const emxArray_real_T *data, double len, double Fs, double
                    f_opt, double sps, double mode, double Pl, double msg_len,
                    double M, double pre_qam, creal_T qam_symbols_data[], int
                    qam_symbols_size[1], double byte_data_data[], int
                    byte_data_size[1], double f_est_data[], int f_est_size[1])
{
  emxArray_creal_T *c_resamp_signal;
  emxArray_creal_T *r3;
  emxArray_real_T *b_resamp_signal;
  emxArray_real_T *b_time;
  emxArray_real_T *c;
  emxArray_real_T *input_data;
  emxArray_real_T *r;
  emxArray_real_T *r1;
  emxArray_real_T *r2;
  emxArray_real_T *resamp_signal;
  emxArray_real_T *trData;
  creal_T b_z_data[255];
  creal_T z_data[255];
  creal_T del;
  double a1;
  double a3;
  double a3_tmp;
  double b_a3_tmp;
  double d;
  double p;
  double q;
  int z_size[2];
  int b_z_size[1];
  int i;
  int i1;
  int k;
  int loop_ub;
  int nx;
  int start_inf_data;
  if (!isInitialized_HS_EWL_RECEIVE) {
    HS_EWL_RECEIVE_initialize();
  }

  HS_EWL_FREQ_ACQ(data, len, Fs, f_opt, sps, mode, Pl, msg_len, M, pre_qam, &a1,
                  &a3, f_est_data, f_est_size);
  start_inf_data = 0;
  if (!b_rxFilter1_not_empty) {
    b_rxFilter1.init();
    b_rxFilter1_not_empty = true;
  }

  emxInit_real_T(&input_data, 2);
  coder::rat(f_est_data[0] * sps / Fs, &p, &q);
  i = input_data->size[0] * input_data->size[1];
  input_data->size[0] = 1;
  nx = static_cast<int>(std::floor(a3));
  input_data->size[1] = nx + 1;
  emxEnsureCapacity_real_T(input_data, i);
  for (i = 0; i <= nx; i++) {
    input_data->data[i] = data->data[static_cast<int>(a1 + static_cast<double>(i))
      - 1];
  }

  //  Input parameters
  //   s   - input signal vector [N x 1];
  //   p   - p paramter of samplarate conversion
  //   q   - q paramter of samplarate conversion
  //   x0  - fractional delay
  //  Ouptut parameters
  //   y   - Resampled signal
  emxInit_real_T(&resamp_signal, 1);
  if (p > 1.0) {
    if (q == 1.0) {
      a1 = std::floor(static_cast<double>(static_cast<int>(std::floor(a3)) + 1) *
                      p);
      i = resamp_signal->size[0];
      resamp_signal->size[0] = static_cast<int>(a1 + 1.0);
      emxEnsureCapacity_real_T(resamp_signal, i);
      nx = static_cast<int>(a1 + 1.0);
      for (i = 0; i < nx; i++) {
        resamp_signal->data[i] = 0.0;
      }
    } else {
      a1 = std::floor(static_cast<double>(static_cast<int>(std::floor(a3)) + 1) *
                      p / q);
      i = resamp_signal->size[0];
      resamp_signal->size[0] = static_cast<int>(a1);
      emxEnsureCapacity_real_T(resamp_signal, i);
      nx = static_cast<int>(a1);
      for (i = 0; i < nx; i++) {
        resamp_signal->data[i] = 0.0;
      }
    }
  } else {
    a1 = std::floor(static_cast<double>(static_cast<int>(std::floor(a3)) + 1) *
                    p / q);
    i = resamp_signal->size[0];
    resamp_signal->size[0] = static_cast<int>(a1);
    emxEnsureCapacity_real_T(resamp_signal, i);
    nx = static_cast<int>(a1);
    for (i = 0; i < nx; i++) {
      resamp_signal->data[i] = 0.0;
    }
  }

  emxInit_real_T(&r, 2);
  i = r->size[0] * r->size[1];
  r->size[0] = 1;
  r->size[1] = input_data->size[1] + 4;
  emxEnsureCapacity_real_T(r, i);
  r->data[0] = 0.0;
  r->data[1] = 0.0;
  nx = input_data->size[1];
  for (i = 0; i < nx; i++) {
    r->data[i + 2] = input_data->data[i];
  }

  r->data[input_data->size[1] + 2] = 0.0;
  r->data[input_data->size[1] + 3] = 0.0;
  i = input_data->size[0] * input_data->size[1];
  input_data->size[0] = 1;
  input_data->size[1] = r->size[1];
  emxEnsureCapacity_real_T(input_data, i);
  nx = r->size[0] * r->size[1];
  for (i = 0; i < nx; i++) {
    input_data->data[i] = r->data[i];
  }

  emxFree_real_T(&r);
  i = resamp_signal->size[0];
  for (k = 0; k < i; k++) {
    double c_a3_tmp;
    a1 = (static_cast<double>(k) + 1.0) * q / p;
    a3 = std::floor(a1);
    d = (a3 + 1.0) - a1;
    a3_tmp = input_data->data[static_cast<int>(a3 + 4.0) - 1];
    b_a3_tmp = input_data->data[static_cast<int>((a3 + 4.0) - 2.0) - 1];
    c_a3_tmp = input_data->data[static_cast<int>((a3 + 4.0) - 1.0) - 1];
    a3 = 0.16666666666666666 * (a3_tmp - input_data->data[static_cast<int>((a3 +
      4.0) - 3.0) - 1]) + 0.5 * (b_a3_tmp - c_a3_tmp);
    a1 = 0.5 * (a3_tmp - b_a3_tmp) - a3;
    resamp_signal->data[k] = ((c_a3_tmp - a1 * d) + (((a3_tmp - c_a3_tmp) - a3)
      - a1) * (d * d)) - a3 * rt_powd_snf(d, 3.0);
  }

  a1 = static_cast<double>(resamp_signal->size[0]) / sps;
  if (a1 < 0.0) {
    a1 = std::ceil(a1);
  } else {
    a1 = std::floor(a1);
  }

  a3 = (a1 - 1.0) * sps;
  if (rtIsNaN(a3 - 1.0)) {
    i = input_data->size[0] * input_data->size[1];
    input_data->size[0] = 1;
    input_data->size[1] = 1;
    emxEnsureCapacity_real_T(input_data, i);
    input_data->data[0] = rtNaN;
  } else if (a3 - 1.0 < 0.0) {
    input_data->size[0] = 1;
    input_data->size[1] = 0;
  } else if (rtIsInf(a3 - 1.0) && (0.0 == a3 - 1.0)) {
    i = input_data->size[0] * input_data->size[1];
    input_data->size[0] = 1;
    input_data->size[1] = 1;
    emxEnsureCapacity_real_T(input_data, i);
    input_data->data[0] = rtNaN;
  } else {
    i = input_data->size[0] * input_data->size[1];
    input_data->size[0] = 1;
    nx = static_cast<int>(std::floor(a3 - 1.0));
    input_data->size[1] = nx + 1;
    emxEnsureCapacity_real_T(input_data, i);
    for (i = 0; i <= nx; i++) {
      input_data->data[i] = i;
    }
  }

  emxInit_real_T(&b_time, 1);
  a1 = f_est_data[0] * sps;
  i = b_time->size[0];
  b_time->size[0] = input_data->size[1];
  emxEnsureCapacity_real_T(b_time, i);
  nx = input_data->size[1];
  for (i = 0; i < nx; i++) {
    b_time->data[i] = input_data->data[i] / a1;
  }

  emxInit_real_T(&c, 1);
  if (1.0 > sps) {
    nx = 0;
    loop_ub = 0;
  } else {
    nx = static_cast<int>(sps);
    loop_ub = static_cast<int>(sps);
  }

  a1 = 6.2831853071795862 * f_est_data[0];
  i = c->size[0];
  c->size[0] = nx;
  emxEnsureCapacity_real_T(c, i);
  for (i = 0; i < nx; i++) {
    c->data[i] = a1 * b_time->data[i];
  }

  nx = c->size[0];
  for (k = 0; k < nx; k++) {
    c->data[k] = std::cos(c->data[k]);
  }

  emxInit_real_T(&r1, 1);
  i = r1->size[0];
  r1->size[0] = loop_ub;
  emxEnsureCapacity_real_T(r1, i);
  for (i = 0; i < loop_ub; i++) {
    r1->data[i] = a1 * b_time->data[i];
  }

  nx = r1->size[0];
  for (k = 0; k < nx; k++) {
    r1->data[k] = std::sin(r1->data[k]);
  }

  emxInit_real_T(&b_resamp_signal, 1);
  b_a3_tmp = sps * 2.0;
  if (1.0 > b_a3_tmp) {
    nx = 0;
  } else {
    nx = static_cast<int>(b_a3_tmp);
  }

  i = b_resamp_signal->size[0];
  b_resamp_signal->size[0] = nx;
  emxEnsureCapacity_real_T(b_resamp_signal, i);
  for (i = 0; i < nx; i++) {
    b_resamp_signal->data[i] = resamp_signal->data[i];
  }

  emxInit_real_T(&r2, 1);
  i = r2->size[0];
  r2->size[0] = c->size[0] + 53;
  emxEnsureCapacity_real_T(r2, i);
  nx = c->size[0];
  for (i = 0; i < nx; i++) {
    r2->data[i] = c->data[i] + r1->data[i];
  }

  emxFree_real_T(&r1);
  for (i = 0; i < 53; i++) {
    r2->data[i + c->size[0]] = 0.0;
  }

  coder::xcorr(b_resamp_signal, r2, c, input_data);

  // find(lags==0,1)
  b_a3_tmp = sps * 2.0 + 1.0;
  emxFree_real_T(&r2);
  emxFree_real_T(&b_resamp_signal);
  if (b_a3_tmp > c->size[0]) {
    i = -1;
    i1 = -1;
  } else {
    i = static_cast<int>(b_a3_tmp) - 2;
    i1 = c->size[0] - 1;
  }

  loop_ub = i1 - i;
  if (loop_ub <= 2) {
    if (loop_ub == 1) {
      nx = 1;
    } else {
      b_a3_tmp = c->data[i + 2];
      if ((c->data[i + 1] < b_a3_tmp) || (rtIsNaN(c->data[i + 1]) && (!rtIsNaN
            (b_a3_tmp)))) {
        nx = 2;
      } else {
        nx = 1;
      }
    }
  } else {
    if (!rtIsNaN(c->data[i + 1])) {
      nx = 1;
    } else {
      boolean_T exitg1;
      nx = 0;
      k = 2;
      exitg1 = false;
      while ((!exitg1) && (k <= loop_ub)) {
        if (!rtIsNaN(c->data[i + k])) {
          nx = k;
          exitg1 = true;
        } else {
          k++;
        }
      }
    }

    if (nx == 0) {
      nx = 1;
    } else {
      a1 = c->data[i + nx];
      i1 = nx + 1;
      for (k = i1; k <= loop_ub; k++) {
        b_a3_tmp = c->data[i + k];
        if (a1 < b_a3_tmp) {
          a1 = b_a3_tmp;
          nx = k;
        }
      }
    }
  }

  emxFree_real_T(&c);
  b_a3_tmp = a3 + (static_cast<double>(nx) - 1.0);
  if (nx > b_a3_tmp) {
    i = -1;
    i1 = -1;
  } else {
    i = nx - 2;
    i1 = static_cast<int>(b_a3_tmp) - 1;
  }

  emxInit_creal_T(&r3, 1);
  if (1.0 > a3) {
    nx = 0;
  } else {
    nx = static_cast<int>(a3);
  }

  del.re = f_est_data[0] * 0.0;
  del.im = f_est_data[0] * 6.2831853071795862;
  k = r3->size[0];
  r3->size[0] = nx;
  emxEnsureCapacity_creal_T(r3, k);
  for (k = 0; k < nx; k++) {
    r3->data[k].re = b_time->data[k] * del.re;
    r3->data[k].im = b_time->data[k] * del.im;
  }

  emxFree_real_T(&b_time);
  nx = r3->size[0];
  for (k = 0; k < nx; k++) {
    if (r3->data[k].im == 0.0) {
      r3->data[k].re = std::exp(r3->data[k].re);
      r3->data[k].im = 0.0;
    } else if (rtIsInf(r3->data[k].im) && rtIsInf(r3->data[k].re) && (r3->data[k]
                .re < 0.0)) {
      r3->data[k].re = 0.0;
      r3->data[k].im = 0.0;
    } else {
      a1 = std::exp(r3->data[k].re / 2.0);
      r3->data[k].re = a1 * (a1 * std::cos(r3->data[k].im));
      r3->data[k].im = a1 * (a1 * std::sin(r3->data[k].im));
    }
  }

  a1 = sps * 10.0 / 2.0;
  if (a1 < 1.0) {
    input_data->size[0] = 1;
    input_data->size[1] = 0;
  } else {
    k = input_data->size[0] * input_data->size[1];
    input_data->size[0] = 1;
    nx = static_cast<int>(std::floor(a1 - 1.0));
    input_data->size[1] = nx + 1;
    emxEnsureCapacity_real_T(input_data, k);
    for (k = 0; k <= nx; k++) {
      input_data->data[k] = static_cast<double>(k) + 1.0;
    }
  }

  emxInit_creal_T(&c_resamp_signal, 1);
  nx = i1 - i;
  k = c_resamp_signal->size[0];
  c_resamp_signal->size[0] = nx + input_data->size[1];
  emxEnsureCapacity_creal_T(c_resamp_signal, k);
  for (k = 0; k < nx; k++) {
    a1 = resamp_signal->data[(i + k) + 1];
    c_resamp_signal->data[k].re = 2.0 * (a1 * r3->data[k].re);
    c_resamp_signal->data[k].im = 2.0 * (a1 * r3->data[k].im);
  }

  emxFree_creal_T(&r3);
  emxFree_real_T(&resamp_signal);
  nx = input_data->size[1];
  for (k = 0; k < nx; k++) {
    loop_ub = (k + i1) - i;
    c_resamp_signal->data[loop_ub].re = input_data->data[k] * 0.0;
    c_resamp_signal->data[loop_ub].im = 0.0;
  }

  emxFree_real_T(&input_data);
  b_rxFilter1.step(c_resamp_signal, z_data, z_size);
  del = coder::qammod(pre_qam, M);
  emxFree_creal_T(&c_resamp_signal);
  if (z_data[9].im == 0.0) {
    if (del.im == 0.0) {
      a3_tmp = del.re / z_data[9].re;
      a1 = 0.0;
    } else if (del.re == 0.0) {
      a3_tmp = 0.0;
      a1 = del.im / z_data[9].re;
    } else {
      a3_tmp = del.re / z_data[9].re;
      a1 = del.im / z_data[9].re;
    }
  } else if (z_data[9].re == 0.0) {
    if (del.re == 0.0) {
      a3_tmp = del.im / z_data[9].im;
      a1 = 0.0;
    } else if (del.im == 0.0) {
      a3_tmp = 0.0;
      a1 = -(del.re / z_data[9].im);
    } else {
      a3_tmp = del.im / z_data[9].im;
      a1 = -(del.re / z_data[9].im);
    }
  } else {
    b_a3_tmp = std::abs(z_data[9].re);
    a1 = std::abs(z_data[9].im);
    if (b_a3_tmp > a1) {
      a1 = z_data[9].im / z_data[9].re;
      d = z_data[9].re + a1 * z_data[9].im;
      a3_tmp = (del.re + a1 * del.im) / d;
      a1 = (del.im - a1 * del.re) / d;
    } else if (a1 == b_a3_tmp) {
      if (z_data[9].re > 0.0) {
        a1 = 0.5;
      } else {
        a1 = -0.5;
      }

      if (z_data[9].im > 0.0) {
        a3 = 0.5;
      } else {
        a3 = -0.5;
      }

      a3_tmp = (del.re * a1 + del.im * a3) / b_a3_tmp;
      a1 = (del.im * a1 - del.re * a3) / b_a3_tmp;
    } else {
      a1 = z_data[9].re / z_data[9].im;
      d = z_data[9].im + a1 * z_data[9].re;
      a3_tmp = (a1 * del.re + del.im) / d;
      a1 = (a1 * del.im - del.re) / d;
    }
  }

  if (6 > z_size[0]) {
    i = 0;
    i1 = 0;
  } else {
    i = 5;
    i1 = z_size[0];
  }

  nx = i1 - i;
  b_z_size[0] = nx;
  for (i1 = 0; i1 < nx; i1++) {
    k = i + i1;
    b_a3_tmp = z_data[k].re;
    a3 = z_data[k].im;
    b_z_data[i1].re = b_a3_tmp * a3_tmp - a3 * a1;
    b_z_data[i1].im = b_a3_tmp * a1 + a3 * a3_tmp;
  }

  emxInit_real_T(&trData, 2);
  coder::qamdemod(b_z_data, b_z_size, M, trData);
  if (6 > z_size[0]) {
    i = 0;
    i1 = 0;
  } else {
    i = 5;
    i1 = z_size[0];
  }

  nx = i1 - i;
  qam_symbols_size[0] = nx;
  for (i1 = 0; i1 < nx; i1++) {
    k = i + i1;
    b_a3_tmp = z_data[k].re;
    a3 = z_data[k].im;
    qam_symbols_data[i1].re = b_a3_tmp * a3_tmp - a3 * a1;
    qam_symbols_data[i1].im = b_a3_tmp * a1 + a3 * a3_tmp;
  }

  for (loop_ub = 0; loop_ub < 50; loop_ub++) {
    if ((trData->data[loop_ub] == pre_qam) && (trData->data[loop_ub + 1] ==
         pre_qam) && (trData->data[loop_ub + 2] == pre_qam) && (trData->
         data[loop_ub + 3] == pre_qam * 2.0 - 1.0)) {
      start_inf_data = loop_ub + 4;
    }
  }

  byte_data_size[0] = 212;
  for (i = 0; i < 212; i++) {
    byte_data_data[i] = trData->data[start_inf_data + i];
  }

  emxFree_real_T(&trData);
}

//
// File trailer for HS_EWL_RECEIVE.cpp
//
// [EOF]
//
