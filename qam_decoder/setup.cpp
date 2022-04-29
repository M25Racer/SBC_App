//
// File: setup.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//

// Include Files
#include "setup.h"
#include "FIRDecimator.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"
#include <cstring>

// Function Declarations
static int div_nde_s32_floor(int numerator, int denominator);

// Function Definitions
//
// Arguments    : int numerator
//                int denominator
// Return Type  : int
//
static int div_nde_s32_floor(int numerator, int denominator)
{
  int b_numerator;
  if (((numerator < 0) != (denominator < 0)) && (numerator % denominator != 0))
  {
    b_numerator = -1;
  } else {
    b_numerator = 0;
  }

  return numerator / denominator + b_numerator;
}

//
// Arguments    : dsp_FIRDecimator_0 *obj
//                const emxArray_creal_T *U0
//                creal_T Y0_data[]
//                int Y0_size[2]
// Return Type  : void
//
namespace coder
{
  void Outputs(dsp_FIRDecimator_0 *obj, const emxArray_creal_T *U0, creal_T
               Y0_data[], int Y0_size[2])
  {
    int curTapIdx;
    int inputSegment;
    int kIdx;
    int maxWindow;
    int outFrameSize;
    int outputSegment;

    // System object Outputs function: dsp.FIRDecimator
    obj->O0_Y0.size[0] = static_cast<int>(static_cast<double>(U0->size[0]) /
      52.0);
    obj->O0_Y0.size[1] = U0->size[1];
    if (((U0->size[1] != 0) || (U0->size[0] != 0)) && (obj->W5_PrevNumChan == -1))
    {
      obj->W5_PrevNumChan = U0->size[1];
    }

    outFrameSize = div_nde_s32_floor(U0->size[0], 52);
    obj->O0_Y0.size[0] = outFrameSize;
    obj->O0_Y0.size[1] = U0->size[1];
    if (U0->size[0] != 0) {
      int cffIdx;
      int inputIdx;
      int phaseIdx;
      cffIdx = obj->W2_CoeffIdx;
      phaseIdx = obj->W0_PhaseIdx;
      inputIdx = 0;
      outputSegment = 0;
      inputSegment = 0;
      curTapIdx = obj->W4_TapDelayIndex;
      kIdx = 0;
      while (kIdx < U0->size[1]) {
        curTapIdx = obj->W4_TapDelayIndex;
        phaseIdx = obj->W0_PhaseIdx;
        cffIdx = obj->W2_CoeffIdx;
        kIdx = 0;
        maxWindow = inputSegment + (phaseIdx + 1) * 10;
        for (int iIdx = 0; iIdx < U0->size[0]; iIdx++) {
          int jIdx;
          int tapIdx;
          tapIdx = inputSegment + curTapIdx;
          obj->W1_Sums.re += U0->data[inputIdx].re * obj->P1_FILT[cffIdx];
          obj->W1_Sums.im += U0->data[inputIdx].im * obj->P1_FILT[cffIdx];
          cffIdx++;
          for (jIdx = tapIdx + 1; jIdx < maxWindow; jIdx++) {
            obj->W1_Sums.re += obj->W3_StatesBuff[jIdx].re * obj->P1_FILT[cffIdx];
            obj->W1_Sums.im += obj->W3_StatesBuff[jIdx].im * obj->P1_FILT[cffIdx];
            cffIdx++;
          }

          for (jIdx = maxWindow - 10; jIdx <= tapIdx; jIdx++) {
            obj->W1_Sums.re += obj->W3_StatesBuff[jIdx].re * obj->P1_FILT[cffIdx];
            obj->W1_Sums.im += obj->W3_StatesBuff[jIdx].im * obj->P1_FILT[cffIdx];
            cffIdx++;
          }

          obj->W3_StatesBuff[tapIdx] = U0->data[inputIdx];
          inputIdx++;
          curTapIdx += 10;
          if (curTapIdx >= 520) {
            curTapIdx -= 520;
          }

          phaseIdx++;
          if (phaseIdx < 52) {
            maxWindow += 10;
          } else {
            obj->O0_Y0.data[outputSegment + kIdx] = obj->W1_Sums;
            kIdx++;
            obj->W1_Sums.re = 0.0;
            obj->W1_Sums.im = 0.0;
            phaseIdx = 0;
            cffIdx = 0;
            curTapIdx--;
            if (curTapIdx < 0) {
              curTapIdx += 10;
            }

            maxWindow = inputSegment + 10;
          }
        }

        outputSegment += outFrameSize;
        inputSegment += 520;
        kIdx = 1;
      }

      obj->W4_TapDelayIndex = curTapIdx;
      obj->W2_CoeffIdx = cffIdx;
      obj->W0_PhaseIdx = phaseIdx;
    }

    Y0_size[0] = obj->O0_Y0.size[0];
    Y0_size[1] = obj->O0_Y0.size[1];
    kIdx = obj->O0_Y0.size[0] * obj->O0_Y0.size[1];
    if (0 <= kIdx - 1) {
      std::memcpy(&Y0_data[0], &obj->O0_Y0.data[0], kIdx * sizeof(creal_T));
    }
  }
}

//
// File trailer for setup.cpp
//
// [EOF]
//
