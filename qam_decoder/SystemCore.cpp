//
// File: SystemCore.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "SystemCore.h"
#include "HS_EWL_FREQ_ACQ_emxutil.h"
#include "HS_EWL_FREQ_ACQ_types.h"
#include "RaisedCosineReceiveFilter.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : emxArray_creal_T *z2
//                coder::comm::RaisedCosineReceiveFilter *b_rxFilter1
//                const emxArray_real_T *testSignal
//                int i1
//                const emxArray_creal_T *r1
// Return Type  : void
//
void binary_expand_op(emxArray_creal_T *z2,
                      coder::comm::RaisedCosineReceiveFilter *b_rxFilter1,
                      const emxArray_real_T *testSignal, int i1,
                      const emxArray_creal_T *r1)
{
  emxArray_creal_T *b_testSignal;
  emxArray_creal_T *c_testSignal;
  const creal_T *r;
  creal_T *b_testSignal_data;
  creal_T *c_testSignal_data;
  const double *testSignal_data;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  r = r1->data;
  testSignal_data = testSignal->data;
  emxInit_creal_T(&b_testSignal, 1);
  i = b_testSignal->size[0];
  if (r1->size[0] == 1) {
    b_testSignal->size[0] = i1 + 1;
  } else {
    b_testSignal->size[0] = r1->size[0];
  }
  emxEnsureCapacity_creal_T(b_testSignal, i);
  b_testSignal_data = b_testSignal->data;
  stride_0_0 = (i1 + 1 != 1);
  stride_1_0 = (r1->size[0] != 1);
  if (r1->size[0] == 1) {
    loop_ub = i1 + 1;
  } else {
    loop_ub = r1->size[0];
  }
  for (i = 0; i < loop_ub; i++) {
    int testSignal_re_tmp;
    testSignal_re_tmp = i * stride_1_0;
    b_testSignal_data[i].re =
        2.0 * (testSignal_data[i * stride_0_0] * r[testSignal_re_tmp].re);
    b_testSignal_data[i].im =
        2.0 * (testSignal_data[i * stride_0_0] * r[testSignal_re_tmp].im);
  }
  emxInit_creal_T(&c_testSignal, 1);
  i = c_testSignal->size[0];
  c_testSignal->size[0] = b_testSignal->size[0] + 40;
  emxEnsureCapacity_creal_T(c_testSignal, i);
  c_testSignal_data = c_testSignal->data;
  loop_ub = b_testSignal->size[0];
  for (i = 0; i < loop_ub; i++) {
    c_testSignal_data[i] = b_testSignal_data[i];
  }
  for (i = 0; i < 40; i++) {
    c_testSignal_data[i + b_testSignal->size[0]].re = 0.0;
    c_testSignal_data[i + b_testSignal->size[0]].im = 0.0;
  }
  emxFree_creal_T(&b_testSignal);
  b_rxFilter1->step(c_testSignal, z2);
  emxFree_creal_T(&c_testSignal);
}

//
// File trailer for SystemCore.cpp
//
// [EOF]
//
