//
// File: rat.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 25-Apr-2022 13:09:26
//

// Include Files
#include "rat.h"
#include "HS_EWL_RECEIVE_rtwutil.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <math.h>

// Function Definitions
//
// Arguments    : double x
//                double *N
//                double *D
// Return Type  : void
//
namespace coder
{
  void rat(double x, double *N, double *D)
  {
    double absx;
    double b_x;
    double tol;
    int exponent;
    boolean_T b;
    boolean_T b1;
    tol = 0.0;
    b = rtIsInf(x);
    b1 = rtIsNaN(x);
    if ((!b) && (!b1)) {
      tol = std::abs(x);
    }

    tol *= 1.0E-6;
    b_x = x;
    absx = std::abs(x);
    if ((!rtIsInf(absx)) && (!rtIsNaN(absx))) {
      if (absx <= 2.2250738585072014E-308) {
        absx = 4.94065645841247E-324;
      } else {
        frexp(absx, &exponent);
        absx = std::ldexp(1.0, exponent - 53);
      }
    } else {
      absx = rtNaN;
    }

    if ((tol > absx) || rtIsNaN(absx)) {
      absx = tol;
    }

    if (b || b1) {
      if (!rtIsNaN(x)) {
        if (x < 0.0) {
          *N = -1.0;
        } else {
          *N = 1.0;
        }
      } else {
        *N = 0.0;
      }

      *D = 0.0;
    } else {
      double d;
      double dlast;
      double nlast;
      tol = 1.0;
      d = 0.0;
      nlast = 0.0;
      dlast = 1.0;
      double rndx;
      int exitg1;
      do {
        exitg1 = 0;
        rndx = rt_roundd_snf(b_x);
        if (!rtIsInf(b_x)) {
          double tmp;
          b_x -= rndx;
          tmp = nlast;
          nlast = tol;
          tol = tol * rndx + tmp;
          tmp = dlast;
          dlast = d;
          d = d * rndx + tmp;
        } else {
          nlast = tol;
          dlast = d;
          tol = b_x;
          d = 0.0;
        }

        if ((b_x == 0.0) || (std::abs(tol / d - x) <= absx)) {
          exitg1 = 1;
        } else {
          b_x = 1.0 / b_x;
        }
      } while (exitg1 == 0);

      b_x = d;
      if (d < 0.0) {
        b_x = -1.0;
      } else if (d > 0.0) {
        b_x = 1.0;
      } else {
        if (d == 0.0) {
          b_x = 0.0;
        }
      }

      *N = tol / b_x;
      *D = std::abs(d);
    }
  }
}

//
// File trailer for rat.cpp
//
// [EOF]
//
