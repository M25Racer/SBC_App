//
// File: colon.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "colon.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : double d
//                emxArray_real_T *y
// Return Type  : void
//
namespace coder
{
  void eml_float_colon(double d, emxArray_real_T *y)
  {
    double apnd;
    double ndbl;
    int nm1d2;
    ndbl = std::floor(279999.0 / d + 0.5);
    apnd = ndbl * d + -140000.0;
    if (std::abs(apnd - 139999.0) < 6.2172489379008766E-11) {
      ndbl++;
      apnd = 139999.0;
    } else if (apnd - 139999.0 > 0.0) {
      apnd = (ndbl - 1.0) * d + -140000.0;
    } else {
      ndbl++;
    }

    nm1d2 = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = static_cast<int>(ndbl);
    emxEnsureCapacity_real_T(y, nm1d2);
    if (static_cast<int>(ndbl) > 0) {
      y->data[0] = -140000.0;
      if (static_cast<int>(ndbl) > 1) {
        double kd;
        y->data[static_cast<int>(ndbl) - 1] = apnd;
        nm1d2 = (static_cast<int>(ndbl) - 1) / 2;
        for (int k = 0; k <= nm1d2 - 2; k++) {
          kd = (static_cast<double>(k) + 1.0) * d;
          y->data[k + 1] = kd + -140000.0;
          y->data[(static_cast<int>(ndbl) - k) - 2] = apnd - kd;
        }

        if (nm1d2 << 1 == static_cast<int>(ndbl) - 1) {
          y->data[nm1d2] = (apnd + -140000.0) / 2.0;
        } else {
          kd = static_cast<double>(nm1d2) * d;
          y->data[nm1d2] = kd + -140000.0;
          y->data[nm1d2 + 1] = apnd - kd;
        }
      }
    }
  }
}

//
// File trailer for colon.cpp
//
// [EOF]
//
