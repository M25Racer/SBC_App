//
// File: HS_EWL_FREQ_EST_FOR_SWEEP.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "HS_EWL_FREQ_EST_FOR_SWEEP.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Declarations
static double freq_adjust(const double t[40000], const double y[40000], double
  Fc, double ph_opt, int len);
static double phase_adjust(const double t[40000], const double y[40000], double
  Fc, double Ph, int len);
static double preambule_from(const double s[33000], double len);

// Function Definitions
//
// Arguments    : const double t[40000]
//                const double y[40000]
//                double Fc
//                double ph_opt
// Return Type  : double
//
static double freq_adjust(const double t[40000], const double y[40000], double
  Fc, double ph_opt, int len)
{
  static double z1[40000];
  double FFnu[50];
  double dnu;
  double nu;
  double nu0;
  int i;
  nu0 = 6.2831853071795862 * (Fc - 50.0);
  dnu = 6.2831853071795862 * ((Fc + 50.0) - (Fc - 50.0)) / 50.0;
  for (i = 0; i < 50; i++) {
    int k;
    nu = nu0 + dnu * ((static_cast<double>(i) + 1.0) - 1.0);
    for (k = 0; k < len; k++) {
      double d;
      d = y[k] - 0.8 * std::sin(nu * t[k] + ph_opt);
      z1[k] = d * d;
    }

    nu = z1[0];
    for (k = 0; k < len-1; k++) {
      nu += z1[k + 1];
    }

    FFnu[i] = nu;

    // FFnu(i) = FFnu(i) + sum((y(bounds(2):end)-y2(bounds(2):end)).^2);
  }

  coder::internal::minimum(FFnu, &nu, &i);
  return (nu0 + dnu * (absolute_min(static_cast<double>(i), FFnu) - 1.0)) / 2.0 /
    3.1415926535897931;
}

//
// Arguments    : const double t[40000]
//                const double y[40000]
//                double Fc
//                double Ph
// Return Type  : double
//
static double phase_adjust(const double t[40000], const double y[40000], double
  Fc, double Ph, int len)
{
  static double z1[40000];
  double FFfi[50];
  double a;
  double dfi;
  double ph;
  int i;
  dfi = ((Ph + 3.1415926535897931) - (Ph - 3.1415926535897931)) / 50.0;
  a = 6.2831853071795862 * Fc;
  for (i = 0; i < 50; i++) {
    int k;
    ph = (Ph - 3.1415926535897931) + dfi * ((static_cast<double>(i) + 1.0) - 1.0);
    for (k = 0; k < len; k++) {
      double d;
      d = y[k] - 0.8 * std::sin(a * t[k] + ph);
      z1[k] = d * d;
    }

    ph = z1[0];
    for (k = 0; k < len-1; k++) {
      ph += z1[k + 1];
    }

    FFfi[i] = ph;

    // FFfi(i) = FFfi(i) + sum((y(bounds(2):end)-y2(bounds(2):end)).^2);
  }

  coder::internal::minimum(FFfi, &ph, &i);
  return (Ph - 3.1415926535897931) + dfi * (absolute_min(static_cast<double>(i),
    FFfi) - 1.0);
}

//
// Arguments    : const double s[33000]
//                double len
// Return Type  : double
//
static double preambule_from(const double s[33000], double len)
{
  double position[4];
  double pre_from;
  double sa;
  int count;
  int flag_amp;
  int i;
  boolean_T exitg1;
  position[0] = 0.0;
  position[1] = 0.0;
  position[2] = 0.0;
  position[3] = 0.0;

  //  win = zeros(1,win_len);
  count = 0;
  flag_amp = 1;

  // win = s(i:i+win_len-1);
  i = 0;
  exitg1 = false;
  while ((!exitg1) && (i <= static_cast<int>(len - 12.0) - 1)) {
    // s(i:i+win_len-1);%(i+(0:win_len-1));
    sa = s[i];
    for (int k = 0; k < 11; k++) {
      sa += s[(k + i) + 1];
    }

    sa /= 12.0;
    if ((sa > 0.2) && (flag_amp == 1)) {
      count++;
      position[count - 1] = static_cast<double>(i) + 1.0;
      flag_amp = 0;
    }

    if (sa < -0.2) {
      flag_amp = 1;
    }

    if (count == 4) {
      exitg1 = true;
    } else {
      i++;
    }
  }

  if (count > 0) {
    pre_from = position[count - 1];
  } else {
    pre_from = -1.0;
  }

  return pre_from;
}

