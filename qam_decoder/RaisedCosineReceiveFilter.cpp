//
// File: RaisedCosineReceiveFilter.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "RaisedCosineReceiveFilter.h"
#include "FIRDecimator.h"
#include "HS_EWL_FREQ_ACQ_emxutil.h"
#include "HS_EWL_FREQ_ACQ_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : RaisedCosineReceiveFilter
//
namespace coder {
namespace comm {
RaisedCosineReceiveFilter::RaisedCosineReceiveFilter()
{
}

//
// Arguments    : void
// Return Type  : void
//
RaisedCosineReceiveFilter::~RaisedCosineReceiveFilter()
{
}

//
// Arguments    : const emxArray_creal_T *varargin_1
//                emxArray_creal_T *varargout_1
// Return Type  : void
//
void RaisedCosineReceiveFilter::step(const emxArray_creal_T *varargin_1,
                                     emxArray_creal_T *varargout_1)
{
  static const double dv[88]{-0.0017298779558532372,
                             -0.0034031309721416028,
                             -0.0075644453237982256,
                             -0.026135719487974832,
                             0.4173729738912712,
                             -0.026099791760883322,
                             -0.0025219967283943308,
                             0.00015905169350232667,
                             0.00073537748812611749,
                             0.00082066555469273948,
                             0.0,
                             -0.0018005961502664354,
                             -0.0027593603338267464,
                             -0.0045752340767565968,
                             -0.00984399472339684,
                             0.35319378977411864,
                             0.0094674285228390151,
                             0.0046029256723631173,
                             0.0028549341306855483,
                             0.0019121671147595033,
                             0.0013049585938980511,
                             0.0,
                             -0.0010131717843874012,
                             -0.00094846920342509975,
                             -0.00022550369501060321,
                             0.0045085326502362529,
                             0.26206670032858376,
                             0.074274010256768441,
                             0.010718508716987759,
                             0.0042281017117658352,
                             0.0020785938308326905,
                             0.0010643740811356712,
                             0.0,
                             0.00014794121589738958,
                             0.00098429811501647875,
                             0.0031831328350833961,
                             0.011609613100788198,
                             0.16292327565821807,
                             0.16292327565821807,
                             0.011609613100788198,
                             0.0031831328350833961,
                             0.00098429811501647875,
                             0.00014794121589738958,
                             0.0,
                             0.0010643740811356712,
                             0.0020785938308326905,
                             0.0042281017117658352,
                             0.010718508716987759,
                             0.074274010256768441,
                             0.26206670032858376,
                             0.0045085326502362529,
                             -0.00022550369501060321,
                             -0.00094846920342509975,
                             -0.0010131717843874012,
                             0.0,
                             0.0013049585938980511,
                             0.0019121671147595033,
                             0.0028549341306855483,
                             0.0046029256723631173,
                             0.0094674285228390151,
                             0.35319378977411864,
                             -0.00984399472339684,
                             -0.0045752340767565968,
                             -0.0027593603338267464,
                             -0.0018005961502664354,
                             0.0,
                             0.00082066555469273948,
                             0.00073537748812611749,
                             0.00015905169350232667,
                             -0.0025219967283943308,
                             -0.026099791760883322,
                             0.4173729738912712,
                             -0.026135719487974832,
                             -0.0075644453237982256,
                             -0.0034031309721416028,
                             -0.0017298779558532372,
                             0.0,
                             -6.9684669136375058E-5,
                             -0.00073636051708893876,
                             -0.002321760285522066,
                             -0.0070976678229411122,
                             -0.035124948036614749,
                             0.44050238276611531,
                             -0.035124948036614749,
                             -0.0070976678229411122,
                             -0.002321760285522066,
                             -0.00073636051708893876,
                             -6.9684669136375058E-5};
  dspcodegen::FIRDecimator *iobj_0_tmp_tmp;
  dsp_FIRDecimator_0 *obj;
  const creal_T *varargin_1_data;
  creal_T *varargout_1_data;
  int curTapIdx;
  int i;
  varargin_1_data = varargin_1->data;
  if (isInitialized != 1) {
    isSetupComplete = false;
    isInitialized = 1;
    iobj_0_tmp_tmp = &_pobj0;
    _pobj0.isInitialized = 0;
    _pobj0.isInitialized = 0;
    // System object Constructor function: dsp.FIRDecimator
    iobj_0_tmp_tmp->cSFunObject.P0_IC.re = 0.0;
    iobj_0_tmp_tmp->cSFunObject.P0_IC.im = 0.0;
    for (i = 0; i < 88; i++) {
      iobj_0_tmp_tmp->cSFunObject.P1_FILT[i] = dv[i];
    }
    _pobj0.matlabCodegenIsDeleted = false;
    pFilter = &_pobj0;
    isSetupComplete = true;
    iobj_0_tmp_tmp = pFilter;
    if (iobj_0_tmp_tmp->isInitialized == 1) {
      // System object Initialization function: dsp.FIRDecimator
      iobj_0_tmp_tmp->cSFunObject.W2_CoeffIdx = 77;
      iobj_0_tmp_tmp->cSFunObject.W0_PhaseIdx = 7;
      iobj_0_tmp_tmp->cSFunObject.W4_TapDelayIndex = 70;
      iobj_0_tmp_tmp->cSFunObject.W1_Sums.re = 0.0;
      iobj_0_tmp_tmp->cSFunObject.W1_Sums.im = 0.0;
      for (i = 0; i < 80; i++) {
        iobj_0_tmp_tmp->cSFunObject.W3_StatesBuff[i].re = 0.0;
        iobj_0_tmp_tmp->cSFunObject.W3_StatesBuff[i].im = 0.0;
      }
      iobj_0_tmp_tmp->cSFunObject.W5_PrevNumChan = -1;
    }
  }
  iobj_0_tmp_tmp = pFilter;
  if (iobj_0_tmp_tmp->isInitialized != 1) {
    iobj_0_tmp_tmp->isSetupComplete = false;
    iobj_0_tmp_tmp->isInitialized = 1;
    iobj_0_tmp_tmp->isSetupComplete = true;
    // System object Initialization function: dsp.FIRDecimator
    iobj_0_tmp_tmp->cSFunObject.W2_CoeffIdx = 77;
    iobj_0_tmp_tmp->cSFunObject.W0_PhaseIdx = 7;
    iobj_0_tmp_tmp->cSFunObject.W4_TapDelayIndex = 70;
    iobj_0_tmp_tmp->cSFunObject.W1_Sums.re = 0.0;
    iobj_0_tmp_tmp->cSFunObject.W1_Sums.im = 0.0;
    for (i = 0; i < 80; i++) {
      iobj_0_tmp_tmp->cSFunObject.W3_StatesBuff[i].re = 0.0;
      iobj_0_tmp_tmp->cSFunObject.W3_StatesBuff[i].im = 0.0;
    }
    iobj_0_tmp_tmp->cSFunObject.W5_PrevNumChan = -1;
  }
  obj = &iobj_0_tmp_tmp->cSFunObject;
  // System object Outputs function: dsp.FIRDecimator
  curTapIdx = iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[0] *
              iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[1];
  iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[0] =
      static_cast<int>(static_cast<double>(varargin_1->size[0]) / 8.0);
  iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[1] = 1;
  emxEnsureCapacity_creal_T(iobj_0_tmp_tmp->cSFunObject.O0_Y0, curTapIdx);
  if (iobj_0_tmp_tmp->cSFunObject.W5_PrevNumChan == -1) {
    iobj_0_tmp_tmp->cSFunObject.W5_PrevNumChan = 1;
  }
  curTapIdx = iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[0] *
              iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[1];
  iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[0] = varargin_1->size[0] >> 3;
  iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[1] = 1;
  emxEnsureCapacity_creal_T(iobj_0_tmp_tmp->cSFunObject.O0_Y0, curTapIdx);
  if (varargin_1->size[0] != 0) {
    int cffIdx;
    int maxWindow;
    int outBufIdx;
    int phaseIdx;
    i = 0;
    curTapIdx = obj->W4_TapDelayIndex;
    phaseIdx = obj->W0_PhaseIdx;
    cffIdx = obj->W2_CoeffIdx;
    outBufIdx = 0;
    maxWindow = (phaseIdx + 1) * 10;
    for (int iIdx{0}; iIdx < varargin_1->size[0]; iIdx++) {
      int jIdx;
      obj->W1_Sums.re += varargin_1_data[i].re * obj->P1_FILT[cffIdx];
      obj->W1_Sums.im += varargin_1_data[i].im * obj->P1_FILT[cffIdx];
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
      obj->W3_StatesBuff[curTapIdx] = varargin_1_data[i];
      i++;
      curTapIdx += 10;
      if (curTapIdx >= 80) {
        curTapIdx -= 80;
      }
      phaseIdx++;
      if (phaseIdx < 8) {
        maxWindow += 10;
      } else {
        obj->O0_Y0->data[outBufIdx] = obj->W1_Sums;
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
    iobj_0_tmp_tmp->cSFunObject.W4_TapDelayIndex = curTapIdx;
    iobj_0_tmp_tmp->cSFunObject.W2_CoeffIdx = cffIdx;
    iobj_0_tmp_tmp->cSFunObject.W0_PhaseIdx = phaseIdx;
  }
  curTapIdx = varargout_1->size[0] * varargout_1->size[1];
  varargout_1->size[0] = iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[0];
  varargout_1->size[1] = iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[1];
  emxEnsureCapacity_creal_T(varargout_1, curTapIdx);
  varargout_1_data = varargout_1->data;
  i = iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[0] *
      iobj_0_tmp_tmp->cSFunObject.O0_Y0->size[1];
  for (curTapIdx = 0; curTapIdx < i; curTapIdx++) {
    varargout_1_data[curTapIdx] =
        iobj_0_tmp_tmp->cSFunObject.O0_Y0->data[curTapIdx];
  }
}

} // namespace comm
} // namespace coder

//
// File trailer for RaisedCosineReceiveFilter.cpp
//
// [EOF]
//
