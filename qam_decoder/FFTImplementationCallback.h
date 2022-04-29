//
// File: FFTImplementationCallback.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//
#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;
struct emxArray_creal_T;
struct emxArray_int32_T;

// Type Definitions
namespace coder
{
  namespace internal
  {
    class FFTImplementationCallback
    {
     public:
      static void get_algo_sizes(int nfft, boolean_T useRadix2, int *n2blue, int
        *nRows);
      static void generate_twiddle_tables(int nRows, boolean_T useRadix2,
        emxArray_real_T *costab, emxArray_real_T *sintab, emxArray_real_T
        *sintabinv);
      static void r2br_r2dit_trig(const emxArray_real_T *x, int n1_unsigned,
        const emxArray_real_T *costab, const emxArray_real_T *sintab,
        emxArray_creal_T *y);
      static void dobluesteinfft(const emxArray_real_T *x, int n2blue, int nfft,
        const emxArray_real_T *costab, const emxArray_real_T *sintab, const
        emxArray_real_T *sintabinv, emxArray_creal_T *y);
      static void r2br_r2dit_trig_impl(const emxArray_creal_T *x, int
        unsigned_nRows, const emxArray_real_T *costab, const emxArray_real_T
        *sintab, emxArray_creal_T *y);
      static void doHalfLengthRadix2(const double x[1820], emxArray_creal_T *y,
        int unsigned_nRows, const emxArray_real_T *costab, const emxArray_real_T
        *sintab);
      static void doHalfLengthBluestein(const double x[1820], emxArray_creal_T
        *y, int nRows, int nfft, const emxArray_creal_T *wwc, const
        emxArray_real_T *costab, const emxArray_real_T *sintab, const
        emxArray_real_T *costabinv, const emxArray_real_T *sintabinv);
     protected:
      static void get_bitrevIndex(int nRowsM1, int nfftLen, emxArray_int32_T
        *bitrevIndex);
      static void b_generate_twiddle_tables(int nRows, emxArray_real_T *costab,
        emxArray_real_T *sintab, emxArray_real_T *sintabinv);
      static void get_half_twiddle_tables(const emxArray_real_T *costab, const
        emxArray_real_T *sintab, const emxArray_real_T *costabinv, const
        emxArray_real_T *sintabinv, emxArray_real_T *hcostab, emxArray_real_T
        *hsintab, emxArray_real_T *hcostabinv, emxArray_real_T *hsintabinv);
      static void doHalfLengthRadix2(const emxArray_real_T *x, emxArray_creal_T *
        y, int unsigned_nRows, const emxArray_real_T *costab, const
        emxArray_real_T *sintab);
      static void getback_radix2_fft(emxArray_creal_T *y, const emxArray_creal_T
        *reconVar1, const emxArray_creal_T *reconVar2, const emxArray_int32_T
        *wrapIndex, int hnRows);
      static void doHalfLengthBluestein(const emxArray_real_T *x,
        emxArray_creal_T *y, int nrowsx, int nRows, int nfft, const
        emxArray_creal_T *wwc, const emxArray_real_T *costab, const
        emxArray_real_T *sintab, const emxArray_real_T *costabinv, const
        emxArray_real_T *sintabinv);
    };
  }
}

#endif

//
// File trailer for FFTImplementationCallback.h
//
// [EOF]
//
