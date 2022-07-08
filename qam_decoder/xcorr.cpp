//
// File: xcorr.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//

// Include Files
#include "xcorr.h"
#include "FFTImplementationCallback.h"
#include "ifft.h"
#include "rt_nonfinite.h"
#include <cstring>

// Function Definitions
//
// Arguments    : const double x[57820]
//                const double varargin_1[1820]
//                double c[115639]
//                double lags[115639]
// Return Type  : void
//
namespace coder
{
  void xcorr(const double x[57820], const double varargin_1[1820], double c
             [115639], double lags[115639])
  {
    static creal_T dcv[131072];
    static creal_T dcv1[131072];
    static double c1[131072];
    static double b_costab[65537];
    static double b_sintab[65537];
    static double costab[65537];
    static double sintab[65537];
    int i;
    internal::FFTImplementationCallback::generate_twiddle_tables((costab),
      (sintab));
    internal::FFTImplementationCallback::generate_twiddle_tables((b_costab),
      (b_sintab));
    std::memset(&dcv[0], 0, 131072U * sizeof(creal_T));
    internal::FFTImplementationCallback::doHalfLengthRadix2((x), (0), (dcv),
      (costab), (sintab));
    std::memset(&dcv1[0], 0, 131072U * sizeof(creal_T));
    internal::FFTImplementationCallback::b_doHalfLengthRadix2((varargin_1), (0),
      (dcv1), (b_costab), (b_sintab));
    for (i = 0; i < 131072; i++) {
      double d;
      double d1;
      double im;
      double re;
      re = dcv1[i].re;
      im = -dcv1[i].im;
      d = dcv[i].re;
      d1 = dcv[i].im;
      dcv1[i].re = d * re - d1 * im;
      dcv1[i].im = d * im + d1 * re;
    }

    ifft(dcv1, dcv);
    for (i = 0; i < 131072; i++) {
      c1[i] = dcv[i].re;
    }

    std::memcpy(&c[0], &c1[73253], 57819U * sizeof(double));
    std::memcpy(&c[57819], &c1[0], 57820U * sizeof(double));
    for (i = 0; i < 115639; i++) {
      lags[i] = static_cast<double>(i) + -57819.0;
    }
  }
}

//
// File trailer for xcorr.cpp
//
// [EOF]
//
