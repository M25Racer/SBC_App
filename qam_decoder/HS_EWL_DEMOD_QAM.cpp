//
// File: HS_EWL_DEMOD_QAM.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "HS_EWL_DEMOD_QAM.h"
#include "FIRDecimator.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_DEMOD_QAM_rtwutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "RaisedCosineReceiveFilter.h"
#include "minOrMax.h"
#include "qamdemod.h"
#include "qammod.h"
#include "rat.h"
#include "rt_nonfinite.h"
#include "xcorr.h"
#include "rt_nonfinite.h"
#include <cmath>

// Variable Definitions
static coder::comm::RaisedCosineReceiveFilter rxFilter1;
static boolean_T rxFilter1_not_empty;

// Function Definitions
//
// Arguments    : const emxArray_real_T *data
//                double index_data
//                double len_data
//                double f_est
//                double Fs
//                double sps
//                double pre_qam
//                double M
//                creal_T qam_symbols_data[]
//                int qam_symbols_size[1]
//                double byte_data_data[]
//                int byte_data_size[1]
// Return Type  : void
//
void HS_EWL_DEMOD_QAM(const emxArray_real_T *data, double index_data, double
                      len_data, double f_est, double Fs, double sps, double
                      pre_qam, double M, creal_T qam_symbols_data[], int
                      qam_symbols_size[1], double byte_data_data[], int
                      byte_data_size[1])
{
  emxArray_creal_T *c_resamp_signal;
  emxArray_creal_T *r;
  emxArray_real_T *b_c;
  emxArray_real_T *b_resamp_signal;
  emxArray_real_T *b_time;
  emxArray_real_T *c;
  emxArray_real_T *input_data;
  emxArray_real_T *r1;
  emxArray_real_T *resamp_signal;
  emxArray_real_T *trData;
  creal_T b_z_data[255];
  creal_T z_data[255];
  creal_T del;
  double Q;
  double b_r;
  int z_size[2];
  int b_z_size[1];
  int unnamed_idx_1;
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  emxInit_real_T(&input_data, 2);
  emxInit_real_T(&resamp_signal, 1);
  emxInit_real_T(&trData, 2);
  emxInit_real_T(&c, 1);
  emxInit_real_T(&b_time, 1);
  emxInit_creal_T(&r, 1);
  emxInit_real_T(&b_resamp_signal, 1);
  emxInit_real_T(&r1, 1);
  emxInit_creal_T(&c_resamp_signal, 1);
  emxInit_real_T(&b_c, 1);
  if ((index_data == 0.0) && (len_data == 0.0)) {
    qam_symbols_size[0] = 1;
    qam_symbols_data[0].re = 0.0;
    qam_symbols_data[0].im = 0.0;
    byte_data_size[0] = 1;
    byte_data_data[0] = 0.0;

    // error = 1;
  } else {
    double maxval;
    double re;
    int i;
    int i1;
    int k;
    int nx;
    int start_inf_data;
    maxval = index_data + len_data;
    if (maxval > data->size[1]) {
      if (index_data > data->size[1]) {
        i = -1;
        i1 = -1;
      } else {
        i = static_cast<int>(index_data) - 2;
        i1 = data->size[1] - 1;
      }

      unnamed_idx_1 = static_cast<int>(maxval - static_cast<double>(data->size[1]));
      k = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = i1 - i;
      input_data->size[1] = nx + unnamed_idx_1;
      emxEnsureCapacity_real_T(input_data, k);
      for (k = 0; k < nx; k++) {
        input_data->data[k] = data->data[(i + k) + 1];
      }

      for (k = 0; k < unnamed_idx_1; k++) {
        input_data->data[(k + i1) - i] = 0.0;
      }
    } else {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = static_cast<int>(std::floor(len_data));
      input_data->size[1] = nx + 1;
      emxEnsureCapacity_real_T(input_data, i);
      for (i = 0; i <= nx; i++) {
        input_data->data[i] = data->data[static_cast<int>(index_data +
          static_cast<double>(i)) - 1];
      }
    }

    start_inf_data = 0;
    if (!rxFilter1_not_empty) {
      rxFilter1.init();
      rxFilter1_not_empty = true;
    }

    coder::rat(f_est * sps / Fs, &b_r, &Q);

    //      input_data = data(index_data: index_data+len_data);
    maxval = coder::internal::maximum(input_data);
    i = input_data->size[0] * input_data->size[1];
    i1 = input_data->size[0] * input_data->size[1];
    input_data->size[0] = 1;
    emxEnsureCapacity_real_T(input_data, i1);
    nx = i - 1;
    for (i = 0; i <= nx; i++) {
      input_data->data[i] /= maxval;
    }

    lagrange_resamp(input_data, b_r, Q, resamp_signal);
    b_r = static_cast<double>(resamp_signal->size[0]) / sps;
    if (b_r < 0.0) {
      b_r = std::ceil(b_r);
    } else {
      b_r = std::floor(b_r);
    }

    Q = (b_r - 1.0) * sps;
    if (rtIsNaN(Q - 1.0)) {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      input_data->size[1] = 1;
      emxEnsureCapacity_real_T(input_data, i);
      input_data->data[0] = rtNaN;
    } else if (Q - 1.0 < 0.0) {
      input_data->size[0] = 1;
      input_data->size[1] = 0;
    } else if (rtIsInf(Q - 1.0) && (0.0 == Q - 1.0)) {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      input_data->size[1] = 1;
      emxEnsureCapacity_real_T(input_data, i);
      input_data->data[0] = rtNaN;
    } else {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = static_cast<int>(std::floor(Q - 1.0));
      input_data->size[1] = nx + 1;
      emxEnsureCapacity_real_T(input_data, i);
      for (i = 0; i <= nx; i++) {
        input_data->data[i] = i;
      }
    }

    b_r = f_est * sps;
    i = b_time->size[0];
    b_time->size[0] = input_data->size[1];
    emxEnsureCapacity_real_T(b_time, i);
    nx = input_data->size[1];
    for (i = 0; i < nx; i++) {
      b_time->data[i] = input_data->data[i] / b_r;
    }

    if (1.0 > sps) {
      nx = 0;
      unnamed_idx_1 = 0;
    } else {
      nx = static_cast<int>(sps);
      unnamed_idx_1 = static_cast<int>(sps);
    }

    b_r = 6.2831853071795862 * f_est;
    i = c->size[0];
    c->size[0] = nx;
    emxEnsureCapacity_real_T(c, i);
    for (i = 0; i < nx; i++) {
      c->data[i] = b_r * b_time->data[i];
    }

    nx = c->size[0];
    for (k = 0; k < nx; k++) {
      c->data[k] = std::cos(c->data[k]);
    }

    i = b_c->size[0];
    b_c->size[0] = unnamed_idx_1;
    emxEnsureCapacity_real_T(b_c, i);
    for (i = 0; i < unnamed_idx_1; i++) {
      b_c->data[i] = b_r * b_time->data[i];
    }

    nx = b_c->size[0];
    for (k = 0; k < nx; k++) {
      b_c->data[k] = std::sin(b_c->data[k]);
    }

    maxval = sps * 2.0;
    if (1.0 > maxval) {
      nx = 0;
    } else {
      nx = static_cast<int>(maxval);
    }

    i = b_resamp_signal->size[0];
    b_resamp_signal->size[0] = nx;
    emxEnsureCapacity_real_T(b_resamp_signal, i);
    for (i = 0; i < nx; i++) {
      b_resamp_signal->data[i] = resamp_signal->data[i];
    }

    i = r1->size[0];
    r1->size[0] = c->size[0] + 53;
    emxEnsureCapacity_real_T(r1, i);
    nx = c->size[0];
    for (i = 0; i < nx; i++) {
      r1->data[i] = c->data[i] + b_c->data[i];
    }

    for (i = 0; i < 53; i++) {
      r1->data[i + c->size[0]] = 0.0;
    }

    coder::xcorr(b_resamp_signal, r1, c, input_data);

    // find(lags==0,1)
    maxval = sps * 2.0 + 1.0;
    if (maxval > c->size[0]) {
      i = 0;
      i1 = 0;
    } else {
      i = static_cast<int>(maxval) - 1;
      i1 = c->size[0];
    }

    nx = i1 - i;
    i1 = b_c->size[0];
    b_c->size[0] = nx;
    emxEnsureCapacity_real_T(b_c, i1);
    for (i1 = 0; i1 < nx; i1++) {
      b_c->data[i1] = c->data[i + i1];
    }

    i = c->size[0];
    c->size[0] = b_c->size[0];
    emxEnsureCapacity_real_T(c, i);
    nx = b_c->size[0];
    for (i = 0; i < nx; i++) {
      c->data[i] = b_c->data[i];
    }

    coder::internal::maximum(c, &b_r, &unnamed_idx_1);
    maxval = Q + (static_cast<double>(unnamed_idx_1) - 1.0);
    if (unnamed_idx_1 > maxval) {
      i = -1;
      i1 = -1;
    } else {
      i = unnamed_idx_1 - 2;
      i1 = static_cast<int>(maxval) - 1;
    }

    if (1.0 > Q) {
      nx = 0;
    } else {
      nx = static_cast<int>(Q);
    }

    del.re = f_est * 0.0;
    del.im = f_est * 6.2831853071795862;
    k = r->size[0];
    r->size[0] = nx;
    emxEnsureCapacity_creal_T(r, k);
    for (k = 0; k < nx; k++) {
      r->data[k].re = b_time->data[k] * del.re;
      r->data[k].im = b_time->data[k] * del.im;
    }

    nx = r->size[0];
    for (k = 0; k < nx; k++) {
      if (r->data[k].im == 0.0) {
        r->data[k].re = std::exp(r->data[k].re);
        r->data[k].im = 0.0;
      } else if (rtIsInf(r->data[k].im) && rtIsInf(r->data[k].re) && (r->data[k]
                  .re < 0.0)) {
        r->data[k].re = 0.0;
        r->data[k].im = 0.0;
      } else {
        b_r = std::exp(r->data[k].re / 2.0);
        r->data[k].re = b_r * (b_r * std::cos(r->data[k].im));
        r->data[k].im = b_r * (b_r * std::sin(r->data[k].im));
      }
    }

    b_r = sps * 10.0 / 2.0;
    if (b_r < 1.0) {
      input_data->size[0] = 1;
      input_data->size[1] = 0;
    } else {
      k = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = static_cast<int>(std::floor(b_r - 1.0));
      input_data->size[1] = nx + 1;
      emxEnsureCapacity_real_T(input_data, k);
      for (k = 0; k <= nx; k++) {
        input_data->data[k] = static_cast<double>(k) + 1.0;
      }
    }

    nx = i1 - i;
    k = c_resamp_signal->size[0];
    c_resamp_signal->size[0] = nx + input_data->size[1];
    emxEnsureCapacity_creal_T(c_resamp_signal, k);
    for (k = 0; k < nx; k++) {
      b_r = resamp_signal->data[(i + k) + 1];
      c_resamp_signal->data[k].re = 2.0 * (b_r * r->data[k].re);
      c_resamp_signal->data[k].im = 2.0 * (b_r * r->data[k].im);
    }

    nx = input_data->size[1];
    for (k = 0; k < nx; k++) {
      unnamed_idx_1 = (k + i1) - i;
      c_resamp_signal->data[unnamed_idx_1].re = input_data->data[k] * 0.0;
      c_resamp_signal->data[unnamed_idx_1].im = 0.0;
    }

    rxFilter1.step(c_resamp_signal, z_data, z_size);
    del = coder::qammod(pre_qam, M);
    if (z_data[9].im == 0.0) {
      if (del.im == 0.0) {
        re = del.re / z_data[9].re;
        b_r = 0.0;
      } else if (del.re == 0.0) {
        re = 0.0;
        b_r = del.im / z_data[9].re;
      } else {
        re = del.re / z_data[9].re;
        b_r = del.im / z_data[9].re;
      }
    } else if (z_data[9].re == 0.0) {
      if (del.re == 0.0) {
        re = del.im / z_data[9].im;
        b_r = 0.0;
      } else if (del.im == 0.0) {
        re = 0.0;
        b_r = -(del.re / z_data[9].im);
      } else {
        re = del.im / z_data[9].im;
        b_r = -(del.re / z_data[9].im);
      }
    } else {
      maxval = std::abs(z_data[9].re);
      b_r = std::abs(z_data[9].im);
      if (maxval > b_r) {
        b_r = z_data[9].im / z_data[9].re;
        Q = z_data[9].re + b_r * z_data[9].im;
        re = (del.re + b_r * del.im) / Q;
        b_r = (del.im - b_r * del.re) / Q;
      } else if (b_r == maxval) {
        if (z_data[9].re > 0.0) {
          b_r = 0.5;
        } else {
          b_r = -0.5;
        }

        if (z_data[9].im > 0.0) {
          Q = 0.5;
        } else {
          Q = -0.5;
        }

        re = (del.re * b_r + del.im * Q) / maxval;
        b_r = (del.im * b_r - del.re * Q) / maxval;
      } else {
        b_r = z_data[9].re / z_data[9].im;
        Q = z_data[9].im + b_r * z_data[9].re;
        re = (b_r * del.re + del.im) / Q;
        b_r = (b_r * del.im - del.re) / Q;
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
      maxval = z_data[k].re;
      Q = z_data[k].im;
      b_z_data[i1].re = maxval * re - Q * b_r;
      b_z_data[i1].im = maxval * b_r + Q * re;
    }

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
      maxval = z_data[k].re;
      Q = z_data[k].im;
      qam_symbols_data[i1].re = maxval * re - Q * b_r;
      qam_symbols_data[i1].im = maxval * b_r + Q * re;
    }

    for (unnamed_idx_1 = 0; unnamed_idx_1 < 50; unnamed_idx_1++) {
      if ((trData->data[unnamed_idx_1] == pre_qam) && (trData->
           data[unnamed_idx_1 + 1] == pre_qam) && (trData->data[unnamed_idx_1 +
           2] == pre_qam) && (trData->data[unnamed_idx_1 + 3] == pre_qam * 2.0 -
           1.0)) {
        start_inf_data = unnamed_idx_1 + 4;
      }
    }

    byte_data_size[0] = 212;
    for (i = 0; i < 212; i++) {
      byte_data_data[i] = trData->data[start_inf_data + i];
    }
  }

  emxFree_real_T(&b_c);
  emxFree_creal_T(&c_resamp_signal);
  emxFree_real_T(&r1);
  emxFree_real_T(&b_resamp_signal);
  emxFree_creal_T(&r);
  emxFree_real_T(&b_time);
  emxFree_real_T(&c);
  emxFree_real_T(&trData);
  emxFree_real_T(&resamp_signal);
  emxFree_real_T(&input_data);
}

