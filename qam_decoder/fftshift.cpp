//
// File: fftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "fftshift.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : creal_T x[56001]
// Return Type  : void
//
namespace coder
{
  void fftshift(creal_T x[56001])
  {
    int i2;
    i2 = 0;
    for (int i = 0; i < 1; i++) {
      int i1;
      i1 = i2;
      i2 += 56000;
      for (int j = 0; j < 1; j++) {
        double xtmp_im;
        double xtmp_re;
        int ia;
        int ib;
        i1++;
        i2++;
        ia = i1 - 1;
        ib = i1 + 27999;
        xtmp_re = x[i1 + 27999].re;
        xtmp_im = x[i1 + 27999].im;
        for (int k = 0; k < 28000; k++) {
          x[ib] = x[ia];
          x[ia] = x[ib + 1];
          ia++;
          ib++;
        }

        x[ib].re = xtmp_re;
        x[ib].im = xtmp_im;
      }
    }
  }
}

//
// File trailer for fftshift.cpp
//
// [EOF]
//