//
// sweep_freq_warning_status = 0;% OK input array
// sweep_freq_warning_status = 1;% all or more than 33% input array equal 0
// sweep_freq_warning_status = 2;% path of input array equal 0, less than 33%
// Arguments    : double s2[33000]
//                double len
//                double Fs
//                double f0
//                double per_numb
//                double sps
//                double *f_opt
//                double *ph_opt
//                double *sweep_freq_warning_status
// Return Type  : void
//
void HS_EWL_FREQ_EST_FOR_SWEEP(double s2[33000], double len, double Fs, double
  f0, double per_numb, double sps, double *f_opt, double *ph_opt, double
  *sweep_freq_warning_status)
{
  static double sine[40000];
  static double t[40000];
  static double x[40000];
  double err2;
  double sine_from;
  double sine_len;
  double sine_length;
  int i;
  int idx;
  int k;
  if (!isInitialized_HS_EWL_DEMOD_QAM) {
    HS_EWL_DEMOD_QAM_initialize();
  }

  // sps = round(sps);
  //  sps = coder.const(sps);
  //  per_numb = coder.const(per_numb);
  //  sps = 92;
  //  per_numb = 500;
  *sweep_freq_warning_status = 0.0;
  std::memset(&sine[0], 0, 40000U * sizeof(double));
  if (!rtIsNaN(s2[0])) {
    idx = 1;
  } else {
    boolean_T exitg1;
    idx = 0;
    k = 2;
    exitg1 = false;
    while ((!exitg1) && (k < 33001)) {
      if (!rtIsNaN(s2[k - 1])) {
        idx = k;
        exitg1 = true;
      } else {
        k++;
      }
    }
  }

  if (idx == 0) {
    sine_from = s2[0];
  } else {
    sine_from = s2[idx - 1];
    i = idx + 1;
    for (k = i; k < 33001; k++) {
      sine_len = s2[k - 1];
      if (sine_from < sine_len) {
        sine_from = sine_len;
      }
    }
  }

  for (i = 0; i < 33000; i++) {
    s2[i] /= sine_from;
  }

  // win_len = zeros(1,1);
  sine_from = preambule_from(s2, len);
  if ((sine_from < 0.0) || ((len - sine_from) / sps < per_numb / 3.0 - 5.0) ||
      (Fs == 0.0)) {
    //  all or more than 33% input array equal 0
    *f_opt = 0.0;
    *ph_opt = 0.0;
  } else {
    double err1;

    //  elseif (len - sine_from)/sps < (per_numb-5)
    //      per_numb    = (len - sine_from)/sps - 30;
    //      sine_to     = sine_from + sps*(per_numb-5);
    //      sweep_freq_warning_status = 2;% path of input array equal 0, less than 33% 
    //  else
    //      sine_to = sine_from + sps*(per_numb-5);
    //      sweep_freq_warning_status = 0;% OK input array
    //  end
    sine_len = sps * (per_numb - 5.0);
    sine_length = sine_len;
    i = static_cast<int>((sine_len + sine_from) + (1.0 - sine_from));
    for (idx = 0; idx < i; idx++) {
      err1 = sine_from + static_cast<double>(idx);
      sine[static_cast<int>((err1 - sine_from) + 1.0) - 1] = s2[static_cast<int>
        (err1) - 1];
    }

    // sine = s2(sine_from+(0:sps*(per_numb-5)));%s2(sine_from:sine_to);%sine_from+(0:sps*(per_numb-5)) 
    std::memset(&t[0], 0, 40000U * sizeof(double));
    i = static_cast<int>(sine_len);
    for (idx = 0; idx < i; idx++) {
      t[idx] = (static_cast<double>(idx) + 1.0) / Fs;
    }

    // t = (1:length(sine))/Fs;
    sine_len = 1.0;
    err2 = 0.0;
    *f_opt = f0;
    sine_from = f0 * t[0];
    if (sine_from < 0.0) {
      err1 = std::ceil(sine_from);
    } else {
      err1 = std::floor(sine_from);
    }

    *ph_opt = 0.2013579207903308 - 6.2831853071795862 * (sine_from - err1);
    if (*ph_opt < 0.0) {
      *ph_opt += 6.2831853071795862;
    }

    while (sine_len > 1.0E-6) {
      *f_opt = freq_adjust(t, sine, *f_opt, *ph_opt, i);
      *ph_opt = phase_adjust(t, sine, *f_opt, *ph_opt, i);
      sine_from = 6.2831853071795862 * *f_opt;
      err1 = err2;

      // idx = idx<=bounds(1) | idx>=bounds(2);
      for (k = 0; k < i; k++) {
        sine_len = 0.8 * std::sin(sine_from * t[k] + *ph_opt) - sine[k];
        x[k] = sine_len * sine_len;
      }

      err2 = x[0];
      for (k = 0; k < i-1; k++) {
        err2 += x[k + 1];
      }

      sine_len = std::abs((err1 - err2) / (err1 + err2));
    }
  }
}

//
// File trailer for HS_EWL_FREQ_EST_FOR_SWEEP.cpp
//
// [EOF]
//
