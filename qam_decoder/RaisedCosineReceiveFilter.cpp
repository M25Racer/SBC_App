//
// File: RaisedCosineReceiveFilter.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "RaisedCosineReceiveFilter.h"
#include "FIRDecimator.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
namespace coder
{
  namespace comm
  {
    void RaisedCosineReceiveFilter::release()
    {
      if (this->isInitialized == 1) {
        this->isInitialized = 2;
        this->releaseWrapper();
      }
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::releaseWrapper()
    {
      if (this->isSetupComplete) {
        this->pFilter->release();
      }
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void b_RaisedCosineReceiveFilter::releaseWrapper()
    {
      if (this->isSetupComplete) {
        this->pFilter->release();
      }
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::setup()
    {
      dspcodegen::FIRDecimator *filt;
      this->isSetupComplete = false;
      this->isInitialized = 1;
      filt = this->_pobj0.init();
      this->pFilter = filt;
      this->isSetupComplete = true;
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void b_RaisedCosineReceiveFilter::setup()
    {
      dspcodegen::b_FIRDecimator *filt;
      this->isSetupComplete = false;
      this->isInitialized = 1;
      filt = this->_pobj0.init();
      this->pFilter = filt;
      this->isSetupComplete = true;
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::setupAndReset()
    {
      this->setup();
      this->pFilter->reset();
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void b_RaisedCosineReceiveFilter::setupAndReset()
    {
      this->setup();
      this->pFilter->reset();
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    RaisedCosineReceiveFilter::RaisedCosineReceiveFilter()
    {
      this->matlabCodegenIsDeleted = true;
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    b_RaisedCosineReceiveFilter::b_RaisedCosineReceiveFilter()
    {
      this->matlabCodegenIsDeleted = true;
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    RaisedCosineReceiveFilter::~RaisedCosineReceiveFilter()
    {
      this->matlabCodegenDestructor();
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    b_RaisedCosineReceiveFilter::~b_RaisedCosineReceiveFilter()
    {
      this->matlabCodegenDestructor();
    }

    //
    // Arguments    : void
    // Return Type  : RaisedCosineReceiveFilter *
    //
    RaisedCosineReceiveFilter *RaisedCosineReceiveFilter::init()
    {
      RaisedCosineReceiveFilter *obj;
      obj = this;
      obj->isInitialized = 0;
      obj->matlabCodegenIsDeleted = false;
      return obj;
    }

    //
    // Arguments    : void
    // Return Type  : b_RaisedCosineReceiveFilter *
    //
    b_RaisedCosineReceiveFilter *b_RaisedCosineReceiveFilter::init()
    {
      b_RaisedCosineReceiveFilter *obj;
      obj = this;
      obj->isInitialized = 0;
      obj->matlabCodegenIsDeleted = false;
      return obj;
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::matlabCodegenDestructor()
    {
      if (!this->matlabCodegenIsDeleted) {
        this->matlabCodegenIsDeleted = true;
        this->release();
      }
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void b_RaisedCosineReceiveFilter::matlabCodegenDestructor()
    {
      if (!this->matlabCodegenIsDeleted) {
        this->matlabCodegenIsDeleted = true;
        this->release();
      }
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void b_RaisedCosineReceiveFilter::release()
    {
      if (this->isInitialized == 1) {
        this->isInitialized = 2;
        this->releaseWrapper();
      }
    }

    //
    // Arguments    : const creal_T varargin_1[14040]
    //                creal_T varargout_1[270]
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::step(const creal_T *varargin_1,int sig_len, creal_T *varargout_1, int z_len)//270
    {
      if (this->isInitialized != 1) {
        this->setupAndReset();
      }

      this->pFilter->step(varargin_1, sig_len, varargout_1, z_len);
    }

    //
    // Arguments    : const creal_T varargin_1[2132]
    //                creal_T varargout_1[41]
    // Return Type  : void
    //
    void b_RaisedCosineReceiveFilter::step(const creal_T varargin_1[2132],
      creal_T varargout_1[41])
    {
      if (this->isInitialized != 1) {
        this->setupAndReset();
      }

      this->pFilter->step(varargin_1, varargout_1);
    }
  }
}

//
// File trailer for RaisedCosineReceiveFilter.cpp
//
// [EOF]
//
