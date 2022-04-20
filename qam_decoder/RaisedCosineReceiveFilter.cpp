//
// File: RaisedCosineReceiveFilter.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 19-Apr-2022 11:19:13
//

// Include Files
#include "RaisedCosineReceiveFilter.h"
#include "FIRDecimator.h"
#include "HS_EWL_RECEIVE_types.h"
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
    void RaisedCosineReceiveFilter::setupAndReset()
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
    RaisedCosineReceiveFilter::~RaisedCosineReceiveFilter()
    {
      this->matlabCodegenDestructor();
    }

    //
    // Arguments    : void
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::handle_matlabCodegenDestructor()
    {
      if (!this->matlabCodegenIsDeleted) {
        this->matlabCodegenIsDeleted = true;
        this->release();
      }
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
    // Arguments    : const emxArray_creal_T *varargin_1
    //                creal_T varargout_1_data[]
    //                int varargout_1_size[2]
    // Return Type  : void
    //
    void RaisedCosineReceiveFilter::step(const emxArray_creal_T *varargin_1,
      creal_T varargout_1_data[], int varargout_1_size[2])
    {
      if (this->isInitialized != 1) {
        this->setupAndReset();
      }

      this->pFilter->step(varargin_1, varargout_1_data, varargout_1_size);
    }
  }
}

//
// File trailer for RaisedCosineReceiveFilter.cpp
//
// [EOF]
//
