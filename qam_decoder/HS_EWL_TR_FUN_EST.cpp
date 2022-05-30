//
// File: HS_EWL_TR_FUN_EST.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
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
#include "diff.h"
#include "eml_fftshift.h"
#include "exp.h"
#include "fft.h"
#include "ifft.h"
#include "ifftshift.h"
#include "interp1.h"
#include "mean.h"
#include "minOrMax.h"
#include "movSumProdOrMean.h"
#include "rat.h"
#include "rt_nonfinite.h"
#include "sign.h"
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
// sweep_warning_status   = 6; %phase dosen't work with input sweep
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
//                double *shift_for_qam_data
//                double *sweep_warning_status
// Return Type  : void
//
void HS_EWL_TR_FUN_EST(const emxArray_real_T *sweep_data, const emxArray_real_T *
  sweep_math, double Fs, double f_opt, double f_sine, double PreSPP, double
  gain_data[], int gain_size[2], double phase_data[], int phase_size[2], double *
  shift_for_qam_data, double *sweep_warning_status)
{
  emxArray_creal_T *TrFunEst;
  emxArray_creal_T *spec_sweep;
  emxArray_real_T b_resampSweepAfter23;
  emxArray_real_T *F_tr;
  emxArray_real_T *b_F_tr;
  emxArray_real_T *b_resamp_sweep;
  emxArray_real_T *c;
  emxArray_real_T *ft;
  emxArray_real_T *gain_test_tmp;
  emxArray_real_T *newPhase;
  emxArray_real_T *r;
  emxArray_real_T *resampSweepAfter23;
  emxArray_real_T *resamp_sweep;
  double b_tmp_data[2048];
  double gain_s_data[2048];
  double phase_s_data[2048];
  double sweepPreamble[1820];
  double b_gain_s[1024];
  double tmp_data[1024];
  double gain_s[513];
  double br;
  double corr_var;
  double pi_count;
  double pi_jump_count;
  int gain_s_size[2];
  int c_resampSweepAfter23[1];
  int iindx;
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  emxInit_real_T(&resamp_sweep, 1);
  emxInit_real_T(&F_tr, 2);
  emxInit_real_T(&resampSweepAfter23, 2);
  emxInit_creal_T(&TrFunEst, 2);
  emxInit_real_T(&newPhase, 2);
  emxInit_real_T(&ft, 2);
  emxInit_creal_T(&spec_sweep, 2);
  emxInit_real_T(&c, 1);
  emxInit_real_T(&gain_test_tmp, 2);
  emxInit_real_T(&b_resamp_sweep, 1);
  emxInit_real_T(&b_F_tr, 2);
  emxInit_real_T(&r, 2);
  if ((f_opt == 0.0) || (Fs == 0.0)) {
    *sweep_warning_status = 1.0;

    // input sine singal freq or sample freq equal 0
    gain_size[0] = 1;
    gain_size[1] = 1;
    gain_data[0] = 0.0;
    phase_size[0] = 1;
    phase_size[1] = 1;
    phase_data[0] = 0.0;
    *shift_for_qam_data = 0.0;
  } else {
    int flag_array_zero;
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
      *shift_for_qam_data = 0.0;
    } else {
      int b_i;
      int loop_ub;
      boolean_T guard1 = false;

      // sample frequency math sweep
      // 280373;
      // %%240MHz / 131
      coder::rat(f_opt * 280000.0 / f_sine / Fs, &pi_count, &pi_jump_count);
      b_i = F_tr->size[0] * F_tr->size[1];
      F_tr->size[0] = 1;
      F_tr->size[1] = sweep_data->size[1];
      emxEnsureCapacity_real_T(F_tr, b_i);
      loop_ub = sweep_data->size[0] * sweep_data->size[1] - 1;
      for (b_i = 0; b_i <= loop_ub; b_i++) {
        F_tr->data[b_i] = sweep_data->data[b_i];
      }

      lagrange_resamp(F_tr, pi_count, pi_jump_count, resamp_sweep);
      pi_count = coder::internal::b_maximum(resamp_sweep);
      loop_ub = resamp_sweep->size[0];
      for (b_i = 0; b_i < loop_ub; b_i++) {
        resamp_sweep->data[b_i] /= pi_count;
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
          *shift_for_qam_data = 0.0;
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
        double b_x;
        double x;
        int i1;
        int i2;
        int i3;
        for (b_i = 0; b_i < 1820; b_i++) {
          sweepPreamble[b_i] = sweep_math->data[b_i];
        }

        if (1820 > sweep_math->size[1]) {
          b_i = 0;
          i1 = 0;
        } else {
          b_i = 1819;
          i1 = sweep_math->size[1];
        }

        pi_jump_count = 280000.0 / static_cast<double>(i1 - b_i);
        if (rtIsInf(pi_jump_count)) {
          b_i = F_tr->size[0] * F_tr->size[1];
          F_tr->size[0] = 1;
          F_tr->size[1] = 1;
          emxEnsureCapacity_real_T(F_tr, b_i);
          F_tr->data[0] = -140000.0;
        } else if (std::floor(pi_jump_count) == pi_jump_count) {
          b_i = F_tr->size[0] * F_tr->size[1];
          F_tr->size[0] = 1;
          loop_ub = static_cast<int>(std::floor(279999.0 / pi_jump_count));
          F_tr->size[1] = loop_ub + 1;
          emxEnsureCapacity_real_T(F_tr, b_i);
          for (b_i = 0; b_i <= loop_ub; b_i++) {
            F_tr->data[b_i] = pi_jump_count * static_cast<double>(b_i) +
              -140000.0;
          }
        } else {
          coder::eml_float_colon(pi_jump_count, F_tr);
        }

        if (1820 > sweep_math->size[1]) {
          b_i = 0;
          i1 = 0;
          i2 = 0;
          i3 = 0;
        } else {
          b_i = 1819;
          i1 = sweep_math->size[1];
          i2 = 1819;
          i3 = sweep_math->size[1];
        }

        x = rt_roundd_snf(140000.0 / (280000.0 / static_cast<double>(i1 - b_i)));
        b_x = rt_roundd_snf(210000.0 / (280000.0 / static_cast<double>(i3 - i2)));
        pi_count = coder::internal::b_maximum(resamp_sweep);
        b_i = b_resamp_sweep->size[0];
        b_resamp_sweep->size[0] = resamp_sweep->size[0];
        emxEnsureCapacity_real_T(b_resamp_sweep, b_i);
        loop_ub = resamp_sweep->size[0];
        for (b_i = 0; b_i < loop_ub; b_i++) {
          b_resamp_sweep->data[b_i] = resamp_sweep->data[b_i] / pi_count;
        }

        coder::xcorr(b_resamp_sweep, sweepPreamble, c, resampSweepAfter23);
        br = static_cast<double>(resamp_sweep->size[0]) - PreSPP * 2.0;
        if (br > c->size[0]) {
          b_i = 0;
          i1 = 0;
        } else {
          b_i = static_cast<int>(br) - 1;
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

        coder::internal::maximum(c, &pi_count, &iindx);
        if (iindx >= PreSPP * 2.0) {
          *sweep_warning_status = 4.0;

          // no found sweep preamble(ampMod)
          gain_size[0] = 1;
          gain_size[1] = 1;
          gain_data[0] = 0.0;
          phase_size[0] = 1;
          phase_size[1] = 1;
          phase_data[0] = 0.0;
          *shift_for_qam_data = 0.0;
        } else {
          double ai;
          double ar;
          double bi;
          double brm;
          double re;
          int i4;
          int i5;
          int loop_ub_tmp;
          int nx_tmp;
          corr_var = 2.0;
          if (1820 > sweep_math->size[1]) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = 1819;
            i1 = sweep_math->size[1];
          }

          loop_ub_tmp = i1 - b_i;
          if (x + 1.0 > b_x + 1.0) {
            i1 = 0;
            i2 = 0;
            i3 = 0;
            i4 = 0;
          } else {
            i1 = static_cast<int>(x + 1.0) - 1;
            i2 = static_cast<int>(b_x + 1.0);
            i3 = static_cast<int>(x + 1.0) - 1;
            i4 = static_cast<int>(b_x + 1.0);
          }

          nx_tmp = i2 - i1;
          int exitg2;
//          do {
            exitg2 = 0;
            br = (PreSPP * 2.0 - static_cast<double>(iindx)) + corr_var;
            pi_jump_count = static_cast<double>(resamp_sweep->size[0]) - br;
            if (1.0 > pi_jump_count) {
              loop_ub = 0;
            } else {
              loop_ub = static_cast<int>(pi_jump_count);
            }

            flag_array_zero = static_cast<int>(br);
            i2 = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
            resampSweepAfter23->size[0] = 1;
            resampSweepAfter23->size[1] = static_cast<int>(br) + loop_ub;
            emxEnsureCapacity_real_T(resampSweepAfter23, i2);
            for (i2 = 0; i2 < flag_array_zero; i2++) {
              resampSweepAfter23->data[i2] = 0.0;
            }

            for (i2 = 0; i2 < loop_ub; i2++) {
              resampSweepAfter23->data[i2 + static_cast<int>(br)] =
                resamp_sweep->data[i2];
            }

            if (1820 > resampSweepAfter23->size[1]) {
              i2 = 0;
              i5 = 0;
            } else {
              i2 = 1819;
              i5 = resampSweepAfter23->size[1];
            }

            flag_array_zero = newPhase->size[0] * newPhase->size[1];
            newPhase->size[0] = 1;
            loop_ub = i5 - i2;
            newPhase->size[1] = loop_ub;
            emxEnsureCapacity_real_T(newPhase, flag_array_zero);
            for (i5 = 0; i5 < loop_ub; i5++) {
              newPhase->data[i5] = resampSweepAfter23->data[i2 + i5];
            }

            coder::fft(newPhase, TrFunEst);
            i2 = newPhase->size[0] * newPhase->size[1];
            newPhase->size[0] = 1;
            newPhase->size[1] = loop_ub_tmp;
            emxEnsureCapacity_real_T(newPhase, i2);
            for (i2 = 0; i2 < loop_ub_tmp; i2++) {
              newPhase->data[i2] = sweep_math->data[b_i + i2];
            }

            coder::fft(newPhase, spec_sweep);
            i2 = TrFunEst->size[0] * TrFunEst->size[1];
            i5 = TrFunEst->size[0] * TrFunEst->size[1];
            TrFunEst->size[0] = 1;
            emxEnsureCapacity_creal_T(TrFunEst, i5);
            loop_ub = i2 - 1;
            for (i2 = 0; i2 <= loop_ub; i2++) {
              ar = TrFunEst->data[i2].re;
              ai = TrFunEst->data[i2].im;
              br = spec_sweep->data[i2].re;
              bi = spec_sweep->data[i2].im;
              if (bi == 0.0) {
                if (ai == 0.0) {
                  re = ar / br;
                  pi_jump_count = 0.0;
                } else if (ar == 0.0) {
                  re = 0.0;
                  pi_jump_count = ai / br;
                } else {
                  re = ar / br;
                  pi_jump_count = ai / br;
                }
              } else if (br == 0.0) {
                if (ar == 0.0) {
                  re = ai / bi;
                  pi_jump_count = 0.0;
                } else if (ai == 0.0) {
                  re = 0.0;
                  pi_jump_count = -(ar / bi);
                } else {
                  re = ai / bi;
                  pi_jump_count = -(ar / bi);
                }
              } else {
                brm = std::abs(br);
                pi_jump_count = std::abs(bi);
                if (brm > pi_jump_count) {
                  pi_count = bi / br;
                  pi_jump_count = br + pi_count * bi;
                  re = (ar + pi_count * ai) / pi_jump_count;
                  pi_jump_count = (ai - pi_count * ar) / pi_jump_count;
                } else if (pi_jump_count == brm) {
                  if (br > 0.0) {
                    pi_count = 0.5;
                  } else {
                    pi_count = -0.5;
                  }

                  if (bi > 0.0) {
                    pi_jump_count = 0.5;
                  } else {
                    pi_jump_count = -0.5;
                  }

                  re = (ar * pi_count + ai * pi_jump_count) / brm;
                  pi_jump_count = (ai * pi_count - ar * pi_jump_count) / brm;
                } else {
                  pi_count = br / bi;
                  pi_jump_count = bi + pi_count * br;
                  re = (pi_count * ar + ai) / pi_jump_count;
                  pi_jump_count = (pi_count * ai - ar) / pi_jump_count;
                }
              }

              TrFunEst->data[i2].re = re;
              TrFunEst->data[i2].im = pi_jump_count;
            }

            coder::eml_fftshift(TrFunEst, 1);
            coder::eml_fftshift(TrFunEst, 2);
            i2 = newPhase->size[0] * newPhase->size[1];
            newPhase->size[0] = 1;
            newPhase->size[1] = nx_tmp;
            emxEnsureCapacity_real_T(newPhase, i2);
            for (loop_ub = 0; loop_ub < nx_tmp; loop_ub++) {
              flag_array_zero = i1 + loop_ub;
              newPhase->data[loop_ub] = rt_atan2d_snf(TrFunEst->
                data[flag_array_zero].im, TrFunEst->data[flag_array_zero].re);
            }

            coder::diff(newPhase, resampSweepAfter23);
            pi_jump_count = 0.0;
            flag_array_zero = 0;
            i2 = resampSweepAfter23->size[1];
            for (i = 0; i < i2; i++) {
              br = resampSweepAfter23->data[i];
              if (((br >= 1.0) || (br <= -1.0)) && (flag_array_zero == 0)) {
                pi_jump_count++;
                flag_array_zero = 40;
              }

              if (flag_array_zero > 0) {
                flag_array_zero--;
              }
            }

            if (pi_jump_count <= 5.0) {
              exitg2 = 1;
            } else {
              corr_var *= 2.0;
            }
//          } while (exitg2 == 0);

          pi_count = 2.0;
          b_i = newPhase->size[1];
          for (i = 0; i <= b_i - 2; i++) {
            br = newPhase->data[i + 1];
            pi_jump_count = newPhase->data[i];
            if (br - pi_jump_count > std::floor((pi_count - 1.0) * 2.0 *
                 3.1415926535897931) - 0.1) {
              if (br - pi_jump_count > std::floor(pi_count * 2.0 *
                   3.1415926535897931) - 0.1) {
                pi_count++;
              }

              br -= (pi_count - 1.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = br;
            } else {
              br -= (pi_count - 2.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = br;
            }
          }

          b_i = newPhase->size[1];
          for (i = 0; i <= b_i - 2; i++) {
            br = newPhase->data[i + 1];
            pi_jump_count = newPhase->data[i];
            if (br - pi_jump_count < -(std::floor((pi_count - 1.0) * 2.0 *
                  3.1415926535897931) - 0.1)) {
              if (br - pi_jump_count < -(std::floor(pi_count * 2.0 *
                    3.1415926535897931) - 0.1)) {
                pi_count++;
              }

              br += (pi_count - 1.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = br;
            } else {
              br += (pi_count - 2.0) * 2.0 * 3.1415926535897931;
              newPhase->data[i + 1] = br;
            }
          }

          pi_jump_count = 280000.0 / static_cast<double>(resamp_sweep->size[0]);
          if (rtIsInf(pi_jump_count)) {
            b_i = ft->size[0] * ft->size[1];
            ft->size[0] = 1;
            ft->size[1] = 1;
            emxEnsureCapacity_real_T(ft, b_i);
            ft->data[0] = -140000.0;
          } else if (std::floor(pi_jump_count) == pi_jump_count) {
            b_i = ft->size[0] * ft->size[1];
            ft->size[0] = 1;
            loop_ub = static_cast<int>(std::floor(279999.0 / pi_jump_count));
            ft->size[1] = loop_ub + 1;
            emxEnsureCapacity_real_T(ft, b_i);
            for (b_i = 0; b_i <= loop_ub; b_i++) {
              ft->data[b_i] = pi_jump_count * static_cast<double>(b_i) +
                -140000.0;
            }
          } else {
            coder::eml_float_colon(pi_jump_count, ft);
          }

          if (x + 1.0 > b_x + 1.0) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = static_cast<int>(x + 1.0) - 1;
            i1 = static_cast<int>(b_x + 1.0);
          }

          nx_tmp = i4 - i3;
          i2 = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
          resampSweepAfter23->size[0] = 1;
          resampSweepAfter23->size[1] = nx_tmp;
          emxEnsureCapacity_real_T(resampSweepAfter23, i2);
          for (loop_ub = 0; loop_ub < nx_tmp; loop_ub++) {
            flag_array_zero = i3 + loop_ub;
            resampSweepAfter23->data[loop_ub] = rt_hypotd_snf(TrFunEst->
              data[flag_array_zero].re, TrFunEst->data[flag_array_zero].im);
          }

          i2 = b_F_tr->size[0] * b_F_tr->size[1];
          b_F_tr->size[0] = 1;
          loop_ub = i1 - b_i;
          b_F_tr->size[1] = loop_ub;
          emxEnsureCapacity_real_T(b_F_tr, i2);
          for (i1 = 0; i1 < loop_ub; i1++) {
            b_F_tr->data[i1] = F_tr->data[b_i + i1];
          }

          coder::vmovfun(resampSweepAfter23, resampSweepAfter23->size[1], r);
          coder::interp1(b_F_tr, r, gain_s);

          // 69725
          std::memcpy(&b_gain_s[0], &gain_s[0], 513U * sizeof(double));
          for (b_i = 0; b_i < 511; b_i++) {
            b_gain_s[b_i + 513] = gain_s[512];
          }

          b_gain_s[0] = b_gain_s[2];
          b_gain_s[1] = b_gain_s[2];
          b_i = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
          resampSweepAfter23->size[0] = 1;
          resampSweepAfter23->size[1] = 2048;
          emxEnsureCapacity_real_T(resampSweepAfter23, b_i);
          for (loop_ub = 0; loop_ub < 2048; loop_ub++) {
            resampSweepAfter23->data[loop_ub] = std::abs(136.71875 *
              static_cast<double>(loop_ub) + -140000.0);
          }

          for (b_i = 0; b_i < 1024; b_i++) {
            tmp_data[b_i] = 136.71875 * (static_cast<double>(b_i) + 1024.0) +
              -140000.0;
          }

          coder::interp1(tmp_data, b_gain_s, (double *)resampSweepAfter23->data,
                         gain_s_data, gain_s_size);
          flag_array_zero = ft->size[1];
          b_i = gain_test_tmp->size[0] * gain_test_tmp->size[1];
          gain_test_tmp->size[0] = 1;
          gain_test_tmp->size[1] = ft->size[1];
          emxEnsureCapacity_real_T(gain_test_tmp, b_i);
          for (loop_ub = 0; loop_ub < flag_array_zero; loop_ub++) {
            gain_test_tmp->data[loop_ub] = std::abs(ft->data[loop_ub]);
          }

          if (x + 1.0 > b_x + 1.0) {
            b_i = 0;
            i1 = 0;
          } else {
            b_i = static_cast<int>(x + 1.0) - 1;
            i1 = static_cast<int>(b_x + 1.0);
          }

          i2 = b_F_tr->size[0] * b_F_tr->size[1];
          b_F_tr->size[0] = 1;
          loop_ub = i1 - b_i;
          b_F_tr->size[1] = loop_ub;
          emxEnsureCapacity_real_T(b_F_tr, i2);
          for (i1 = 0; i1 < loop_ub; i1++) {
            b_F_tr->data[i1] = F_tr->data[b_i + i1];
          }

          coder::vmovfun(newPhase, newPhase->size[1], r);
          coder::interp1(b_F_tr, r, gain_s);

          // 70000
          std::memcpy(&b_gain_s[0], &gain_s[0], 513U * sizeof(double));
          for (b_i = 0; b_i < 511; b_i++) {
            b_gain_s[b_i + 513] = gain_s[512];
          }

          // 70000
          b_gain_s[0] = b_gain_s[2];
          b_gain_s[1] = b_gain_s[2];
          for (b_i = 0; b_i < 1024; b_i++) {
            tmp_data[b_i] = 136.71875 * (static_cast<double>(b_i) + 1024.0) +
              -140000.0;
          }

          coder::interp1(tmp_data, b_gain_s, (double *)resampSweepAfter23->data,
                         phase_s_data, gain_s_size);
          for (b_i = 0; b_i < 2048; b_i++) {
            b_tmp_data[b_i] = 136.71875 * static_cast<double>(b_i) + -140000.0;
          }

          coder::interp1(b_tmp_data, gain_s_data, gain_test_tmp, r);
          for (b_i = 0; b_i < 2048; b_i++) {
            b_tmp_data[b_i] = 136.71875 * static_cast<double>(b_i) + -140000.0;
          }

          coder::interp1(b_tmp_data, phase_s_data, gain_test_tmp,
                         resampSweepAfter23);
          b_i = TrFunEst->size[0] * TrFunEst->size[1];
          TrFunEst->size[0] = 1;
          TrFunEst->size[1] = resampSweepAfter23->size[1];
          emxEnsureCapacity_creal_T(TrFunEst, b_i);
          loop_ub = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
          for (b_i = 0; b_i < loop_ub; b_i++) {
            TrFunEst->data[b_i].re = resampSweepAfter23->data[b_i] * 0.0;
            TrFunEst->data[b_i].im = resampSweepAfter23->data[b_i];
          }

          coder::b_exp(TrFunEst);
          b_i = TrFunEst->size[0] * TrFunEst->size[1];
          TrFunEst->size[0] = 1;
          TrFunEst->size[1] = r->size[1];
          emxEnsureCapacity_creal_T(TrFunEst, b_i);
          loop_ub = r->size[0] * r->size[1] - 1;
          for (b_i = 0; b_i <= loop_ub; b_i++) {
            TrFunEst->data[b_i].re *= r->data[b_i];
            TrFunEst->data[b_i].im *= r->data[b_i];
          }

          b_i = newPhase->size[0] * newPhase->size[1];
          newPhase->size[0] = 1;
          newPhase->size[1] = resamp_sweep->size[0];
          emxEnsureCapacity_real_T(newPhase, b_i);
          loop_ub = resamp_sweep->size[0];
          for (b_i = 0; b_i < loop_ub; b_i++) {
            newPhase->data[b_i] = resamp_sweep->data[b_i];
          }

          coder::b_fft(newPhase, static_cast<double>(resamp_sweep->size[0]),
                       spec_sweep);
          coder::eml_fftshift(spec_sweep, 1);
          coder::eml_fftshift(spec_sweep, 2);
          coder::b_sign(ft);
          b_i = spec_sweep->size[0] * spec_sweep->size[1];
          i1 = spec_sweep->size[0] * spec_sweep->size[1];
          spec_sweep->size[0] = 1;
          emxEnsureCapacity_creal_T(spec_sweep, i1);
          loop_ub = b_i - 1;
          for (b_i = 0; b_i <= loop_ub; b_i++) {
            ar = spec_sweep->data[b_i].re;
            ai = spec_sweep->data[b_i].im;
            br = TrFunEst->data[b_i].re + TrFunEst->data[b_i].im * 0.0 *
              ft->data[b_i];
            bi = TrFunEst->data[b_i].im * ft->data[b_i];
            if (bi == 0.0) {
              if (ai == 0.0) {
                re = ar / br;
                pi_jump_count = 0.0;
              } else if (ar == 0.0) {
                re = 0.0;
                pi_jump_count = ai / br;
              } else {
                re = ar / br;
                pi_jump_count = ai / br;
              }
            } else if (br == 0.0) {
              if (ar == 0.0) {
                re = ai / bi;
                pi_jump_count = 0.0;
              } else if (ai == 0.0) {
                re = 0.0;
                pi_jump_count = -(ar / bi);
              } else {
                re = ai / bi;
                pi_jump_count = -(ar / bi);
              }
            } else {
              brm = std::abs(br);
              pi_jump_count = std::abs(bi);
              if (brm > pi_jump_count) {
                pi_count = bi / br;
                pi_jump_count = br + pi_count * bi;
                re = (ar + pi_count * ai) / pi_jump_count;
                pi_jump_count = (ai - pi_count * ar) / pi_jump_count;
              } else if (pi_jump_count == brm) {
                if (br > 0.0) {
                  pi_count = 0.5;
                } else {
                  pi_count = -0.5;
                }

                if (bi > 0.0) {
                  pi_jump_count = 0.5;
                } else {
                  pi_jump_count = -0.5;
                }

                re = (ar * pi_count + ai * pi_jump_count) / brm;
                pi_jump_count = (ai * pi_count - ar * pi_jump_count) / brm;
              } else {
                pi_count = br / bi;
                pi_jump_count = bi + pi_count * br;
                re = (pi_count * ar + ai) / pi_jump_count;
                pi_jump_count = (pi_count * ai - ar) / pi_jump_count;
              }
            }

            spec_sweep->data[b_i].re = re;
            spec_sweep->data[b_i].im = pi_jump_count;
          }

          coder::ifftshift(spec_sweep);
          coder::b_ifft(spec_sweep, TrFunEst);
          b_i = ft->size[0] * ft->size[1];
          ft->size[0] = 1;
          ft->size[1] = TrFunEst->size[1];
          emxEnsureCapacity_real_T(ft, b_i);
          loop_ub = TrFunEst->size[0] * TrFunEst->size[1];
          for (b_i = 0; b_i < loop_ub; b_i++) {
            ft->data[b_i] = TrFunEst->data[b_i].re;
          }

          if (1 > resamp_sweep->size[0]) {
            b_i = 0;
          } else {
            b_i = resamp_sweep->size[0];
          }

          i1 = ft->size[0] * ft->size[1];
          ft->size[1] = b_i;
          emxEnsureCapacity_real_T(ft, i1);
          if (1820 > sweep_math->size[1]) {
            i1 = 0;
            i2 = 0;
            i3 = 0;
            i4 = 0;
          } else {
            i1 = 1819;
            i2 = sweep_math->size[1];
            i3 = 1819;
            i4 = sweep_math->size[1];
          }

          x = rt_roundd_snf(142000.0 / (280000.0 / static_cast<double>(i2 - i1)));
          b_x = rt_roundd_snf(209900.0 / (280000.0 / static_cast<double>(i4 - i3)));
          pi_count = coder::internal::maximum(ft);
          i1 = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
          resampSweepAfter23->size[0] = 1;
          resampSweepAfter23->size[1] = b_i;
          emxEnsureCapacity_real_T(resampSweepAfter23, i1);
          loop_ub = ft->size[0] * b_i;
          for (i1 = 0; i1 < loop_ub; i1++) {
            resampSweepAfter23->data[i1] = ft->data[i1] / pi_count;
          }

          flag_array_zero = resampSweepAfter23->size[1];
          b_resampSweepAfter23 = *resampSweepAfter23;
          c_resampSweepAfter23[0] = flag_array_zero;
          b_resampSweepAfter23.size = &c_resampSweepAfter23[0];
          b_resampSweepAfter23.numDimensions = 1;
          coder::xcorr(&b_resampSweepAfter23, sweepPreamble, resamp_sweep,
                       newPhase);
          br = static_cast<double>(b_i) - PreSPP * 2.0;
          if (br > resamp_sweep->size[0]) {
            i1 = 0;
            i2 = 0;
          } else {
            i1 = static_cast<int>(br) - 1;
            i2 = resamp_sweep->size[0];
          }

          i3 = newPhase->size[0] * newPhase->size[1];
          newPhase->size[0] = 1;
          loop_ub = i2 - i1;
          newPhase->size[1] = loop_ub;
          emxEnsureCapacity_real_T(newPhase, i3);
          for (i2 = 0; i2 < loop_ub; i2++) {
            newPhase->data[i2] = resamp_sweep->data[i1 + i2];
          }

          coder::internal::b_maximum(newPhase, &pi_count, &iindx);

          //  [~,Im] = max(c(find(lags==-PreSPP*2):end));
          corr_var = 2.0;
          i = 0;
          exitg1 = false;
          while ((!exitg1) && (i < 10)) {
            br = PreSPP * 2.0 - (static_cast<double>(iindx) - corr_var);
            pi_jump_count = static_cast<double>(b_i) - br;
            if (1.0 > pi_jump_count) {
              loop_ub = 0;
            } else {
              loop_ub = static_cast<int>(pi_jump_count);
            }

            flag_array_zero = static_cast<int>(br);
            i1 = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
            resampSweepAfter23->size[0] = 1;
            resampSweepAfter23->size[1] = static_cast<int>(br) + loop_ub;
            emxEnsureCapacity_real_T(resampSweepAfter23, i1);
            for (i1 = 0; i1 < flag_array_zero; i1++) {
              resampSweepAfter23->data[i1] = 0.0;
            }

            for (i1 = 0; i1 < loop_ub; i1++) {
              resampSweepAfter23->data[i1 + static_cast<int>(br)] = ft->data[i1];
            }

            if (1820 > resampSweepAfter23->size[1]) {
              i1 = 0;
              i2 = 0;
            } else {
              i1 = 1819;
              i2 = resampSweepAfter23->size[1];
            }

            if (1820 > sweep_math->size[1]) {
              i3 = 0;
              i4 = 0;
            } else {
              i3 = 1819;
              i4 = sweep_math->size[1];
            }

            i5 = newPhase->size[0] * newPhase->size[1];
            newPhase->size[0] = 1;
            loop_ub = i2 - i1;
            newPhase->size[1] = loop_ub;
            emxEnsureCapacity_real_T(newPhase, i5);
            for (i2 = 0; i2 < loop_ub; i2++) {
              newPhase->data[i2] = resampSweepAfter23->data[i1 + i2];
            }

            coder::fft(newPhase, TrFunEst);
            i1 = newPhase->size[0] * newPhase->size[1];
            newPhase->size[0] = 1;
            loop_ub = i4 - i3;
            newPhase->size[1] = loop_ub;
            emxEnsureCapacity_real_T(newPhase, i1);
            for (i1 = 0; i1 < loop_ub; i1++) {
              newPhase->data[i1] = sweep_math->data[i3 + i1];
            }

            coder::fft(newPhase, spec_sweep);
            i1 = TrFunEst->size[0] * TrFunEst->size[1];
            i2 = TrFunEst->size[0] * TrFunEst->size[1];
            TrFunEst->size[0] = 1;
            emxEnsureCapacity_creal_T(TrFunEst, i2);
            loop_ub = i1 - 1;
            for (i1 = 0; i1 <= loop_ub; i1++) {
              ar = TrFunEst->data[i1].re;
              ai = TrFunEst->data[i1].im;
              br = spec_sweep->data[i1].re;
              bi = spec_sweep->data[i1].im;
              if (bi == 0.0) {
                if (ai == 0.0) {
                  re = ar / br;
                  pi_jump_count = 0.0;
                } else if (ar == 0.0) {
                  re = 0.0;
                  pi_jump_count = ai / br;
                } else {
                  re = ar / br;
                  pi_jump_count = ai / br;
                }
              } else if (br == 0.0) {
                if (ar == 0.0) {
                  re = ai / bi;
                  pi_jump_count = 0.0;
                } else if (ai == 0.0) {
                  re = 0.0;
                  pi_jump_count = -(ar / bi);
                } else {
                  re = ai / bi;
                  pi_jump_count = -(ar / bi);
                }
              } else {
                brm = std::abs(br);
                pi_jump_count = std::abs(bi);
                if (brm > pi_jump_count) {
                  pi_count = bi / br;
                  pi_jump_count = br + pi_count * bi;
                  re = (ar + pi_count * ai) / pi_jump_count;
                  pi_jump_count = (ai - pi_count * ar) / pi_jump_count;
                } else if (pi_jump_count == brm) {
                  if (br > 0.0) {
                    pi_count = 0.5;
                  } else {
                    pi_count = -0.5;
                  }

                  if (bi > 0.0) {
                    pi_jump_count = 0.5;
                  } else {
                    pi_jump_count = -0.5;
                  }

                  re = (ar * pi_count + ai * pi_jump_count) / brm;
                  pi_jump_count = (ai * pi_count - ar * pi_jump_count) / brm;
                } else {
                  pi_count = br / bi;
                  pi_jump_count = bi + pi_count * br;
                  re = (pi_count * ar + ai) / pi_jump_count;
                  pi_jump_count = (pi_count * ai - ar) / pi_jump_count;
                }
              }

              TrFunEst->data[i1].re = re;
              TrFunEst->data[i1].im = pi_jump_count;
            }

            coder::eml_fftshift(TrFunEst, 1);
            coder::eml_fftshift(TrFunEst, 2);
            if (x + 1.0 > b_x + 1.0) {
              i1 = 0;
              i2 = 0;
            } else {
              i1 = static_cast<int>(x + 1.0) - 1;
              i2 = static_cast<int>(b_x + 1.0);
            }

            nx_tmp = i2 - i1;
            i2 = resampSweepAfter23->size[0] * resampSweepAfter23->size[1];
            resampSweepAfter23->size[0] = 1;
            resampSweepAfter23->size[1] = nx_tmp;
            emxEnsureCapacity_real_T(resampSweepAfter23, i2);
            for (loop_ub = 0; loop_ub < nx_tmp; loop_ub++) {
              flag_array_zero = i1 + loop_ub;
              resampSweepAfter23->data[loop_ub] = rt_atan2d_snf(TrFunEst->
                data[flag_array_zero].im, TrFunEst->data[flag_array_zero].re);
            }

            pi_count = coder::mean(resampSweepAfter23);

            // *ones(1,length(TrFunEst(firstFreq:lastFreq)));
            if (pi_count >= 0.08) {
              corr_var++;
              i++;
            } else if (pi_count <= -0.08) {
              corr_var--;
              i++;
            } else {
              exitg1 = true;
            }
          }

          phase_size[0] = 1;
          phase_size[1] = 2048;
          gain_size[0] = 1;
          gain_size[1] = 2048;
          std::memcpy(&phase_data[0], &phase_s_data[0], 2048U * sizeof(double));
          std::memcpy(&gain_data[0], &gain_s_data[0], 2048U * sizeof(double));
          *shift_for_qam_data = rt_roundd_snf(phase_s_data[1280] /
            6.2831853071795862 * 8.0);
        }
      }
    }
  }

  emxFree_real_T(&r);
  emxFree_real_T(&b_F_tr);
  emxFree_real_T(&b_resamp_sweep);
  emxFree_real_T(&gain_test_tmp);
  emxFree_real_T(&c);
  emxFree_creal_T(&spec_sweep);
  emxFree_real_T(&ft);
  emxFree_real_T(&newPhase);
  emxFree_creal_T(&TrFunEst);
  emxFree_real_T(&resampSweepAfter23);
  emxFree_real_T(&F_tr);
  emxFree_real_T(&resamp_sweep);
}

//
// File trailer for HS_EWL_TR_FUN_EST.cpp
//
// [EOF]
//
