//
// File: HS_EWL_RECEIVE.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
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
// Arguments    : emxArray_real_T *data
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
//                double *warning_satus
// Return Type  : void
//
void HS_EWL_RECEIVE(emxArray_real_T *data, double len, double Fs, double f_opt,
                    double sps, double mode, double Pl, double msg_len, double M,
                    double pre_qam, creal_T qam_symbols_data[], int
                    qam_symbols_size[1], double byte_data_data[], int
                    byte_data_size[1], double f_est_data[], int f_est_size[1],
                    double *warning_satus)
{
  emxArray_creal_T *c_resamp_signal;
  emxArray_creal_T *r1;
  emxArray_real_T *b_resamp_signal;
  emxArray_real_T *b_time;
  emxArray_real_T *c;
  emxArray_real_T *input_data;
  emxArray_real_T *r;
  emxArray_real_T *r2;
  emxArray_real_T *resamp_signal;
  emxArray_real_T *trData;
  creal_T b_z_data[255];
  creal_T z_data[255];
  creal_T del;
  double len_data;
  double receiveByteLen;
  int z_size[2];
  int b_z_size[1];
  int i;
  int i1;
  int warningStatusLocal;
  if (!isInitialized_HS_EWL_RECEIVE) {
    HS_EWL_RECEIVE_initialize();
  }

  warningStatusLocal = 0;
  receiveByteLen = rt_roundd_snf(len / sps);
  if (receiveByteLen < msg_len) {
    i = data->size[1];
    receiveByteLen = (msg_len - receiveByteLen) * sps;
    warningStatusLocal = static_cast<int>(receiveByteLen);
    i1 = data->size[0] * data->size[1];
    data->size[1] += static_cast<int>(receiveByteLen);
    emxEnsureCapacity_real_T(data, i1);
    for (i1 = 0; i1 < warningStatusLocal; i1++) {
      data->data[i + i1] = 0.0;
    }

    len += receiveByteLen;
    warningStatusLocal = 1;
  }

  HS_EWL_FREQ_ACQ(data, len, Fs, f_opt, sps, mode, Pl, msg_len, M, pre_qam,
                  static_cast<double>(warningStatusLocal), &receiveByteLen,
                  &len_data, f_est_data, f_est_size, warning_satus);
  emxInit_real_T(&input_data, 2);
  emxInit_real_T(&resamp_signal, 1);
  emxInit_real_T(&trData, 2);
  emxInit_real_T(&c, 1);
  emxInit_real_T(&b_time, 1);
  emxInit_real_T(&r, 1);
  emxInit_creal_T(&r1, 1);
  emxInit_real_T(&b_resamp_signal, 1);
  emxInit_real_T(&r2, 1);
  emxInit_creal_T(&c_resamp_signal, 1);
  if ((receiveByteLen == 0.0) && (len_data == 0.0)) {
    qam_symbols_size[0] = 1;
    qam_symbols_data[0].re = 0.0;
    qam_symbols_data[0].im = 0.0;
    byte_data_size[0] = 1;
    byte_data_data[0] = 0.0;
  } else {
    double brm;
    double re;
    int k;
    int nx;
    int start_inf_data;
    brm = receiveByteLen + len_data;
    if (brm > data->size[1]) {
      if (receiveByteLen > data->size[1]) {
        i = -1;
        i1 = -1;
      } else {
        i = static_cast<int>(receiveByteLen) - 2;
        i1 = data->size[1] - 1;
      }

      warningStatusLocal = static_cast<int>(brm - static_cast<double>(data->
        size[1]));
      k = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = i1 - i;
      input_data->size[1] = nx + warningStatusLocal;
      emxEnsureCapacity_real_T(input_data, k);
      for (k = 0; k < nx; k++) {
        input_data->data[k] = data->data[(i + k) + 1];
      }

      for (k = 0; k < warningStatusLocal; k++) {
        input_data->data[(k + i1) - i] = 0.0;
      }
    } else {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = static_cast<int>(std::floor(len_data));
      input_data->size[1] = nx + 1;
      emxEnsureCapacity_real_T(input_data, i);
      for (i = 0; i <= nx; i++) {
        input_data->data[i] = data->data[static_cast<int>(receiveByteLen +
          static_cast<double>(i)) - 1];
      }
    }

    start_inf_data = 0;
    if (!b_rxFilter1_not_empty) {
      b_rxFilter1.init();
      b_rxFilter1_not_empty = true;
    }

    coder::rat(f_est_data[0] * sps / Fs, &receiveByteLen, &len_data);

    //      input_data = data(index_data: index_data+len_data);
    lagrange_resamp(input_data, receiveByteLen, len_data, resamp_signal);
    receiveByteLen = static_cast<double>(resamp_signal->size[0]) / sps;
    if (receiveByteLen < 0.0) {
      receiveByteLen = std::ceil(receiveByteLen);
    } else {
      receiveByteLen = std::floor(receiveByteLen);
    }

    len_data = (receiveByteLen - 1.0) * sps;
    if (rtIsNaN(len_data - 1.0)) {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      input_data->size[1] = 1;
      emxEnsureCapacity_real_T(input_data, i);
      input_data->data[0] = rtNaN;
    } else if (len_data - 1.0 < 0.0) {
      input_data->size[0] = 1;
      input_data->size[1] = 0;
    } else if (rtIsInf(len_data - 1.0) && (0.0 == len_data - 1.0)) {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      input_data->size[1] = 1;
      emxEnsureCapacity_real_T(input_data, i);
      input_data->data[0] = rtNaN;
    } else {
      i = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = static_cast<int>(std::floor(len_data - 1.0));
      input_data->size[1] = nx + 1;
      emxEnsureCapacity_real_T(input_data, i);
      for (i = 0; i <= nx; i++) {
        input_data->data[i] = i;
      }
    }

    receiveByteLen = f_est_data[0] * sps;
    i = b_time->size[0];
    b_time->size[0] = input_data->size[1];
    emxEnsureCapacity_real_T(b_time, i);
    nx = input_data->size[1];

    // Kostyl check
    if(receiveByteLen == 0)
    {
        *warning_satus = 4.0;
        return;
    }

    for (i = 0; i < nx; i++) {
      b_time->data[i] = input_data->data[i] / receiveByteLen;
    }

    if (1.0 > sps) {
      nx = 0;
      warningStatusLocal = 0;
    } else {
      nx = static_cast<int>(sps);
      warningStatusLocal = static_cast<int>(sps);
    }

    receiveByteLen = 6.2831853071795862 * f_est_data[0];
    i = c->size[0];
    c->size[0] = nx;
    emxEnsureCapacity_real_T(c, i);
    for (i = 0; i < nx; i++) {

      // Kostyl check
      if(b_time->allocatedSize < i)
      {
        *warning_satus = 4.0;
        return;
      }
      c->data[i] = receiveByteLen * b_time->data[i];
    }

    nx = c->size[0];
    for (k = 0; k < nx; k++) {
      c->data[k] = std::cos(c->data[k]);
    }

    i = r->size[0];
    r->size[0] = warningStatusLocal;
    emxEnsureCapacity_real_T(r, i);
    for (i = 0; i < warningStatusLocal; i++) {
      r->data[i] = receiveByteLen * b_time->data[i];
    }

    nx = r->size[0];
    for (k = 0; k < nx; k++) {
      r->data[k] = std::sin(r->data[k]);
    }

    brm = sps * 2.0;
    if (1.0 > brm) {
      nx = 0;
    } else {
      nx = static_cast<int>(brm);
    }

    i = b_resamp_signal->size[0];
    b_resamp_signal->size[0] = nx;
    emxEnsureCapacity_real_T(b_resamp_signal, i);
    for (i = 0; i < nx; i++) {
      b_resamp_signal->data[i] = resamp_signal->data[i];
    }

    i = r2->size[0];
    r2->size[0] = c->size[0] + 53;
    emxEnsureCapacity_real_T(r2, i);
    nx = c->size[0];
    for (i = 0; i < nx; i++) {
      r2->data[i] = c->data[i] + r->data[i];
    }

    for (i = 0; i < 53; i++) {
      r2->data[i + c->size[0]] = 0.0;
    }

    coder::xcorr(b_resamp_signal, r2, c, input_data);

    // find(lags==0,1)
    brm = sps * 2.0 + 1.0;
    if (brm > c->size[0]) {
      i = 0;
      i1 = 0;
    } else {
      i = static_cast<int>(brm) - 1;
      i1 = c->size[0];
    }

    warningStatusLocal = i1 - i;
    if (warningStatusLocal <= 2) {
      if (warningStatusLocal == 1) {
        nx = 1;
      } else {
        brm = c->data[i + 1];
        if ((c->data[i] < brm) || (rtIsNaN(c->data[i]) && (!rtIsNaN(brm)))) {
          nx = 2;
        } else {
          nx = 1;
        }
      }
    } else {
      if (!rtIsNaN(c->data[i])) {
        nx = 1;
      } else {
        boolean_T exitg1;
        nx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k <= warningStatusLocal)) {
          if (!rtIsNaN(c->data[(i + k) - 1])) {
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
        receiveByteLen = c->data[(i + nx) - 1];
        i1 = nx + 1;
        for (k = i1; k <= warningStatusLocal; k++) {
          brm = c->data[(i + k) - 1];
          if (receiveByteLen < brm) {
            receiveByteLen = brm;
            nx = k;
          }
        }
      }
    }

    brm = len_data + (static_cast<double>(nx) - 1.0);
    if (nx > brm) {
      i = -1;
      i1 = -1;
    } else {
      i = nx - 2;
      i1 = static_cast<int>(brm) - 1;
    }

    if (1.0 > len_data) {
      nx = 0;
    } else {
      nx = static_cast<int>(len_data);
    }

    del.re = f_est_data[0] * 0.0;
    del.im = f_est_data[0] * 6.2831853071795862;
    k = r1->size[0];
    r1->size[0] = nx;
    emxEnsureCapacity_creal_T(r1, k);
    for (k = 0; k < nx; k++) {
      r1->data[k].re = b_time->data[k] * del.re;
      r1->data[k].im = b_time->data[k] * del.im;
    }

    nx = r1->size[0];
    for (k = 0; k < nx; k++) {
      if (r1->data[k].im == 0.0) {
        r1->data[k].re = std::exp(r1->data[k].re);
        r1->data[k].im = 0.0;
      } else if (rtIsInf(r1->data[k].im) && rtIsInf(r1->data[k].re) && (r1->
                  data[k].re < 0.0)) {
        r1->data[k].re = 0.0;
        r1->data[k].im = 0.0;
      } else {
        receiveByteLen = std::exp(r1->data[k].re / 2.0);
        r1->data[k].re = receiveByteLen * (receiveByteLen * std::cos(r1->data[k]
          .im));
        r1->data[k].im = receiveByteLen * (receiveByteLen * std::sin(r1->data[k]
          .im));
      }
    }

    receiveByteLen = sps * 10.0 / 2.0;
    if (receiveByteLen < 1.0) {
      input_data->size[0] = 1;
      input_data->size[1] = 0;
    } else {
      k = input_data->size[0] * input_data->size[1];
      input_data->size[0] = 1;
      nx = static_cast<int>(std::floor(receiveByteLen - 1.0));
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
      receiveByteLen = resamp_signal->data[(i + k) + 1];
      c_resamp_signal->data[k].re = 2.0 * (receiveByteLen * r1->data[k].re);
      c_resamp_signal->data[k].im = 2.0 * (receiveByteLen * r1->data[k].im);
    }

    nx = input_data->size[1];
    for (k = 0; k < nx; k++) {
      warningStatusLocal = (k + i1) - i;
      c_resamp_signal->data[warningStatusLocal].re = input_data->data[k] * 0.0;
      c_resamp_signal->data[warningStatusLocal].im = 0.0;
    }

    b_rxFilter1.step(c_resamp_signal, z_data, z_size);
    del = coder::qammod(pre_qam, M);
    if (z_data[9].im == 0.0) {
      if (del.im == 0.0) {
        re = del.re / z_data[9].re;
        receiveByteLen = 0.0;
      } else if (del.re == 0.0) {
        re = 0.0;
        receiveByteLen = del.im / z_data[9].re;
      } else {
        re = del.re / z_data[9].re;
        receiveByteLen = del.im / z_data[9].re;
      }
    } else if (z_data[9].re == 0.0) {
      if (del.re == 0.0) {
        re = del.im / z_data[9].im;
        receiveByteLen = 0.0;
      } else if (del.im == 0.0) {
        re = 0.0;
        receiveByteLen = -(del.re / z_data[9].im);
      } else {
        re = del.im / z_data[9].im;
        receiveByteLen = -(del.re / z_data[9].im);
      }
    } else {
      brm = std::abs(z_data[9].re);
      receiveByteLen = std::abs(z_data[9].im);
      if (brm > receiveByteLen) {
        receiveByteLen = z_data[9].im / z_data[9].re;
        len_data = z_data[9].re + receiveByteLen * z_data[9].im;
        re = (del.re + receiveByteLen * del.im) / len_data;
        receiveByteLen = (del.im - receiveByteLen * del.re) / len_data;
      } else if (receiveByteLen == brm) {
        if (z_data[9].re > 0.0) {
          receiveByteLen = 0.5;
        } else {
          receiveByteLen = -0.5;
        }

        if (z_data[9].im > 0.0) {
          len_data = 0.5;
        } else {
          len_data = -0.5;
        }

        re = (del.re * receiveByteLen + del.im * len_data) / brm;
        receiveByteLen = (del.im * receiveByteLen - del.re * len_data) / brm;
      } else {
        receiveByteLen = z_data[9].re / z_data[9].im;
        len_data = z_data[9].im + receiveByteLen * z_data[9].re;
        re = (receiveByteLen * del.re + del.im) / len_data;
        receiveByteLen = (receiveByteLen * del.im - del.re) / len_data;
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
      brm = z_data[k].re;
      len_data = z_data[k].im;
      b_z_data[i1].re = brm * re - len_data * receiveByteLen;
      b_z_data[i1].im = brm * receiveByteLen + len_data * re;
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
      brm = z_data[k].re;
      len_data = z_data[k].im;
      qam_symbols_data[i1].re = brm * re - len_data * receiveByteLen;
      qam_symbols_data[i1].im = brm * receiveByteLen + len_data * re;
    }

    for (warningStatusLocal = 0; warningStatusLocal < 50; warningStatusLocal++)
    {
      if ((trData->data[warningStatusLocal] == pre_qam) && (trData->
           data[warningStatusLocal + 1] == pre_qam) && (trData->
           data[warningStatusLocal + 2] == pre_qam) && (trData->
           data[warningStatusLocal + 3] == pre_qam * 2.0 - 1.0)) {
        start_inf_data = warningStatusLocal + 4;
      }
    }

    byte_data_size[0] = 212;
    for (i = 0; i < 212; i++) {
      byte_data_data[i] = trData->data[start_inf_data + i];
    }
  }

  emxFree_creal_T(&c_resamp_signal);
  emxFree_real_T(&r2);
  emxFree_real_T(&b_resamp_signal);
  emxFree_creal_T(&r1);
  emxFree_real_T(&r);
  emxFree_real_T(&b_time);
  emxFree_real_T(&c);
  emxFree_real_T(&trData);
  emxFree_real_T(&resamp_signal);
  emxFree_real_T(&input_data);

  //  warning_satus = 0 unput array correct
  //  warning_satus = 1 not enough sample in the end array
  //  warning_satus = 2 all or more than 0.2 array equal 0
  //  warning_satus = 3 start array sample equal 0 less than 0.2
}

//
// File trailer for HS_EWL_RECEIVE.cpp
//
// [EOF]
//
