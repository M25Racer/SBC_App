//
// File: HS_EWL_DEMOD_QAM_initialize.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "HS_EWL_DEMOD_QAM_initialize.h"
#include "HS_EWL_DEMOD_QAM.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_initialize()
{
  //omp_init_nest_lock(&emlrtNestLockGlobal);
  HS_EWL_DEMOD_QAM_init();
  HS_EWL_FREQ_ACQ_init();
  isInitialized_HS_EWL_DEMOD_QAM = true;
}

//
// File trailer for HS_EWL_DEMOD_QAM_initialize.cpp
//
// [EOF]
//
