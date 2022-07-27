//
// File: getGrayMapping.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 03-Jul-2022 14:48:03
//

// Include Files
#include "getGrayMapping.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : double mapping[256]
//                double outSymbolIndex[256]
// Return Type  : void
//
namespace coder
{
  namespace comm
  {
    namespace internal
    {
      namespace qam
      {
        void getGrayMapping(double mapping[256], double outSymbolIndex[256])
        {
          int i;
          signed char symbolI[256];
          signed char symbolQ[256];
          for (i = 0; i < 256; i++) {
            mapping[i] = 0.0;
            symbolI[i] = static_cast<signed char>(i >> 4);
            symbolQ[i] = static_cast<signed char>(i & 15);
          }

          for (i = 1; i < 4; i += i) {
            for (int b_i = 0; b_i < 256; b_i++) {
              int varargin_1;
              varargin_1 = symbolI[b_i];
              symbolI[b_i] = static_cast<signed char>(varargin_1 ^ varargin_1 >>
                i);
              varargin_1 = symbolQ[b_i];
              symbolQ[b_i] = static_cast<signed char>(varargin_1 ^ varargin_1 >>
                i);
            }
          }

          for (i = 0; i < 256; i++) {
            outSymbolIndex[i] = (symbolI[i] << 4) + symbolQ[i];
            mapping[static_cast<int>(outSymbolIndex[i])] = i;
          }
        }
      }
    }
  }
}

//
// File trailer for getGrayMapping.cpp
//
// [EOF]
//
