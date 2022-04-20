//
// File: FIRDecimator.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 19-Apr-2022 11:19:13
//
#ifndef FIRDECIMATOR_H
#define FIRDECIMATOR_H

// Include Files
#include "rtwtypes.h"
#include <cstddef>
#include <cstdlib>

// Type Declarations
struct emxArray_creal_T;

// Type Definitions
struct emxArray_creal_T_255x1
{
  creal_T data[255];
  int size[2];
};

struct dsp_FIRDecimator_0
{
  int S0_isInitialized;
  int W0_PhaseIdx;
  creal_T W1_Sums;
  int W2_CoeffIdx;
  creal_T W3_StatesBuff[520];
  int W4_TapDelayIndex;
  int W5_PrevNumChan;
  creal_T P0_IC;
  double P1_FILT[572];
  emxArray_creal_T_255x1 O0_Y0;
};

namespace coder
{
  namespace dspcodegen
  {
    class FIRDecimator
    {
     public:
      FIRDecimator *init();
      void reset();
      void step(const emxArray_creal_T *varargin_1, creal_T varargout_1_data[],
                int varargout_1_size[2]);
      void setupAndReset();
      void setup();
      void matlabCodegenDestructor();
      void release();
      void handle_matlabCodegenDestructor();
      ~FIRDecimator();
      FIRDecimator();
      boolean_T matlabCodegenIsDeleted;
      dsp_FIRDecimator_0 cSFunObject;
     private:
      int isInitialized;
      boolean_T isSetupComplete;
    };
  }
}

#endif

//
// File trailer for FIRDecimator.h
//
// [EOF]
//
