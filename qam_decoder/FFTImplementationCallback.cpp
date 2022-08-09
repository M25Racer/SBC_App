//
// File: FFTImplementationCallback.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "FFTImplementationCallback.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <cstring>

// Function Definitions
//
// Arguments    : double costab[57821]
//                double sintab[57821]
//                double sintabinv[57821]
// Return Type  : void
//
namespace coder
{
  namespace internal
  {
    void FFTImplementationCallback::b_generate_twiddle_tables(double costab
      [57821], double sintab[57821], double sintabinv[57821])
    {
      static double costab1q[28911];
      int k;
      costab1q[0] = 1.0;
      for (k = 0; k < 14455; k++) {
        costab1q[k + 1] = std::cos(5.4334013379276946E-5 * (static_cast<double>
          (k) + 1.0));
      }

      for (k = 0; k < 14454; k++) {
        costab1q[k + 14456] = std::sin(5.4334013379276946E-5 * (28910.0 - (
          static_cast<double>(k) + 14456.0)));
      }

      costab1q[28910] = 0.0;
      costab[0] = 1.0;
      sintab[0] = 0.0;
      for (k = 0; k < 28910; k++) {
        double b_sintabinv_tmp;
        double sintabinv_tmp;
        sintabinv_tmp = costab1q[28909 - k];
        sintabinv[k + 1] = sintabinv_tmp;
        b_sintabinv_tmp = costab1q[k + 1];
        sintabinv[k + 28911] = b_sintabinv_tmp;
        costab[k + 1] = b_sintabinv_tmp;
        sintab[k + 1] = -sintabinv_tmp;
        costab[k + 28911] = -sintabinv_tmp;
        sintab[k + 28911] = -b_sintabinv_tmp;
      }
    }

