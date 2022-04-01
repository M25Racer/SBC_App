//
// File: unsafeSxfun.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "unsafeSxfun.h"
#include "HS_EWL_FREQ_ACQ_emxutil.h"
#include "HS_EWL_FREQ_ACQ_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : emxArray_real_T *r1
//                const emxArray_real_T *s2
//                int i1
//                const emxArray_real_T *y2
//                int i2
// Return Type  : void
//
void binary_expand_op(emxArray_real_T *r1, const emxArray_real_T *s2, int i1,
                      const emxArray_real_T *y2, int i2)
{
  emxArray_real_T *b_s2;
  const double *s2_data;
  const double *y2_data;
  double *b_s2_data;
  double *r;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  y2_data = y2->data;
  s2_data = s2->data;
  emxInit_real_T(&b_s2, 1);
  i = b_s2->size[0];
  if (i2 + 1 == 1) {
    b_s2->size[0] = i1 + 1;
  } else {
    b_s2->size[0] = i2 + 1;
  }
  emxEnsureCapacity_real_T(b_s2, i);
  b_s2_data = b_s2->data;
  stride_0_0 = (i1 + 1 != 1);
  stride_1_0 = (i2 + 1 != 1);
  if (i2 + 1 == 1) {
    loop_ub = i1 + 1;
  } else {
    loop_ub = i2 + 1;
  }
  for (i = 0; i < loop_ub; i++) {
    b_s2_data[i] = s2_data[i * stride_0_0] - y2_data[i * stride_1_0];
  }
  i = r1->size[0];
  r1->size[0] = b_s2->size[0];
  emxEnsureCapacity_real_T(r1, i);
  r = r1->data;
  loop_ub = b_s2->size[0];
  for (i = 0; i < loop_ub; i++) {
    double varargin_1;
    varargin_1 = b_s2_data[i];
    r[i] = varargin_1 * varargin_1;
  }
  emxFree_real_T(&b_s2);
}

//
// Arguments    : emxArray_real_T *r1
//                const emxArray_real_T *s2
//                int i1
//                int i2
//                const emxArray_real_T *y2
//                int i3
//                int i4
// Return Type  : void
//
void binary_expand_op(emxArray_real_T *r1, const emxArray_real_T *s2, int i1,
                      int i2, const emxArray_real_T *y2, int i3, int i4)
{
  emxArray_real_T *b_s2;
  const double *s2_data;
  const double *y2_data;
  double *b_s2_data;
  double *r;
  int i;
  int loop_ub;
  int stride_0_0;
  int stride_1_0;
  y2_data = y2->data;
  s2_data = s2->data;
  emxInit_real_T(&b_s2, 1);
  i = b_s2->size[0];
  if ((i4 - i3) + 1 == 1) {
    b_s2->size[0] = (i2 - i1) + 1;
  } else {
    b_s2->size[0] = (i4 - i3) + 1;
  }
  emxEnsureCapacity_real_T(b_s2, i);
  b_s2_data = b_s2->data;
  stride_0_0 = ((i2 - i1) + 1 != 1);
  stride_1_0 = ((i4 - i3) + 1 != 1);
  if ((i4 - i3) + 1 == 1) {
    loop_ub = (i2 - i1) + 1;
  } else {
    loop_ub = (i4 - i3) + 1;
  }
  for (i = 0; i < loop_ub; i++) {
    b_s2_data[i] = s2_data[i1 + i * stride_0_0] - y2_data[i3 + i * stride_1_0];
  }
  i = r1->size[0];
  r1->size[0] = b_s2->size[0];
  emxEnsureCapacity_real_T(r1, i);
  r = r1->data;
  loop_ub = b_s2->size[0];
  for (i = 0; i < loop_ub; i++) {
    double varargin_1;
    varargin_1 = b_s2_data[i];
    r[i] = varargin_1 * varargin_1;
  }
  emxFree_real_T(&b_s2);
}

//
// File trailer for unsafeSxfun.cpp
//
// [EOF]
//
