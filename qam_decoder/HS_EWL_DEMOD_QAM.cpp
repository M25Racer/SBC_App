//
// File: HS_EWL_DEMOD_QAM.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//

// Include Files
#include "HS_EWL_DEMOD_QAM.h"
#include "FIRDecimator.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_DEMOD_QAM_rtwutil.h"
#include "RaisedCosineReceiveFilter.h"
#include "minOrMax.h"
#include "qammod.h"
#include "rat.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include "stdint.h"
#include <cmath>
#include <cstring>
#include <omp.h>

// Variable Definitions
static coder::comm::RaisedCosineReceiveFilter rxFilter1;
static boolean_T rxFilter1_not_empty;
extern double ref_cos[];
extern double ref_sin[];

// Function Definitions
//
// Arguments    : const double data[14000]
//                double len
//                double index_data
//                double len_data
//                double f_est
//                double Fs
//                double sps
//                double pre_qam
//                double M
//                double *qam_symbols_real
//                double *qam_symbols_imag
//                double *byte_data
// Return Type  : void
//
void HS_EWL_DEMOD_QAM(const double data[14000], double len, double index_data,
                      double len_data, double f_est, double Fs, double, double,
                      double, double *qam_symbols_real, double *qam_symbols_imag,
                      double *byte_data)
{
  static creal_T b_y1[14040];
  static double s[14044];
  static double input_data[14040];
  creal_T z[270];
  double resamp_signal[14040];
  double intX[265];
  double c1[209];
  double b_varargin_1[105];
  double a3;
  double p;
  double q;
  int i;
  unsigned char mapping[256];
  signed char symbolI[256];
  signed char symbolQ[256];
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  std::memset(&input_data[0], 0, 14040U * sizeof(double));
  if ((index_data == 0.0) && (len_data == 0.0)) {
    *qam_symbols_real = 0.0;
    *qam_symbols_imag = 0.0;
    *byte_data = 0.0;

    // error = 1;
  } else {
    creal_T dc;
    double Fd;
    double a1;
    double a3_tmp;
    double d;
    double del_re;
    double resamp_len;
    double x;
    int idx;
    int ihi;
    int k;
    int start_inf_data;
    int varargin_1;
    if (index_data + len_data > len) {
      varargin_1 = static_cast<int>((len - index_data) + 1.0);
      for (i = 0; i < varargin_1; i++) {
        input_data[i] = data[static_cast<int>(index_data + static_cast<double>(i))
          - 1];
      }
    } else {
      varargin_1 = static_cast<int>(len_data + 1.0);
      for (i = 0; i < varargin_1; i++) {
        input_data[i] = data[static_cast<int>(index_data + static_cast<double>(i))
          - 1];
      }
    }

    start_inf_data = 0;
    if (!rxFilter1_not_empty) {
      rxFilter1.init();
      rxFilter1_not_empty = true;
    }

    Fd = f_est * 52.0;
    coder::rat(Fd / Fs, &p, &q);

    //      input_data = data(index_data: index_data+len_data);
    a1 = coder::internal::maximum(input_data);

    //  Input parameters
    //   s   - input signal vector [N x 1];
    //   p   - p paramter of samplarate conversion
    //   q   - q paramter of samplarate conversion
    //   x0  - fractional delay
    //  Ouptut parameters
    //   y   - Resampled signal
    for (i = 0; i < 14040; i++) {
      input_data[i] /= a1;
      resamp_signal[i] = 0.0;
    }

    if (p > 1.0) {
      if (q == 1.0) {
        resamp_len = std::floor(len_data * p) + 1.0;

        // resamp_data = zeros(floor(length(s)*p/q)+1,1);
      } else {
        resamp_len = std::floor(len_data * p / q);

        // resamp_data = zeros(floor(length(s)*p/q),1);
      }
    } else {
      resamp_len = std::floor(len_data * p / q);

      // resamp_data = zeros(floor(length(s)*p/q),1);
    }

    s[0] = 0.0;
    s[1] = 0.0;
    std::memcpy(&s[2], &input_data[0], 14040U * sizeof(double));
    s[14042] = 0.0;
    s[14043] = 0.0;
    varargin_1 = static_cast<int>(resamp_len);
    for (k = 0; k < varargin_1; k++) {
      x = (static_cast<double>(k) + 1.0) * q / p;
      a3 = std::floor(x);
      d = (a3 + 1.0) - x;
      x = s[static_cast<int>(a3 + 4.0) - 1];
      a3_tmp = s[static_cast<int>((a3 + 4.0) - 2.0) - 1];
      del_re = s[static_cast<int>((a3 + 4.0) - 1.0) - 1];
      a3 = 0.16666666666666666 * (x - s[static_cast<int>((a3 + 4.0) - 3.0) - 1])
        + 0.5 * (a3_tmp - del_re);
      a1 = 0.5 * (x - a3_tmp) - a3;
      resamp_signal[k] = ((del_re - a1 * d) + (((x - del_re) - a3) - a1) * (d *
          d)) - a3 * d * d * d;//rt_powd_snf(d, 3.0);
    }

    x = resamp_len / 52.0;
    if (x < 0.0) {
      x = std::ceil(x);
    } else {
      x = std::floor(x);
    }

    std::memset(&input_data[0], 0, 14040U * sizeof(double));
    varargin_1 = static_cast<int>(((x - 1.0) * 52.0 - 1.0) + 1.0);
    for (i = 0; i < varargin_1; i++) {
      input_data[i] = static_cast<double>(i) / Fd;
    }

    // time = time';
    a3 = 6.2831853071795862 * f_est;
    for (k = 0; k < 52; k++) {
      a1 = a3 * input_data[k];
      b_varargin_1[k] = std::cos(a1) + std::sin(a1);
    }

    std::memset(&b_varargin_1[52], 0, 53U * sizeof(double));
    std::memset(&c1[0], 0, 209U * sizeof(double));
    for (k = 0; k < 105; k++) {
      ihi = 103 - k;
      a3 = 0.0;
      for (i = 0; i <= ihi; i++) {
        a3 += b_varargin_1[i] * resamp_signal[k + i];
      }

      c1[k + 104] = a3;
    }

    for (k = 0; k < 104; k++) {
      ihi = 103 - k;
      a3 = 0.0;
      for (i = 0; i <= ihi; i++) {
        a3 += b_varargin_1[(k + i) + 1] * resamp_signal[i];
      }

      c1[103 - k] = a3;
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
      a3 = c1[idx + 103];
      varargin_1 = idx + 1;
      for (k = varargin_1; k < 106; k++) {
        a3_tmp = c1[k + 103];
        if (a3 < a3_tmp) {
          a3 = a3_tmp;
          idx = k;
        }
      }
    }

    // coder.varsize('y1', [255*52, 1]);
    std::memset(&b_y1[0], 0, 14040U * sizeof(creal_T));
    varargin_1 = static_cast<int>((((x - 1.0) * 52.0 + static_cast<double>(idx))
      - 1.0) + (1.0 - static_cast<double>(idx)));
    unsigned int b_i;
    uint8_t count_gar = 0;
    for (i = 0; i < varargin_1; i++) {
      b_i = static_cast<unsigned int>(idx) + i;
      //a3 = input_data[static_cast<int>((static_cast<double>(b_i) - static_cast<
      //  double>(idx)) + 1.0) - 1];
      //del_re = a3 * (f_est * 0.0);
      //x = a3 * (f_est * 6.2831853071795862);
      //if (x == 0.0) {
      //  del_re = std::exp(del_re);
      //  x = 0.0;
      //} else {
      //  a3 = std::exp(del_re / 2.0);
      //  del_re = a3 * (a3 * std::cos(x));
      //  x = a3 * (a3 * std::sin(x));
      //}

      a1 = resamp_signal[static_cast<int>(b_i) - 1];
      ihi = static_cast<int>(b_i) - idx;
      b_y1[ihi].re = 2.0 * (a1 * ref_cos[count_gar]);
      b_y1[ihi].im = 2.0 * (a1 * ref_sin[count_gar]);
      count_gar++;
      if (count_gar >= 52)
          count_gar = 0;

    }

    //      y1 = resamp_signal(Im+(0:receive_symbols*samples_per_period-1)).*exp(1j*2*pi*(f_est(1))*time)*2;%(Im:receive_symbols*samples_per_period+(Im-1)) 
    //      y1 = [y1 ;(1:samples_per_period*span/2)'*0];
    //      dataIn = [];
    //      for dat = 1:256
    //          dataIn(dat) = dat-1;
    //      end
    //      dataMod = qammod(dataIn,256,'gray');
    rxFilter1.step(b_y1, z);
    dc = coder::qammod();
    if (z[9].im == 0.0) {
      if (dc.im == 0.0) {
        del_re = dc.re / z[9].re;
        x = 0.0;
      } else if (dc.re == 0.0) {
        del_re = 0.0;
        x = dc.im / z[9].re;
      } else {
        del_re = dc.re / z[9].re;
        x = dc.im / z[9].re;
      }
    } else if (z[9].re == 0.0) {
      if (dc.re == 0.0) {
        del_re = dc.im / z[9].im;
        x = 0.0;
      } else if (dc.im == 0.0) {
        del_re = 0.0;
        x = -(dc.re / z[9].im);
      } else {
        del_re = dc.im / z[9].im;
        x = -(dc.re / z[9].im);
      }
    } else {
      a3_tmp = std::abs(z[9].re);
      a3 = std::abs(z[9].im);
      if (a3_tmp > a3) {
        a3 = z[9].im / z[9].re;
        d = z[9].re + a3 * z[9].im;
        del_re = (dc.re + a3 * dc.im) / d;
        x = (dc.im - a3 * dc.re) / d;
      } else if (a3 == a3_tmp) {
        if (z[9].re > 0.0) {
          a1 = 0.5;
        } else {
          a1 = -0.5;
        }

        if (z[9].im > 0.0) {
          a3 = 0.5;
        } else {
          a3 = -0.5;
        }

        del_re = (dc.re * a1 + dc.im * a3) / a3_tmp;
        x = (dc.im * a1 - dc.re * a3) / a3_tmp;
      } else {
        a3 = z[9].re / z[9].im;
        d = z[9].im + a3 * z[9].re;
        del_re = (a3 * dc.re + dc.im) / d;
        x = (a3 * dc.im - dc.re) / d;
      }
    }

    for (k = 0; k < 265; k++) {
      a3_tmp = z[k + 5].re;
      a1 = z[k + 5].im;
      a3 = rt_roundd_snf(((a3_tmp * del_re - a1 * x) + 15.0) / 2.0);
      if (a3 < 0.0) {
        a3 = 0.0;
      } else {
        if (a3 > 15.0) {
          a3 = 15.0;
        }
      }

      a3_tmp = rt_roundd_snf(((a3_tmp * x + a1 * del_re) + 15.0) / 2.0);
      if (a3_tmp < 0.0) {
        a3_tmp = 0.0;
      } else {
        if (a3_tmp > 15.0) {
          a3_tmp = 15.0;
        }
      }

      intX[k] = ((16.0 - a3_tmp) - 1.0) + 16.0 * a3;
    }

    for (i = 0; i < 256; i++) {
      mapping[i] = 0U;
      symbolI[i] = static_cast<signed char>(i >> 4);
      symbolQ[i] = static_cast<signed char>(i & 15);
    }

    for (i = 1; i < 4; i += i) {
      for (ihi = 0; ihi < 256; ihi++) {
        varargin_1 = symbolI[ihi];
        symbolI[ihi] = static_cast<signed char>(varargin_1 ^ varargin_1 >> i);
        varargin_1 = symbolQ[ihi];
        symbolQ[ihi] = static_cast<signed char>(varargin_1 ^ varargin_1 >> i);
      }
    }

    for (i = 0; i < 256; i++) {
      mapping[(symbolI[i] << 4) + symbolQ[i]] = static_cast<unsigned char>(i);
    }

    //      qam_symbols = zeros(273,1,'like',0.0000 + 0.0000i);
    for (i = 0; i < 265; i++) {
      a3_tmp = z[i + 5].re;
      a1 = z[i + 5].im;
      qam_symbols_real[i] = a3_tmp * del_re - a1 * x;
      qam_symbols_imag[i] = a3_tmp * x + a1 * del_re;
    }

    for (i = 0; i < 50; i++) {
      if ((mapping[static_cast<int>(intX[i] + 1.0) - 1] == 128) && (mapping[
           static_cast<int>(intX[i + 1] + 1.0) - 1] == 128) && (mapping[
           static_cast<int>(intX[i + 2] + 1.0) - 1] == 128) && (mapping[
           static_cast<int>(intX[i + 3] + 1.0) - 1] == 255)) {
        start_inf_data = i + 4;
      }
    }

    for (i = 0; i < 212; i++) {
      byte_data[i] = mapping[static_cast<int>(intX[start_inf_data + i] + 1.0) - 1];
    }

    // byte_data = trData(start_inf_data+(1:255-43));
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
