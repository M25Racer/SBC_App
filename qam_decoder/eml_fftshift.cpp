//
// File: eml_fftshift.cpp
//
// MATLAB Coder version            : 5.1
// C/C++ source code generated on  : 08-Jul-2022 10:21:26
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

    //@>26386e
    if (                               //@>2637b9
        dim                            //@>324912
        <=                             //@>263950
        2) {
      int i1;

      //@>263814
      i1                               //@>263815
        =                              //@>26395e
        57819                          //@>2d1acb
        * (                            //@>263894
           dim                         //@>263812
           -                           //@>263811
           1) +                        //@>26395d
        1;

      //@>263873
      if (                             //@>263896
          i1                           //@>324917
          >                            //@>2637bb
          1) {
        int k;
        int midoffset;
        int vlend2;
        int vspread;

        //@>2638a8
        vlend2                         //@>2638a7
          =                            //@>263964
          i1                           //@>2d1ab9
          /                            //@>263951
          2;

        //@>2638af
        vstride                        //@>2638ae
          =                            //@>3b441d
          1;

        //@>2638d8
        for (                          //@>3b448e
             k =                       //@>2638bf
             0;                        //@>3b4496
             k <=                      //@>2c3030
             dim                       //@>29aff2
             -                         //@>29aff1
             2;                        //@>3b4493
             k                         //@>3b4498
             ++) {
          //@>324925
          if (                         //@>2c3045
              k                        //@>2c3043
              +                        //@>2c3046
              1                        //@>2c3042
              <=                       //@>2c3047
              2) {
            //@>324929
            npages                     //@>324928
              =                        //@>2c304a
              57819                    //@>2d1abf
              *                        //@>2c304c
              k +                      //@>2c304d
              1;
          } else {
            //@>321aa9
            npages                     //@>321aa8
              =                        //@>3b4423
              1;
          }

          //@>2638b3
          vstride                      //@>3b449a
            *=                         //@>321aa2
            npages;
        }

        //@>2638ea
        npages                         //@>2638e9
          =                            //@>3b4420
          1;

        //@>2638f7
        i2                             //@>2638f8
          =                            //@>26396c
          dim                          //@>29affc
          +                            //@>29affb
          1;

        //@>26390c
        for (                          //@>3b4480
             k =                       //@>2638f5
             i2;                       //@>3b4488
             k <                       //@>2638f3
             3;                        //@>3b4485
             k                         //@>3b4499
             ++) {
          //@>2638ed
          npages                       //@>3b449b
            *=                         //@>2c3058
            57819                      //@>2d1ac7
            * (                        //@>2c305b
               k                       //@>2c3059
               -                       //@>2c305c
               1) +                    //@>2c305d
            1;
        }

        //@>263919
        vspread                        //@>263918
          = (                          //@>26399e
             i1                        //@>26399b
             -                         //@>26399f
             1)                        //@>2d1ab6
          *                            //@>26396e
          vstride;

        //@>26391d
        midoffset                      //@>26391c
          =                            //@>263970
          vlend2                       //@>2d1ab2
          *                            //@>263972
          vstride                      //@>29b006
          -                            //@>29b005
          1;

        //@>263874
        if (                           //@>2639a2
            vlend2                     //@>2d1ab0
            <<                         //@>2f0d01
            1                          //@>26381a
            ==                         //@>263898
            i1) {
          //@>2637bd
          i2                           //@>2637bc
            =                          //@>3b4429
            0;

          //@>263876
          for (int                     //@>3b4459
               i =                     //@>26381f
               0;                      //@>3b4461
               i <                     //@>2638a0
               npages;                 //@>3b445e
               i                       //@>3b449d
               ++) {
            //@>2637c0
            i1                         //@>2637bf
              =                        //@>2637c1
              i2;

            //@>263925
            i2                         //@>3b449f
              +=                       //@>26398a
              vspread;

            //@>263879
            for (int                   //@>3b4466
                 j =                   //@>263824
                 0;                    //@>3b446e
                 j <                   //@>2638a2
                 vstride;              //@>3b446b
                 j                     //@>3b44a1
                 ++) {
              //@>26392d
              i1                       //@>3b44a3
                ++;

              //@>263935
              i2                       //@>3b44a5
                ++;

              //@>2637c5
              ia                       //@>2637c4
                =                      //@>2637c6
                i1                     //@>270772
                -                      //@>270771
                1;

              //@>26393d
              ib                       //@>26393c
                =                      //@>263990
                i1                     //@>26393e
                +                      //@>263992
                midoffset;

              //@>26387c
              for (                    //@>3b4473
                   k =                 //@>263829
                   0;                  //@>3b447b
                   k <                 //@>2638a4
                   vlend2;             //@>3b4478
                   k                   //@>3b44a6
                   ++) {
                double xtmp_im;
                double xtmp_re;

                //@>2ccce8
                xtmp_re                //@>2c7cab
                  =                    //@>2c7cb0
                  x                    //@>33c4cf
                  [                    //@>2c7cb2
                  ia].re;

                //@>2ccceb
                xtmp_im                //@>2637c9
                  =                    //@>29a4c3
                  x                    //@>33c4d2
                  [                    //@>29a4c9
                  ia].im;

                //@>2ccee9
                x                      //@>2ccee7
                  [                    //@>2cceeb
                  ia]                  //@>2cceec
                  =                    //@>2ccee4
                  x                    //@>33c4d5
                  [                    //@>2ccee6
                  ib];

                //@>2c7cd4
                x                      //@>2c7cd2
                  [                    //@>2c7cd6
                  ib].                 //@>3b5b27
                  re                   //@>2c7cd1
                  =                    //@>2ccced
                  xtmp_re;

                //@>29a5ca
                x                      //@>2bbef1
                  [                    //@>29a5d0
                  ib].                 //@>3b5b2a
                  im                   //@>2637d7
                  =                    //@>2cccf0
                  xtmp_im;

                //@>263945
                ia                     //@>3b44a9
                  +=                   //@>263996
                  vstride;

                //@>26394d
                ib                     //@>3b44aa
                  +=                   //@>26399a
                  vstride;
              }
            }
          }
        } else {
          //@>2637de
          i2                           //@>2637dd
            =                          //@>3b4426
            0;

          //@>263882
          for (int                     //@>3b4432
               i =                     //@>263842
               0;                      //@>3b443a
               i <                     //@>26389a
               npages;                 //@>3b4437
               i                       //@>3b449c
               ++) {
            //@>2637e1
            i1                         //@>2637e0
              =                        //@>2637e2
              i2;

            //@>263921
            i2                         //@>3b449e
              +=                       //@>263976
              vspread;

            //@>263885
            for (int                   //@>3b443f
                 j =                   //@>263847
                 0;                    //@>3b4447
                 j <                   //@>26389c
                 vstride;              //@>3b4444
                 j                     //@>3b44a0
                 ++) {
              double xtmp_im;
              double xtmp_re;

              //@>263929
              i1                       //@>3b44a2
                ++;

              //@>263931
              i2                       //@>3b44a4
                ++;

              //@>2637e6
              ia                       //@>2637e5
                =                      //@>2637e7
                i1                     //@>27076f
                -                      //@>27076e
                1;

              //@>263939
              ib                       //@>263938
                =                      //@>26397c
                i1                     //@>26393a
                +                      //@>26397e
                midoffset;

              //@>2cccf3
              xtmp_re                  //@>2c7cde
                =                      //@>2c7ce3
                x                      //@>33c4c9
                [                      //@>2c7ce5
                ib].re;

              //@>2cccf5
              xtmp_im                  //@>2637e9
                =                      //@>29a495
                x                      //@>33c4cc
                [                      //@>29a49b
                ib].im;

              //@>263889
              for (                    //@>3b444c
                   k =                 //@>263851
                   0;                  //@>3b4454
                   k <                 //@>26389e
                   vlend2;             //@>3b4451
                   k                   //@>3b44a7
                   ++) {
                int ic;

                //@>263941
                ic                     //@>263940
                  =                    //@>263980
                  ib                   //@>263942
                  +                    //@>263982
                  vstride;

                //@>2ccef6
                x                      //@>2ccef4
                  [                    //@>2ccef8
                  ib]                  //@>2ccef9
                  =                    //@>2ccef1
                  x                    //@>33c4d8
                  [                    //@>2ccef3
                  ia];

                //@>2ccf03
                x                      //@>2ccf01
                  [                    //@>2ccf05
                  ia]                  //@>2ccf06
                  =                    //@>2ccefe
                  x                    //@>33c4db
                  [                    //@>2ccf00
                  ic];

                //@>263949
                ia                     //@>3b44a8
                  +=                   //@>263986
                  vstride;

                //@>263801
                ib                     //@>263800
                  =                    //@>263802
                  ic;
              }

              //@>2c7d17
              x                        //@>2c7d15
                [                      //@>2c7d19
                ib].                   //@>3b5b2d
                re                     //@>2c7d14
                =                      //@>2cccf7
                xtmp_re;

              //@>29a4f1
              x                        //@>2bbebd
                [                      //@>29a4f7
                ib].                   //@>3b5b30
                im                     //@>263804
                =                      //@>2cccfa
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
