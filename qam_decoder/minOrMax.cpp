//
// File: minOrMax.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//

// Include Files
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const double x[57820]
// Return Type  : double
//
namespace coder
{
  namespace internal
  {
    double b_maximum(const double x[57820])
    {
      double ex;
      int idx;
      int k;
      if (!rtIsNaN(x[0])) {
        idx = 1;
      } else {
        boolean_T exitg1;
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k < 57821)) {
          if (!rtIsNaN(x[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = x[0];
      } else {
        ex = x[idx - 1];
        idx++;
        for (k = idx; k < 57821; k++) {
          double d;
          d = x[k - 1];
          if (ex < d) {
            ex = d;
          }
        }
      }

      return ex;
    }

    //
    // Arguments    : const double x[14040]
    // Return Type  : double
    //
    double maximum(const double x[14040])
    {
      double ex;
      int idx;
      int k;
      if (!rtIsNaN(x[0])) {
        idx = 1;
      } else {
        boolean_T exitg1;
        idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k < 14041)) {
          if (!rtIsNaN(x[k - 1])) {
            idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (idx == 0) {
        ex = x[0];
      } else {
        ex = x[idx - 1];
        idx++;
        for (k = idx; k < 14041; k++) {
          double d;
          d = x[k - 1];
          if (ex < d) {
            ex = d;
          }
        }
      }

      return ex;
    }

    //
    // Arguments    : const double x[58820]
    //                double *ex
    //                int *idx
    // Return Type  : void
    //
    void maximum(const double x[58820], double *ex, int *idx)
    {
      int k;
      if (!rtIsNaN(x[0])) {
        *idx = 1;
      } else {
        boolean_T exitg1;
        *idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k < 58821)) {
          if (!rtIsNaN(x[k - 1])) {
            *idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (*idx == 0) {
        *ex = x[0];
        *idx = 1;
      } else {
        int i;
        *ex = x[*idx - 1];
        i = *idx + 1;
        for (k = i; k < 58821; k++) {
          double d;
          d = x[k - 1];
          if (*ex < d) {
            *ex = d;
            *idx = k;
          }
        }
      }
    }

    //
    // Arguments    : const double x[50]
    //                double *ex
    //                int *idx
    // Return Type  : void
    //
    void minimum(const double x[50], double *ex, int *idx)
    {
      int k;
      if (!rtIsNaN(x[0])) {
        *idx = 1;
      } else {
        boolean_T exitg1;
        *idx = 0;
        k = 2;
        exitg1 = false;
        while ((!exitg1) && (k < 51)) {
          if (!rtIsNaN(x[k - 1])) {
            *idx = k;
            exitg1 = true;
          } else {
            k++;
          }
        }
      }

      if (*idx == 0) {
        *ex = x[0];
        *idx = 1;
      } else {
        int i;
        *ex = x[*idx - 1];
        i = *idx + 1;
        for (k = i; k < 51; k++) {
          double d;
          d = x[k - 1];
          if (*ex > d) {
            *ex = d;
            *idx = k;
          }
        }
      }
    }
  }
}

//
// File trailer for minOrMax.cpp
//
// [EOF]
//
