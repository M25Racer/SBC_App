//
// File: HS_EWL_DEMOD_QAM.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
//

// Include Files
#include "HS_EWL_DEMOD_QAM.h"
#include "FIRDecimator.h"
#include "HS_EWL_RECEIVE_data.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "RaisedCosineReceiveFilter.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_free()
{
  b_rxFilter1.matlabCodegenDestructor();
  b_rxFilter1._pobj0.matlabCodegenDestructor();
}

//
// Arguments    : void
// Return Type  : void
//
void HS_EWL_DEMOD_QAM_init()
{
  emxInitStruct_RaisedCosineReceiveFilter(&b_rxFilter1);
  b_rxFilter1_not_empty = false;
  b_rxFilter1._pobj0.matlabCodegenIsDeleted = true;
  b_rxFilter1.matlabCodegenIsDeleted = true;
}

//
// File trailer for HS_EWL_DEMOD_QAM.cpp
//
// [EOF]
//
