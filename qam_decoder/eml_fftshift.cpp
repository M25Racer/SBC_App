//
// File: eml_fftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 20-Jul-2022 11:04:49
//

// Include Files
#include "eml_fftshift.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : creal_T x[57820]
//                int dim
// Return Type  : void
//
namespace coder
{
  void eml_fftshift(creal_T x[57820], int dim)
  {
    int i2;
    int ia;
    int ib;
    int npages;
    int vstride;

    //@>25a00a
    if (                               //@>259f55
        dim                            //@>314a12
        <=                             //@>25a0ec
        2) {
      int i1;

      //@>259fb0
      i1                               //@>259fb1
        =                              //@>25a0fa
        57819                          //@>2c46ce
        * (                            //@>25a030
           dim                         //@>259fae
           -                           //@>259fad
           1) +                        //@>25a0f9
        1;

      //@>25a00f
      if (                             //@>25a032
          i1                           //@>314a17
          >                            //@>259f57
          1) {
        int k;
        int midoffset;
        int vlend2;
        int vspread;

        //@>25a044
        vlend2                         //@>25a043
          =                            //@>25a100
          i1                           //@>2c46bc
          /                            //@>25a0ed
          2;

        //@>25a04b
        vstride                        //@>25a04a
          =                            //@>39ed31
          1;

        //@>25a074
        for (                          //@>39eda2
             k =                       //@>25a05b
             0;                        //@>39edaa
             k <=                      //@>2b605a
             dim                       //@>28f8dd
             -                         //@>28f8dc
             2;                        //@>39eda7
             k                         //@>39edac
             ++) {
          //@>314a25
          if (                         //@>2b606f
              k                        //@>2b606d
              +                        //@>2b6070
              1                        //@>2b606c
              <=                       //@>2b6071
              2) {
            //@>314a29
            npages                     //@>314a28
              =                        //@>2b6074
              57819                    //@>2c46c2
              *                        //@>2b6076
              k +                      //@>2b6077
              1;
          } else {
            //@>311e2a
            npages                     //@>311e29
              =                        //@>39ed37
              1;
          }

          //@>25a04f
          vstride                      //@>39edae
            *=                         //@>311e23
            npages;
        }

        //@>25a086
        npages                         //@>25a085
          =                            //@>39ed34
          1;

        //@>25a093
        i2                             //@>25a094
          =                            //@>25a108
          dim                          //@>28f8e7
          +                            //@>28f8e6
          1;

        //@>25a0a8
        for (                          //@>39ed94
             k =                       //@>25a091
             i2;                       //@>39ed9c
             k <                       //@>25a08f
             3;                        //@>39ed99
             k                         //@>39edad
             ++) {
          //@>25a089
          npages                       //@>39edaf
            *=                         //@>2b6082
            57819                      //@>2c46ca
            * (                        //@>2b6085
               k                       //@>2b6083
               -                       //@>2b6086
               1) +                    //@>2b6087
            1;
        }

        //@>25a0b5
        vspread                        //@>25a0b4
          = (                          //@>25a13a
             i1                        //@>25a137
             -                         //@>25a13b
             1)                        //@>2c46b9
          *                            //@>25a10a
          vstride;

        //@>25a0b9
        midoffset                      //@>25a0b8
          =                            //@>25a10c
          vlend2                       //@>2c46b5
          *                            //@>25a10e
          vstride                      //@>28f8f1
          -                            //@>28f8f0
          1;

        //@>25a010
        if (                           //@>25a13e
            vlend2                     //@>2c46b3
            <<                         //@>2e2c80
            1                          //@>259fb6
            ==                         //@>25a034
            i1) {
          //@>259f59
          i2                           //@>259f58
            =                          //@>39ed3d
            0;

          //@>25a012
          for (int                     //@>39ed6d
               i =                     //@>259fbb
               0;                      //@>39ed75
               i <                     //@>25a03c
               npages;                 //@>39ed72
               i                       //@>39edb1
               ++) {
            //@>259f5c
            i1                         //@>259f5b
              =                        //@>259f5d
              i2;

            //@>25a0c1
            i2                         //@>39edb3
              +=                       //@>25a126
              vspread;

            //@>25a015
            for (int                   //@>39ed7a
                 j =                   //@>259fc0
                 0;                    //@>39ed82
                 j <                   //@>25a03e
                 vstride;              //@>39ed7f
                 j                     //@>39edb5
                 ++) {
              //@>25a0c9
              i1                       //@>39edb7
                ++;

              //@>25a0d1
              i2                       //@>39edb9
                ++;

              //@>259f61
              ia                       //@>259f60
                =                      //@>259f62
                i1                     //@>266a71
                -                      //@>266a70
                1;

              //@>25a0d9
              ib                       //@>25a0d8
                =                      //@>25a12c
                i1                     //@>25a0da
                +                      //@>25a12e
                midoffset;

              //@>25a018
              for (                    //@>39ed87
                   k =                 //@>259fc5
                   0;                  //@>39ed8f
                   k <                 //@>25a040
                   vlend2;             //@>39ed8c
                   k                   //@>39edba
                   ++) {
                double xtmp_im;
                double xtmp_re;

                //@>2bf9aa
                xtmp_re                //@>2babb4
                  =                    //@>2babb9
                  x                    //@>32b841
                  [                    //@>2babbb
                  ia].re;

                //@>2bf9ad
                xtmp_im                //@>259f65
                  =                    //@>28edb9
                  x                    //@>32b844
                  [                    //@>28edbf
                  ia].im;

                //@>2bfb83
                x                      //@>2bfb81
                  [                    //@>2bfb85
                  ia]                  //@>2bfb86
                  =                    //@>2bfb7e
                  x                    //@>32b847
                  [                    //@>2bfb80
                  ib];

                //@>2babdd
                x                      //@>2babdb
                  [                    //@>2babdf
                  ib].                 //@>3a03c2
                  re                   //@>2babda
                  =                    //@>2bf9af
                  xtmp_re;

                //@>28eec0
                x                      //@>2af326
                  [                    //@>28eec6
                  ib].                 //@>3a03c5
                  im                   //@>259f73
                  =                    //@>2bf9b2
                  xtmp_im;

                //@>25a0e1
                ia                     //@>39edbd
                  +=                   //@>25a132
                  vstride;

                //@>25a0e9
                ib                     //@>39edbe
                  +=                   //@>25a136
                  vstride;
              }
            }
          }
        } else {
          //@>259f7a
          i2                           //@>259f79
            =                          //@>39ed3a
            0;

          //@>25a01e
          for (int                     //@>39ed46
               i =                     //@>259fde
               0;                      //@>39ed4e
               i <                     //@>25a036
               npages;                 //@>39ed4b
               i                       //@>39edb0
               ++) {
            //@>259f7d
            i1                         //@>259f7c
              =                        //@>259f7e
              i2;

            //@>25a0bd
            i2                         //@>39edb2
              +=                       //@>25a112
              vspread;

            //@>25a021
            for (int                   //@>39ed53
                 j =                   //@>259fe3
                 0;                    //@>39ed5b
                 j <                   //@>25a038
                 vstride;              //@>39ed58
                 j                     //@>39edb4
                 ++) {
              double xtmp_im;
              double xtmp_re;

              //@>25a0c5
              i1                       //@>39edb6
                ++;

              //@>25a0cd
              i2                       //@>39edb8
                ++;

              //@>259f82
              ia                       //@>259f81
                =                      //@>259f83
                i1                     //@>266a6e
                -                      //@>266a6d
                1;

              //@>25a0d5
              ib                       //@>25a0d4
                =                      //@>25a118
                i1                     //@>25a0d6
                +                      //@>25a11a
                midoffset;

              //@>2bf9b5
              xtmp_re                  //@>2babe7
                =                      //@>2babec
                x                      //@>32b83b
                [                      //@>2babee
                ib].re;

              //@>2bf9b7
              xtmp_im                  //@>259f85
                =                      //@>28ed8b
                x                      //@>32b83e
                [                      //@>28ed91
                ib].im;

              //@>25a025
              for (                    //@>39ed60
                   k =                 //@>259fed
                   0;                  //@>39ed68
                   k <                 //@>25a03a
                   vlend2;             //@>39ed65
                   k                   //@>39edbb
                   ++) {
                int ic;

                //@>25a0dd
                ic                     //@>25a0dc
                  =                    //@>25a11c
                  ib                   //@>25a0de
                  +                    //@>25a11e
                  vstride;

                //@>2bfb90
                x                      //@>2bfb8e
                  [                    //@>2bfb92
                  ib]                  //@>2bfb93
                  =                    //@>2bfb8b
                  x                    //@>32b84a
                  [                    //@>2bfb8d
                  ia];

                //@>2bfb9d
                x                      //@>2bfb9b
                  [                    //@>2bfb9f
                  ia]                  //@>2bfba0
                  =                    //@>2bfb98
                  x                    //@>32b84d
                  [                    //@>2bfb9a
                  ic];

                //@>25a0e5
                ia                     //@>39edbc
                  +=                   //@>25a122
                  vstride;

                //@>259f9d
                ib                     //@>259f9c
                  =                    //@>259f9e
                  ic;
              }

              //@>2bac20
              x                        //@>2bac1e
                [                      //@>2bac22
                ib].                   //@>3a03c8
                re                     //@>2bac1d
                =                      //@>2bf9b9
                xtmp_re;

              //@>28ede7
              x                        //@>2af2f2
                [                      //@>28eded
                ib].                   //@>3a03cb
                im                     //@>259fa0
                =                      //@>2bf9bc
                xtmp_im;
            }
          }
        }
      }
    }
  }
}

//
// File trailer for eml_fftshift.cpp
//
// [EOF]
//
