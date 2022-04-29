//
// File: interp1.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 29-Apr-2022 10:21:15
//

// Include Files
#include "interp1.h"
#include "HS_EWL_DEMOD_QAM_emxutil.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "bsearch.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <cstring>

// Type Definitions
struct emxArray_real_T_1x1024
{
  double data[1024];
  int size[2];
};

struct emxArray_real_T_1023x4
{
  double data[4092];
  int size[2];
};

struct struct_T
{
  emxArray_real_T_1x1024 breaks;
  emxArray_real_T_1023x4 coefs;
};

// Function Declarations
namespace coder
{
  static void interp1Linear(const emxArray_real_T *y, const double xi[513],
    double yi[513], const emxArray_real_T *varargin_1);
  static void interp1SplineOrPCHIP(const double y[1024], const double xi_data[],
    double yi_data[], const double x_data[]);
}

// Function Definitions
//
// Arguments    : const emxArray_real_T *y
//                const double xi[513]
//                double yi[513]
//                const emxArray_real_T *varargin_1
// Return Type  : void
//
namespace coder
{
  static void interp1Linear(const emxArray_real_T *y, const double xi[513],
    double yi[513], const emxArray_real_T *varargin_1)
  {
    double d;
    double maxx;
    double minx;
    double r;
    int n;
    minx = varargin_1->data[0];
    maxx = varargin_1->data[varargin_1->size[1] - 1];

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(d,n,r)

    for (int k = 0; k < 513; k++) {
      d = xi[k];
      if (rtIsNaN(d)) {
        yi[k] = rtNaN;
      } else {
        if ((!(d > maxx)) && (!(d < minx))) {
          n = internal::b_bsearch(varargin_1, d) - 1;
          r = (d - varargin_1->data[n]) / (varargin_1->data[n + 1] -
            varargin_1->data[n]);
          if (r == 0.0) {
            yi[k] = y->data[n];
          } else if (r == 1.0) {
            yi[k] = y->data[n + 1];
          } else {
            d = y->data[n + 1];
            if (y->data[n] == d) {
              yi[k] = y->data[n];
            } else {
              yi[k] = (1.0 - r) * y->data[n] + r * d;
            }
          }
        }
      }
    }
  }

  //
  // Arguments    : const double y[1024]
  //                const double xi_data[]
  //                double yi_data[]
  //                const double x_data[]
  // Return Type  : void
  //
  static void interp1SplineOrPCHIP(const double y[1024], const double xi_data[],
    double yi_data[], const double x_data[])
  {
    static struct_T pp;
    emxArray_real_T b_pp;
    double pp_coefs_data[4092];
    double md_data[1024];
    double s_data[1024];
    double dvdf_data[1023];
    double dx_data[1023];
    double d;
    double d31;
    double dnnm2;
    double r;
    double xloc;
    int ip;
    int k;
    for (k = 0; k < 1023; k++) {
      d = x_data[k + 1] - x_data[k];
      dx_data[k] = d;
      dvdf_data[k] = (y[k + 1] - y[k]) / d;
    }

    d31 = x_data[2] - x_data[0];
    dnnm2 = x_data[1023] - x_data[1021];
    s_data[0] = ((dx_data[0] + 2.0 * d31) * dx_data[1] * dvdf_data[0] + dx_data
                 [0] * dx_data[0] * dvdf_data[1]) / d31;
    s_data[1023] = ((dx_data[1022] + 2.0 * dnnm2) * dx_data[1021] * dvdf_data
                    [1022] + dx_data[1022] * dx_data[1022] * dvdf_data[1021]) /
      dnnm2;
    md_data[0] = dx_data[1];
    md_data[1023] = dx_data[1021];
    for (k = 0; k < 1022; k++) {
      r = dx_data[k + 1];
      d = dx_data[k];
      s_data[k + 1] = 3.0 * (r * dvdf_data[k] + d * dvdf_data[k + 1]);
      md_data[k + 1] = 2.0 * (r + d);
    }

    r = dx_data[1] / md_data[0];
    md_data[1] -= r * d31;
    s_data[1] -= r * s_data[0];
    for (k = 0; k < 1021; k++) {
      r = dx_data[k + 2] / md_data[k + 1];
      md_data[k + 2] -= r * dx_data[k];
      s_data[k + 2] -= r * s_data[k + 1];
    }

    r = dnnm2 / md_data[1022];
    md_data[1023] -= r * dx_data[1021];
    s_data[1023] -= r * s_data[1022];
    s_data[1023] /= md_data[1023];
    for (k = 1021; k >= 0; k--) {
      s_data[k + 1] = (s_data[k + 1] - dx_data[k] * s_data[k + 2]) / md_data[k +
        1];
    }

    s_data[0] = (s_data[0] - d31 * s_data[1]) / md_data[0];
    for (k = 0; k < 1023; k++) {
      double dzzdx;
      d = dvdf_data[k];
      d31 = s_data[k];
      dnnm2 = dx_data[k];
      dzzdx = (d - d31) / dnnm2;
      r = (s_data[k + 1] - d) / dnnm2;
      pp_coefs_data[k] = (r - dzzdx) / dnnm2;
      pp_coefs_data[k + 1023] = 2.0 * dzzdx - r;
      pp_coefs_data[k + 2046] = d31;
      pp_coefs_data[k + 3069] = y[k];
    }

    pp.breaks.size[0] = 1;
    pp.breaks.size[1] = 1024;
    std::memcpy(&pp.breaks.data[0], &x_data[0], 1024U * sizeof(double));
    pp.coefs.size[0] = 1023;
    pp.coefs.size[1] = 4;
    std::memcpy(&pp.coefs.data[0], &pp_coefs_data[0], 4092U * sizeof(double));

#pragma omp parallel for \
 num_threads(omp_get_max_threads()) \
 private(b_pp,xloc,ip)

    for (int b_k = 0; b_k < 2048; b_k++) {
      if (rtIsNaN(xi_data[b_k])) {
        yi_data[b_k] = rtNaN;
      } else {
        if (rtIsNaN(xi_data[b_k])) {
          xloc = xi_data[b_k];
        } else {
          b_pp.data = &pp.breaks.data[0];
          b_pp.size = &pp.breaks.size[0];
          b_pp.allocatedSize = 1024;
          b_pp.numDimensions = 2;
          b_pp.canFreeData = false;
          ip = internal::b_bsearch(&b_pp, xi_data[b_k]) - 1;
          xloc = xi_data[b_k] - pp.breaks.data[ip];
          xloc = xloc * (xloc * (xloc * pp.coefs.data[ip] + pp.coefs.data[ip +
            1023]) + pp.coefs.data[ip + 2046]) + pp.coefs.data[ip + 3069];
        }

        yi_data[b_k] = xloc;
      }
    }
  }

