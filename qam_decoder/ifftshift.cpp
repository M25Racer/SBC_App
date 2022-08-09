//
// File: ifftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "ifftshift.h"
#include "eml_fftshift.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : creal_T x[57820]
// Return Type  : void
//
namespace coder
{
  void ifftshift(creal_T x[57820])
  {
    eml_fftshift(x, 2);
  }
}

//
// File trailer for ifftshift.cpp
//
// [EOF]
//
