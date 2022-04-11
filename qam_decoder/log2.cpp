//
// File: log2.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "log2.h"
#include "rt_nonfinite.h"
#include <cmath>
#include <math.h>

// Function Definitions
//
// Arguments    : double x
// Return Type  : double
//
namespace coder {
double b_log2(double x)
{
  double f;
  int eint;
  if (x == 0.0) {
    f = rtMinusInf;
  } else if (x < 0.0) {
    f = rtNaN;
  } else if ((!std::isinf(x)) && (!std::isnan(x))) {
    double t;
    t = frexp(x, &eint);
    if (t == 0.5) {
      f = static_cast<double>(eint) - 1.0;
    } else if ((eint == 1) && (t < 0.75)) {
      f = std::log(2.0 * t) / 0.69314718055994529;
    } else {
      f = std::log(t) / 0.69314718055994529 + static_cast<double>(eint);
    }
  } else {
    f = x;
  }
  return f;
}

} // namespace coder

//
// File trailer for log2.cpp
//
// [EOF]
//