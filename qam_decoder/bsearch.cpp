//
// File: bsearch.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//

// Include Files
#include "bsearch.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
//                double xi
// Return Type  : int
//
namespace coder
{
  namespace internal
  {
    int b_bsearch(const emxArray_real_T *x, double xi)
    {
      int high_i;
      int low_ip1;
      int n;
      high_i = x->size[1];
      n = 1;
      low_ip1 = 2;
      while (high_i > low_ip1) {
        int mid_i;
        mid_i = (n >> 1) + (high_i >> 1);
        if (((n & 1) == 1) && ((high_i & 1) == 1)) {
          mid_i++;
        }

        if (xi >= x->data[mid_i - 1]) {
          n = mid_i;
          low_ip1 = mid_i + 1;
        } else {
          high_i = mid_i;
        }
      }

      return n;
    }
  }
}

//
// File trailer for bsearch.cpp
//
// [EOF]
//
