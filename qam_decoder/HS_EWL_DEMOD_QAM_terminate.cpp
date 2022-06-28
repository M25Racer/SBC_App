//
// File: HS_EWL_DEMOD_QAM_terminate.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "HS_EWL_DEMOD_QAM_terminate.h"
#include "HS_EWL_DEMOD_QAM.h"
#include "HS_EWL_DEMOD_QAM_data.h"
#include "HS_EWL_FREQ_ACQ.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_terminate(coder::comm::RaisedCosineReceiveFilter *b_rxFilter1, coder::comm::RaisedCosineReceiveFilter *b_rxFilter2)
{
  HS_EWL_FREQ_ACQ_free(b_rxFilter1);
  HS_EWL_DEMOD_QAM_free(b_rxFilter2);
//  omp_destroy_nest_lock(&emlrtNestLockGlobal);
  isInitialized_HS_EWL_DEMOD_QAM = false;
}

//
// File trailer for HS_EWL_DEMOD_QAM_terminate.cpp
//
// [EOF]
//
