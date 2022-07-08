//
// File: HS_EWL_DEMOD_QAM_terminate.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
//

// Include Files
#include "HS_EWL_DEMOD_QAM_terminate.h"
#include "HS_EWL_DEMOD_QAM.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "rt_nonfinite.h"
#include "omp.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_terminate()
{
  HS_EWL_FREQ_ACQ_free();
  HS_EWL_DEMOD_QAM_free();
  omp_destroy_nest_lock(&emlrtNestLockGlobal);
  isInitialized_HS_EWL_DEMOD_QAM = false;
}

//
// File trailer for HS_EWL_DEMOD_QAM_terminate.cpp
//
// [EOF]
//
