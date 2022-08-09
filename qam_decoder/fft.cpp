//
// File: fft.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "fft.h"
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : const double x[57820]
//                creal_T y[57820]
// Return Type  : void
//
namespace coder
{
  void b_fft(const double x[57820], creal_T y[57820])
  {
    static creal_T wwc[57819];
    static double costab[65537];
    static double sintab[65537];
    static double sintabinv[65537];
    int idx;
    int k;
    int rt;
    internal::FFTImplementationCallback::generate_twiddle_tables((costab),
      (sintab), (sintabinv));
    idx = 28908;
    rt = 0;
    wwc[28909].re = 1.0;
    wwc[28909].im = 0.0;
    for (k = 0; k < 28909; k++) {
      double nt_im;
      double nt_re;
      int b_y;
      b_y = ((k + 1) << 1) - 1;
      if (57820 - rt <= b_y) {
        rt = (b_y + rt) - 57820;
      } else {
        rt += b_y;
      }

      nt_im = -3.1415926535897931 * static_cast<double>(rt) / 28910.0;
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
    for (k = 28908; k >= 0; k--) {
      wwc[k + 28910] = wwc[idx];
      idx++;
    }

    internal::FFTImplementationCallback::doHalfLengthBluestein((x), (0), (y),
      (wwc), (costab), (sintab), (costab), (sintabinv));
  }

  //
  // Arguments    : const double x[56001]
  //                creal_T y[56001]
  // Return Type  : void
  //
  void fft(const double x[56001], creal_T y[56001])
  {
    static creal_T b_fv[131072];
    static creal_T fv[131072];
    static creal_T wwc[112001];
    static double costab[65537];
    static double sintab[65537];
    static double sintabinv[65537];
    double d;
    double d1;
    double d2;
    double nt_im;
    double nt_re;
    int idx;
    int k;
    int rt;
    internal::FFTImplementationCallback::generate_twiddle_tables((costab),
      (sintab), (sintabinv));
    idx = 55999;
    rt = 0;
    wwc[56000].re = 1.0;
    wwc[56000].im = 0.0;
    for (k = 0; k < 56000; k++) {
      int b_y;
      b_y = ((k + 1) << 1) - 1;
      if (112002 - rt <= b_y) {
        rt = (b_y + rt) - 112002;
      } else {
        rt += b_y;
      }

      nt_im = -3.1415926535897931 * static_cast<double>(rt) / 56001.0;
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
    for (k = 55999; k >= 0; k--) {
      wwc[k + 56001] = wwc[idx];
      idx++;
    }

    idx = 0;
    for (k = 0; k < 56001; k++) {
      y[k].re = wwc[k + 56000].re * x[idx];
      y[k].im = wwc[k + 56000].im * -x[idx];
      idx++;
    }

    internal::FFTImplementationCallback::r2br_r2dit_trig_impl((y), (costab),
      (sintab), (fv));
    internal::FFTImplementationCallback::b_r2br_r2dit_trig((wwc), (costab),
      (sintab), (b_fv));
    for (idx = 0; idx < 131072; idx++) {
      nt_im = fv[idx].re;
      d = fv[idx].im;
      d1 = b_fv[idx].im;
      d2 = b_fv[idx].re;
      nt_re = nt_im * d1 + d * d2;
      d2 = nt_im * d2 - d * d1;
      b_fv[idx].re = d2;
      b_fv[idx].im = nt_re;
    }

    internal::FFTImplementationCallback::r2br_r2dit_trig((b_fv), (costab),
      (sintabinv), (fv));
    idx = 0;
    for (k = 0; k < 56001; k++) {
      nt_im = wwc[k + 56000].re;
      d = wwc[k + 56000].im;
      d1 = fv[k + 56000].re;
      d2 = fv[k + 56000].im;
      y[idx].re = nt_im * d1 + d * d2;
      y[idx].im = nt_im * d2 - d * d1;
      idx++;
    }
  }
}

//
// File trailer for fft.cpp
//
// [EOF]
//
