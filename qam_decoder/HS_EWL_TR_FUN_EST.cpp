//
// File: HS_EWL_TR_FUN_EST.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//

// Include Files
#include "HS_EWL_TR_FUN_EST.h"
#include "HS_EWL_DEMOD_QAM.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_DEMOD_QAM_rtwutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "colon.h"
#include "fft.h"
#include "fftshift.h"
#include "interp1.h"
#include "minOrMax.h"
#include "movSumProdOrMean.h"
#include "rat.h"
#include "rt_nonfinite.h"
#include "xcorr.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// sweep_warning_status   = 0; %input array OK
// sweep_warning_status   = 1; %input sine singal freq or sample freq equal 0
// sweep_warning_status   = 2; %not enough len input array len < 80%
// sweep_warning_status   = 3; % len input array > 80% or < 100%
// sweep_warning_status   = 4; %no found sweep preamble(ampMod)
// sweep_warning_status   = 5; %all input array equal 0
// Arguments    : const emxArray_real_T *sweep_data
//                const emxArray_real_T *sweep_math
//                double Fs
//                double f_opt
//                double f_sine
//                double PreSPP
//                double gain_data[]
//                int gain_size[2]
//                double phase_data[]
//                int phase_size[2]
//                double *sweep_warning_status
// Return Type  : void
//
void HS_EWL_TR_FUN_EST(const emxArray_real_T *sweep_data, const emxArray_real_T *
  sweep_math, double Fs, double f_opt, double f_sine, double PreSPP, double
  gain_data[], int gain_size[2], double phase_data[], int phase_size[2], double *
  sweep_warning_status)
{
  emxArray_creal_T *TrFunEst;
  emxArray_creal_T *y;
  emxArray_real_T *b_resamp_sweep;
  emxArray_real_T *c;
  emxArray_real_T *newGain;
  emxArray_real_T *newPhase;
  emxArray_real_T *r;
  emxArray_real_T *resampSweepAfter23;
  emxArray_real_T *resamp_sweep;
  double b_tmp_data[2048];
  double y_data[2048];
  double b_gain_s[1024];
  double tmp_data[1024];
  double gain_s[513];
  double P;
  double Q;
  int tmp_size[2];
  int flag_array_zero;
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  emxInit_real_T(&resamp_sweep, 1);
  emxInit_real_T(&resampSweepAfter23, 2);
  emxInit_creal_T(&TrFunEst, 2);
  emxInit_real_T(&newPhase, 2);
  emxInit_real_T(&newGain, 2);
  emxInit_real_T(&c, 1);
  emxInit_creal_T(&y, 2);
  emxInit_real_T(&r, 2);
  emxInit_real_T(&b_resamp_sweep, 1);
  if ((f_opt == 0.0) || (Fs == 0.0)) {
    *sweep_warning_status = 1.0;

    // input sine singal freq or sample freq equal 0
    gain_size[0] = 1;
    gain_size[1] = 1;
    gain_data[0] = 0.0;
    phase_size[0] = 1;
    phase_size[1] = 1;
    phase_data[0] = 0.0;
  } else {
    int i;
    boolean_T exitg1;
    flag_array_zero = 1;
    i = 0;
    exitg1 = false;
    while ((!exitg1) && (i <= sweep_data->size[1] - 1)) {
      if ((sweep_data->data[i] > 0.05) || (sweep_data->data[i] < -0.05)) {
        flag_array_zero = 0;
        exitg1 = true;
      } else {
        i++;
      }
    }

    if (flag_array_zero == 1) {
      *sweep_warning_status = 5.0;

      // all input array equal 0
      gain_size[0] = 1;
      gain_size[1] = 1;
      gain_data[0] = 0.0;
      phase_size[0] = 1;
      phase_size[1] = 1;
      phase_data[0] = 0.0;
    } else {
      int b_i;
      int loop_ub;
      boolean_T guard1 = false;

      // sample frequency math sweep
      // 280373;
      // %%240MHz / 131
      coder::rat(f_opt * 280000.0 / f_sine / Fs, &P, &Q);
      b_i = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
      resampSweepAfter23->size[0] = 1;
      resampSweepAfter23->size[1] = sweep_data->size[1];
      emxEnsureCapacity_real_T(resampSweepAfter23, b_i);
      loop_ub = sweep_data->size[0] * sweep_data->size[1] - 1;
      for (b_i = 0; b_i <= loop_ub; b_i++) {
        resampSweepAfter23->data[b_i] = sweep_data->data[b_i];
      }

      lagrange_resamp(resampSweepAfter23, P, Q, resamp_sweep);
      P = coder::internal::b_maximum(resamp_sweep);
      loop_ub = resamp_sweep->size[0];
      for (b_i = 0; b_i < loop_ub; b_i++) {
        resamp_sweep->data[b_i] /= P;
      }

      guard1 = false;
      if (resamp_sweep->size[0] < sweep_math->size[1]) {
        if (resamp_sweep->size[0] < static_cast<double>(sweep_math->size[1]) *
            0.8) {
          *sweep_warning_status = 2.0;

          // not enough len input array len < 80%
          gain_size[0] = 1;
          gain_size[1] = 1;
          gain_data[0] = 0.0;
          phase_size[0] = 1;
          phase_size[1] = 1;
          phase_data[0] = 0.0;
        } else {
          //          resamp_sweep = resamp_sweep(1:length(sweep_math));
          loop_ub = sweep_math->size[1] - resamp_sweep->size[0];
          b_i = b_resamp_sweep->size[0];
          b_resamp_sweep->size[0] = resamp_sweep->size[0] + loop_ub;
          emxEnsureCapacity_real_T(b_resamp_sweep, b_i);
          flag_array_zero = resamp_sweep->size[0];
          for (b_i = 0; b_i < flag_array_zero; b_i++) {
            b_resamp_sweep->data[b_i] = resamp_sweep->data[b_i];
          }

          for (b_i = 0; b_i < loop_ub; b_i++) {
            b_resamp_sweep->data[b_i + resamp_sweep->size[0]] = 0.0;
          }

          b_i = resamp_sweep->size[0];
          resamp_sweep->size[0] = b_resamp_sweep->size[0];
          emxEnsureCapacity_real_T(resamp_sweep, b_i);
          loop_ub = b_resamp_sweep->size[0];
          for (b_i = 0; b_i < loop_ub; b_i++) {
            resamp_sweep->data[b_i] = b_resamp_sweep->data[b_i];
          }

          *sweep_warning_status = 3.0;

          //  len input array > 80% or < 100%
          guard1 = true;
        }
      } else {
        b_i = resamp_sweep->size[0];
        if (1 > sweep_math->size[1]) {
          resamp_sweep->size[0] = 0;
        } else {
          resamp_sweep->size[0] = sweep_math->size[1];
        }

        emxEnsureCapacity_real_T(resamp_sweep, b_i);
        *sweep_warning_status = 0.0;

        //  input array OK
        guard1 = true;
      }

      if (guard1) {
        int i1;
        P = coder::internal::b_maximum(resamp_sweep);
        b_i = b_resamp_sweep->size[0];
        b_resamp_sweep->size[0] = resamp_sweep->size[0];
        emxEnsureCapacity_real_T(b_resamp_sweep, b_i);
        loop_ub = resamp_sweep->size[0];
        for (b_i = 0; b_i < loop_ub; b_i++) {
          b_resamp_sweep->data[b_i] = resamp_sweep->data[b_i] / P;
        }

        coder::xcorr(b_resamp_sweep, *(double (*)[1820])&sweep_math->data[0], c,
                     resampSweepAfter23);
        P = static_cast<double>(resamp_sweep->size[0]) - PreSPP * 2.0;
        if (P > c->size[0]) {
          b_i = 0;
          i1 = 0;
        } else {
          b_i = static_cast<int>(P) - 1;
          i1 = c->size[0];
        }

        loop_ub = i1 - b_i;
        i1 = b_resamp_sweep->size[0];
        b_resamp_sweep->size[0] = loop_ub;
        emxEnsureCapacity_real_T(b_resamp_sweep, i1);
        for (i1 = 0; i1 < loop_ub; i1++) {
          b_resamp_sweep->data[i1] = c->data[b_i + i1];
        }

        b_i = c->size[0];
        c->size[0] = b_resamp_sweep->size[0];
        emxEnsureCapacity_real_T(c, b_i);
        loop_ub = b_resamp_sweep->size[0];
        for (b_i = 0; b_i < loop_ub; b_i++) {
          c->data[b_i] = b_resamp_sweep->data[b_i];
        }

        coder::internal::maximum(c, &P, &flag_array_zero);
        if (flag_array_zero >= PreSPP * 2.0) {
          *sweep_warning_status = 4.0;

          // no found sweep preamble(ampMod)
          gain_size[0] = 1;
          gain_size[1] = 1;
          gain_data[0] = 0.0;
          phase_size[0] = 1;
          phase_size[1] = 1;
          phase_data[0] = 0.0;
        } else {
          double bi;
          double br;
          double pi_count;
          int k;
          P = (PreSPP * 2.0 - static_cast<double>(flag_array_zero)) + 2.0;
          Q = static_cast<double>(resamp_sweep->size[0]) - P;
          if (1.0 > Q) {
            loop_ub = 0;
          } else {
            loop_ub = static_cast<int>(Q);
          }

          flag_array_zero = static_cast<int>(P);
          b_i = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
          resampSweepAfter23->size[0] = 1;
          resampSweepAfter23->size[1] = static_cast<int>(P) + loop_ub;
          emxEnsureCapacity_real_T(resampSweepAfter23, b_i);
          for (b_i = 0; b_i < flag_array_zero; b_i++) {
            resampSweepAfter23->data[b_i] = 0.0;
          }

          for (b_i = 0; b_i < loop_ub; b_i++) {
            resampSweepAfter23->data[b_i + static_cast<int>(P)] =
              resamp_sweep->data[b_i];
          }

          if (1820 > resampSweepAfter23->size[1]) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = 1819;
            i1 = resampSweepAfter23->size[1];
          }

          if (1820 > sweep_math->size[1]) {
            flag_array_zero = 0;
            i = 0;
          } else {
            flag_array_zero = 1819;
            i = sweep_math->size[1];
          }

          k = newGain->size[0] * newGain->size[1];
          newGain->size[0] = 1;
          loop_ub = i1 - b_i;
          newGain->size[1] = loop_ub;
          emxEnsureCapacity_real_T(newGain, k);
          for (i1 = 0; i1 < loop_ub; i1++) {
            newGain->data[i1] = resampSweepAfter23->data[b_i + i1];
          }

          coder::fft(newGain, TrFunEst);
          b_i = newGain->size[0] * newGain->size[1];
          newGain->size[0] = 1;
          loop_ub = i - flag_array_zero;
          newGain->size[1] = loop_ub;
          emxEnsureCapacity_real_T(newGain, b_i);
          for (b_i = 0; b_i < loop_ub; b_i++) {
            newGain->data[b_i] = sweep_math->data[flag_array_zero + b_i];
          }

          coder::fft(newGain, y);
          b_i = TrFunEst->size[0] * TrFunEst->size[1];
          i1 = TrFunEst->size[0] * TrFunEst->size[1];
          TrFunEst->size[0] = 1;
          emxEnsureCapacity_creal_T(TrFunEst, i1);
          loop_ub = b_i - 1;
          for (b_i = 0; b_i <= loop_ub; b_i++) {
            double ai;
            double ar;
            double re;
            ar = TrFunEst->data[b_i].re;
            ai = TrFunEst->data[b_i].im;
            br = y->data[b_i].re;
            bi = y->data[b_i].im;
            if (bi == 0.0) {
              if (ai == 0.0) {
                re = ar / br;
                P = 0.0;
              } else if (ar == 0.0) {
                re = 0.0;
                P = ai / br;
              } else {
                re = ar / br;
                P = ai / br;
              }
            } else if (br == 0.0) {
              if (ar == 0.0) {
                re = ai / bi;
                P = 0.0;
              } else if (ai == 0.0) {
                re = 0.0;
                P = -(ar / bi);
              } else {
                re = ai / bi;
                P = -(ar / bi);
              }
            } else {
              pi_count = std::abs(br);
              P = std::abs(bi);
              if (pi_count > P) {
                Q = bi / br;
                P = br + Q * bi;
                re = (ar + Q * ai) / P;
                P = (ai - Q * ar) / P;
              } else if (P == pi_count) {
                if (br > 0.0) {
                  Q = 0.5;
                } else {
                  Q = -0.5;
                }

                if (bi > 0.0) {
                  P = 0.5;
                } else {
                  P = -0.5;
                }

                re = (ar * Q + ai * P) / pi_count;
                P = (ai * Q - ar * P) / pi_count;
              } else {
                Q = br / bi;
                P = bi + Q * br;
                re = (Q * ar + ai) / P;
                P = (Q * ai - ar) / P;
              }
            }

            TrFunEst->data[b_i].re = re;
            TrFunEst->data[b_i].im = P;
          }

          coder::fftshift(TrFunEst);
          if (1820 > sweep_math->size[1]) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = 1819;
            i1 = sweep_math->size[1];
          }

          P = 280000.0 / static_cast<double>(i1 - b_i);
          if (rtIsInf(P)) {
            b_i = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
            resampSweepAfter23->size[0] = 1;
            resampSweepAfter23->size[1] = 1;
            emxEnsureCapacity_real_T(resampSweepAfter23, b_i);
            resampSweepAfter23->data[0] = -140000.0;
          } else if (std::floor(P) == P) {
            b_i = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
            resampSweepAfter23->size[0] = 1;
            loop_ub = static_cast<int>(std::floor(279999.0 / P));
            resampSweepAfter23->size[1] = loop_ub + 1;
            emxEnsureCapacity_real_T(resampSweepAfter23, b_i);
            for (b_i = 0; b_i <= loop_ub; b_i++) {
              resampSweepAfter23->data[b_i] = P * static_cast<double>(b_i) +
                -140000.0;
            }
          } else {
            coder::eml_float_colon(P, resampSweepAfter23);
          }

          if (1820 > sweep_math->size[1]) {
            b_i = 0;
            i1 = 0;
            flag_array_zero = 0;
            i = 0;
          } else {
            b_i = 1819;
            i1 = sweep_math->size[1];
            flag_array_zero = 1819;
            i = sweep_math->size[1];
          }

          br = rt_roundd_snf(140000.0 / (280000.0 / static_cast<double>(i1 - b_i)));
          bi = rt_roundd_snf(210000.0 / (280000.0 / static_cast<double>(i -
            flag_array_zero)));
          if (br + 1.0 > bi + 1.0) {
            b_i = 0;
            i1 = -1;
          } else {
            b_i = static_cast<int>(br + 1.0) - 1;
            i1 = static_cast<int>(bi + 1.0) - 1;
          }

          i = (i1 - b_i) + 1;
          i1 = newPhase->size[0] * newPhase->size[1];
          newPhase->size[0] = 1;
          newPhase->size[1] = i;
          emxEnsureCapacity_real_T(newPhase, i1);
          for (k = 0; k < i; k++) {
            flag_array_zero = b_i + k;
            newPhase->data[k] = rt_atan2d_snf(TrFunEst->data[flag_array_zero].im,
              TrFunEst->data[flag_array_zero].re);
          }

          if (br + 1.0 > bi + 1.0) {
            b_i = 0;
            i1 = -1;
          } else {
            b_i = static_cast<int>(br + 1.0) - 1;
            i1 = static_cast<int>(bi + 1.0) - 1;
          }

          i = (i1 - b_i) + 1;
          i1 = newGain->size[0] * newGain->size[1];
          newGain->size[0] = 1;
          newGain->size[1] = i;
          emxEnsureCapacity_real_T(newGain, i1);
          for (k = 0; k < i; k++) {
            flag_array_zero = b_i + k;
            newGain->data[k] = rt_hypotd_snf(TrFunEst->data[flag_array_zero].re,
              TrFunEst->data[flag_array_zero].im);
          }

          pi_count = 2.0;
          b_i = newPhase->size[1];
          for (i = 0; i <= b_i - 2; i++) {
            P = newPhase->data[i + 1];
            Q = newPhase->data[i];
            if (P - Q > std::floor((pi_count - 1.0) * 2.0 * 3.1415926535897931)
                - 0.1) {
              if (P - Q > std::floor(pi_count * 2.0 * 3.1415926535897931) - 0.1)
              {
                pi_count++;
              }

              P -= (pi_count - 1.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = P;
            } else {
              P -= (pi_count - 2.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = P;
            }
          }

          b_i = newPhase->size[1];
          for (i = 0; i <= b_i - 2; i++) {
            P = newPhase->data[i + 1];
            Q = newPhase->data[i];
            if (P - Q < -(std::floor((pi_count - 1.0) * 2.0 * 3.1415926535897931)
                          - 0.1)) {
              if (P - Q < -(std::floor(pi_count * 2.0 * 3.1415926535897931) -
                            0.1)) {
                pi_count++;
              }

              P += (pi_count - 1.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = P;
            } else {
              P += (pi_count - 2.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = P;
            }
          }

          if (br + 1.0 > bi + 1.0) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = static_cast<int>(br + 1.0) - 1;
            i1 = static_cast<int>(bi + 1.0);
          }

          coder::vmovfun(newGain, newGain->size[1], r);
          flag_array_zero = newGain->size[0] * newGain->size[1];
          newGain->size[0] = 1;
          loop_ub = i1 - b_i;
          newGain->size[1] = loop_ub;
          emxEnsureCapacity_real_T(newGain, flag_array_zero);
          for (i1 = 0; i1 < loop_ub; i1++) {
            newGain->data[i1] = resampSweepAfter23->data[b_i + i1];
          }

          coder::interp1(newGain, r, gain_s);

          // 69725
          std::memcpy(&b_gain_s[0], &gain_s[0], 513U * sizeof(double));
          for (b_i = 0; b_i < 511; b_i++) {
            b_gain_s[b_i + 513] = gain_s[512];
          }

          b_gain_s[0] = b_gain_s[2];
          b_gain_s[1] = b_gain_s[2];
          for (k = 0; k < 2048; k++) {
            y_data[k] = std::abs(136.71875 * static_cast<double>(k) + -140000.0);
          }

          for (b_i = 0; b_i < 1024; b_i++) {
            tmp_data[b_i] = 136.71875 * (static_cast<double>(b_i) + 1024.0) +
              -140000.0;
          }

          coder::interp1(tmp_data, b_gain_s, y_data, b_tmp_data, tmp_size);
          gain_size[0] = 1;
          gain_size[1] = 2048;
          std::memcpy(&gain_data[0], &b_tmp_data[0], 2048U * sizeof(double));
          if (br + 1.0 > bi + 1.0) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = static_cast<int>(br + 1.0) - 1;
            i1 = static_cast<int>(bi + 1.0);
          }

          flag_array_zero = newGain->size[0] * newGain->size[1];
          newGain->size[0] = 1;
          loop_ub = i1 - b_i;
          newGain->size[1] = loop_ub;
          emxEnsureCapacity_real_T(newGain, flag_array_zero);
          for (i1 = 0; i1 < loop_ub; i1++) {
            newGain->data[i1] = resampSweepAfter23->data[b_i + i1];
          }

          coder::vmovfun(newPhase, newPhase->size[1], r);
          coder::interp1(newGain, r, gain_s);

          // 70000
          std::memcpy(&b_gain_s[0], &gain_s[0], 513U * sizeof(double));
          for (b_i = 0; b_i < 511; b_i++) {
            b_gain_s[b_i + 513] = gain_s[512];
          }

          // 70000
          b_gain_s[0] = b_gain_s[2];
          b_gain_s[1] = b_gain_s[2];
          for (k = 0; k < 2048; k++) {
            y_data[k] = std::abs(136.71875 * static_cast<double>(k) + -140000.0);
          }

          for (b_i = 0; b_i < 1024; b_i++) {
            tmp_data[b_i] = 136.71875 * (static_cast<double>(b_i) + 1024.0) +
              -140000.0;
          }

          coder::interp1(tmp_data, b_gain_s, y_data, b_tmp_data, tmp_size);
          phase_size[0] = 1;
          phase_size[1] = 2048;
          std::memcpy(&phase_data[0], &b_tmp_data[0], 2048U * sizeof(double));
        }
      }
    }
  }

  emxFree_real_T(&b_resamp_sweep);
  emxFree_real_T(&r);
  emxFree_creal_T(&y);
  emxFree_real_T(&c);
  emxFree_real_T(&newGain);
  emxFree_real_T(&newPhase);
  emxFree_creal_T(&TrFunEst);
  emxFree_real_T(&resampSweepAfter23);
  emxFree_real_T(&resamp_sweep);
}

//
// File trailer for HS_EWL_TR_FUN_EST.cpp
//
// [EOF]
//
