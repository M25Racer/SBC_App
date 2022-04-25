//
// File: qammod.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//

// Include Files
#include "qammod.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "HS_EWL_RECEIVE_rtwutil.h"
#include "HS_EWL_RECEIVE_types.h"
#include "getSquareConstellation.h"
#include "log2.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : double x
//                double M
// Return Type  : creal_T
//
namespace coder
{
  creal_T qammod(double x, double M)
  {
    emxArray_creal_T *b_const;
    emxArray_int32_T *binMapping;
    emxArray_int32_T *symbolI;
    emxArray_int32_T *symbolQ;
    emxArray_real_T *binMapping_tmp;
    creal_T y;
    double d;
    int b_i;
    int i;
    int loop_ub;
    int nBitsBy2;
    int varargin_1;
    emxInit_creal_T(&b_const, 2);
    b_log2(M);
    b_log2(M);
    comm::internal::qam::getSquareConstellation(M, b_const);
    d = rt_roundd_snf(b_log2(M));
    if (d < 2.147483648E+9) {
      if (d >= -2.147483648E+9) {
        nBitsBy2 = static_cast<int>(d);
      } else {
        nBitsBy2 = MIN_int32_T;
      }
    } else if (d >= 2.147483648E+9) {
      nBitsBy2 = MAX_int32_T;
    } else {
      nBitsBy2 = 0;
    }

    emxInit_real_T(&binMapping_tmp, 2);
    if (rtIsNaN(M - 1.0)) {
      i = binMapping_tmp->size[0] * binMapping_tmp->size[1];
      binMapping_tmp->size[0] = 1;
      binMapping_tmp->size[1] = 1;
      emxEnsureCapacity_real_T(binMapping_tmp, i);
      binMapping_tmp->data[0] = rtNaN;
    } else if (M - 1.0 < 0.0) {
      binMapping_tmp->size[0] = 1;
      binMapping_tmp->size[1] = 0;
    } else if (rtIsInf(M - 1.0) && (0.0 == M - 1.0)) {
      i = binMapping_tmp->size[0] * binMapping_tmp->size[1];
      binMapping_tmp->size[0] = 1;
      binMapping_tmp->size[1] = 1;
      emxEnsureCapacity_real_T(binMapping_tmp, i);
      binMapping_tmp->data[0] = rtNaN;
    } else {
      i = binMapping_tmp->size[0] * binMapping_tmp->size[1];
      binMapping_tmp->size[0] = 1;
      loop_ub = static_cast<int>(std::floor(M - 1.0));
      binMapping_tmp->size[1] = loop_ub + 1;
      emxEnsureCapacity_real_T(binMapping_tmp, i);
      for (i = 0; i <= loop_ub; i++) {
        binMapping_tmp->data[i] = i;
      }
    }

    emxInit_int32_T(&binMapping, 1);
    i = binMapping->size[0];
    binMapping->size[0] = binMapping_tmp->size[1];
    emxEnsureCapacity_int32_T(binMapping, i);
    loop_ub = binMapping_tmp->size[1];
    for (i = 0; i < loop_ub; i++) {
      d = rt_roundd_snf(binMapping_tmp->data[i]);
      if (d < 2.147483648E+9) {
        if (d >= -2.147483648E+9) {
          b_i = static_cast<int>(d);
        } else {
          b_i = MIN_int32_T;
        }
      } else if (d >= 2.147483648E+9) {
        b_i = MAX_int32_T;
      } else {
        b_i = 0;
      }

      binMapping->data[i] = b_i;
    }

    emxFree_real_T(&binMapping_tmp);
    d = rt_roundd_snf(M);
    if (d < 2.147483648E+9) {
      if (d >= -2.147483648E+9) {
        i = static_cast<int>(d);
      } else {
        i = MIN_int32_T;
      }
    } else if (d >= 2.147483648E+9) {
      i = MAX_int32_T;
    } else {
      i = 0;
    }

    emxInit_int32_T(&symbolI, 1);
    emxInit_int32_T(&symbolQ, 1);
    if ((nBitsBy2 & 1) != 0) {
      int b_varargin_1;
      int nBitsI;
      if (nBitsBy2 > 2147483646) {
        b_i = MAX_int32_T;
      } else {
        b_i = nBitsBy2 + 1;
      }

      nBitsI = b_i >> 1;
      if (nBitsBy2 < -2147483647) {
        b_i = MIN_int32_T;
      } else {
        b_i = nBitsBy2 - 1;
      }

      nBitsBy2 = b_i >> 1;
      b_i = symbolI->size[0];
      symbolI->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolI, b_i);
      loop_ub = binMapping->size[0];
      for (b_i = 0; b_i < loop_ub; b_i++) {
        varargin_1 = binMapping->data[b_i];
        if (-nBitsBy2 >= 0) {
          if (-nBitsBy2 <= 31) {
            symbolI->data[b_i] = varargin_1 << -nBitsBy2;
          } else {
            symbolI->data[b_i] = 0;
          }
        } else if (-nBitsBy2 >= -31) {
          symbolI->data[b_i] = varargin_1 >> nBitsBy2;
        } else if (varargin_1 < 0) {
          symbolI->data[b_i] = -1;
        } else {
          symbolI->data[b_i] = 0;
        }
      }

      if (-nBitsI >= 0) {
        if (-nBitsI <= 31) {
          b_i = (i - 1) << -nBitsI;
        } else {
          b_i = 0;
        }
      } else if (-nBitsI >= -31) {
        b_i = (i - 1) >> nBitsI;
      } else if (i - 1 < 0) {
        b_i = -1;
      } else {
        b_i = 0;
      }

      i = symbolQ->size[0];
      symbolQ->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolQ, i);
      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        varargin_1 = binMapping->data[i];
        symbolQ->data[i] = varargin_1 & b_i;
      }

      for (b_i = 1; b_i < nBitsI; b_i += b_i) {
        loop_ub = symbolI->size[0];
        for (i = 0; i < loop_ub; i++) {
          varargin_1 = symbolI->data[i];
          b_varargin_1 = symbolI->data[i];
          if (-b_i >= -31) {
            varargin_1 >>= static_cast<unsigned char>(-static_cast<double>(-b_i));
          } else if (varargin_1 < 0) {
            varargin_1 = -1;
          } else {
            varargin_1 = 0;
          }

          symbolI->data[i] = b_varargin_1 ^ varargin_1;
        }
      }

      for (b_i = 1; b_i < nBitsBy2; b_i += b_i) {
        loop_ub = symbolQ->size[0];
        for (i = 0; i < loop_ub; i++) {
          varargin_1 = symbolQ->data[i];
          b_varargin_1 = symbolQ->data[i];
          if (-b_i >= -31) {
            varargin_1 >>= static_cast<unsigned char>(-static_cast<double>(-b_i));
          } else if (varargin_1 < 0) {
            varargin_1 = -1;
          } else {
            varargin_1 = 0;
          }

          symbolQ->data[i] = b_varargin_1 ^ varargin_1;
        }
      }

      i = binMapping->size[0];
      binMapping->size[0] = symbolI->size[0];
      emxEnsureCapacity_int32_T(binMapping, i);
      loop_ub = symbolI->size[0];
      for (i = 0; i < loop_ub; i++) {
        varargin_1 = symbolI->data[i];
        if (nBitsBy2 >= 0) {
          if (nBitsBy2 <= 31) {
            binMapping->data[i] = varargin_1 << nBitsBy2;
          } else {
            binMapping->data[i] = 0;
          }
        } else if (nBitsBy2 >= -31) {
          binMapping->data[i] = varargin_1 >> -nBitsBy2;
        } else if (varargin_1 < 0) {
          binMapping->data[i] = -1;
        } else {
          binMapping->data[i] = 0;
        }
      }

      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        nBitsBy2 = binMapping->data[i];
        b_i = symbolQ->data[i];
        if ((nBitsBy2 < 0) && (b_i < MIN_int32_T - nBitsBy2)) {
          b_i = MIN_int32_T;
        } else if ((nBitsBy2 > 0) && (b_i > MAX_int32_T - nBitsBy2)) {
          b_i = MAX_int32_T;
        } else {
          b_i += nBitsBy2;
        }

        binMapping->data[i] = b_i;
      }
    } else {
      nBitsBy2 >>= 1;
      b_i = symbolI->size[0];
      symbolI->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolI, b_i);
      loop_ub = binMapping->size[0];
      for (b_i = 0; b_i < loop_ub; b_i++) {
        varargin_1 = binMapping->data[b_i];
        if (-nBitsBy2 >= 0) {
          if (-nBitsBy2 <= 31) {
            symbolI->data[b_i] = varargin_1 << -nBitsBy2;
          } else {
            symbolI->data[b_i] = 0;
          }
        } else if (-nBitsBy2 >= -31) {
          symbolI->data[b_i] = varargin_1 >> nBitsBy2;
        } else if (varargin_1 < 0) {
          symbolI->data[b_i] = -1;
        } else {
          symbolI->data[b_i] = 0;
        }
      }

      if (-nBitsBy2 >= 0) {
        if (-nBitsBy2 <= 31) {
          b_i = (i - 1) << -nBitsBy2;
        } else {
          b_i = 0;
        }
      } else if (-nBitsBy2 >= -31) {
        b_i = (i - 1) >> nBitsBy2;
      } else if (i - 1 < 0) {
        b_i = -1;
      } else {
        b_i = 0;
      }

      i = symbolQ->size[0];
      symbolQ->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolQ, i);
      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        varargin_1 = binMapping->data[i];
        symbolQ->data[i] = varargin_1 & b_i;
      }

      for (b_i = 1; b_i < nBitsBy2; b_i += b_i) {
        int b_varargin_1;
        loop_ub = symbolI->size[0];
        for (i = 0; i < loop_ub; i++) {
          varargin_1 = symbolI->data[i];
          b_varargin_1 = symbolI->data[i];
          if (-b_i >= -31) {
            varargin_1 >>= static_cast<unsigned char>(-static_cast<double>(-b_i));
          } else if (varargin_1 < 0) {
            varargin_1 = -1;
          } else {
            varargin_1 = 0;
          }

          symbolI->data[i] = b_varargin_1 ^ varargin_1;
        }

        loop_ub = symbolQ->size[0];
        for (i = 0; i < loop_ub; i++) {
          varargin_1 = symbolQ->data[i];
          b_varargin_1 = symbolQ->data[i];
          if (-b_i >= -31) {
            varargin_1 >>= static_cast<unsigned char>(-static_cast<double>(-b_i));
          } else if (varargin_1 < 0) {
            varargin_1 = -1;
          } else {
            varargin_1 = 0;
          }

          symbolQ->data[i] = b_varargin_1 ^ varargin_1;
        }
      }

      i = binMapping->size[0];
      binMapping->size[0] = symbolI->size[0];
      emxEnsureCapacity_int32_T(binMapping, i);
      loop_ub = symbolI->size[0];
      for (i = 0; i < loop_ub; i++) {
        varargin_1 = symbolI->data[i];
        if (nBitsBy2 >= 0) {
          if (nBitsBy2 <= 31) {
            binMapping->data[i] = varargin_1 << nBitsBy2;
          } else {
            binMapping->data[i] = 0;
          }
        } else if (nBitsBy2 >= -31) {
          binMapping->data[i] = varargin_1 >> -nBitsBy2;
        } else if (varargin_1 < 0) {
          binMapping->data[i] = -1;
        } else {
          binMapping->data[i] = 0;
        }
      }

      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        nBitsBy2 = binMapping->data[i];
        b_i = symbolQ->data[i];
        if ((nBitsBy2 < 0) && (b_i < MIN_int32_T - nBitsBy2)) {
          b_i = MIN_int32_T;
        } else if ((nBitsBy2 > 0) && (b_i > MAX_int32_T - nBitsBy2)) {
          b_i = MAX_int32_T;
        } else {
          b_i += nBitsBy2;
        }

        binMapping->data[i] = b_i;
      }
    }

    emxFree_int32_T(&symbolQ);
    emxFree_int32_T(&symbolI);
    y = b_const->data[binMapping->data[static_cast<int>(x + 1.0) - 1]];
    emxFree_int32_T(&binMapping);
    emxFree_creal_T(&b_const);
    return y;
  }
}

//
// File trailer for qammod.cpp
//
// [EOF]
//
