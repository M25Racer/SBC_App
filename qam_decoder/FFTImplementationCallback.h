//
// File: FFTImplementationCallback.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
//
#ifndef FFTIMPLEMENTATIONCALLBACK_H
#define FFTIMPLEMENTATIONCALLBACK_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_real_T;
struct emxArray_creal_T;

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
      static void dobluesteinfft(const emxArray_real_T *x, int n2blue, int nfft,
        const emxArray_real_T *costab, const emxArray_real_T *sintab, const
        emxArray_real_T *sintabinv, emxArray_creal_T *y);
      static void r2br_r2dit_trig_impl(const emxArray_creal_T *x, int
        unsigned_nRows, const emxArray_real_T *costab, const emxArray_real_T
        *sintab, emxArray_creal_T *y);
      static void doHalfLengthRadix2(const emxArray_real_T *x, emxArray_creal_T *
        y, int unsigned_nRows, const emxArray_real_T *costab, const
        emxArray_real_T *sintab);
     protected:
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
