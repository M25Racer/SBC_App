//
// File: ifft.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "ifft.h"
#include "FFTImplementationCallback.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const creal_T x[57820]
//                creal_T y[57820]
// Return Type  : void
//
namespace coder
{
  void b_ifft(const creal_T x[57820], creal_T y[57820])
  {
    static creal_T b_fv[131072];
    static creal_T fv[131072];
    static creal_T wwc[115639];
    static double costab[65537];
    static double sintab[65537];
    static double sintabinv[65537];
    double d;
    double d1;
    double im;
    double nt_im;
    double nt_re;
    int idx;
    int k;
    int rt;
    internal::FFTImplementationCallback::generate_twiddle_tables((costab),
      (sintab), (sintabinv));
    idx = 57818;
    rt = 0;
    wwc[57819].re = 1.0;
    wwc[57819].im = 0.0;
    for (k = 0; k < 57819; k++) {
      int b_y;
      b_y = ((k + 1) << 1) - 1;
      if (115640 - rt <= b_y) {
        rt = (b_y + rt) - 115640;
      } else {
        rt += b_y;
      }

      nt_im = 3.1415926535897931 * static_cast<double>(rt) / 57820.0;
      if (nt_im == 0.0) {
        nt_re = 1.0;
        nt_im = 0.0;
      } else {
        nt_re = std::cos(nt_im);
        nt_im = std::sin(nt_im);
      }

      wwc[idx].re = nt_re;
      wwc[idx].im = -nt_im;
      idx--;
    }

    idx = 0;
    for (k = 57818; k >= 0; k--) {
      wwc[k + 57820] = wwc[idx];
      idx++;
    }

    idx = 0;
    for (k = 0; k < 57820; k++) {
      nt_re = wwc[k + 57819].re;
      nt_im = wwc[k + 57819].im;
      y[k].re = nt_re * x[idx].re + nt_im * x[idx].im;
      y[k].im = nt_re * x[idx].im - nt_im * x[idx].re;
      idx++;
    }

    internal::FFTImplementationCallback::c_r2br_r2dit_trig_impl((y), (costab),
      (sintab), (fv));
    internal::FFTImplementationCallback::e_r2br_r2dit_trig((wwc), (costab),
      (sintab), (b_fv));
    for (idx = 0; idx < 131072; idx++) {
      nt_re = fv[idx].re;
      nt_im = fv[idx].im;
      d = b_fv[idx].im;
      d1 = b_fv[idx].re;
      im = nt_re * d + nt_im * d1;
      d1 = nt_re * d1 - nt_im * d;
      b_fv[idx].re = d1;
      b_fv[idx].im = im;
    }

    internal::FFTImplementationCallback::r2br_r2dit_trig((b_fv), (costab),
      (sintabinv), (fv));
    idx = 0;
    for (k = 0; k < 57820; k++) {
      nt_re = wwc[k + 57819].re;
      nt_im = wwc[k + 57819].im;
      d = fv[k + 57819].re;
      d1 = fv[k + 57819].im;
      y[idx].re = nt_re * d + nt_im * d1;
      y[idx].im = nt_re * d1 - nt_im * d;
      nt_re = y[idx].re;
      nt_im = y[idx].im;
      if (nt_im == 0.0) {
        nt_re /= 57820.0;
        im = 0.0;
      } else if (nt_re == 0.0) {
        nt_re = 0.0;
        im = nt_im / 57820.0;
      } else {
        nt_re /= 57820.0;
        im = nt_im / 57820.0;
      }

      y[idx].re = nt_re;
      y[idx].im = im;
      idx++;
    }
  }

  //
  // Arguments    : const creal_T x[131072]
  //                creal_T y[131072]
  // Return Type  : void
  //
  void ifft(const creal_T x[131072], creal_T y[131072])
  {
    static double costab[65537];
    static double sintab[65537];
    static double costab1q[32769];
    int k;
    costab1q[0] = 1.0;
    for (k = 0; k < 16384; k++) {
      costab1q[k + 1] = std::cos(4.7936899621426287E-5 * (static_cast<double>(k)
        + 1.0));
    }

    for (k = 0; k < 16383; k++) {
      costab1q[k + 16385] = std::sin(4.7936899621426287E-5 * (32768.0 - (
        static_cast<double>(k) + 16385.0)));
    }

    costab1q[32768] = 0.0;
    costab[0] = 1.0;
    sintab[0] = 0.0;
    for (k = 0; k < 32768; k++) {
      double costab_tmp;
      double sintab_tmp;
      costab_tmp = costab1q[k + 1];
      costab[k + 1] = costab_tmp;
      sintab_tmp = costab1q[32767 - k];
      sintab[k + 1] = sintab_tmp;
      costab[k + 32769] = -sintab_tmp;
      sintab[k + 32769] = costab_tmp;
    }

    internal::FFTImplementationCallback::r2br_r2dit_trig((x), (costab), (sintab),
      (y));
  }
}

//
// File trailer for ifft.cpp
//
// [EOF]
//
