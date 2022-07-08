//
// File: FIRDecimator.h
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//
#ifndef FIRDECIMATOR_H
#define FIRDECIMATOR_H

// Include Files
#include "rtwtypes.h"
#include "omp.h"
#include <cstddef>
#include <cstdlib>

// Type Definitions
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
  creal_T O0_Y0[270];
};

struct dsp_FIRDecimator_1
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
  creal_T O0_Y0[41];
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
      void step(const creal_T varargin_1[14040], creal_T varargout_1[270]);
      void setupAndReset();
      void setup();
      void matlabCodegenDestructor();
      void release();
      ~FIRDecimator();
      FIRDecimator();
      boolean_T matlabCodegenIsDeleted;
     private:
      int isInitialized;
      boolean_T isSetupComplete;
      dsp_FIRDecimator_0 cSFunObject;
    };

    class b_FIRDecimator
    {
     public:
      b_FIRDecimator *init();
      void reset();
      void step(const creal_T varargin_1[2132], creal_T varargout_1[41]);
      void setupAndReset();
      void setup();
      void matlabCodegenDestructor();
      void release();
      ~b_FIRDecimator();
      b_FIRDecimator();
      boolean_T matlabCodegenIsDeleted;
     private:
      int isInitialized;
      boolean_T isSetupComplete;
      dsp_FIRDecimator_1 cSFunObject;
    };
  }
}

#endif

//
// File trailer for FIRDecimator.h
//
// [EOF]
//
