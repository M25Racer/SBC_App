//
// File: HS_EWL_DEMOD_QAM_data.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "HS_EWL_DEMOD_QAM_data.h"
#include "rt_nonfinite.h"

// Variable Definitions
omp_nest_lock_t emlrtNestLockGlobal;
const double dv[52] = { 1.0, 0.9927089, 0.9709418, 0.9350162, 0.885456,
  0.8229839, 0.7485107, 0.6631227, 0.5680647, 0.4647232, 0.3546049, 0.2393157,
  0.1205367, 6.123234E-17, -0.1205367, -0.2393157, -0.3546049, -0.4647232,
  -0.5680647, -0.6631227, -0.7485107, -0.8229839, -0.885456, -0.9350162,
  -0.9709418, -0.9927089, -1.0, -0.9927089, -0.9709418, -0.9350162, -0.885456,
  -0.8229839, -0.7485107, -0.6631227, -0.5680647, -0.4647232, -0.3546049,
  -0.2393157, -0.1205367, -1.83697E-16, 0.1205367, 0.2393157, 0.3546049,
  0.4647232, 0.5680647, 0.6631227, 0.7485107, 0.8229839, 0.885456, 0.9350162,
  0.9709418, 0.9927089 };

const double dv1[52] = { 0.0, 0.1205367, 0.2393157, 0.3546049, 0.4647232,
  0.5680647, 0.6631227, 0.7485107, 0.8229839, 0.885456, 0.9350162, 0.9709418,
  0.9927089, 1.0, 0.9927089, 0.9709418, 0.9350162, 0.885456, 0.8229839,
  0.7485107, 0.6631227, 0.5680647, 0.4647232, 0.3546049, 0.2393157, 0.1205367,
  1.224647E-16, -0.1205367, -0.2393157, -0.3546049, -0.4647232, -0.5680647,
  -0.6631227, -0.7485107, -0.8229839, -0.885456, -0.9350162, -0.9709418,
  -0.9927089, -1.0, -0.9927089, -0.9709418, -0.9350162, -0.885456, -0.8229839,
  -0.7485107, -0.6631227, -0.5680647, -0.4647232, -0.3546049, -0.2393157,
  -0.1205367 };

boolean_T isInitialized_HS_EWL_DEMOD_QAM = false;

//
// File trailer for HS_EWL_DEMOD_QAM_data.cpp
//
// [EOF]
//