//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_free()
{
  rxFilter1.matlabCodegenDestructor();
  rxFilter1._pobj0.matlabCodegenDestructor();
}

//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_init()
{
  emxInitStruct_RaisedCosineReceiveFilter(&rxFilter1);
  rxFilter1_not_empty = false;
  rxFilter1._pobj0.matlabCodegenIsDeleted = true;
  rxFilter1.matlabCodegenIsDeleted = true;
}

//
// Input parameters
//   s   - input signal vector [N x 1];
//   p   - p paramter of samplarate conversion
//   q   - q paramter of samplarate conversion
//   x0  - fractional delay
// Arguments    : emxArray_real_T *s
//                double p
//                double q
//                emxArray_real_T *resamp_data
// Return Type  : void
//
void lagrange_resamp(emxArray_real_T *s, double p, double q, emxArray_real_T
                     *resamp_data)
{
  emxArray_real_T *r;
  double a1;
  int i;
  int loop_ub;

  //  Ouptut parameters
  //   y   - Resampled signal
  if (p > 1.0) {
    if (q == 1.0) {
      a1 = std::floor(static_cast<double>(s->size[1]) * p);
      i = resamp_data->size[0];
      resamp_data->size[0] = static_cast<int>(a1 + 1.0);
      emxEnsureCapacity_real_T(resamp_data, i);
      loop_ub = static_cast<int>(a1 + 1.0);
      for (i = 0; i < loop_ub; i++) {
        resamp_data->data[i] = 0.0;
      }
    } else {
      a1 = std::floor(static_cast<double>(s->size[1]) * p / q);
      i = resamp_data->size[0];
      resamp_data->size[0] = static_cast<int>(a1);
      emxEnsureCapacity_real_T(resamp_data, i);
      loop_ub = static_cast<int>(a1);
      for (i = 0; i < loop_ub; i++) {
        resamp_data->data[i] = 0.0;
      }
    }
  } else {
    a1 = std::floor(static_cast<double>(s->size[1]) * p / q);
    i = resamp_data->size[0];
    resamp_data->size[0] = static_cast<int>(a1);
    emxEnsureCapacity_real_T(resamp_data, i);
    loop_ub = static_cast<int>(a1);
    for (i = 0; i < loop_ub; i++) {
      resamp_data->data[i] = 0.0;
    }
  }

  emxInit_real_T(&r, 2);
  i = r->size[0] * r->size[1];
  r->size[0] = 1;
  r->size[1] = s->size[1] + 4;
  emxEnsureCapacity_real_T(r, i);
  r->data[0] = 0.0;
  r->data[1] = 0.0;
  loop_ub = s->size[1];
  for (i = 0; i < loop_ub; i++) {
    r->data[i + 2] = s->data[i];
  }

  r->data[s->size[1] + 2] = 0.0;
  r->data[s->size[1] + 3] = 0.0;
  i = s->size[0] * s->size[1];
  s->size[0] = 1;
  s->size[1] = r->size[1];
  emxEnsureCapacity_real_T(s, i);
  loop_ub = r->size[0] * r->size[1];
  for (i = 0; i < loop_ub; i++) {
    s->data[i] = r->data[i];
  }

  emxFree_real_T(&r);
  i = resamp_data->size[0];
  for (loop_ub = 0; loop_ub < i; loop_ub++) {
    double a3;
    double a3_tmp;
    double b_a3_tmp;
    double d;
    a1 = (static_cast<double>(loop_ub) + 1.0) * q / p;
    a3 = std::floor(a1);
    d = (a3 + 1.0) - a1;
    a3_tmp = s->data[static_cast<int>(a3 + 4.0) - 1];
    a1 = s->data[static_cast<int>((a3 + 4.0) - 2.0) - 1];
    b_a3_tmp = s->data[static_cast<int>((a3 + 4.0) - 1.0) - 1];
    a3 = 0.16666666666666666 * (a3_tmp - s->data[static_cast<int>((a3 + 4.0) -
      3.0) - 1]) + 0.5 * (a1 - b_a3_tmp);
    a1 = 0.5 * (a3_tmp - a1) - a3;
    resamp_data->data[loop_ub] = ((b_a3_tmp - a1 * d) + (((a3_tmp - b_a3_tmp) -
      a3) - a1) * (d * d)) - a3 * rt_powd_snf(d, 3.0);
  }
}

//
// File trailer for HS_EWL_DEMOD_QAM.cpp
//
// [EOF]
//
