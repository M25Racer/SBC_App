//
// File: SystemCore.h
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

#ifndef SYSTEMCORE_H
#define SYSTEMCORE_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_creal_T;

namespace coder {
namespace comm {
class RaisedCosineReceiveFilter;

}
} // namespace coder
struct emxArray_real_T;

// Function Declarations
void binary_expand_op(emxArray_creal_T *z2,
                      coder::comm::RaisedCosineReceiveFilter *b_rxFilter1,
                      const emxArray_real_T *testSignal, int i1,
                      const emxArray_creal_T *r1);

#endif
//
// File trailer for SystemCore.h
//
// [EOF]
//
