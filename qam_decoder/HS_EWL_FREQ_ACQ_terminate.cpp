//
// File: HS_EWL_FREQ_ACQ_terminate.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "HS_EWL_FREQ_ACQ_terminate.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "HS_EWL_FREQ_ACQ_data.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_FREQ_ACQ_terminate()
{
  HS_EWL_FREQ_ACQ_free();
  isInitialized_HS_EWL_FREQ_ACQ = false;
}

//
// File trailer for HS_EWL_FREQ_ACQ_terminate.cpp
//
// [EOF]
//
