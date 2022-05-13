//
// File: RaisedCosineReceiveFilter.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//
#ifndef RAISEDCOSINERECEIVEFILTER_H
#define RAISEDCOSINERECEIVEFILTER_H

// Include Files
#include "FIRDecimator.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
namespace coder
{
  namespace dspcodegen
  {
    class FIRDecimator;
  }
}

struct emxArray_creal_T;

// Type Definitions
namespace coder
{
  namespace comm
  {
    class RaisedCosineReceiveFilter
    {
     public:
      RaisedCosineReceiveFilter *init();
      void step(const emxArray_creal_T *varargin_1, creal_T varargout_1_data[],
                int varargout_1_size[2]);
      void setupAndReset();
      void setup();
      void matlabCodegenDestructor();
      void release();
      void handle_matlabCodegenDestructor();
      void releaseWrapper();
      ~RaisedCosineReceiveFilter();
      RaisedCosineReceiveFilter();
      boolean_T matlabCodegenIsDeleted;
      dspcodegen::FIRDecimator _pobj0;
     protected:
      dspcodegen::FIRDecimator *pFilter;
     private:
      int isInitialized;
      boolean_T isSetupComplete;
    };
  }
}

#endif

//
// File trailer for RaisedCosineReceiveFilter.h
//
// [EOF]
//
