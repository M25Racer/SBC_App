//
// File: qammod.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//

// Include Files
#include "qammod.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : creal_T
//
namespace coder
{
  creal_T qammod()
  {
    static const cint8_T table[256] = { { -15,// re
        15                             // im
      }, { -15,                        // re
        13                             // im
      }, { -15,                        // re
        11                             // im
      }, { -15,                        // re
        9                              // im
      }, { -15,                        // re
        7                              // im
      }, { -15,                        // re
        5                              // im
      }, { -15,                        // re
        3                              // im
      }, { -15,                        // re
        1                              // im
      }, { -15,                        // re
        -1                             // im
      }, { -15,                        // re
        -3                             // im
      }, { -15,                        // re
        -5                             // im
      }, { -15,                        // re
        -7                             // im
      }, { -15,                        // re
        -9                             // im
      }, { -15,                        // re
        -11                            // im
      }, { -15,                        // re
        -13                            // im
      }, { -15,                        // re
        -15                            // im
      }, { -13,                        // re
        15                             // im
      }, { -13,                        // re
        13                             // im
      }, { -13,                        // re
        11                             // im
      }, { -13,                        // re
        9                              // im
      }, { -13,                        // re
        7                              // im
      }, { -13,                        // re
        5                              // im
      }, { -13,                        // re
        3                              // im
      }, { -13,                        // re
        1                              // im
      }, { -13,                        // re
        -1                             // im
      }, { -13,                        // re
        -3                             // im
      }, { -13,                        // re
        -5                             // im
      }, { -13,                        // re
        -7                             // im
      }, { -13,                        // re
        -9                             // im
      }, { -13,                        // re
        -11                            // im
      }, { -13,                        // re
        -13                            // im
      }, { -13,                        // re
        -15                            // im
      }, { -11,                        // re
        15                             // im
      }, { -11,                        // re
        13                             // im
      }, { -11,                        // re
        11                             // im
      }, { -11,                        // re
        9                              // im
      }, { -11,                        // re
        7                              // im
      }, { -11,                        // re
        5                              // im
      }, { -11,                        // re
        3                              // im
      }, { -11,                        // re
        1                              // im
      }, { -11,                        // re
        -1                             // im
      }, { -11,                        // re
        -3                             // im
      }, { -11,                        // re
        -5                             // im
      }, { -11,                        // re
        -7                             // im
      }, { -11,                        // re
        -9                             // im
      }, { -11,                        // re
        -11                            // im
      }, { -11,                        // re
        -13                            // im
      }, { -11,                        // re
        -15                            // im
      }, { -9,                         // re
        15                             // im
      }, { -9,                         // re
        13                             // im
      }, { -9,                         // re
        11                             // im
      }, { -9,                         // re
        9                              // im
      }, { -9,                         // re
        7                              // im
      }, { -9,                         // re
        5                              // im
      }, { -9,                         // re
        3                              // im
      }, { -9,                         // re
        1                              // im
      }, { -9,                         // re
        -1                             // im
      }, { -9,                         // re
        -3                             // im
      }, { -9,                         // re
        -5                             // im
      }, { -9,                         // re
        -7                             // im
      }, { -9,                         // re
        -9                             // im
      }, { -9,                         // re
        -11                            // im
      }, { -9,                         // re
        -13                            // im
      }, { -9,                         // re
        -15                            // im
      }, { -7,                         // re
        15                             // im
      }, { -7,                         // re
        13                             // im
      }, { -7,                         // re
        11                             // im
      }, { -7,                         // re
        9                              // im
      }, { -7,                         // re
        7                              // im
      }, { -7,                         // re
        5                              // im
      }, { -7,                         // re
        3                              // im
      }, { -7,                         // re
        1                              // im
      }, { -7,                         // re
        -1                             // im
      }, { -7,                         // re
        -3                             // im
      }, { -7,                         // re
        -5                             // im
      }, { -7,                         // re
        -7                             // im
      }, { -7,                         // re
        -9                             // im
      }, { -7,                         // re
        -11                            // im
      }, { -7,                         // re
        -13                            // im
      }, { -7,                         // re
        -15                            // im
      }, { -5,                         // re
        15                             // im
      }, { -5,                         // re
        13                             // im
      }, { -5,                         // re
        11                             // im
      }, { -5,                         // re
        9                              // im
      }, { -5,                         // re
        7                              // im
      }, { -5,                         // re
        5                              // im
      }, { -5,                         // re
        3                              // im
      }, { -5,                         // re
        1                              // im
      }, { -5,                         // re
        -1                             // im
      }, { -5,                         // re
        -3                             // im
      }, { -5,                         // re
        -5                             // im
      }, { -5,                         // re
        -7                             // im
      }, { -5,                         // re
        -9                             // im
      }, { -5,                         // re
        -11                            // im
      }, { -5,                         // re
        -13                            // im
      }, { -5,                         // re
        -15                            // im
      }, { -3,                         // re
        15                             // im
      }, { -3,                         // re
        13                             // im
      }, { -3,                         // re
        11                             // im
      }, { -3,                         // re
        9                              // im
      }, { -3,                         // re
        7                              // im
      }, { -3,                         // re
        5                              // im
      }, { -3,                         // re
        3                              // im
      }, { -3,                         // re
        1                              // im
      }, { -3,                         // re
        -1                             // im
      }, { -3,                         // re
        -3                             // im
      }, { -3,                         // re
        -5                             // im
      }, { -3,                         // re
        -7                             // im
      }, { -3,                         // re
        -9                             // im
      }, { -3,                         // re
        -11                            // im
      }, { -3,                         // re
        -13                            // im
      }, { -3,                         // re
        -15                            // im
      }, { -1,                         // re
        15                             // im
      }, { -1,                         // re
        13                             // im
      }, { -1,                         // re
        11                             // im
      }, { -1,                         // re
        9                              // im
      }, { -1,                         // re
        7                              // im
      }, { -1,                         // re
        5                              // im
      }, { -1,                         // re
        3                              // im
      }, { -1,                         // re
        1                              // im
      }, { -1,                         // re
        -1                             // im
      }, { -1,                         // re
        -3                             // im
      }, { -1,                         // re
        -5                             // im
      }, { -1,                         // re
        -7                             // im
      }, { -1,                         // re
        -9                             // im
      }, { -1,                         // re
        -11                            // im
      }, { -1,                         // re
        -13                            // im
      }, { -1,                         // re
        -15                            // im
      }, { 1,                          // re
        15                             // im
      }, { 1,                          // re
        13                             // im
      }, { 1,                          // re
        11                             // im
      }, { 1,                          // re
        9                              // im
      }, { 1,                          // re
        7                              // im
      }, { 1,                          // re
        5                              // im
      }, { 1,                          // re
        3                              // im
      }, { 1,                          // re
        1                              // im
      }, { 1,                          // re
        -1                             // im
      }, { 1,                          // re
        -3                             // im
      }, { 1,                          // re
        -5                             // im
      }, { 1,                          // re
        -7                             // im
      }, { 1,                          // re
        -9                             // im
      }, { 1,                          // re
        -11                            // im
      }, { 1,                          // re
        -13                            // im
      }, { 1,                          // re
        -15                            // im
      }, { 3,                          // re
        15                             // im
      }, { 3,                          // re
        13                             // im
      }, { 3,                          // re
        11                             // im
      }, { 3,                          // re
        9                              // im
      }, { 3,                          // re
        7                              // im
      }, { 3,                          // re
        5                              // im
      }, { 3,                          // re
        3                              // im
      }, { 3,                          // re
        1                              // im
      }, { 3,                          // re
        -1                             // im
      }, { 3,                          // re
        -3                             // im
      }, { 3,                          // re
        -5                             // im
      }, { 3,                          // re
        -7                             // im
      }, { 3,                          // re
        -9                             // im
      }, { 3,                          // re
        -11                            // im
      }, { 3,                          // re
        -13                            // im
      }, { 3,                          // re
        -15                            // im
      }, { 5,                          // re
        15                             // im
      }, { 5,                          // re
        13                             // im
      }, { 5,                          // re
        11                             // im
      }, { 5,                          // re
        9                              // im
      }, { 5,                          // re
        7                              // im
      }, { 5,                          // re
        5                              // im
      }, { 5,                          // re
        3                              // im
      }, { 5,                          // re
        1                              // im
      }, { 5,                          // re
        -1                             // im
      }, { 5,                          // re
        -3                             // im
      }, { 5,                          // re
        -5                             // im
      }, { 5,                          // re
        -7                             // im
      }, { 5,                          // re
        -9                             // im
      }, { 5,                          // re
        -11                            // im
      }, { 5,                          // re
        -13                            // im
      }, { 5,                          // re
        -15                            // im
      }, { 7,                          // re
        15                             // im
      }, { 7,                          // re
        13                             // im
      }, { 7,                          // re
        11                             // im
      }, { 7,                          // re
        9                              // im
      }, { 7,                          // re
        7                              // im
      }, { 7,                          // re
        5                              // im
      }, { 7,                          // re
        3                              // im
      }, { 7,                          // re
        1                              // im
      }, { 7,                          // re
        -1                             // im
      }, { 7,                          // re
        -3                             // im
      }, { 7,                          // re
        -5                             // im
      }, { 7,                          // re
        -7                             // im
      }, { 7,                          // re
        -9                             // im
      }, { 7,                          // re
        -11                            // im
      }, { 7,                          // re
        -13                            // im
      }, { 7,                          // re
        -15                            // im
      }, { 9,                          // re
        15                             // im
      }, { 9,                          // re
        13                             // im
      }, { 9,                          // re
        11                             // im
      }, { 9,                          // re
        9                              // im
      }, { 9,                          // re
        7                              // im
      }, { 9,                          // re
        5                              // im
      }, { 9,                          // re
        3                              // im
      }, { 9,                          // re
        1                              // im
      }, { 9,                          // re
        -1                             // im
      }, { 9,                          // re
        -3                             // im
      }, { 9,                          // re
        -5                             // im
      }, { 9,                          // re
        -7                             // im
      }, { 9,                          // re
        -9                             // im
      }, { 9,                          // re
        -11                            // im
      }, { 9,                          // re
        -13                            // im
      }, { 9,                          // re
        -15                            // im
      }, { 11,                         // re
        15                             // im
      }, { 11,                         // re
        13                             // im
      }, { 11,                         // re
        11                             // im
      }, { 11,                         // re
        9                              // im
      }, { 11,                         // re
        7                              // im
      }, { 11,                         // re
        5                              // im
      }, { 11,                         // re
        3                              // im
      }, { 11,                         // re
        1                              // im
      }, { 11,                         // re
        -1                             // im
      }, { 11,                         // re
        -3                             // im
      }, { 11,                         // re
        -5                             // im
      }, { 11,                         // re
        -7                             // im
      }, { 11,                         // re
        -9                             // im
      }, { 11,                         // re
        -11                            // im
      }, { 11,                         // re
        -13                            // im
      }, { 11,                         // re
        -15                            // im
      }, { 13,                         // re
        15                             // im
      }, { 13,                         // re
        13                             // im
      }, { 13,                         // re
        11                             // im
      }, { 13,                         // re
        9                              // im
      }, { 13,                         // re
        7                              // im
      }, { 13,                         // re
        5                              // im
      }, { 13,                         // re
        3                              // im
      }, { 13,                         // re
        1                              // im
      }, { 13,                         // re
        -1                             // im
      }, { 13,                         // re
        -3                             // im
      }, { 13,                         // re
        -5                             // im
      }, { 13,                         // re
        -7                             // im
      }, { 13,                         // re
        -9                             // im
      }, { 13,                         // re
        -11                            // im
      }, { 13,                         // re
        -13                            // im
      }, { 13,                         // re
        -15                            // im
      }, { 15,                         // re
        15                             // im
      }, { 15,                         // re
        13                             // im
      }, { 15,                         // re
        11                             // im
      }, { 15,                         // re
        9                              // im
      }, { 15,                         // re
        7                              // im
      }, { 15,                         // re
        5                              // im
      }, { 15,                         // re
        3                              // im
      }, { 15,                         // re
        1                              // im
      }, { 15,                         // re
        -1                             // im
      }, { 15,                         // re
        -3                             // im
      }, { 15,                         // re
        -5                             // im
      }, { 15,                         // re
        -7                             // im
      }, { 15,                         // re
        -9                             // im
      }, { 15,                         // re
        -11                            // im
      }, { 15,                         // re
        -13                            // im
      }, { 15,                         // re
        -15                            // im
      } };

    creal_T y;
    int i;
    signed char symbolI[256];
    unsigned char symbolIndex[256];
    signed char symbolQ[256];
    for (i = 0; i < 256; i++) {
      symbolI[i] = static_cast<signed char>(i >> 4);
      symbolQ[i] = static_cast<signed char>(i & 15);
    }

    for (i = 1; i < 4; i += i) {
      for (int b_i = 0; b_i < 256; b_i++) {
        int varargin_1;
        varargin_1 = symbolI[b_i];
        symbolI[b_i] = static_cast<signed char>(varargin_1 ^ varargin_1 >> i);
        varargin_1 = symbolQ[b_i];
        symbolQ[b_i] = static_cast<signed char>(varargin_1 ^ varargin_1 >> i);
      }
    }

    for (i = 0; i < 256; i++) {
      symbolIndex[i] = static_cast<unsigned char>((symbolI[i] << 4) + symbolQ[i]);
    }

    y.re = table[symbolIndex[128]].re;
    y.im = table[symbolIndex[128]].im;
    return y;
  }
}

//
// File trailer for qammod.cpp
//
// [EOF]
//
