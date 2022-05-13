//
// File: diff.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "diff.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const emxArray_real_T *x
//                emxArray_real_T *y
// Return Type  : void
//
namespace coder
{
  void diff(const emxArray_real_T *x, emxArray_real_T *y)
  {
    double tmp1;
    int dimSize;
    dimSize = x->size[1];
    if (x->size[1] == 0) {
      y->size[0] = 1;
      y->size[1] = 0;
    } else {
      int ixLead;
      ixLead = x->size[1] - 1;
      if (ixLead >= 1) {
        ixLead = 1;
      }

      if (ixLead < 1) {
        y->size[0] = 1;
        y->size[1] = 0;
      } else {
        ixLead = y->size[0] * y->size[1];
        y->size[0] = 1;
        y->size[1] = x->size[1] - 1;
        emxEnsureCapacity_real_T(y, ixLead);
        if (x->size[1] - 1 != 0) {
          double work_data_idx_0;
          int iyLead;
          ixLead = 1;
          iyLead = 0;
          work_data_idx_0 = x->data[0];
          for (int m = 2; m <= dimSize; m++) {
            double d;
            tmp1 = x->data[ixLead];
            d = tmp1;
            tmp1 -= work_data_idx_0;
            work_data_idx_0 = d;
            ixLead++;
            y->data[iyLead] = tmp1;
            iyLead++;
          }
        }
      }
    }
  }
}

//
// File trailer for diff.cpp
//
// [EOF]
//
