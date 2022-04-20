//
// File: HS_EWL_RECEIVE_initialize.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 19-Apr-2022 11:19:13
//

// Include Files
#include "HS_EWL_RECEIVE_initialize.h"
#include "HS_EWL_DEMOD_QAM.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "HS_EWL_RECEIVE_data.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_RECEIVE_initialize()
{
  HS_EWL_FREQ_ACQ_init();
  HS_EWL_DEMOD_QAM_init();
  isInitialized_HS_EWL_RECEIVE = true;
}

//
// File trailer for HS_EWL_RECEIVE_initialize.cpp
//
// [EOF]
//
