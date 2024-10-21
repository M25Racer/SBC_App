//
// File: RaisedCosineReceiveFilter.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//
#ifndef RAISEDCOSINERECEIVEFILTER_H
#define RAISEDCOSINERECEIVEFILTER_H

// Include Files
#include "FIRDecimator.h"
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>
#include "stdint.h"

// Type Declarations
namespace coder
{
  namespace dspcodegen
  {
    class FIRDecimator;
    class b_FIRDecimator;
  }
}

// Type Definitions
namespace coder
{
  namespace comm
  {
    class RaisedCosineReceiveFilter
    {
     public:
      RaisedCosineReceiveFilter *init();
      void step(const creal_T *varargin_1,int sig_len, creal_T *varargout_1, int z_len);
      void setupAndReset();
      void setup();
      void matlabCodegenDestructor();
      void release();
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

    class b_RaisedCosineReceiveFilter
    {
     public:
      void step(const creal_T varargin_1[2132], creal_T varargout_1[41]);
      void setupAndReset();
      void setup();
      b_RaisedCosineReceiveFilter *init();
      void matlabCodegenDestructor();
      void release();
      void releaseWrapper();
      ~b_RaisedCosineReceiveFilter();
      b_RaisedCosineReceiveFilter();
      boolean_T matlabCodegenIsDeleted;
      dspcodegen::b_FIRDecimator _pobj0;
     protected:
      dspcodegen::b_FIRDecimator *pFilter;
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
