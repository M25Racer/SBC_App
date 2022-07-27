//
// File: HS_EWL_DEMOD_QAM.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "HS_EWL_DEMOD_QAM.h"
#include "FIRDecimator.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_DEMOD_QAM_rtwutil.h"
#include "RaisedCosineReceiveFilter.h"
#include "qammod.h"
#include "rat.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Variable Definitions
static coder::comm::RaisedCosineReceiveFilter rxFilter1;
static boolean_T rxFilter1_not_empty;

// Function Definitions
//
// Arguments    : const double data[14040]
//                double len_data
//                double f_est
//                double Fs
//                double qam_symbols_real[275]
//                double qam_symbols_imag[275]
//                double byte_data[212]
//                double *start_inf_data
// Return Type  : void
//
int HS_EWL_DEMOD_QAM(const double data[14040], double len_data, double f_est,
                      double Fs, double qam_symbols_real[275], double
                      qam_symbols_imag[275], double byte_data[212], double
                      *start_inf_data)
{

    //return 1 input data LEN <= 0
    //return 2 incorrect input sample freq for lagrange_resamp func
    //return 0 OK

  static creal_T b_y1[14040];
  static const double y[105] = { 1.0, 1.1132456, 1.2102575, 1.2896211, 1.3501792,
    1.3910486, 1.4116334, 1.4116334, 1.3910486, 1.3501792, 1.2896211, 1.2102575,
    1.1132456, 1.0, 0.87217220000000006, 0.7316261, 0.5804113, 0.4207328,
    0.2549192, 0.085388000000000019, -0.085388000000000019, -0.2549192,
    -0.4207328, -0.5804113, -0.7316261, -0.87217220000000006,
    -0.99999999999999989, -1.1132456, -1.2102575, -1.2896211, -1.3501792,
    -1.3910486, -1.4116334, -1.4116334, -1.3910486, -1.3501792, -1.2896211,
    -1.2102575, -1.1132456, -1.0000000000000002, -0.87217220000000006,
    -0.7316261, -0.5804113, -0.4207328, -0.2549192, -0.085388000000000019,
    0.085388000000000019, 0.2549192, 0.4207328, 0.5804113, 0.7316261,
    0.87217220000000006, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
    0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

  static double s[14044];
  creal_T z[270];
  double resamp_signal[14040];
  double intX[265];
  double c1[209];
  double Q;
  double del_re;
  double x;
  int b_i;
  unsigned char mapping[256];
  signed char symbolI[256];
  signed char symbolQ[256];
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  std::memset(&qam_symbols_real[0], 0, 275U * sizeof(double));
  std::memset(&qam_symbols_imag[0], 0, 275U * sizeof(double));
  std::memset(&byte_data[0], 0, 212U * sizeof(double));
  *start_inf_data = 1.0;
  if (!(len_data <= 0.0)) {
    if (!rxFilter1_not_empty) {
      rxFilter1.init();
      rxFilter1_not_empty = true;
    }

    coder::rat(f_est * 52.0 / Fs, &del_re, &Q);
    if ((!(del_re <= 0.0)) && (!(Q <= 0.0))) {
      creal_T dc;
      double a3;
      double a3_tmp;
      double b_a3_tmp;
      double d;
      double resamp_len;
      int i;
      int idx;
      int ihi;
      int k;

      //  Input parameters
      //   s   - input signal vector [N x 1];
      //   p   - p paramter of samplarate conversion
      //   q   - q paramter of samplarate conversion
      //   x0  - fractional delay
      //  Ouptut parameters
      //   y   - Resampled signal
      std::memset(&resamp_signal[0], 0, 14040U * sizeof(double));
      if (del_re > 1.0) {
        if (Q == 1.0) {
          resamp_len = std::floor(len_data * del_re) + 1.0;

          // resamp_data = zeros(floor(length(s)*p/q)+1,1);
        } else {
          resamp_len = std::floor(len_data * del_re / Q);

          // resamp_data = zeros(floor(length(s)*p/q),1);
        }
      } else {
        resamp_len = std::floor(len_data * del_re / Q);

        // resamp_data = zeros(floor(length(s)*p/q),1);
      }

      s[0] = 0.0;
      s[1] = 0.0;
      std::memcpy(&s[2], &data[0], 14040U * sizeof(double));
      s[14042] = 0.0;
      s[14043] = 0.0;
      double sampl_freq_coef = Q / del_re;
      i = static_cast<int>(resamp_len);
      for (k = 0; k < i; k++) {
        x = (static_cast<double>(k) + 1.0) * sampl_freq_coef;
        //a3 = std::floor(x);
        a3 = (int)(x + 32768.) - 32768;
        d = (a3 + 1.0) - x;
        a3_tmp = s[static_cast<int>(a3 + 4.0) - 1];
        x = s[static_cast<int>((a3 + 4.0) - 2.0) - 1];
        b_a3_tmp = s[static_cast<int>((a3 + 4.0) - 1.0) - 1];
        a3 = 0.16666666666666666 * (a3_tmp - s[static_cast<int>((a3 + 4.0) - 3.0)
          - 1]) + 0.5 * (x - b_a3_tmp);
        x = 0.5 * (a3_tmp - x) - a3;
        resamp_signal[k] = ((b_a3_tmp - x * d) + (((a3_tmp - b_a3_tmp) - a3) - x)
                            * d * d) - a3 * d * d * d;
      }

      // cos(2*pi*f_est(1)*time(1:samples_per_period))+sin(2*pi*f_est(1)*time(1:samples_per_period)); 
      std::memset(&c1[0], 0, 209U * sizeof(double));
      for (k = 0; k < 105; k++) {
        ihi = 103 - k;
        x = 0.0;
        for (b_i = 0; b_i <= ihi; b_i++) {
          x += y[b_i] * resamp_signal[k + b_i];
        }

        c1[k + 104] = x;
      }

      for (k = 0; k < 104; k++) {
        ihi = 103 - k;
        x = 0.0;
        for (b_i = 0; b_i <= ihi; b_i++) {
          x += y[(k + b_i) + 1] * resamp_signal[b_i];
        }

        c1[103 - k] = x;
      }

      if (!rtIsNaN(c1[104])) {
        idx = 1;
      } else {
        boolean_T exitg1;
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k < 106)) {
          if (!rtIsNaN(c1[k + 103])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        idx = 1;
      } else {
        x = c1[idx + 103];
        i = idx + 1;
        for (k = i; k < 106; k++) {
          b_a3_tmp = c1[k + 103];
          if (x < b_a3_tmp) {
            x = b_a3_tmp;
            idx = k;
          }
        }
      }

      // coder.varsize('y1', [255*52, 1]);
      std::memset(&b_y1[0], 0, 14040U * sizeof(creal_T));
      ihi = 0;
      i = static_cast<int>((((std::floor(resamp_len / 52.0) - 1.0) * 52.0 +
        static_cast<double>(idx)) - 1.0) + (1.0 - static_cast<double>(idx)));
      for (b_i = 0; b_i < i; b_i++) {
        unsigned int c_i;
        c_i = static_cast<unsigned int>(idx) + b_i;

        // y1(i-Im+1) = resamp_signal(i)*exp(1j*2*pi*f_est*time(i-Im+1))*2;
        x = resamp_signal[static_cast<int>(c_i) - 1];
        k = static_cast<int>(c_i) - idx;
        b_y1[k].re = x * dv[ihi] * 2.0;
        b_y1[k].im = dv1[ihi] * x * 2.0;
        ihi++;
        if (ihi + 1 >= 53) {
          ihi = 0;
        }
      }

      rxFilter1.step(b_y1, z);
      dc = coder::qammod();
      if (z[9].im == 0.0) {
        if (dc.im == 0.0) {
          del_re = dc.re / z[9].re;
          a3 = 0.0;
        } else if (dc.re == 0.0) {
          del_re = 0.0;
          a3 = dc.im / z[9].re;
        } else {
          del_re = dc.re / z[9].re;
          a3 = dc.im / z[9].re;
        }
      } else if (z[9].re == 0.0) {
        if (dc.re == 0.0) {
          del_re = dc.im / z[9].im;
          a3 = 0.0;
        } else if (dc.im == 0.0) {
          del_re = 0.0;
          a3 = -(dc.re / z[9].im);
        } else {
          del_re = dc.im / z[9].im;
          a3 = -(dc.re / z[9].im);
        }
      } else {
        a3_tmp = std::abs(z[9].re);
        x = std::abs(z[9].im);
        if (a3_tmp > x) {
          x = z[9].im / z[9].re;
          d = z[9].re + x * z[9].im;
          del_re = (dc.re + x * dc.im) / d;
          a3 = (dc.im - x * dc.re) / d;
        } else if (x == a3_tmp) {
          if (z[9].re > 0.0) {
            x = 0.5;
          } else {
            x = -0.5;
          }

          if (z[9].im > 0.0) {
            a3 = 0.5;
          } else {
            a3 = -0.5;
          }

          del_re = (dc.re * x + dc.im * a3) / a3_tmp;
          a3 = (dc.im * x - dc.re * a3) / a3_tmp;
        } else {
          x = z[9].re / z[9].im;
          d = z[9].im + x * z[9].re;
          del_re = (x * dc.re + dc.im) / d;
          a3 = (x * dc.im - dc.re) / d;
        }
      }

      for (k = 0; k < 265; k++) {
        b_a3_tmp = z[k + 5].re;
        x = z[k + 5].im;
        a3_tmp = rt_roundd_snf(((b_a3_tmp * del_re - x * a3) + 15.0) / 2.0);
        if (a3_tmp < 0.0) {
          a3_tmp = 0.0;
        } else {
          if (a3_tmp > 15.0) {
            a3_tmp = 15.0;
          }
        }

        b_a3_tmp = rt_roundd_snf(((b_a3_tmp * a3 + x * del_re) + 15.0) / 2.0);
        if (b_a3_tmp < 0.0) {
          b_a3_tmp = 0.0;
        } else {
          if (b_a3_tmp > 15.0) {
            b_a3_tmp = 15.0;
          }
        }

        intX[k] = ((16.0 - b_a3_tmp) - 1.0) + 16.0 * a3_tmp;
      }

      for (b_i = 0; b_i < 256; b_i++) {
        mapping[b_i] = 0U;
        symbolI[b_i] = static_cast<signed char>(b_i >> 4);
        symbolQ[b_i] = static_cast<signed char>(b_i & 15);
      }

      for (b_i = 1; b_i < 4; b_i += b_i) {
        for (ihi = 0; ihi < 256; ihi++) {
          k = symbolI[ihi];
          symbolI[ihi] = static_cast<signed char>(k ^ k >> b_i);
          k = symbolQ[ihi];
          symbolQ[ihi] = static_cast<signed char>(k ^ k >> b_i);
        }
      }

      for (b_i = 0; b_i < 256; b_i++) {
        mapping[(symbolI[b_i] << 4) + symbolQ[b_i]] = static_cast<unsigned char>
          (b_i);
      }

      //      qam_symbols = zeros(273,1,'like',0.0000 + 0.0000i);
      for (b_i = 0; b_i < 265; b_i++) {
        b_a3_tmp = z[b_i + 5].re;
        x = z[b_i + 5].im;
        qam_symbols_real[b_i] = b_a3_tmp * del_re - x * a3;
        qam_symbols_imag[b_i] = b_a3_tmp * a3 + x * del_re;
      }

      for (b_i = 0; b_i < 50; b_i++) {
        if ((mapping[static_cast<int>(intX[b_i] + 1.0) - 1] == 128) && (mapping[
             static_cast<int>(intX[b_i + 1] + 1.0) - 1] == 128) && (mapping[
             static_cast<int>(intX[b_i + 2] + 1.0) - 1] == 128) && (mapping[
             static_cast<int>(intX[b_i + 3] + 1.0) - 1] == 255)) {
          *start_inf_data = (static_cast<double>(b_i) + 1.0) + 3.0;
        }
      }

      for (b_i = 0; b_i < 212; b_i++) {
        byte_data[b_i] = mapping[static_cast<int>(intX[static_cast<int>
          (*start_inf_data) + b_i] + 1.0) - 1];
      }

      // byte_data = trData(start_inf_data+(1:255-43));
      return 0;
    }
    else {
    return 2;
    }
  } else {
    // error = 1;
    return 1;
  }
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
  rxFilter1_not_empty = false;
  rxFilter1._pobj0.matlabCodegenIsDeleted = true;
  rxFilter1.matlabCodegenIsDeleted = true;
}

//
// File trailer for HS_EWL_DEMOD_QAM.cpp
//
// [EOF]
//
