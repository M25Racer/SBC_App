//
// File: minOrMax.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "minOrMax.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
// Return Type  : double
//
namespace coder
{
  namespace internal
  {
    double b_maximum(const emxArray_real_T *x)
    {
      double ex;
      int n;
      n = x->size[0];
      if (x->size[0] <= 2) {
        if (x->size[0] == 1) {
          ex = x->data[0];
        } else if ((x->data[0] < x->data[1]) || (rtIsNaN(x->data[0]) &&
                    (!rtIsNaN(x->data[1])))) {
          ex = x->data[1];
        } else {
          ex = x->data[0];
        }
      } else {
        int idx;
        int k;
        if (!rtIsNaN(x->data[0])) {
          idx = 1;
        } else {
          boolean_T exitg1;
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= x->size[0])) {
            if (!rtIsNaN(x->data[k - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          ex = x->data[0];
        } else {
          ex = x->data[idx - 1];
          idx++;
          for (k = idx; k <= n; k++) {
            double d;
            d = x->data[k - 1];
            if (ex < d) {
              ex = d;
            }
          }
        }
      }

      return ex;
    }

    //
    // Arguments    : const emxArray_real_T *x
    //                double *ex
    //                int *idx
    // Return Type  : void
    //
    void b_maximum(const emxArray_real_T *x, double *ex, int *idx)
    {
      int n;
      n = x->size[1];
      if (x->size[1] <= 2) {
        if (x->size[1] == 1) {
          *ex = x->data[0];
          *idx = 1;
        } else if ((x->data[0] < x->data[1]) || (rtIsNaN(x->data[0]) &&
                    (!rtIsNaN(x->data[1])))) {
          *ex = x->data[1];
          *idx = 2;
        } else {
          *ex = x->data[0];
          *idx = 1;
        }
      } else {
        int k;
        if (!rtIsNaN(x->data[0])) {
          *idx = 1;
        } else {
          boolean_T exitg1;
          *idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= x->size[1])) {
            if (!rtIsNaN(x->data[k - 1])) {
              *idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (*idx == 0) {
          *ex = x->data[0];
          *idx = 1;
        } else {
          int i;
          *ex = x->data[*idx - 1];
          i = *idx + 1;
          for (k = i; k <= n; k++) {
            double d;
            d = x->data[k - 1];
            if (*ex < d) {
              *ex = d;
              *idx = k;
            }
          }
        }
      }
    }

    //
    // Arguments    : const emxArray_real_T *x
    //                double *ex
    //                int *idx
    // Return Type  : void
    //
    void maximum(const emxArray_real_T *x, double *ex, int *idx)
    {
      int n;
      n = x->size[0];
      if (x->size[0] <= 2) {
        if (x->size[0] == 1) {
          *ex = x->data[0];
          *idx = 1;
        } else if ((x->data[0] < x->data[1]) || (rtIsNaN(x->data[0]) &&
                    (!rtIsNaN(x->data[1])))) {
          *ex = x->data[1];
          *idx = 2;
        } else {
          *ex = x->data[0];
          *idx = 1;
        }
      } else {
        int k;
        if (!rtIsNaN(x->data[0])) {
          *idx = 1;
        } else {
          boolean_T exitg1;
          *idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= x->size[0])) {
            if (!rtIsNaN(x->data[k - 1])) {
              *idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (*idx == 0) {
          *ex = x->data[0];
          *idx = 1;
        } else {
          int i;
          *ex = x->data[*idx - 1];
          i = *idx + 1;
          for (k = i; k <= n; k++) {
            double d;
            d = x->data[k - 1];
            if (*ex < d) {
              *ex = d;
              *idx = k;
            }
          }
        }
      }
    }

    //
    // Arguments    : const emxArray_real_T *x
    // Return Type  : double
    //
    double maximum(const emxArray_real_T *x)
    {
      double ex;
      int n;
      n = x->size[1];
      if (x->size[1] <= 2) {
        if (x->size[1] == 1) {
          ex = x->data[0];
        } else if ((x->data[0] < x->data[1]) || (rtIsNaN(x->data[0]) &&
                    (!rtIsNaN(x->data[1])))) {
          ex = x->data[1];
        } else {
          ex = x->data[0];
        }
      } else {
        int idx;
        int k;
        if (!rtIsNaN(x->data[0])) {
          idx = 1;
        } else {
          boolean_T exitg1;
          idx = 0;
          k = 2;
          exitg1 = false;
          while ((!exitg1) && (k <= x->size[1])) {
            if (!rtIsNaN(x->data[k - 1])) {
              idx = k;
              exitg1 = true;
            } else {
              k++;
            }
          }
        }

        if (idx == 0) {
          ex = x->data[0];
        } else {
          ex = x->data[idx - 1];
          idx++;
          for (k = idx; k <= n; k++) {
            double d;
            d = x->data[k - 1];
            if (ex < d) {
              ex = d;
            }
          }
        }
      }

      return ex;
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
