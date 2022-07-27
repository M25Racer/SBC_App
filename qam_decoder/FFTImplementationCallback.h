//
// File: FFTImplementationCallback.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//
#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
namespace coder
{
  namespace internal
  {
    class FFTImplementationCallback
    {
     public:
      static void generate_twiddle_tables(double costab[65537], double sintab
        [65537]);
      static void r2br_r2dit_trig(const creal_T x[131072], const double costab
        [65537], const double sintab[65537], creal_T y[131072]);
      static void generate_twiddle_tables(double costab[65537], double sintab
        [65537], double sintabinv[65537]);
      static void r2br_r2dit_trig_impl(const creal_T x[56001], const double
        costab[65537], const double sintab[65537], creal_T y[131072]);
      static void b_r2br_r2dit_trig(const creal_T x[112001], const double
        costab[65537], const double sintab[65537], creal_T y[131072]);
      static void c_r2br_r2dit_trig_impl(const creal_T x[57820], const double
        costab[65537], const double sintab[65537], creal_T y[131072]);
      static void e_r2br_r2dit_trig(const creal_T x[115639], const double
        costab[65537], const double sintab[65537], creal_T y[131072]);
      static void doHalfLengthRadix2(const double x[57820], int xoffInit,
        creal_T y[131072], const double costab[65537], const double sintab[65537]);
      static void b_doHalfLengthRadix2(const double x[1820], int xoffInit,
        creal_T y[131072], const double costab[65537], const double sintab[65537]);
      static void doHalfLengthBluestein(const double x[57820], int xoffInit,
        creal_T y[57820], const creal_T wwc[57819], const double costab[65537],
        const double sintab[65537], const double costabinv[65537], const double
        sintabinv[65537]);
     protected:
      static void get_bitrevIndex(int bitrevIndex[65536]);
      static void b_generate_twiddle_tables(double costab[57821], double sintab
        [57821], double sintabinv[57821]);
      static void b_r2br_r2dit_trig_impl(const creal_T x[28910], const double
        costab[32768], const double sintab[32768], creal_T y[65536]);
      static void c_r2br_r2dit_trig(const creal_T x[57819], const double costab
        [32768], const double sintab[32768], creal_T y[65536]);
      static void d_r2br_r2dit_trig(const creal_T x[65536], const double costab
        [32768], const double sintab[32768], creal_T y[65536]);
      static void getback_radix2_fft(creal_T y[131072], int yoff, const creal_T
        reconVar1[65536], const creal_T reconVar2[65536], const int wrapIndex
        [65536], int hnRows);
    };
  }
}

#endif

//
// File trailer for FFTImplementationCallback.h
//
// [EOF]
//
