//
// File: setup.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "setup.h"
#include "FIRDecimator.h"
#include "rt_nonfinite.h"
#include <cstring>

// Function Definitions
//
// Arguments    : dsp_FIRDecimator_0 *obj
//                const creal_T U0[14040]
//                creal_T Y0[270]
// Return Type  : void
//
namespace coder
{
  void Outputs(dsp_FIRDecimator_0 *obj, const creal_T U0[14040], creal_T Y0[270])
  {
    int cffIdx;
    int curTapIdx;
    int inputIdx;
    int maxWindow;
    int outBufIdx;
    int phaseIdx;

    // System object Outputs function: dsp.FIRDecimator
    cffIdx = obj->W2_CoeffIdx;
    phaseIdx = obj->W0_PhaseIdx;
    inputIdx = 0;
    curTapIdx = obj->W4_TapDelayIndex;
    outBufIdx = 0;
    maxWindow = (phaseIdx + 1) * 10;
    for (int iIdx = 0; iIdx < 14040; iIdx++) {
      int jIdx;
      obj->W1_Sums.re += U0[inputIdx].re * obj->P1_FILT[cffIdx];
      obj->W1_Sums.im += U0[inputIdx].im * obj->P1_FILT[cffIdx];
      cffIdx++;
      for (jIdx = curTapIdx + 1; jIdx < maxWindow; jIdx++) {
        obj->W1_Sums.re += obj->W3_StatesBuff[jIdx].re * obj->P1_FILT[cffIdx];
        obj->W1_Sums.im += obj->W3_StatesBuff[jIdx].im * obj->P1_FILT[cffIdx];
        cffIdx++;
      }

      for (jIdx = maxWindow - 10; jIdx <= curTapIdx; jIdx++) {
        obj->W1_Sums.re += obj->W3_StatesBuff[jIdx].re * obj->P1_FILT[cffIdx];
        obj->W1_Sums.im += obj->W3_StatesBuff[jIdx].im * obj->P1_FILT[cffIdx];
        cffIdx++;
      }

      obj->W3_StatesBuff[curTapIdx] = U0[inputIdx];
      inputIdx++;
      curTapIdx += 10;
      if (curTapIdx >= 520) {
        curTapIdx -= 520;
      }

      phaseIdx++;
      if (phaseIdx < 52) {
        maxWindow += 10;
      } else {
        obj->O0_Y0[outBufIdx] = obj->W1_Sums;
        outBufIdx++;
        obj->W1_Sums.re = 0.0;
        obj->W1_Sums.im = 0.0;
        phaseIdx = 0;
        cffIdx = 0;
        curTapIdx--;
        if (curTapIdx < 0) {
          curTapIdx += 10;
        }

        maxWindow = 10;
      }
    }

    obj->W4_TapDelayIndex = curTapIdx;
    obj->W2_CoeffIdx = cffIdx;
    obj->W0_PhaseIdx = phaseIdx;
    std::memcpy(&Y0[0], &obj->O0_Y0[0], 270U * sizeof(creal_T));
  }

  //
  // Arguments    : dsp_FIRDecimator_1 *obj
  //                const creal_T U0[2132]
  //                creal_T Y0[41]
  // Return Type  : void
  //
  void Outputs(dsp_FIRDecimator_1 *obj, const creal_T U0[2132], creal_T Y0[41])
  {
    int cffIdx;
    int curTapIdx;
    int inputIdx;
    int maxWindow;
    int outBufIdx;
    int phaseIdx;

    // System object Outputs function: dsp.FIRDecimator
    cffIdx = obj->W2_CoeffIdx;
    phaseIdx = obj->W0_PhaseIdx;
    inputIdx = 0;
    curTapIdx = obj->W4_TapDelayIndex;
    outBufIdx = 0;
    maxWindow = (phaseIdx + 1) * 10;
    for (int iIdx = 0; iIdx < 2132; iIdx++) {
      int jIdx;
      obj->W1_Sums.re += U0[inputIdx].re * obj->P1_FILT[cffIdx];
      obj->W1_Sums.im += U0[inputIdx].im * obj->P1_FILT[cffIdx];
      cffIdx++;
      for (jIdx = curTapIdx + 1; jIdx < maxWindow; jIdx++) {
        obj->W1_Sums.re += obj->W3_StatesBuff[jIdx].re * obj->P1_FILT[cffIdx];
        obj->W1_Sums.im += obj->W3_StatesBuff[jIdx].im * obj->P1_FILT[cffIdx];
        cffIdx++;
      }

      for (jIdx = maxWindow - 10; jIdx <= curTapIdx; jIdx++) {
        obj->W1_Sums.re += obj->W3_StatesBuff[jIdx].re * obj->P1_FILT[cffIdx];
        obj->W1_Sums.im += obj->W3_StatesBuff[jIdx].im * obj->P1_FILT[cffIdx];
        cffIdx++;
      }

      obj->W3_StatesBuff[curTapIdx] = U0[inputIdx];
      inputIdx++;
      curTapIdx += 10;
      if (curTapIdx >= 520) {
        curTapIdx -= 520;
      }

      phaseIdx++;
      if (phaseIdx < 52) {
        maxWindow += 10;
      } else {
        obj->O0_Y0[outBufIdx] = obj->W1_Sums;
        outBufIdx++;
        obj->W1_Sums.re = 0.0;
        obj->W1_Sums.im = 0.0;
        phaseIdx = 0;
        cffIdx = 0;
        curTapIdx--;
        if (curTapIdx < 0) {
          curTapIdx += 10;
        }

        maxWindow = 10;
      }
    }

    obj->W4_TapDelayIndex = curTapIdx;
    obj->W2_CoeffIdx = cffIdx;
    obj->W0_PhaseIdx = phaseIdx;
    std::memcpy(&Y0[0], &obj->O0_Y0[0], 41U * sizeof(creal_T));
  }
}

//
// File trailer for setup.cpp
//
// [EOF]
//
