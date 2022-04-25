//
// File: qamdemod.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//

// Include Files
#include "qamdemod.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "HS_EWL_RECEIVE_rtwutil.h"
#include "HS_EWL_RECEIVE_types.h"
#include "getSquareConstellation.h"
#include "log2.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Declarations
static double rt_hypotd_snf(double u0, double u1);

// Function Definitions
//
// Arguments    : double u0
//                double u1
// Return Type  : double
//
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = std::abs(u0);
  y = std::abs(u1);
  if (a < y) {
    a /= y;
    y *= std::sqrt(a * a + 1.0);
  } else if (a > y) {
    y /= a;
    y = a * std::sqrt(y * y + 1.0);
  } else {
    if (!rtIsNaN(y)) {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

//
// Arguments    : const creal_T y_data[]
//                const int y_size[1]
//                double M
//                emxArray_real_T *x
// Return Type  : void
//
namespace coder
{
  void qamdemod(const creal_T y_data[], const int y_size[1], double M,
                emxArray_real_T *x)
  {
    emxArray_creal_T *b_const;
    emxArray_creal_T *c_x;
    emxArray_int32_T *binMapping;
    emxArray_int32_T *symbolI;
    emxArray_int32_T *symbolQ;
    emxArray_real_T *mapping;
    emxArray_real_T *varargin_1;
    double iIdx_data[255];
    double rIdx_data[255];
    double b_x;
    double sqrtM;
    int i;
    int i1;
    int iIdx_size_idx_0;
    int k;
    int loop_ub;
    int n;
    int nBitsI;
    int nx;
    int trueCount;
    unsigned char b_tmp_data[255];
    unsigned char tmp_data[255];
    b_log2(M);
    sqrtM = b_log2(M);
    if (rtIsNaN(sqrtM) || rtIsInf(sqrtM)) {
      b_x = rtNaN;
    } else if (sqrtM == 0.0) {
      b_x = 0.0;
    } else {
      b_x = std::fmod(sqrtM, 2.0);
      if (b_x == 0.0) {
        b_x = 0.0;
      } else {
        if (sqrtM < 0.0) {
          b_x += 2.0;
        }
      }
    }

    emxInit_real_T(&varargin_1, 2);
    if (b_x != 0.0) {
      emxInit_creal_T(&b_const, 2);
      comm::internal::qam::getSquareConstellation(M, b_const);
      iIdx_size_idx_0 = y_size[0];
      i = y_size[0];
      emxInit_creal_T(&c_x, 2);
      for (k = 0; k < i; k++) {
        i1 = c_x->size[0] * c_x->size[1];
        c_x->size[0] = 1;
        c_x->size[1] = b_const->size[1];
        emxEnsureCapacity_creal_T(c_x, i1);
        loop_ub = b_const->size[0] * b_const->size[1];
        for (i1 = 0; i1 < loop_ub; i1++) {
          c_x->data[i1].re = y_data[k].re - b_const->data[i1].re;
          c_x->data[i1].im = y_data[k].im - b_const->data[i1].im;
        }

        nx = c_x->size[1];
        i1 = varargin_1->size[0] * varargin_1->size[1];
        varargin_1->size[0] = 1;
        varargin_1->size[1] = c_x->size[1];
        emxEnsureCapacity_real_T(varargin_1, i1);
        for (nBitsI = 0; nBitsI < nx; nBitsI++) {
          varargin_1->data[nBitsI] = rt_hypotd_snf(c_x->data[nBitsI].re,
            c_x->data[nBitsI].im);
        }

        n = varargin_1->size[1];
        if (varargin_1->size[1] <= 2) {
          if (varargin_1->size[1] == 1) {
            trueCount = 1;
          } else if ((varargin_1->data[0] > varargin_1->data[1]) || (rtIsNaN
                      (varargin_1->data[0]) && (!rtIsNaN(varargin_1->data[1]))))
          {
            trueCount = 2;
          } else {
            trueCount = 1;
          }
        } else {
          if (!rtIsNaN(varargin_1->data[0])) {
            trueCount = 1;
          } else {
            boolean_T exitg1;
            trueCount = 0;
            nBitsI = 2;
            exitg1 = false;
            while ((!exitg1) && (nBitsI <= varargin_1->size[1])) {
              if (!rtIsNaN(varargin_1->data[nBitsI - 1])) {
                trueCount = nBitsI;
                exitg1 = true;
              } else {
                nBitsI++;
              }
            }
          }

          if (trueCount == 0) {
            trueCount = 1;
          } else {
            sqrtM = varargin_1->data[trueCount - 1];
            i1 = trueCount + 1;
            for (nBitsI = i1; nBitsI <= n; nBitsI++) {
              b_x = varargin_1->data[nBitsI - 1];
              if (sqrtM > b_x) {
                sqrtM = b_x;
                trueCount = nBitsI;
              }
            }
          }
        }

        iIdx_data[k] = static_cast<double>(trueCount) - 1.0;
      }

      emxFree_creal_T(&c_x);
      emxFree_creal_T(&b_const);
    } else {
      sqrtM = std::sqrt(M);
      nx = y_size[0];
      loop_ub = y_size[0];
      for (i = 0; i < loop_ub; i++) {
        rIdx_data[i] = (y_data[i].re + (sqrtM - 1.0)) / 2.0;
      }

      for (k = 0; k < nx; k++) {
        rIdx_data[k] = rt_roundd_snf(rIdx_data[k]);
      }

      for (k = 0; k < nx; k++) {
        if (rIdx_data[k] < 0.0) {
          rIdx_data[k] = 0.0;
        }
      }

      nx = y_size[0] - 1;
      trueCount = 0;
      n = 0;
      for (k = 0; k <= nx; k++) {
        if (rIdx_data[k] > sqrtM - 1.0) {
          trueCount++;
          tmp_data[n] = static_cast<unsigned char>(k + 1);
          n++;
        }
      }

      for (i = 0; i < trueCount; i++) {
        rIdx_data[tmp_data[i] - 1] = sqrtM - 1.0;
      }

      iIdx_size_idx_0 = y_size[0];
      loop_ub = y_size[0];
      for (i = 0; i < loop_ub; i++) {
        iIdx_data[i] = (y_data[i].im + (sqrtM - 1.0)) / 2.0;
      }

      for (k = 0; k < iIdx_size_idx_0; k++) {
        iIdx_data[k] = rt_roundd_snf(iIdx_data[k]);
      }

      for (k = 0; k < iIdx_size_idx_0; k++) {
        if (iIdx_data[k] < 0.0) {
          iIdx_data[k] = 0.0;
        }
      }

      nx = y_size[0] - 1;
      trueCount = 0;
      n = 0;
      for (k = 0; k <= nx; k++) {
        if (iIdx_data[k] > sqrtM - 1.0) {
          trueCount++;
          b_tmp_data[n] = static_cast<unsigned char>(k + 1);
          n++;
        }
      }

      for (i = 0; i < trueCount; i++) {
        iIdx_data[b_tmp_data[i] - 1] = sqrtM - 1.0;
      }

      for (i = 0; i < iIdx_size_idx_0; i++) {
        iIdx_data[i] = ((sqrtM - iIdx_data[i]) - 1.0) + sqrtM * rIdx_data[i];
      }
    }

    b_x = rt_roundd_snf(b_log2(M));
    if (b_x < 2.147483648E+9) {
      if (b_x >= -2.147483648E+9) {
        trueCount = static_cast<int>(b_x);
      } else {
        trueCount = MIN_int32_T;
      }
    } else if (b_x >= 2.147483648E+9) {
      trueCount = MAX_int32_T;
    } else {
      trueCount = 0;
    }

    if (rtIsNaN(M - 1.0)) {
      i = varargin_1->size[0] * varargin_1->size[1];
      varargin_1->size[0] = 1;
      varargin_1->size[1] = 1;
      emxEnsureCapacity_real_T(varargin_1, i);
      varargin_1->data[0] = rtNaN;
    } else if (M - 1.0 < 0.0) {
      varargin_1->size[0] = 1;
      varargin_1->size[1] = 0;
    } else if (rtIsInf(M - 1.0) && (0.0 == M - 1.0)) {
      i = varargin_1->size[0] * varargin_1->size[1];
      varargin_1->size[0] = 1;
      varargin_1->size[1] = 1;
      emxEnsureCapacity_real_T(varargin_1, i);
      varargin_1->data[0] = rtNaN;
    } else {
      i = varargin_1->size[0] * varargin_1->size[1];
      varargin_1->size[0] = 1;
      loop_ub = static_cast<int>(std::floor(M - 1.0));
      varargin_1->size[1] = loop_ub + 1;
      emxEnsureCapacity_real_T(varargin_1, i);
      for (i = 0; i <= loop_ub; i++) {
        varargin_1->data[i] = i;
      }
    }

    emxInit_int32_T(&binMapping, 1);
    i = binMapping->size[0];
    binMapping->size[0] = varargin_1->size[1];
    emxEnsureCapacity_int32_T(binMapping, i);
    loop_ub = varargin_1->size[1];
    for (i = 0; i < loop_ub; i++) {
      b_x = rt_roundd_snf(varargin_1->data[i]);
      if (b_x < 2.147483648E+9) {
        if (b_x >= -2.147483648E+9) {
          i1 = static_cast<int>(b_x);
        } else {
          i1 = MIN_int32_T;
        }
      } else if (b_x >= 2.147483648E+9) {
        i1 = MAX_int32_T;
      } else {
        i1 = 0;
      }

      binMapping->data[i] = i1;
    }

    emxInit_real_T(&mapping, 1);
    i = mapping->size[0];
    mapping->size[0] = static_cast<int>(M);
    emxEnsureCapacity_real_T(mapping, i);
    loop_ub = static_cast<int>(M);
    for (i = 0; i < loop_ub; i++) {
      mapping->data[i] = 0.0;
    }

    b_x = rt_roundd_snf(M);
    if (b_x < 2.147483648E+9) {
      if (b_x >= -2.147483648E+9) {
        i = static_cast<int>(b_x);
      } else {
        i = MIN_int32_T;
      }
    } else if (b_x >= 2.147483648E+9) {
      i = MAX_int32_T;
    } else {
      i = 0;
    }

    emxInit_int32_T(&symbolI, 1);
    emxInit_int32_T(&symbolQ, 1);
    if ((trueCount & 1) != 0) {
      if (trueCount > 2147483646) {
        n = MAX_int32_T;
      } else {
        n = trueCount + 1;
      }

      nBitsI = n >> 1;
      if (trueCount < -2147483647) {
        n = MIN_int32_T;
      } else {
        n = trueCount - 1;
      }

      nx = n >> 1;
      i1 = symbolI->size[0];
      symbolI->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolI, i1);
      loop_ub = binMapping->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        n = binMapping->data[i1];
        if (-nx >= 0) {
          if (-nx <= 31) {
            symbolI->data[i1] = n << -nx;
          } else {
            symbolI->data[i1] = 0;
          }
        } else if (-nx >= -31) {
          symbolI->data[i1] = n >> nx;
        } else if (n < 0) {
          symbolI->data[i1] = -1;
        } else {
          symbolI->data[i1] = 0;
        }
      }

      if (-nBitsI >= 0) {
        if (-nBitsI <= 31) {
          trueCount = (i - 1) << -nBitsI;
        } else {
          trueCount = 0;
        }
      } else if (-nBitsI >= -31) {
        trueCount = (i - 1) >> nBitsI;
      } else if (i - 1 < 0) {
        trueCount = -1;
      } else {
        trueCount = 0;
      }

      i = symbolQ->size[0];
      symbolQ->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolQ, i);
      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        n = binMapping->data[i];
        symbolQ->data[i] = n & trueCount;
      }

      for (k = 1; k < nBitsI; k += k) {
        loop_ub = symbolI->size[0];
        for (i = 0; i < loop_ub; i++) {
          n = symbolI->data[i];
          trueCount = symbolI->data[i];
          if (-k >= -31) {
            n >>= static_cast<unsigned char>(-static_cast<double>(-k));
          } else if (n < 0) {
            n = -1;
          } else {
            n = 0;
          }

          symbolI->data[i] = trueCount ^ n;
        }
      }

      for (k = 1; k < nx; k += k) {
        loop_ub = symbolQ->size[0];
        for (i = 0; i < loop_ub; i++) {
          n = symbolQ->data[i];
          trueCount = symbolQ->data[i];
          if (-k >= -31) {
            n >>= static_cast<unsigned char>(-static_cast<double>(-k));
          } else if (n < 0) {
            n = -1;
          } else {
            n = 0;
          }

          symbolQ->data[i] = trueCount ^ n;
        }
      }

      i = binMapping->size[0];
      binMapping->size[0] = symbolI->size[0];
      emxEnsureCapacity_int32_T(binMapping, i);
      loop_ub = symbolI->size[0];
      for (i = 0; i < loop_ub; i++) {
        n = symbolI->data[i];
        if (nx >= 0) {
          if (nx <= 31) {
            binMapping->data[i] = n << nx;
          } else {
            binMapping->data[i] = 0;
          }
        } else if (nx >= -31) {
          binMapping->data[i] = n >> -nx;
        } else if (n < 0) {
          binMapping->data[i] = -1;
        } else {
          binMapping->data[i] = 0;
        }
      }

      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        trueCount = binMapping->data[i];
        nx = symbolQ->data[i];
        if ((trueCount < 0) && (nx < MIN_int32_T - trueCount)) {
          n = MIN_int32_T;
        } else if ((trueCount > 0) && (nx > MAX_int32_T - trueCount)) {
          n = MAX_int32_T;
        } else {
          n = trueCount + nx;
        }

        binMapping->data[i] = n;
      }
    } else {
      nx = trueCount >> 1;
      i1 = symbolI->size[0];
      symbolI->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolI, i1);
      loop_ub = binMapping->size[0];
      for (i1 = 0; i1 < loop_ub; i1++) {
        n = binMapping->data[i1];
        if (-nx >= 0) {
          if (-nx <= 31) {
            symbolI->data[i1] = n << -nx;
          } else {
            symbolI->data[i1] = 0;
          }
        } else if (-nx >= -31) {
          symbolI->data[i1] = n >> nx;
        } else if (n < 0) {
          symbolI->data[i1] = -1;
        } else {
          symbolI->data[i1] = 0;
        }
      }

      if (-nx >= 0) {
        if (-nx <= 31) {
          trueCount = (i - 1) << -nx;
        } else {
          trueCount = 0;
        }
      } else if (-nx >= -31) {
        trueCount = (i - 1) >> nx;
      } else if (i - 1 < 0) {
        trueCount = -1;
      } else {
        trueCount = 0;
      }

      i = symbolQ->size[0];
      symbolQ->size[0] = binMapping->size[0];
      emxEnsureCapacity_int32_T(symbolQ, i);
      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        n = binMapping->data[i];
        symbolQ->data[i] = n & trueCount;
      }

      for (k = 1; k < nx; k += k) {
        loop_ub = symbolI->size[0];
        for (i = 0; i < loop_ub; i++) {
          n = symbolI->data[i];
          trueCount = symbolI->data[i];
          if (-k >= -31) {
            n >>= static_cast<unsigned char>(-static_cast<double>(-k));
          } else if (n < 0) {
            n = -1;
          } else {
            n = 0;
          }

          symbolI->data[i] = trueCount ^ n;
        }

        loop_ub = symbolQ->size[0];
        for (i = 0; i < loop_ub; i++) {
          n = symbolQ->data[i];
          trueCount = symbolQ->data[i];
          if (-k >= -31) {
            n >>= static_cast<unsigned char>(-static_cast<double>(-k));
          } else if (n < 0) {
            n = -1;
          } else {
            n = 0;
          }

          symbolQ->data[i] = trueCount ^ n;
        }
      }

      i = binMapping->size[0];
      binMapping->size[0] = symbolI->size[0];
      emxEnsureCapacity_int32_T(binMapping, i);
      loop_ub = symbolI->size[0];
      for (i = 0; i < loop_ub; i++) {
        n = symbolI->data[i];
        if (nx >= 0) {
          if (nx <= 31) {
            binMapping->data[i] = n << nx;
          } else {
            binMapping->data[i] = 0;
          }
        } else if (nx >= -31) {
          binMapping->data[i] = n >> -nx;
        } else if (n < 0) {
          binMapping->data[i] = -1;
        } else {
          binMapping->data[i] = 0;
        }
      }

      loop_ub = binMapping->size[0];
      for (i = 0; i < loop_ub; i++) {
        trueCount = binMapping->data[i];
        nx = symbolQ->data[i];
        if ((trueCount < 0) && (nx < MIN_int32_T - trueCount)) {
          n = MIN_int32_T;
        } else if ((trueCount > 0) && (nx > MAX_int32_T - trueCount)) {
          n = MAX_int32_T;
        } else {
          n = trueCount + nx;
        }

        binMapping->data[i] = n;
      }
    }

    emxFree_int32_T(&symbolQ);
    emxFree_int32_T(&symbolI);
    loop_ub = binMapping->size[0];
    for (i = 0; i < loop_ub; i++) {
      binMapping->data[i]++;
    }

    loop_ub = binMapping->size[0];
    for (i = 0; i < loop_ub; i++) {
      mapping->data[binMapping->data[i] - 1] = varargin_1->data[i];
    }

    emxFree_int32_T(&binMapping);
    emxFree_real_T(&varargin_1);
    if (iIdx_size_idx_0 == 1) {
      i = x->size[0] * x->size[1];
      x->size[0] = 1;
      x->size[1] = 1;
      emxEnsureCapacity_real_T(x, i);
      for (i = 0; i < 1; i++) {
        x->data[0] = mapping->data[static_cast<int>(iIdx_data[0] + 1.0) - 1];
      }
    } else {
      for (i = 0; i < iIdx_size_idx_0; i++) {
        rIdx_data[i] = mapping->data[static_cast<int>(iIdx_data[i] + 1.0) - 1];
      }

      i = x->size[0] * x->size[1];
      x->size[0] = iIdx_size_idx_0;
      x->size[1] = 1;
      emxEnsureCapacity_real_T(x, i);
      for (i = 0; i < iIdx_size_idx_0; i++) {
        x->data[i] = rIdx_data[i];
      }
    }

    emxFree_real_T(&mapping);
  }
}

//
// File trailer for qamdemod.cpp
//
// [EOF]
//