  //
  // Arguments    : const emxArray_real_T *varargin_1
  //                const emxArray_real_T *varargin_2
  //                double Vq[513]
  // Return Type  : void
  //
  void interp1(const emxArray_real_T *varargin_1, const emxArray_real_T
               *varargin_2, double Vq[513])
  {
    emxArray_real_T *x;
    emxArray_real_T *y;
    double dv[513];
    int i;
    int nd2;
    int nx;
    emxInit_real_T(&y, 2);
    i = y->size[0] * y->size[1];
    y->size[0] = 1;
    y->size[1] = varargin_2->size[1];
    emxEnsureCapacity_real_T(y, i);
    nd2 = varargin_2->size[0] * varargin_2->size[1];
    for (i = 0; i < nd2; i++) {
      y->data[i] = varargin_2->data[i];
    }

    emxInit_real_T(&x, 2);
    i = x->size[0] * x->size[1];
    x->size[0] = 1;
    x->size[1] = varargin_1->size[1];
    emxEnsureCapacity_real_T(x, i);
    nd2 = varargin_1->size[0] * varargin_1->size[1];
    for (i = 0; i < nd2; i++) {
      x->data[i] = varargin_1->data[i];
    }

    nx = varargin_1->size[1] - 1;
    nd2 = 0;
    int exitg1;
    do {
      exitg1 = 0;
      if (nd2 <= nx) {
        if (rtIsNaN(varargin_1->data[nd2])) {
          exitg1 = 1;
        } else {
          nd2++;
        }
      } else {
        if (varargin_1->data[1] < varargin_1->data[0]) {
          double xtmp;
          int b_j1;
          i = (nx + 1) >> 1;
          for (b_j1 = 0; b_j1 < i; b_j1++) {
            xtmp = x->data[b_j1];
            nd2 = nx - b_j1;
            x->data[b_j1] = x->data[nd2];
            x->data[nd2] = xtmp;
          }

          nd2 = varargin_2->size[1] >> 1;
          for (b_j1 = 0; b_j1 < nd2; b_j1++) {
            nx = (varargin_2->size[1] - b_j1) - 1;
            xtmp = y->data[b_j1];
            y->data[b_j1] = y->data[nx];
            y->data[nx] = xtmp;
          }
        }

        for (i = 0; i < 513; i++) {
          Vq[i] = rtNaN;
          dv[i] = 136.71875 * static_cast<double>(i);
        }

        interp1Linear(y, dv, Vq, x);
        exitg1 = 1;
      }
    } while (exitg1 == 0);

    emxFree_real_T(&x);
    emxFree_real_T(&y);
  }

  //
  // Arguments    : const double varargin_1_data[]
  //                const double varargin_2[1024]
  //                const double varargin_3_data[]
  //                double Vq_data[]
  //                int Vq_size[2]
  // Return Type  : void
  //
  void interp1(const double varargin_1_data[], const double varargin_2[1024],
               const double varargin_3_data[], double Vq_data[], int Vq_size[2])
  {
    double x_data[1024];
    double y[1024];
    int k;
    std::memcpy(&y[0], &varargin_2[0], 1024U * sizeof(double));
    std::memcpy(&x_data[0], &varargin_1_data[0], 1024U * sizeof(double));
    Vq_size[0] = 1;
    Vq_size[1] = 2048;
    std::memset(&Vq_data[0], 0, 2048U * sizeof(double));
    k = 0;
    int exitg1;
    do {
      exitg1 = 0;
      if (k < 1024) {
        if (rtIsNaN(varargin_1_data[k])) {
          exitg1 = 1;
        } else {
          k++;
        }
      } else {
        if (varargin_1_data[1] < varargin_1_data[0]) {
          for (k = 0; k < 512; k++) {
            double xtmp;
            xtmp = x_data[k];
            x_data[k] = x_data[1023 - k];
            x_data[1023 - k] = xtmp;
            xtmp = y[k];
            y[k] = y[1023 - k];
            y[1023 - k] = xtmp;
          }
        }

        interp1SplineOrPCHIP(y, varargin_3_data, Vq_data, x_data);
        exitg1 = 1;
      }
    } while (exitg1 == 0);
  }
}

//
// File trailer for interp1.cpp
//
// [EOF]
//
