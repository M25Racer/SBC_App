//
// File: find.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 14-Apr-2022 16:12:53
//

// Include Files
#include "find.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : const boolean_T x[1001]
//                int i_data[]
//                int i_size[1]
// Return Type  : void
//
namespace coder
{
  void eml_find(const boolean_T x[1001], int i_data[], int i_size[1])
  {
    int idx;
    int ii;
    boolean_T exitg1;
    idx = 0;
    i_size[0] = 1;
    ii = 0;
    exitg1 = false;
    while ((!exitg1) && (ii < 1001)) {
      if (x[ii]) {
        idx = 1;
        i_data[0] = ii + 1;
        exitg1 = true;
      } else {
        ii++;
      }
    }

    if (idx == 0) {
      i_size[0] = 0;
    }
  }
}

//
// File trailer for find.cpp
//
// [EOF]
//