    //
    // Arguments    : const creal_T x[28910]
    //                const double costab[32768]
    //                const double sintab[32768]
    //                creal_T y[65536]
    // Return Type  : void
    //
    void FFTImplementationCallback::b_r2br_r2dit_trig_impl(const creal_T x[28910],
      const double costab[32768], const double sintab[32768], creal_T y[65536])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      std::memset(&y[0], 0, 65536U * sizeof(creal_T));
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 28909; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 65536;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 65534; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 16384;
      iheight = 65533;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 32768; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }
    }

    //
    // Arguments    : const creal_T x[57819]
    //                const double costab[32768]
    //                const double sintab[32768]
    //                creal_T y[65536]
    // Return Type  : void
    //
    void FFTImplementationCallback::c_r2br_r2dit_trig(const creal_T x[57819],
      const double costab[32768], const double sintab[32768], creal_T y[65536])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      std::memset(&y[0], 0, 65536U * sizeof(creal_T));
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 57818; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 65536;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 65534; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 16384;
      iheight = 65533;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 32768; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }
    }

    //
    // Arguments    : const creal_T x[65536]
    //                const double costab[32768]
    //                const double sintab[32768]
    //                creal_T y[65536]
    // Return Type  : void
    //
    void FFTImplementationCallback::d_r2br_r2dit_trig(const creal_T x[65536],
      const double costab[32768], const double sintab[32768], creal_T y[65536])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 65535; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 65536;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 65534; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 16384;
      iheight = 65533;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 32768; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }

      for (iy = 0; iy < 65536; iy++) {
        y[iy].re *= 1.52587890625E-5;
        y[iy].im *= 1.52587890625E-5;
      }
    }

    //
    // Arguments    : int bitrevIndex[65536]
    // Return Type  : void
    //
    void FFTImplementationCallback::get_bitrevIndex(int bitrevIndex[65536])
    {
      int iy;
      int ju;
      ju = 0;
      iy = 1;
      for (int b_j1 = 0; b_j1 < 65535; b_j1++) {
        boolean_T tst;
        bitrevIndex[b_j1] = iy;
        iy = 65536;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju + 1;
      }

      bitrevIndex[65535] = iy;
    }

    //
    // Arguments    : creal_T y[131072]
    //                int yoff
    //                const creal_T reconVar1[65536]
    //                const creal_T reconVar2[65536]
    //                const int wrapIndex[65536]
    //                int hnRows
    // Return Type  : void
    //
    void FFTImplementationCallback::getback_radix2_fft(creal_T y[131072], int
      yoff, const creal_T reconVar1[65536], const creal_T reconVar2[65536],
      const int wrapIndex[65536], int hnRows)
    {
      double d;
      double d1;
      double temp1_im;
      double temp1_re;
      double temp2_im;
      double temp2_re;
      int i;
      int iterVar;
      int temp1_re_tmp;
      iterVar = hnRows / 2;
      temp2_re = y[yoff].re;
      temp1_im = y[yoff].im;
      temp2_im = y[yoff].re * reconVar1[0].im + y[yoff].im * reconVar1[0].re;
      temp1_re = -y[yoff].im;
      d = temp2_re * reconVar2[0].re;
      y[yoff].re = 0.5 * ((y[yoff].re * reconVar1[0].re - y[yoff].im *
                           reconVar1[0].im) + (d - temp1_re * reconVar2[0].im));
      d1 = temp2_re * reconVar2[0].im;
      y[yoff].im = 0.5 * (temp2_im + (d1 + temp1_re * reconVar2[0].re));
      i = yoff + hnRows;
      y[i].re = 0.5 * ((d - temp1_im * reconVar2[0].im) + (temp2_re * reconVar1
        [0].re - -temp1_im * reconVar1[0].im));
      y[i].im = 0.5 * ((d1 + temp1_im * reconVar2[0].re) + (temp2_re *
        reconVar1[0].im + -temp1_im * reconVar1[0].re));
      for (int b_i = 2; b_i <= iterVar; b_i++) {
        int b_temp2_re_tmp_tmp;
        int temp2_re_tmp_tmp;
        temp1_re_tmp = (yoff + b_i) - 1;
        temp1_re = y[temp1_re_tmp].re;
        temp1_im = y[temp1_re_tmp].im;
        temp2_re_tmp_tmp = wrapIndex[b_i - 1];
        b_temp2_re_tmp_tmp = yoff + temp2_re_tmp_tmp;
        temp2_re = y[b_temp2_re_tmp_tmp - 1].re;
        temp2_im = y[b_temp2_re_tmp_tmp - 1].im;
        y[temp1_re_tmp].re = 0.5 * ((temp1_re * reconVar1[b_i - 1].re - temp1_im
          * reconVar1[b_i - 1].im) + (temp2_re * reconVar2[b_i - 1].re -
          -temp2_im * reconVar2[b_i - 1].im));
        y[temp1_re_tmp].im = 0.5 * ((temp1_re * reconVar1[b_i - 1].im + temp1_im
          * reconVar1[b_i - 1].re) + (temp2_re * reconVar2[b_i - 1].im +
          -temp2_im * reconVar2[b_i - 1].re));
        temp1_re_tmp = (i + b_i) - 1;
        y[temp1_re_tmp].re = 0.5 * ((temp1_re * reconVar2[b_i - 1].re - temp1_im
          * reconVar2[b_i - 1].im) + (temp2_re * reconVar1[b_i - 1].re -
          -temp2_im * reconVar1[b_i - 1].im));
        y[temp1_re_tmp].im = 0.5 * ((temp1_re * reconVar2[b_i - 1].im + temp1_im
          * reconVar2[b_i - 1].re) + (temp2_re * reconVar1[b_i - 1].im +
          -temp2_im * reconVar1[b_i - 1].re));
        y[b_temp2_re_tmp_tmp - 1].re = 0.5 * ((temp2_re *
          reconVar1[temp2_re_tmp_tmp - 1].re - temp2_im *
          reconVar1[temp2_re_tmp_tmp - 1].im) + (temp1_re *
          reconVar2[temp2_re_tmp_tmp - 1].re - -temp1_im *
          reconVar2[temp2_re_tmp_tmp - 1].im));
        y[b_temp2_re_tmp_tmp - 1].im = 0.5 * ((temp2_re *
          reconVar1[temp2_re_tmp_tmp - 1].im + temp2_im *
          reconVar1[temp2_re_tmp_tmp - 1].re) + (temp1_re *
          reconVar2[temp2_re_tmp_tmp - 1].im + -temp1_im *
          reconVar2[temp2_re_tmp_tmp - 1].re));
        temp1_re_tmp = (b_temp2_re_tmp_tmp + hnRows) - 1;
        y[temp1_re_tmp].re = 0.5 * ((temp2_re * reconVar2[temp2_re_tmp_tmp - 1].
          re - temp2_im * reconVar2[temp2_re_tmp_tmp - 1].im) + (temp1_re *
          reconVar1[temp2_re_tmp_tmp - 1].re - -temp1_im *
          reconVar1[temp2_re_tmp_tmp - 1].im));
        y[temp1_re_tmp].im = 0.5 * ((temp2_re * reconVar2[temp2_re_tmp_tmp - 1].
          im + temp2_im * reconVar2[temp2_re_tmp_tmp - 1].re) + (temp1_re *
          reconVar1[temp2_re_tmp_tmp - 1].im + -temp1_im *
          reconVar1[temp2_re_tmp_tmp - 1].re));
      }

      if (iterVar != 0) {
        temp1_re_tmp = yoff + iterVar;
        temp1_re = y[temp1_re_tmp].re;
        temp1_im = y[temp1_re_tmp].im;
        d = temp1_re * reconVar2[iterVar].re;
        d1 = temp1_re * reconVar1[iterVar].re;
        y[temp1_re_tmp].re = 0.5 * ((d1 - temp1_im * reconVar1[iterVar].im) + (d
          - -temp1_im * reconVar2[iterVar].im));
        temp2_im = temp1_re * reconVar2[iterVar].im;
        temp2_re = temp1_re * reconVar1[iterVar].im;
        y[temp1_re_tmp].im = 0.5 * ((temp2_re + temp1_im * reconVar1[iterVar].re)
          + (temp2_im + -temp1_im * reconVar2[iterVar].re));
        i += iterVar;
        y[i].re = 0.5 * ((d - temp1_im * reconVar2[iterVar].im) + (d1 -
          -temp1_im * reconVar1[iterVar].im));
        y[i].im = 0.5 * ((temp2_im + temp1_im * reconVar2[iterVar].re) +
                         (temp2_re + -temp1_im * reconVar1[iterVar].re));
      }
    }

    //
    // Arguments    : const double x[1820]
    //                int xoffInit
    //                creal_T y[131072]
    //                const double costab[65537]
    //                const double sintab[65537]
    // Return Type  : void
    //
    void FFTImplementationCallback::b_doHalfLengthRadix2(const double x[1820],
      int xoffInit, creal_T y[131072], const double costab[65537], const double
      sintab[65537])
    {
      static creal_T reconVar1[65536];
      static creal_T reconVar2[65536];
      static double hcostab[32768];
      static double hsintab[32768];
      static int bitrevIndex[65536];
      static int wrapIndex[65536];
      double temp_im;
      double temp_re;
      int i;
      int iDelta2;
      int iheight;
      int ix;
      int j;
      int k;
      for (i = 0; i < 32768; i++) {
        ix = ((i + 1) << 1) - 2;
        hcostab[i] = costab[ix];
        hsintab[i] = sintab[ix];
      }

      for (i = 0; i < 65536; i++) {
        temp_re = sintab[i];
        reconVar1[i].re = temp_re + 1.0;
        temp_im = costab[i];
        reconVar1[i].im = -temp_im;
        reconVar2[i].re = 1.0 - temp_re;
        reconVar2[i].im = temp_im;
        if (i + 1 != 1) {
          wrapIndex[i] = 65537 - i;
        } else {
          wrapIndex[0] = 1;
        }
      }

      FFTImplementationCallback::get_bitrevIndex((bitrevIndex));
      ix = xoffInit;
      for (i = 0; i < 910; i++) {
        iDelta2 = bitrevIndex[i];
        y[iDelta2 - 1].re = x[ix];
        y[iDelta2 - 1].im = x[ix + 1];
        ix += 2;
      }

      for (i = 0; i <= 65534; i += 2) {
        temp_re = y[i + 1].re;
        temp_im = y[i + 1].im;
        y[i + 1].re = y[i].re - y[i + 1].re;
        y[i + 1].im = y[i].im - y[i + 1].im;
        y[i].re += temp_re;
        y[i].im += temp_im;
      }

      ix = 2;
      iDelta2 = 4;
      k = 16384;
      iheight = 65533;
      while (k > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += iDelta2) {
          temp_re_tmp = i + ix;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = k; j < 32768; j += k) {
          double twid_im;
          double twid_re;
          int ihi;
          twid_re = hcostab[j];
          twid_im = hsintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + ix;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += iDelta2;
          }

          istart++;
        }

        k /= 2;
        ix = iDelta2;
        iDelta2 += iDelta2;
        iheight -= ix;
      }

      FFTImplementationCallback::getback_radix2_fft((y), (0), (reconVar1),
        (reconVar2), (wrapIndex), (65536));
    }

    //
    // Arguments    : const creal_T x[112001]
    //                const double costab[65537]
    //                const double sintab[65537]
    //                creal_T y[131072]
    // Return Type  : void
    //
    void FFTImplementationCallback::b_r2br_r2dit_trig(const creal_T x[112001],
      const double costab[65537], const double sintab[65537], creal_T y[131072])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      std::memset(&y[0], 0, 131072U * sizeof(creal_T));
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 112000; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 131072;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 131070; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 32768;
      iheight = 131069;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 65536; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }
    }

    //
    // Arguments    : const creal_T x[57820]
    //                const double costab[65537]
    //                const double sintab[65537]
    //                creal_T y[131072]
    // Return Type  : void
    //
    void FFTImplementationCallback::c_r2br_r2dit_trig_impl(const creal_T x[57820],
      const double costab[65537], const double sintab[65537], creal_T y[131072])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      std::memset(&y[0], 0, 131072U * sizeof(creal_T));
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 57819; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 131072;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 131070; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 32768;
      iheight = 131069;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 65536; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }
    }

    //
    // Arguments    : const double x[57820]
    //                int xoffInit
    //                creal_T y[57820]
    //                const creal_T wwc[57819]
    //                const double costab[65537]
    //                const double sintab[65537]
    //                const double costabinv[65537]
    //                const double sintabinv[65537]
    // Return Type  : void
    //
    void FFTImplementationCallback::doHalfLengthBluestein(const double x[57820],
      int xoffInit, creal_T y[57820], const creal_T wwc[57819], const double
      costab[65537], const double sintab[65537], const double costabinv[65537],
      const double sintabinv[65537])
    {
      static creal_T b_fv[65536];
      static creal_T fv[65536];
      static creal_T reconVar1[28910];
      static creal_T reconVar2[28910];
      static creal_T ytmp[28910];
      static double costable[57821];
      static double sintable[57821];
      static double unusedU0[57821];
      static double hcostab[32768];
      static double hcostabinv[32768];
      static double hsintab[32768];
      static double hsintabinv[32768];
      double b_im;
      double b_re;
      double d;
      double d1;
      double d2;
      double d3;
      int wrapIndex[28910];
      int i;
      int idx;
      int xidx;
      FFTImplementationCallback::b_generate_twiddle_tables((costable), (sintable),
        (unusedU0));
      for (i = 0; i < 32768; i++) {
        xidx = ((i + 1) << 1) - 2;
        hcostab[i] = costab[xidx];
        hsintab[i] = sintab[xidx];
        hcostabinv[i] = costabinv[xidx];
        hsintabinv[i] = sintabinv[xidx];
      }

      idx = 0;
      xidx = xoffInit;
      for (i = 0; i < 28910; i++) {
        reconVar1[i].re = sintable[idx] + 1.0;
        reconVar1[i].im = -costable[idx];
        reconVar2[i].re = 1.0 - sintable[idx];
        reconVar2[i].im = costable[idx];
        idx += 2;
        if (i + 1 != 1) {
          wrapIndex[i] = 28911 - i;
        } else {
          wrapIndex[0] = 1;
        }

        b_re = x[xidx];
        b_im = x[xidx + 1];
        d = wwc[i + 28909].re;
        d1 = wwc[i + 28909].im;
        ytmp[i].re = d * b_re + d1 * b_im;
        ytmp[i].im = d * b_im - d1 * b_re;
        xidx += 2;
      }

      FFTImplementationCallback::b_r2br_r2dit_trig_impl((ytmp), (hcostab),
        (hsintab), (fv));
      FFTImplementationCallback::c_r2br_r2dit_trig((wwc), (hcostab), (hsintab),
        (b_fv));
      for (xidx = 0; xidx < 65536; xidx++) {
        d = fv[xidx].re;
        d1 = fv[xidx].im;
        d2 = b_fv[xidx].im;
        d3 = b_fv[xidx].re;
        b_re = d * d2 + d1 * d3;
        d3 = d * d3 - d1 * d2;
        b_fv[xidx].re = d3;
        b_fv[xidx].im = b_re;
      }

      FFTImplementationCallback::d_r2br_r2dit_trig((b_fv), (hcostabinv),
        (hsintabinv), (fv));
      idx = 0;
      for (xidx = 0; xidx < 28910; xidx++) {
        d = wwc[xidx + 28909].re;
        d1 = wwc[xidx + 28909].im;
        d2 = fv[xidx + 28909].re;
        d3 = fv[xidx + 28909].im;
        ytmp[idx].re = d * d2 + d1 * d3;
        ytmp[idx].im = d * d3 - d1 * d2;
        idx++;
      }

      for (i = 0; i < 28910; i++) {
        xidx = wrapIndex[i];
        b_re = ytmp[xidx - 1].re;
        b_im = -ytmp[xidx - 1].im;
        d = reconVar1[i].re;
        d1 = reconVar1[i].im;
        d2 = reconVar2[i].re;
        d3 = reconVar2[i].im;
        y[i].re = 0.5 * ((ytmp[i].re * d - ytmp[i].im * d1) + (b_re * d2 - b_im *
          d3));
        y[i].im = 0.5 * ((ytmp[i].re * d1 + ytmp[i].im * d) + (b_re * d3 + b_im *
          d2));
        y[i + 28910].re = 0.5 * ((ytmp[i].re * d2 - ytmp[i].im * d3) + (b_re * d
          - b_im * d1));
        y[i + 28910].im = 0.5 * ((ytmp[i].re * d3 + ytmp[i].im * d2) + (b_re *
          d1 + b_im * d));
      }
    }

    //
    // Arguments    : const double x[57820]
    //                int xoffInit
    //                creal_T y[131072]
    //                const double costab[65537]
    //                const double sintab[65537]
    // Return Type  : void
    //
    void FFTImplementationCallback::doHalfLengthRadix2(const double x[57820],
      int xoffInit, creal_T y[131072], const double costab[65537], const double
      sintab[65537])
    {
      static creal_T reconVar1[65536];
      static creal_T reconVar2[65536];
      static double hcostab[32768];
      static double hsintab[32768];
      static int bitrevIndex[65536];
      static int wrapIndex[65536];
      double temp_im;
      double temp_re;
      int i;
      int iDelta2;
      int iheight;
      int ix;
      int j;
      int k;
      for (i = 0; i < 32768; i++) {
        ix = ((i + 1) << 1) - 2;
        hcostab[i] = costab[ix];
        hsintab[i] = sintab[ix];
      }

      for (i = 0; i < 65536; i++) {
        temp_re = sintab[i];
        reconVar1[i].re = temp_re + 1.0;
        temp_im = costab[i];
        reconVar1[i].im = -temp_im;
        reconVar2[i].re = 1.0 - temp_re;
        reconVar2[i].im = temp_im;
        if (i + 1 != 1) {
          wrapIndex[i] = 65537 - i;
        } else {
          wrapIndex[0] = 1;
        }
      }

      FFTImplementationCallback::get_bitrevIndex((bitrevIndex));
      ix = xoffInit;
      for (i = 0; i < 28910; i++) {
        iDelta2 = bitrevIndex[i];
        y[iDelta2 - 1].re = x[ix];
        y[iDelta2 - 1].im = x[ix + 1];
        ix += 2;
      }

      for (i = 0; i <= 65534; i += 2) {
        temp_re = y[i + 1].re;
        temp_im = y[i + 1].im;
        y[i + 1].re = y[i].re - y[i + 1].re;
        y[i + 1].im = y[i].im - y[i + 1].im;
        y[i].re += temp_re;
        y[i].im += temp_im;
      }

      ix = 2;
      iDelta2 = 4;
      k = 16384;
      iheight = 65533;
      while (k > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += iDelta2) {
          temp_re_tmp = i + ix;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = k; j < 32768; j += k) {
          double twid_im;
          double twid_re;
          int ihi;
          twid_re = hcostab[j];
          twid_im = hsintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + ix;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += iDelta2;
          }

          istart++;
        }

        k /= 2;
        ix = iDelta2;
        iDelta2 += iDelta2;
        iheight -= ix;
      }

      FFTImplementationCallback::getback_radix2_fft((y), (0), (reconVar1),
        (reconVar2), (wrapIndex), (65536));
    }

    //
    // Arguments    : const creal_T x[115639]
    //                const double costab[65537]
    //                const double sintab[65537]
    //                creal_T y[131072]
    // Return Type  : void
    //
    void FFTImplementationCallback::e_r2br_r2dit_trig(const creal_T x[115639],
      const double costab[65537], const double sintab[65537], creal_T y[131072])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      std::memset(&y[0], 0, 131072U * sizeof(creal_T));
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 115638; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 131072;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 131070; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 32768;
      iheight = 131069;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 65536; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }
    }

    //
    // Arguments    : double costab[65537]
    //                double sintab[65537]
    // Return Type  : void
    //
    void FFTImplementationCallback::generate_twiddle_tables(double costab[65537],
      double sintab[65537])
    {
      static double costab1q[32769];
      int k;
      costab1q[0] = 1.0;
      for (k = 0; k < 16384; k++) {
        costab1q[k + 1] = std::cos(4.7936899621426287E-5 * (static_cast<double>
          (k) + 1.0));
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
        sintab_tmp = -costab1q[32767 - k];
        sintab[k + 1] = sintab_tmp;
        costab[k + 32769] = sintab_tmp;
        sintab[k + 32769] = -costab_tmp;
      }
    }

    //
    // Arguments    : double costab[65537]
    //                double sintab[65537]
    //                double sintabinv[65537]
    // Return Type  : void
    //
    void FFTImplementationCallback::generate_twiddle_tables(double costab[65537],
      double sintab[65537], double sintabinv[65537])
    {
      static double costab1q[32769];
      int k;
      costab1q[0] = 1.0;
      for (k = 0; k < 16384; k++) {
        costab1q[k + 1] = std::cos(4.7936899621426287E-5 * (static_cast<double>
          (k) + 1.0));
      }

      for (k = 0; k < 16383; k++) {
        costab1q[k + 16385] = std::sin(4.7936899621426287E-5 * (32768.0 - (
          static_cast<double>(k) + 16385.0)));
      }

      costab1q[32768] = 0.0;
      costab[0] = 1.0;
      sintab[0] = 0.0;
      for (k = 0; k < 32768; k++) {
        double b_sintabinv_tmp;
        double sintabinv_tmp;
        sintabinv_tmp = costab1q[32767 - k];
        sintabinv[k + 1] = sintabinv_tmp;
        b_sintabinv_tmp = costab1q[k + 1];
        sintabinv[k + 32769] = b_sintabinv_tmp;
        costab[k + 1] = b_sintabinv_tmp;
        sintab[k + 1] = -sintabinv_tmp;
        costab[k + 32769] = -sintabinv_tmp;
        sintab[k + 32769] = -b_sintabinv_tmp;
      }
    }

    //
    // Arguments    : const creal_T x[131072]
    //                const double costab[65537]
    //                const double sintab[65537]
    //                creal_T y[131072]
    // Return Type  : void
    //
    void FFTImplementationCallback::r2br_r2dit_trig(const creal_T x[131072],
      const double costab[65537], const double sintab[65537], creal_T y[131072])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 131071; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 131072;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 131070; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 32768;
      iheight = 131069;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 65536; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }

      for (iy = 0; iy < 131072; iy++) {
        y[iy].re *= 7.62939453125E-6;
        y[iy].im *= 7.62939453125E-6;
      }
    }

    //
    // Arguments    : const creal_T x[56001]
    //                const double costab[65537]
    //                const double sintab[65537]
    //                creal_T y[131072]
    // Return Type  : void
    //
    void FFTImplementationCallback::r2br_r2dit_trig_impl(const creal_T x[56001],
      const double costab[65537], const double sintab[65537], creal_T y[131072])
    {
      double temp_im;
      double temp_re;
      double twid_im;
      double twid_re;
      int i;
      int iheight;
      int ix;
      int iy;
      int j;
      int ju;
      std::memset(&y[0], 0, 131072U * sizeof(creal_T));
      ix = 0;
      iy = 0;
      ju = 0;
      for (i = 0; i < 56000; i++) {
        boolean_T tst;
        y[iy] = x[ix];
        iy = 131072;
        tst = true;
        while (tst) {
          iy >>= 1;
          ju ^= iy;
          tst = ((ju & iy) == 0);
        }

        iy = ju;
        ix++;
      }

      y[iy] = x[ix];
      for (i = 0; i <= 131070; i += 2) {
        double im;
        double re;
        twid_re = y[i + 1].re;
        temp_re = twid_re;
        twid_im = y[i + 1].im;
        temp_im = twid_im;
        re = y[i].re;
        im = y[i].im;
        twid_re = re - twid_re;
        y[i + 1].re = twid_re;
        twid_im = im - twid_im;
        y[i + 1].im = twid_im;
        y[i].re = re + temp_re;
        y[i].im = im + temp_im;
      }

      iy = 2;
      ix = 4;
      ju = 32768;
      iheight = 131069;
      while (ju > 0) {
        int istart;
        int temp_re_tmp;
        for (i = 0; i < iheight; i += ix) {
          temp_re_tmp = i + iy;
          temp_re = y[temp_re_tmp].re;
          temp_im = y[temp_re_tmp].im;
          y[temp_re_tmp].re = y[i].re - temp_re;
          y[temp_re_tmp].im = y[i].im - temp_im;
          y[i].re += temp_re;
          y[i].im += temp_im;
        }

        istart = 1;
        for (j = ju; j < 65536; j += ju) {
          int ihi;
          twid_re = costab[j];
          twid_im = sintab[j];
          i = istart;
          ihi = istart + iheight;
          while (i < ihi) {
            temp_re_tmp = i + iy;
            temp_re = twid_re * y[temp_re_tmp].re - twid_im * y[temp_re_tmp].im;
            temp_im = twid_re * y[temp_re_tmp].im + twid_im * y[temp_re_tmp].re;
            y[temp_re_tmp].re = y[i].re - temp_re;
            y[temp_re_tmp].im = y[i].im - temp_im;
            y[i].re += temp_re;
            y[i].im += temp_im;
            i += ix;
          }

          istart++;
        }

        ju /= 2;
        iy = ix;
        ix += ix;
        iheight -= iy;
      }
    }
  }
}

//
// File trailer for FFTImplementationCallback.cpp
//
// [EOF]
//
