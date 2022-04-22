//
// File: getSquareConstellation.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 21-Apr-2022 14:09:19
//

// Include Files
#include "getSquareConstellation.h"
#include "HS_EWL_RECEIVE_emxutil.h"
#include "HS_EWL_RECEIVE_rtwutil.h"
#include "HS_EWL_RECEIVE_types.h"
#include "log2.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cmath>

// Function Definitions
//
// Arguments    : double M
//                emxArray_creal_T *constellation
// Return Type  : void
//
namespace coder
{
  namespace comm
  {
    namespace internal
    {
      namespace qam
      {
        void getSquareConstellation(double M, emxArray_creal_T *constellation)
        {
          static const cint8_T icv[8] = { { -3,// re
              1                        // im
            }, { -3,                   // re
              -1                       // im
            }, { -1,                   // re
              1                        // im
            }, { -1,                   // re
              -1                       // im
            }, { 1,                    // re
              1                        // im
            }, { 1,                    // re
              -1                       // im
            }, { 3,                    // re
              1                        // im
            }, { 3,                    // re
              -1                       // im
            } };

          emxArray_real_T *a;
          emxArray_real_T *x;
          emxArray_real_T *xPoints;
          emxArray_real_T *y;
          emxArray_real_T *yPoints;
          creal_T dcv[2];
          double nbits;
          double sqrtM;
          int k;
          int kd;
          nbits = b_log2(M);
          k = constellation->size[0] * constellation->size[1];
          constellation->size[0] = 1;
          constellation->size[1] = static_cast<int>(M);
          emxEnsureCapacity_creal_T(constellation, k);
          if (nbits == 1.0) {
            dcv[0].re = -1.0;
            dcv[0].im = -0.0;
            dcv[1].re = 1.0;
            dcv[1].im = -0.0;
            k = constellation->size[0] * constellation->size[1];
            constellation->size[0] = 1;
            constellation->size[1] = static_cast<int>(M);
            emxEnsureCapacity_creal_T(constellation, k);
            kd = static_cast<int>(M);
            for (k = 0; k < kd; k++) {
              constellation->data[k] = dcv[k];
            }
          } else {
            if (rtIsNaN(nbits) || rtIsInf(nbits)) {
              sqrtM = rtNaN;
            } else if (nbits == 0.0) {
              sqrtM = 0.0;
            } else {
              sqrtM = std::fmod(nbits, 2.0);
              if (sqrtM == 0.0) {
                sqrtM = 0.0;
              } else {
                if (nbits < 0.0) {
                  sqrtM += 2.0;
                }
              }
            }

            emxInit_real_T(&xPoints, 2);
            emxInit_real_T(&yPoints, 2);
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
              nbits = (M - 1.0) / mI;
              if (nbits < 0.0) {
                nbits = -0.0;
              } else {
                nbits = std::floor(nbits);
              }

              tmp2 = 3.0 * mI / 4.0;
              tmp3 = mI / 2.0;
              tmp4 = mQ / 2.0;
              tmp5 = 2.0 * mQ;
              k = static_cast<int>((M - 1.0) + 1.0);
              if (0 <= k - 1) {
                if (nbits < 2.147483648E+9) {
                  if (nbits >= -2.147483648E+9) {
                    kd = static_cast<int>(nbits);
                  } else {
                    kd = MIN_int32_T;
                  }
                } else if (nbits >= 2.147483648E+9) {
                  kd = MAX_int32_T;
                } else {
                  kd = 0;
                }
              }

              for (int ntilerows = 0; ntilerows < k; ntilerows++) {
                double apnd;
                nbits = 2.0 * std::floor(static_cast<double>(ntilerows) / mQ) +
                  (1.0 - mI);
                sqrtM = -(2.0 * static_cast<double>(ntilerows & kd) + (1.0 - mQ));
                apnd = std::abs(std::floor(nbits));
                if (apnd > tmp2) {
                  double cdiff;
                  int ibmat;
                  int nm1d2;
                  cdiff = std::abs(std::floor(sqrtM));
                  if (nbits < 0.0) {
                    nm1d2 = -1;
                  } else {
                    nm1d2 = (nbits > 0.0);
                  }

                  if (sqrtM < 0.0) {
                    ibmat = -1;
                  } else {
                    ibmat = (sqrtM > 0.0);
                  }

                  if (cdiff > tmp4) {
                    nbits = static_cast<double>(nm1d2) * (apnd - tmp3);
                    sqrtM = static_cast<double>(ibmat) * (tmp5 - cdiff);
                  } else {
                    nbits = static_cast<double>(nm1d2) * (mI - apnd);
                    sqrtM = static_cast<double>(ibmat) * (mQ + cdiff);
                  }
                }

                constellation->data[ntilerows].re = nbits;
                constellation->data[ntilerows].im = sqrtM;
              }
            } else if (nbits == 3.0) {
              k = constellation->size[0] * constellation->size[1];
              constellation->size[0] = 1;
              constellation->size[1] = static_cast<int>(M);
              emxEnsureCapacity_creal_T(constellation, k);
              kd = static_cast<int>(M);
              for (k = 0; k < kd; k++) {
                constellation->data[k].re = icv[k].re;
                constellation->data[k].im = icv[k].im;
              }
            } else {
              double apnd;
              double cdiff;
              double mQ;
              double tmp2;
              int ibmat;
              int itilerow;
              int nm1d2;
              int ntilerows;
              sqrtM = rt_powd_snf(2.0, nbits / 2.0);
              if (rtIsNaN(-(sqrtM - 1.0)) || rtIsNaN(sqrtM - 1.0)) {
                k = xPoints->size[0] * xPoints->size[1];
                xPoints->size[0] = 1;
                xPoints->size[1] = 1;
                emxEnsureCapacity_real_T(xPoints, k);
                xPoints->data[0] = rtNaN;
              } else if (sqrtM - 1.0 < -(sqrtM - 1.0)) {
                xPoints->size[0] = 1;
                xPoints->size[1] = 0;
              } else if ((rtIsInf(-(sqrtM - 1.0)) || rtIsInf(sqrtM - 1.0)) &&
                         (-(sqrtM - 1.0) == sqrtM - 1.0)) {
                k = xPoints->size[0] * xPoints->size[1];
                xPoints->size[0] = 1;
                xPoints->size[1] = 1;
                emxEnsureCapacity_real_T(xPoints, k);
                xPoints->data[0] = rtNaN;
              } else if (std::floor(-(sqrtM - 1.0)) == -(sqrtM - 1.0)) {
                k = xPoints->size[0] * xPoints->size[1];
                xPoints->size[0] = 1;
                kd = static_cast<int>(std::floor(((sqrtM - 1.0) - (-(sqrtM - 1.0)))
                  / 2.0));
                xPoints->size[1] = kd + 1;
                emxEnsureCapacity_real_T(xPoints, k);
                for (k = 0; k <= kd; k++) {
                  xPoints->data[k] = -(sqrtM - 1.0) + 2.0 * static_cast<double>
                    (k);
                }
              } else {
                nbits = std::floor(((sqrtM - 1.0) - (-(sqrtM - 1.0))) / 2.0 +
                                   0.5);
                apnd = -(sqrtM - 1.0) + nbits * 2.0;
                cdiff = apnd - (sqrtM - 1.0);
                mQ = std::abs(-(sqrtM - 1.0));
                tmp2 = std::abs(sqrtM - 1.0);
                if ((mQ > tmp2) || rtIsNaN(tmp2)) {
                  tmp2 = mQ;
                }

                if (std::abs(cdiff) < 4.4408920985006262E-16 * tmp2) {
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

                k = xPoints->size[0] * xPoints->size[1];
                xPoints->size[0] = 1;
                xPoints->size[1] = ibmat;
                emxEnsureCapacity_real_T(xPoints, k);
                if (ibmat > 0) {
                  xPoints->data[0] = -(sqrtM - 1.0);
                  if (ibmat > 1) {
                    xPoints->data[ibmat - 1] = apnd;
                    nm1d2 = (ibmat - 1) / 2;
                    for (k = 0; k <= nm1d2 - 2; k++) {
                      kd = (k + 1) << 1;
                      xPoints->data[k + 1] = -(sqrtM - 1.0) + static_cast<double>
                        (kd);
                      xPoints->data[(ibmat - k) - 2] = apnd - static_cast<double>
                        (kd);
                    }

                    k = nm1d2 << 1;
                    if (k == ibmat - 1) {
                      xPoints->data[nm1d2] = (-(sqrtM - 1.0) + apnd) / 2.0;
                    } else {
                      xPoints->data[nm1d2] = -(sqrtM - 1.0) + static_cast<double>
                        (k);
                      xPoints->data[nm1d2 + 1] = apnd - static_cast<double>(k);
                    }
                  }
                }
              }

              if (rtIsNaN(sqrtM - 1.0) || rtIsNaN(-(sqrtM - 1.0))) {
                k = yPoints->size[0] * yPoints->size[1];
                yPoints->size[0] = 1;
                yPoints->size[1] = 1;
                emxEnsureCapacity_real_T(yPoints, k);
                yPoints->data[0] = rtNaN;
              } else if (sqrtM - 1.0 < -(sqrtM - 1.0)) {
                yPoints->size[0] = 1;
                yPoints->size[1] = 0;
              } else if ((rtIsInf(sqrtM - 1.0) || rtIsInf(-(sqrtM - 1.0))) &&
                         (sqrtM - 1.0 == -(sqrtM - 1.0))) {
                k = yPoints->size[0] * yPoints->size[1];
                yPoints->size[0] = 1;
                yPoints->size[1] = 1;
                emxEnsureCapacity_real_T(yPoints, k);
                yPoints->data[0] = rtNaN;
              } else if (std::floor(sqrtM - 1.0) == sqrtM - 1.0) {
                k = yPoints->size[0] * yPoints->size[1];
                yPoints->size[0] = 1;
                kd = static_cast<int>(std::floor((-(sqrtM - 1.0) - (sqrtM - 1.0))
                  / -2.0));
                yPoints->size[1] = kd + 1;
                emxEnsureCapacity_real_T(yPoints, k);
                for (k = 0; k <= kd; k++) {
                  yPoints->data[k] = (sqrtM - 1.0) + -2.0 * static_cast<double>
                    (k);
                }
              } else {
                nbits = std::floor((-(sqrtM - 1.0) - (sqrtM - 1.0)) / -2.0 + 0.5);
                apnd = (sqrtM - 1.0) + nbits * -2.0;
                cdiff = -(sqrtM - 1.0) - apnd;
                mQ = std::abs(sqrtM - 1.0);
                tmp2 = std::abs(-(sqrtM - 1.0));
                if ((mQ > tmp2) || rtIsNaN(tmp2)) {
                  tmp2 = mQ;
                }

                if (std::abs(cdiff) < 4.4408920985006262E-16 * tmp2) {
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

                k = yPoints->size[0] * yPoints->size[1];
                yPoints->size[0] = 1;
                yPoints->size[1] = ibmat;
                emxEnsureCapacity_real_T(yPoints, k);
                if (ibmat > 0) {
                  yPoints->data[0] = sqrtM - 1.0;
                  if (ibmat > 1) {
                    yPoints->data[ibmat - 1] = apnd;
                    nm1d2 = (ibmat - 1) / 2;
                    for (k = 0; k <= nm1d2 - 2; k++) {
                      kd = (k + 1) * -2;
                      yPoints->data[k + 1] = (sqrtM - 1.0) + static_cast<double>
                        (kd);
                      yPoints->data[(ibmat - k) - 2] = apnd - static_cast<double>
                        (kd);
                    }

                    if (nm1d2 << 1 == ibmat - 1) {
                      yPoints->data[nm1d2] = ((sqrtM - 1.0) + apnd) / 2.0;
                    } else {
                      kd = nm1d2 * -2;
                      yPoints->data[nm1d2] = (sqrtM - 1.0) + static_cast<double>
                        (kd);
                      yPoints->data[nm1d2 + 1] = apnd - static_cast<double>(kd);
                    }
                  }
                }
              }

              k = x->size[0] * x->size[1];
              x->size[0] = static_cast<int>(sqrtM);
              x->size[1] = xPoints->size[1];
              emxEnsureCapacity_real_T(x, k);
              kd = xPoints->size[1];
              ntilerows = static_cast<int>(sqrtM);
              for (nm1d2 = 0; nm1d2 < kd; nm1d2++) {
                ibmat = nm1d2 * static_cast<int>(sqrtM);
                for (itilerow = 0; itilerow < ntilerows; itilerow++) {
                  x->data[ibmat + itilerow] = xPoints->data[nm1d2];
                }
              }

              k = a->size[0];
              a->size[0] = yPoints->size[1];
              emxEnsureCapacity_real_T(a, k);
              kd = yPoints->size[1];
              for (k = 0; k < kd; k++) {
                a->data[k] = yPoints->data[k];
              }

              k = y->size[0];
              y->size[0] = a->size[0] * static_cast<int>(sqrtM);
              emxEnsureCapacity_real_T(y, k);
              kd = a->size[0];
              ntilerows = static_cast<int>(sqrtM);
              for (itilerow = 0; itilerow < ntilerows; itilerow++) {
                nm1d2 = itilerow * kd;
                for (k = 0; k < kd; k++) {
                  y->data[nm1d2 + k] = a->data[k];
                }
              }

              kd = x->size[0] * x->size[1];
              k = constellation->size[0] * constellation->size[1];
              constellation->size[0] = 1;
              constellation->size[1] = kd;
              emxEnsureCapacity_creal_T(constellation, k);
              for (k = 0; k < kd; k++) {
                constellation->data[k].re = x->data[k];
                constellation->data[k].im = y->data[k];
              }
            }

            emxFree_real_T(&a);
            emxFree_real_T(&y);
            emxFree_real_T(&x);
            emxFree_real_T(&yPoints);
            emxFree_real_T(&xPoints);
            k = constellation->size[0] * constellation->size[1];
            kd = constellation->size[0] * constellation->size[1];
            constellation->size[0] = 1;
            emxEnsureCapacity_creal_T(constellation, kd);
            kd = k - 1;
            for (k = 0; k <= kd; k++) {
              sqrtM = constellation->data[k].re * 0.0 + constellation->data[k].
                im;
              constellation->data[k].re -= constellation->data[k].im * 0.0;
              constellation->data[k].im = sqrtM;
            }
          }
        }
      }
    }
  }
}

//
// File trailer for getSquareConstellation.cpp
//
// [EOF]
//
