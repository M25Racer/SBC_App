//
// File: HS_EWL_RECEIVE_terminate.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
//

// Include Files
#include "HS_EWL_RECEIVE_terminate.h"
#include "HS_EWL_DEMOD_QAM.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "HS_EWL_RECEIVE_data.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_RECEIVE_terminate()
{
  HS_EWL_DEMOD_QAM_free();
  HS_EWL_FREQ_ACQ_free();
  isInitialized_HS_EWL_RECEIVE = false;
}

//
// File trailer for HS_EWL_RECEIVE_terminate.cpp
//
// [EOF]
//
