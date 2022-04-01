//
// File: getSquareConstellation.cpp
//
// MATLAB Coder version            : 5.3
// C/C++ source code generated on  : 24-Mar-2022 17:52:15
//

// Include Files
#include "getSquareConstellation.h"
#include "HS_EWL_FREQ_ACQ_emxutil.h"
#include "HS_EWL_FREQ_ACQ_rtwutil.h"
#include "HS_EWL_FREQ_ACQ_types.h"
#include "log2.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : double M
//                emxArray_creal_T *constellation
// Return Type  : void
//
namespace coder {
namespace comm {
namespace internal {
namespace qam {
void getSquareConstellation(double M, emxArray_creal_T *constellation)
{
  static const cint8_T icv[8]{{
                                  -3, // re
                                  1   // im
                              },
                              {
                                  -3, // re
                                  -1  // im
                              },
                              {
                                  -1, // re
                                  1   // im
                              },
                              {
                                  -1, // re
                                  -1  // im
                              },
                              {
                                  1, // re
                                  1  // im
                              },
                              {
                                  1, // re
                                  -1 // im
                              },
                              {
                                  3, // re
                                  1  // im
                              },
                              {
                                  3, // re
                                  -1 // im
                              }};
  emxArray_real_T *a;
  emxArray_real_T *x;
  emxArray_real_T *xPoints;
  emxArray_real_T *y;
  emxArray_real_T *yPoints;
  creal_T dcv[2];
  creal_T *constellation_data;
  double nbits;
  double *a_data;
  double *xPoints_data;
  double *x_data;
  double *yPoints_data;
  int i;
  int k;
  int kd;
  nbits = b_log2(M);
  i = constellation->size[0] * constellation->size[1];
  constellation->size[0] = 1;
  kd = static_cast<int>(M);
  constellation->size[1] = static_cast<int>(M);
  emxEnsureCapacity_creal_T(constellation, i);
  constellation_data = constellation->data;
  if (nbits == 1.0) {
    dcv[0].re = -1.0;
    dcv[0].im = -0.0;
    dcv[1].re = 1.0;
    dcv[1].im = -0.0;
    i = constellation->size[0] * constellation->size[1];
    constellation->size[0] = 1;
    constellation->size[1] = static_cast<int>(M);
    emxEnsureCapacity_creal_T(constellation, i);
    constellation_data = constellation->data;
    for (i = 0; i < kd; i++) {
      constellation_data[i] = dcv[i];
    }
  } else {
    double sqrtM;
    if (std::isnan(nbits) || std::isinf(nbits)) {
      sqrtM = rtNaN;
    } else if (nbits == 0.0) {
      sqrtM = 0.0;
    } else {
      sqrtM = std::fmod(nbits, 2.0);
      if (sqrtM == 0.0) {
        sqrtM = 0.0;
      } else if (nbits < 0.0) {
        sqrtM += 2.0;
      }
    }
    emxInit_real_T(&xPoints, 2);
    xPoints_data = xPoints->data;
    emxInit_real_T(&yPoints, 2);
    yPoints_data = yPoints->data;
    emxInit_real_T(&x, 2);
    emxInit_real_T(&y, 1);
    emxInit_real_T(&a, 1);
    if ((sqrtM != 0.0) && (nbits > 3.0)) {
      double mI;
      double mQ;
      double tmp2;
      double tmp3;
      double tmp4;
      double tmp5;
      mI = rt_powd_snf(2.0, (nbits + 1.0) / 2.0);
      mQ = rt_powd_snf(2.0, (nbits - 1.0) / 2.0);
      nbits = std::trunc((M - 1.0) / mI);
      tmp2 = 3.0 * mI / 4.0;
      tmp3 = mI / 2.0;
      tmp4 = mQ / 2.0;
      tmp5 = 2.0 * mQ;
      i = static_cast<int>((M - 1.0) + 1.0);
      if (0 <= i - 1) {
        if (nbits < 2.147483648E+9) {
          if (nbits >= -2.147483648E+9) {
            k = static_cast<int>(nbits);
          } else {
            k = MIN_int32_T;
          }
        } else if (nbits >= 2.147483648E+9) {
          k = MAX_int32_T;
        } else {
          k = 0;
        }
      }
      for (int ibmat{0}; ibmat < i; ibmat++) {
        double apnd;
        nbits = 2.0 * std::trunc(static_cast<double>(ibmat) / mQ) + (1.0 - mI);
        sqrtM = -(2.0 * static_cast<double>(ibmat & k) + (1.0 - mQ));
        apnd = std::abs(std::floor(nbits));
        if (apnd > tmp2) {
          double cdiff;
          int nm1d2;
          cdiff = std::abs(std::floor(sqrtM));
          if (nbits < 0.0) {
            kd = -1;
          } else {
            kd = (nbits > 0.0);
          }
          if (sqrtM < 0.0) {
            nm1d2 = -1;
          } else {
            nm1d2 = (sqrtM > 0.0);
          }
          if (cdiff > tmp4) {
            nbits = static_cast<double>(kd) * (apnd - tmp3);
            sqrtM = static_cast<double>(nm1d2) * (tmp5 - cdiff);
          } else {
            nbits = static_cast<double>(kd) * (mI - apnd);
            sqrtM = static_cast<double>(nm1d2) * (mQ + cdiff);
          }
        }
        constellation_data[ibmat].re = nbits;
        constellation_data[ibmat].im = sqrtM;
      }
    } else if (nbits == 3.0) {
      i = constellation->size[0] * constellation->size[1];
      constellation->size[0] = 1;
      constellation->size[1] = static_cast<int>(M);
      emxEnsureCapacity_creal_T(constellation, i);
      constellation_data = constellation->data;
      for (i = 0; i < kd; i++) {
        constellation_data[i].re = icv[i].re;
        constellation_data[i].im = icv[i].im;
      }
    } else {
      double apnd;
      double cdiff;
      int ibmat;
      int itilerow;
      int nm1d2;
      sqrtM = rt_powd_snf(2.0, nbits / 2.0);
      if (std::isnan(-(sqrtM - 1.0)) || std::isnan(sqrtM - 1.0)) {
        i = xPoints->size[0] * xPoints->size[1];
        xPoints->size[0] = 1;
        xPoints->size[1] = 1;
        emxEnsureCapacity_real_T(xPoints, i);
        xPoints_data = xPoints->data;
        xPoints_data[0] = rtNaN;
      } else if (sqrtM - 1.0 < -(sqrtM - 1.0)) {
        xPoints->size[0] = 1;
        xPoints->size[1] = 0;
      } else if ((std::isinf(-(sqrtM - 1.0)) || std::isinf(sqrtM - 1.0)) &&
                 (-(sqrtM - 1.0) == sqrtM - 1.0)) {
        i = xPoints->size[0] * xPoints->size[1];
        xPoints->size[0] = 1;
        xPoints->size[1] = 1;
        emxEnsureCapacity_real_T(xPoints, i);
        xPoints_data = xPoints->data;
        xPoints_data[0] = rtNaN;
      } else if (std::floor(-(sqrtM - 1.0)) == -(sqrtM - 1.0)) {
        i = xPoints->size[0] * xPoints->size[1];
        xPoints->size[0] = 1;
        kd = static_cast<int>(
            std::floor(((sqrtM - 1.0) - (-(sqrtM - 1.0))) / 2.0));
        xPoints->size[1] = kd + 1;
        emxEnsureCapacity_real_T(xPoints, i);
        xPoints_data = xPoints->data;
        for (i = 0; i <= kd; i++) {
          xPoints_data[i] = -(sqrtM - 1.0) + 2.0 * static_cast<double>(i);
        }
      } else {
        nbits = std::floor(((sqrtM - 1.0) - (-(sqrtM - 1.0))) / 2.0 + 0.5);
        apnd = -(sqrtM - 1.0) + nbits * 2.0;
        cdiff = apnd - (sqrtM - 1.0);
        if (std::abs(cdiff) <
            4.4408920985006262E-16 *
                std::fmax(std::abs(-(sqrtM - 1.0)), std::abs(sqrtM - 1.0))) {
          nbits++;
          apnd = sqrtM - 1.0;
        } else if (cdiff > 0.0) {
          apnd = -(sqrtM - 1.0) + (nbits - 1.0) * 2.0;
        } else {
          nbits++;
        }
        if (nbits >= 0.0) {
          ibmat = static_cast<int>(nbits);
        } else {
          ibmat = 0;
        }
        i = xPoints->size[0] * xPoints->size[1];
        xPoints->size[0] = 1;
        xPoints->size[1] = ibmat;
        emxEnsureCapacity_real_T(xPoints, i);
        xPoints_data = xPoints->data;
        if (ibmat > 0) {
          xPoints_data[0] = -(sqrtM - 1.0);
          if (ibmat > 1) {
            xPoints_data[ibmat - 1] = apnd;
            nm1d2 = (ibmat - 1) / 2;
            for (k = 0; k <= nm1d2 - 2; k++) {
              kd = (k + 1) << 1;
              xPoints_data[k + 1] = -(sqrtM - 1.0) + static_cast<double>(kd);
              xPoints_data[(ibmat - k) - 2] = apnd - static_cast<double>(kd);
            }
            i = nm1d2 << 1;
            if (i == ibmat - 1) {
              xPoints_data[nm1d2] = (-(sqrtM - 1.0) + apnd) / 2.0;
            } else {
              xPoints_data[nm1d2] = -(sqrtM - 1.0) + static_cast<double>(i);
              xPoints_data[nm1d2 + 1] = apnd - static_cast<double>(i);
            }
          }
        }
      }
      if (std::isnan(sqrtM - 1.0) || std::isnan(-(sqrtM - 1.0))) {
        i = yPoints->size[0] * yPoints->size[1];
        yPoints->size[0] = 1;
        yPoints->size[1] = 1;
        emxEnsureCapacity_real_T(yPoints, i);
        yPoints_data = yPoints->data;
        yPoints_data[0] = rtNaN;
      } else if (sqrtM - 1.0 < -(sqrtM - 1.0)) {
        yPoints->size[0] = 1;
        yPoints->size[1] = 0;
      } else if ((std::isinf(sqrtM - 1.0) || std::isinf(-(sqrtM - 1.0))) &&
                 (sqrtM - 1.0 == -(sqrtM - 1.0))) {
        i = yPoints->size[0] * yPoints->size[1];
        yPoints->size[0] = 1;
        yPoints->size[1] = 1;
        emxEnsureCapacity_real_T(yPoints, i);
        yPoints_data = yPoints->data;
        yPoints_data[0] = rtNaN;
      } else if (std::floor(sqrtM - 1.0) == sqrtM - 1.0) {
        i = yPoints->size[0] * yPoints->size[1];
        yPoints->size[0] = 1;
        kd = static_cast<int>(
            std::floor((-(sqrtM - 1.0) - (sqrtM - 1.0)) / -2.0));
        yPoints->size[1] = kd + 1;
        emxEnsureCapacity_real_T(yPoints, i);
        yPoints_data = yPoints->data;
        for (i = 0; i <= kd; i++) {
          yPoints_data[i] = (sqrtM - 1.0) + -2.0 * static_cast<double>(i);
        }
      } else {
        nbits = std::floor((-(sqrtM - 1.0) - (sqrtM - 1.0)) / -2.0 + 0.5);
        apnd = (sqrtM - 1.0) + nbits * -2.0;
        cdiff = -(sqrtM - 1.0) - apnd;
        if (std::abs(cdiff) <
            4.4408920985006262E-16 *
                std::fmax(std::abs(sqrtM - 1.0), std::abs(-(sqrtM - 1.0)))) {
          nbits++;
          apnd = -(sqrtM - 1.0);
        } else if (cdiff > 0.0) {
          apnd = (sqrtM - 1.0) + (nbits - 1.0) * -2.0;
        } else {
          nbits++;
        }
        if (nbits >= 0.0) {
          ibmat = static_cast<int>(nbits);
        } else {
          ibmat = 0;
        }
        i = yPoints->size[0] * yPoints->size[1];
        yPoints->size[0] = 1;
        yPoints->size[1] = ibmat;
        emxEnsureCapacity_real_T(yPoints, i);
        yPoints_data = yPoints->data;
        if (ibmat > 0) {
          yPoints_data[0] = sqrtM - 1.0;
          if (ibmat > 1) {
            yPoints_data[ibmat - 1] = apnd;
            nm1d2 = (ibmat - 1) / 2;
            for (k = 0; k <= nm1d2 - 2; k++) {
              kd = (k + 1) * -2;
              yPoints_data[k + 1] = (sqrtM - 1.0) + static_cast<double>(kd);
              yPoints_data[(ibmat - k) - 2] = apnd - static_cast<double>(kd);
            }
            if (nm1d2 << 1 == ibmat - 1) {
              yPoints_data[nm1d2] = ((sqrtM - 1.0) + apnd) / 2.0;
            } else {
              kd = nm1d2 * -2;
              yPoints_data[nm1d2] = (sqrtM - 1.0) + static_cast<double>(kd);
              yPoints_data[nm1d2 + 1] = apnd - static_cast<double>(kd);
            }
          }
        }
      }
      i = static_cast<int>(sqrtM);
      k = x->size[0] * x->size[1];
      x->size[0] = static_cast<int>(sqrtM);
      x->size[1] = xPoints->size[1];
      emxEnsureCapacity_real_T(x, k);
      x_data = x->data;
      kd = xPoints->size[1];
      for (nm1d2 = 0; nm1d2 < kd; nm1d2++) {
        ibmat = nm1d2 * static_cast<int>(sqrtM);
        for (itilerow = 0; itilerow < i; itilerow++) {
          x_data[ibmat + itilerow] = xPoints_data[nm1d2];
        }
      }
      k = a->size[0];
      a->size[0] = yPoints->size[1];
      emxEnsureCapacity_real_T(a, k);
      a_data = a->data;
      kd = yPoints->size[1];
      for (k = 0; k < kd; k++) {
        a_data[k] = yPoints_data[k];
      }
      k = y->size[0];
      y->size[0] = a->size[0] * static_cast<int>(sqrtM);
      emxEnsureCapacity_real_T(y, k);
      xPoints_data = y->data;
      kd = a->size[0];
      for (itilerow = 0; itilerow < i; itilerow++) {
        nm1d2 = itilerow * kd;
        for (k = 0; k < kd; k++) {
          xPoints_data[nm1d2 + k] = a_data[k];
        }
      }
      kd = x->size[0] * x->size[1];
      i = constellation->size[0] * constellation->size[1];
      constellation->size[0] = 1;
      constellation->size[1] = kd;
      emxEnsureCapacity_creal_T(constellation, i);
      constellation_data = constellation->data;
      for (i = 0; i < kd; i++) {
        constellation_data[i].re = x_data[i];
        constellation_data[i].im = xPoints_data[i];
      }
    }
    emxFree_real_T(&a);
    emxFree_real_T(&y);
    emxFree_real_T(&x);
    emxFree_real_T(&yPoints);
    emxFree_real_T(&xPoints);
    i = constellation->size[0] * constellation->size[1];
    constellation->size[0] = 1;
    emxEnsureCapacity_creal_T(constellation, i);
    constellation_data = constellation->data;
    kd = constellation->size[1] - 1;
    for (i = 0; i <= kd; i++) {
      sqrtM = constellation_data[i].re;
      nbits = constellation_data[i].im;
      constellation_data[i].re = sqrtM - nbits * 0.0;
      constellation_data[i].im = sqrtM * 0.0 + nbits;
    }
  }
}

} // namespace qam
} // namespace internal
} // namespace comm
} // namespace coder

//
// File trailer for getSquareConstellation.cpp
//
// [EOF]
//
