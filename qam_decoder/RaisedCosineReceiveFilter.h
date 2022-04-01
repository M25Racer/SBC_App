//
// File: RaisedCosineReceiveFilter.h
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

#ifndef RAISEDCOSINERECEIVEFILTER_H
#define RAISEDCOSINERECEIVEFILTER_H

// Include Files
#include "FIRDecimator.h"
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_creal_T;

// Type Definitions
namespace coder {
namespace comm {
class RaisedCosineReceiveFilter {
public:
  void step(const emxArray_creal_T *varargin_1, emxArray_creal_T *varargout_1);
  RaisedCosineReceiveFilter();
  ~RaisedCosineReceiveFilter();
  boolean_T matlabCodegenIsDeleted;
  int isInitialized;
  boolean_T isSetupComplete;
  dspcodegen::FIRDecimator *pFilter;
  dspcodegen::FIRDecimator _pobj0;
};

} // namespace comm
} // namespace coder

#endif
//
// File trailer for RaisedCosineReceiveFilter.h
//
// [EOF]
//
