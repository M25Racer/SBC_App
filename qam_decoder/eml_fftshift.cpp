//
// File: eml_fftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 06-May-2022 14:49:51
//

// Include Files
#include "eml_fftshift.h"
#include "HS_EWL_DEMOD_QAM_types.h"
#include "rt_nonfinite.h"

// Function Definitions
//
// Arguments    : emxArray_creal_T *x
//                int dim
// Return Type  : void
//
namespace coder
{
  void eml_fftshift(emxArray_creal_T *x, int dim)
  {
    int a;
    int i1;
    int i2;
    int npages;

    //@>213316
    a                                  //@>213315
      =                                //@>30bfdf
      x->size[                         //@>27e2f2
      dim                              //@>27e2f0
      -                                //@>27e2f3
      1]                               //@>228722
      -                                //@>228721
      1;

    //@>213370
    if (                               //@>30bfec
        x->size[                       //@>27e327
        dim                            //@>27e325
        -                              //@>27e328
        1]                             //@>379c1a
        >                              //@>21336c
        1) {
      int k;
      int vlend2;

      //@>21339a
      vlend2                           //@>213399
        =                              //@>30bff9
        x->size[                       //@>27e35c
        dim                            //@>27e35a
        -                              //@>27e35d
        1]                             //@>2d099b
        /                              //@>21339d
        2                              //@>2831a8
        -                              //@>2831a7
        1;

      //@>2133ae
      npages                           //@>2133af
        =                              //@>4817da
        1;

      //@>2133a2
      i1                               //@>2133a1
        =                              //@>2133e7
        dim                            //@>2831b5
        +                              //@>2831b4
        1;

      //@>2133b2
      for (                            //@>48181a
           k =                         //@>2133a4
           i1;                         //@>481822
           k <                         //@>2133a6
           3;                          //@>48181f
           k                           //@>481826
           ++) {
        //@>2133ab
        npages                         //@>481827
          *=                           //@>30c01c
          x->size[                     //@>27e3c9
          1];
      }

      //@>213371
      if ((                            //@>21341a
           vlend2                      //@>2831ab
           +                           //@>2831aa
           1)                          //@>2d099a
          <<                           //@>318fe1
          1                            //@>213313
          ==                           //@>30c00e
          x->size[                     //@>27e396
          dim                          //@>27e394
          -                            //@>27e397
          1]) {
        //@>21336a
        i2                             //@>21336b
          =                            //@>4817e0
          0;

        //@>213373
        for (int                       //@>481801
             i =                       //@>21330e
             0;                        //@>481809
             i <                       //@>213396
             npages;                   //@>481806
             i                         //@>481829
             ++) {
          int ia;

          //@>213367
          i1                           //@>213368
            =                          //@>213366
            i2;

          //@>2133b9
          i2                           //@>48182b
            +=                         //@>213431
            a;

          //@>2cbcc0
          i1                           //@>48182d
            ++;

          //@>2cbcc7
          i2                           //@>48182f
            ++;

          //@>2cbcce
          ia                           //@>2cbccd
            =                          //@>2cbcd1
            i1                         //@>2cbccf
            -                          //@>2cbcd2
            1;

          //@>2cbcd5
          i1                           //@>481831
            +=                         //@>2cbcda
            vlend2;

          //@>2cbcdb
          for (                        //@>48180e
               k =                     //@>2cbcde
               0;                      //@>481816
               k <=                    //@>2cbce1
               vlend2;                 //@>481813
               k                       //@>481832
               ++) {
            double xtmp_im;
            double xtmp_re;

            //@>2cbcfd
            xtmp_re                    //@>2cbcfc
              =                        //@>30c0d8
              x->data[                 //@>2cbd05
              ia].re;

            //@>2cbd09
            xtmp_im                    //@>2cbd08
              =                        //@>30c10d
              x->data[                 //@>2cbd11
              ia].im;

            //@>30c1ce
            x->data[                   //@>2cbd1a
              ia]                      //@>2cbd14
              =                        //@>30c1fd
              x->data[                 //@>2cbd21
              i1];

            //@>30c2bd
            x->data[                   //@>2cbd2a
              i1].                     //@>483a37
              re                       //@>2cbd23
              =                        //@>2cbd2d
              xtmp_re;

            //@>30c2ef
            x->data[                   //@>2cbd36
              i1].                     //@>483a3a
              im                       //@>2cbd2f
              =                        //@>2cbd39
              xtmp_im;

            //@>2cbd3c
            ia                         //@>481836
              ++;

            //@>2cbd43
            i1                         //@>481837
              ++;
          }
        }
      } else {
        //@>213349
        i2                             //@>21334a
          =                            //@>4817dd
          0;

        //@>21337f
        for (int                       //@>4817e8
             i =                       //@>2132d9
             0;                        //@>4817f0
             i <                       //@>213392
             npages;                   //@>4817ed
             i                         //@>481828
             ++) {
          double xtmp_im;
          double xtmp_re;
          int ia;

          //@>213346
          i1                           //@>213347
            =                          //@>213345
            i2;

          //@>2133b6
          i2                           //@>48182a
            +=                         //@>213427
            a;

          //@>2cbc19
          i1                           //@>48182c
            ++;

          //@>2cbc20
          i2                           //@>48182e
            ++;

          //@>2cbc27
          ia                           //@>2cbc26
            =                          //@>2cbc2a
            i1                         //@>2cbc28
            -                          //@>2cbc2b
            1;

          //@>2cbc2e
          i1                           //@>481830
            +=                         //@>2cbc33
            vlend2;

          //@>2cbc4e
          xtmp_re                      //@>2cbc4d
            =                          //@>30c06a
            x->data[                   //@>2cbc56
            i1].re;

          //@>2cbc5a
          xtmp_im                      //@>2cbc59
            =                          //@>30c0a0
            x->data[                   //@>2cbc62
            i1].im;

          //@>2cbc64
          for (                        //@>4817f5
               k =                     //@>2cbc67
               0;                      //@>4817fd
               k <=                    //@>2cbc6a
               vlend2;                 //@>4817fa
               k                       //@>481833
               ++) {
            //@>30c16e
            x->data[                   //@>2cbc7f
              i1]                      //@>2cbc79
              =                        //@>30c19d
              x->data[                 //@>2cbc86
              ia];

            //@>30c25d
            x->data[                   //@>2cbc9a
              ia]                      //@>2cbc94
              =                        //@>30c28c
              x->data[                 //@>2cbca2
              i1                       //@>2cbca0
              +                        //@>2cbca3
              1];

            //@>2cbca6
            ia                         //@>481834
              ++;

            //@>2cbcad
            i1                         //@>481835
              ++;
          }

          //@>30c13f
          x->data[                     //@>2cbc74
            i1].                       //@>483a31
            re                         //@>2cbc6d
            =                          //@>2cbc77
            xtmp_re;

          //@>30c22e
          x->data[                     //@>2cbc8f
            i1].                       //@>483a34
            im                         //@>2cbc88
            =                          //@>2cbc92
            xtmp_im;
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
