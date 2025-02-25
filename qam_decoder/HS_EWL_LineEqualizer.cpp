/*
 * File: HS_EWL_LineEqualizer.c
 *
 * MATLAB Coder version            : 5.1
 * C/C++ source code generated on  : 13-Feb-2025 15:55:57
 */

/* Include Files */
#include "HS_EWL_LineEqualizer.h"
#include "HS_EWL_LineEqualizer_data.h"
#include "HS_EWL_LineEqualizer_initialize.h"
#include "minOrMax.h"
#include "rt_nonfinite.h"
#include "rt_nonfinite.h"
#include <math.h>
#include <string.h>


/* Type Definitions */
#ifndef typedef_cell_wrap_3
#define typedef_cell_wrap_3

typedef struct {
  unsigned int f1[8];
} cell_wrap_3;

#endif                                 /*typedef_cell_wrap_3*/

#ifndef typedef_struct_T
#define typedef_struct_T

typedef struct {
  double Length;
  creal_T Buffer[3];
  double Pointer;
} struct_T;

#endif                                 /*typedef_struct_T*/

#ifndef typedef_comm_LinearEqualizer
#define typedef_comm_LinearEqualizer

typedef struct {
  int isInitialized;
  cell_wrap_3 inputVarSize[3];
  double StepSize;
  creal_T TapDelayLine[13];
  creal_T ReferenceSymbol;
  creal_T Weights[13];
  boolean_T PreviousTrainingFlag;
  double ActiveDelay;
  struct_T TrainingDelayBuffer;
  double SymbolCounter;
  double NumTrainingSymbols;
  double WeightUpdateCounter;
} comm_LinearEqualizer;

#endif                                 /*typedef_comm_LinearEqualizer*/

/* Variable Definitions */
static comm_LinearEqualizer LineEqualizerStr;
static boolean_T LineEqualizerStr_not_empty;

/* Function Declarations */
static double rt_hypotd_snf(double u0, double u1);

/* Function Definitions */
/*
 * Arguments    : double u0
 *                double u1
 * Return Type  : double
 */
static double rt_hypotd_snf(double u0, double u1)
{
  double a;
  double y;
  a = fabs(u0);
  y = fabs(u1);
  if (a < y) 
  {
    a /= y;
    y *= sqrt(a * a + 1.0);
  } 
  else if (a > y) 
  {
    y /= a;
    y = a * sqrt(y * y + 1.0);
  } 
  else 
  {
    if (!rtIsNaN(y)) 
    {
      y = a * 1.4142135623730951;
    }
  }

  return y;
}

/*
 * Arguments    : const creal_T input_buf[256]
 *                const creal_T training_buf[256]
 *                boolean_T trainingFlag
 *                creal_T EqualizedData[256]
 *                creal_T channel_resp[13]
 * Return Type  : void
 */
void HS_EWL_LineEqualizer(const creal_T input_buf[256], const creal_T
  training_buf[256], boolean_T trainingFlag, creal_T EqualizedData[256], creal_T
  channel_resp[13])
{
  static const cint8_T icv[352] = { { -15,/* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -15,                          /* re */
      -9                               /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -15,                          /* re */
      -1                               /* im */
    }, { -15,                          /* re */
      -3                               /* im */
    }, { -15,                          /* re */
      -7                               /* im */
    }, { -15,                          /* re */
      -5                               /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -15,                          /* re */
      9                                /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -15,                          /* re */
      1                                /* im */
    }, { -15,                          /* re */
      3                                /* im */
    }, { -15,                          /* re */
      7                                /* im */
    }, { -15,                          /* re */
      5                                /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -13,                          /* re */
      -9                               /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -13,                          /* re */
      -1                               /* im */
    }, { -13,                          /* re */
      -3                               /* im */
    }, { -13,                          /* re */
      -7                               /* im */
    }, { -13,                          /* re */
      -5                               /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -13,                          /* re */
      9                                /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -13,                          /* re */
      1                                /* im */
    }, { -13,                          /* re */
      3                                /* im */
    }, { -13,                          /* re */
      7                                /* im */
    }, { -13,                          /* re */
      5                                /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -9,                           /* re */
      -9                               /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -9,                           /* re */
      -1                               /* im */
    }, { -9,                           /* re */
      -3                               /* im */
    }, { -9,                           /* re */
      -7                               /* im */
    }, { -9,                           /* re */
      -5                               /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -9,                           /* re */
      9                                /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -9,                           /* re */
      1                                /* im */
    }, { -9,                           /* re */
      3                                /* im */
    }, { -9,                           /* re */
      7                                /* im */
    }, { -9,                           /* re */
      5                                /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -11,                          /* re */
      -9                               /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -11,                          /* re */
      -1                               /* im */
    }, { -11,                          /* re */
      -3                               /* im */
    }, { -11,                          /* re */
      -7                               /* im */
    }, { -11,                          /* re */
      -5                               /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -11,                          /* re */
      9                                /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -11,                          /* re */
      1                                /* im */
    }, { -11,                          /* re */
      3                                /* im */
    }, { -11,                          /* re */
      7                                /* im */
    }, { -11,                          /* re */
      5                                /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -1,                           /* re */
      -9                               /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -1,                           /* re */
      -1                               /* im */
    }, { -1,                           /* re */
      -3                               /* im */
    }, { -1,                           /* re */
      -7                               /* im */
    }, { -1,                           /* re */
      -5                               /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -1,                           /* re */
      9                                /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -1,                           /* re */
      1                                /* im */
    }, { -1,                           /* re */
      3                                /* im */
    }, { -1,                           /* re */
      7                                /* im */
    }, { -1,                           /* re */
      5                                /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -3,                           /* re */
      -9                               /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -3,                           /* re */
      -1                               /* im */
    }, { -3,                           /* re */
      -3                               /* im */
    }, { -3,                           /* re */
      -7                               /* im */
    }, { -3,                           /* re */
      -5                               /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -3,                           /* re */
      9                                /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -3,                           /* re */
      1                                /* im */
    }, { -3,                           /* re */
      3                                /* im */
    }, { -3,                           /* re */
      7                                /* im */
    }, { -3,                           /* re */
      5                                /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -7,                           /* re */
      -9                               /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -7,                           /* re */
      -1                               /* im */
    }, { -7,                           /* re */
      -3                               /* im */
    }, { -7,                           /* re */
      -7                               /* im */
    }, { -7,                           /* re */
      -5                               /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -7,                           /* re */
      9                                /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -7,                           /* re */
      1                                /* im */
    }, { -7,                           /* re */
      3                                /* im */
    }, { -7,                           /* re */
      7                                /* im */
    }, { -7,                           /* re */
      5                                /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -5,                           /* re */
      -9                               /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -5,                           /* re */
      -1                               /* im */
    }, { -5,                           /* re */
      -3                               /* im */
    }, { -5,                           /* re */
      -7                               /* im */
    }, { -5,                           /* re */
      -5                               /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -5,                           /* re */
      9                                /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -5,                           /* re */
      1                                /* im */
    }, { -5,                           /* re */
      3                                /* im */
    }, { -5,                           /* re */
      7                                /* im */
    }, { -5,                           /* re */
      5                                /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 15,                           /* re */
      -9                               /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 15,                           /* re */
      -1                               /* im */
    }, { 15,                           /* re */
      -3                               /* im */
    }, { 15,                           /* re */
      -7                               /* im */
    }, { 15,                           /* re */
      -5                               /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 15,                           /* re */
      9                                /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 15,                           /* re */
      1                                /* im */
    }, { 15,                           /* re */
      3                                /* im */
    }, { 15,                           /* re */
      7                                /* im */
    }, { 15,                           /* re */
      5                                /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 13,                           /* re */
      -9                               /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 13,                           /* re */
      -1                               /* im */
    }, { 13,                           /* re */
      -3                               /* im */
    }, { 13,                           /* re */
      -7                               /* im */
    }, { 13,                           /* re */
      -5                               /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 13,                           /* re */
      9                                /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 13,                           /* re */
      1                                /* im */
    }, { 13,                           /* re */
      3                                /* im */
    }, { 13,                           /* re */
      7                                /* im */
    }, { 13,                           /* re */
      5                                /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 9,                            /* re */
      -9                               /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 9,                            /* re */
      -1                               /* im */
    }, { 9,                            /* re */
      -3                               /* im */
    }, { 9,                            /* re */
      -7                               /* im */
    }, { 9,                            /* re */
      -5                               /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 9,                            /* re */
      9                                /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 9,                            /* re */
      1                                /* im */
    }, { 9,                            /* re */
      3                                /* im */
    }, { 9,                            /* re */
      7                                /* im */
    }, { 9,                            /* re */
      5                                /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 11,                           /* re */
      -9                               /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 11,                           /* re */
      -1                               /* im */
    }, { 11,                           /* re */
      -3                               /* im */
    }, { 11,                           /* re */
      -7                               /* im */
    }, { 11,                           /* re */
      -5                               /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 11,                           /* re */
      9                                /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 11,                           /* re */
      1                                /* im */
    }, { 11,                           /* re */
      3                                /* im */
    }, { 11,                           /* re */
      7                                /* im */
    }, { 11,                           /* re */
      5                                /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 1,                            /* re */
      -9                               /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 1,                            /* re */
      -1                               /* im */
    }, { 1,                            /* re */
      -3                               /* im */
    }, { 1,                            /* re */
      -7                               /* im */
    }, { 1,                            /* re */
      -5                               /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 1,                            /* re */
      9                                /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 1,                            /* re */
      1                                /* im */
    }, { 1,                            /* re */
      3                                /* im */
    }, { 1,                            /* re */
      7                                /* im */
    }, { 1,                            /* re */
      5                                /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 3,                            /* re */
      -9                               /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 3,                            /* re */
      -1                               /* im */
    }, { 3,                            /* re */
      -3                               /* im */
    }, { 3,                            /* re */
      -7                               /* im */
    }, { 3,                            /* re */
      -5                               /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 3,                            /* re */
      9                                /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 3,                            /* re */
      1                                /* im */
    }, { 3,                            /* re */
      3                                /* im */
    }, { 3,                            /* re */
      7                                /* im */
    }, { 3,                            /* re */
      5                                /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 7,                            /* re */
      -9                               /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 7,                            /* re */
      -1                               /* im */
    }, { 7,                            /* re */
      -3                               /* im */
    }, { 7,                            /* re */
      -7                               /* im */
    }, { 7,                            /* re */
      -5                               /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 7,                            /* re */
      9                                /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 7,                            /* re */
      1                                /* im */
    }, { 7,                            /* re */
      3                                /* im */
    }, { 7,                            /* re */
      7                                /* im */
    }, { 7,                            /* re */
      5                                /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 5,                            /* re */
      -9                               /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 5,                            /* re */
      -1                               /* im */
    }, { 5,                            /* re */
      -3                               /* im */
    }, { 5,                            /* re */
      -7                               /* im */
    }, { 5,                            /* re */
      -5                               /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 5,                            /* re */
      9                                /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 5,                            /* re */
      1                                /* im */
    }, { 5,                            /* re */
      3                                /* im */
    }, { 5,                            /* re */
      7                                /* im */
    }, { 5,                            /* re */
      5                                /* im */
    } };

  static const cint8_T sigConst[352] = { { -15,/* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -15,                          /* re */
      -9                               /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -15,                          /* re */
      -1                               /* im */
    }, { -15,                          /* re */
      -3                               /* im */
    }, { -15,                          /* re */
      -7                               /* im */
    }, { -15,                          /* re */
      -5                               /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -15,                          /* re */
      9                                /* im */
    }, { -15,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -15,                          /* re */
      1                                /* im */
    }, { -15,                          /* re */
      3                                /* im */
    }, { -15,                          /* re */
      7                                /* im */
    }, { -15,                          /* re */
      5                                /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -13,                          /* re */
      -9                               /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -13,                          /* re */
      -1                               /* im */
    }, { -13,                          /* re */
      -3                               /* im */
    }, { -13,                          /* re */
      -7                               /* im */
    }, { -13,                          /* re */
      -5                               /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -13,                          /* re */
      9                                /* im */
    }, { -13,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -13,                          /* re */
      1                                /* im */
    }, { -13,                          /* re */
      3                                /* im */
    }, { -13,                          /* re */
      7                                /* im */
    }, { -13,                          /* re */
      5                                /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -9,                           /* re */
      -9                               /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -9,                           /* re */
      -1                               /* im */
    }, { -9,                           /* re */
      -3                               /* im */
    }, { -9,                           /* re */
      -7                               /* im */
    }, { -9,                           /* re */
      -5                               /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -9,                           /* re */
      9                                /* im */
    }, { -9,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -9,                           /* re */
      1                                /* im */
    }, { -9,                           /* re */
      3                                /* im */
    }, { -9,                           /* re */
      7                                /* im */
    }, { -9,                           /* re */
      5                                /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -11,                          /* re */
      -9                               /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -11,                          /* re */
      -1                               /* im */
    }, { -11,                          /* re */
      -3                               /* im */
    }, { -11,                          /* re */
      -7                               /* im */
    }, { -11,                          /* re */
      -5                               /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -11,                          /* re */
      9                                /* im */
    }, { -11,                          /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -11,                          /* re */
      1                                /* im */
    }, { -11,                          /* re */
      3                                /* im */
    }, { -11,                          /* re */
      7                                /* im */
    }, { -11,                          /* re */
      5                                /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -1,                           /* re */
      -9                               /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -1,                           /* re */
      -1                               /* im */
    }, { -1,                           /* re */
      -3                               /* im */
    }, { -1,                           /* re */
      -7                               /* im */
    }, { -1,                           /* re */
      -5                               /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -1,                           /* re */
      9                                /* im */
    }, { -1,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -1,                           /* re */
      1                                /* im */
    }, { -1,                           /* re */
      3                                /* im */
    }, { -1,                           /* re */
      7                                /* im */
    }, { -1,                           /* re */
      5                                /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -3,                           /* re */
      -9                               /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -3,                           /* re */
      -1                               /* im */
    }, { -3,                           /* re */
      -3                               /* im */
    }, { -3,                           /* re */
      -7                               /* im */
    }, { -3,                           /* re */
      -5                               /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -3,                           /* re */
      9                                /* im */
    }, { -3,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -3,                           /* re */
      1                                /* im */
    }, { -3,                           /* re */
      3                                /* im */
    }, { -3,                           /* re */
      7                                /* im */
    }, { -3,                           /* re */
      5                                /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -7,                           /* re */
      -9                               /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -7,                           /* re */
      -1                               /* im */
    }, { -7,                           /* re */
      -3                               /* im */
    }, { -7,                           /* re */
      -7                               /* im */
    }, { -7,                           /* re */
      -5                               /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -7,                           /* re */
      9                                /* im */
    }, { -7,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -7,                           /* re */
      1                                /* im */
    }, { -7,                           /* re */
      3                                /* im */
    }, { -7,                           /* re */
      7                                /* im */
    }, { -7,                           /* re */
      5                                /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { -5,                           /* re */
      -9                               /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { -5,                           /* re */
      -1                               /* im */
    }, { -5,                           /* re */
      -3                               /* im */
    }, { -5,                           /* re */
      -7                               /* im */
    }, { -5,                           /* re */
      -5                               /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { -5,                           /* re */
      9                                /* im */
    }, { -5,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { -5,                           /* re */
      1                                /* im */
    }, { -5,                           /* re */
      3                                /* im */
    }, { -5,                           /* re */
      7                                /* im */
    }, { -5,                           /* re */
      5                                /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 15,                           /* re */
      -9                               /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 15,                           /* re */
      -1                               /* im */
    }, { 15,                           /* re */
      -3                               /* im */
    }, { 15,                           /* re */
      -7                               /* im */
    }, { 15,                           /* re */
      -5                               /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 15,                           /* re */
      9                                /* im */
    }, { 15,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 15,                           /* re */
      1                                /* im */
    }, { 15,                           /* re */
      3                                /* im */
    }, { 15,                           /* re */
      7                                /* im */
    }, { 15,                           /* re */
      5                                /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 13,                           /* re */
      -9                               /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 13,                           /* re */
      -1                               /* im */
    }, { 13,                           /* re */
      -3                               /* im */
    }, { 13,                           /* re */
      -7                               /* im */
    }, { 13,                           /* re */
      -5                               /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 13,                           /* re */
      9                                /* im */
    }, { 13,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 13,                           /* re */
      1                                /* im */
    }, { 13,                           /* re */
      3                                /* im */
    }, { 13,                           /* re */
      7                                /* im */
    }, { 13,                           /* re */
      5                                /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 9,                            /* re */
      -9                               /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 9,                            /* re */
      -1                               /* im */
    }, { 9,                            /* re */
      -3                               /* im */
    }, { 9,                            /* re */
      -7                               /* im */
    }, { 9,                            /* re */
      -5                               /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 9,                            /* re */
      9                                /* im */
    }, { 9,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 9,                            /* re */
      1                                /* im */
    }, { 9,                            /* re */
      3                                /* im */
    }, { 9,                            /* re */
      7                                /* im */
    }, { 9,                            /* re */
      5                                /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 11,                           /* re */
      -9                               /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 11,                           /* re */
      -1                               /* im */
    }, { 11,                           /* re */
      -3                               /* im */
    }, { 11,                           /* re */
      -7                               /* im */
    }, { 11,                           /* re */
      -5                               /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 11,                           /* re */
      9                                /* im */
    }, { 11,                           /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 11,                           /* re */
      1                                /* im */
    }, { 11,                           /* re */
      3                                /* im */
    }, { 11,                           /* re */
      7                                /* im */
    }, { 11,                           /* re */
      5                                /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 1,                            /* re */
      -9                               /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 1,                            /* re */
      -1                               /* im */
    }, { 1,                            /* re */
      -3                               /* im */
    }, { 1,                            /* re */
      -7                               /* im */
    }, { 1,                            /* re */
      -5                               /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 1,                            /* re */
      9                                /* im */
    }, { 1,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 1,                            /* re */
      1                                /* im */
    }, { 1,                            /* re */
      3                                /* im */
    }, { 1,                            /* re */
      7                                /* im */
    }, { 1,                            /* re */
      5                                /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 3,                            /* re */
      -9                               /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 3,                            /* re */
      -1                               /* im */
    }, { 3,                            /* re */
      -3                               /* im */
    }, { 3,                            /* re */
      -7                               /* im */
    }, { 3,                            /* re */
      -5                               /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 3,                            /* re */
      9                                /* im */
    }, { 3,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 3,                            /* re */
      1                                /* im */
    }, { 3,                            /* re */
      3                                /* im */
    }, { 3,                            /* re */
      7                                /* im */
    }, { 3,                            /* re */
      5                                /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 7,                            /* re */
      -9                               /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 7,                            /* re */
      -1                               /* im */
    }, { 7,                            /* re */
      -3                               /* im */
    }, { 7,                            /* re */
      -7                               /* im */
    }, { 7,                            /* re */
      -5                               /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 7,                            /* re */
      9                                /* im */
    }, { 7,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 7,                            /* re */
      1                                /* im */
    }, { 7,                            /* re */
      3                                /* im */
    }, { 7,                            /* re */
      7                                /* im */
    }, { 7,                            /* re */
      5                                /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -15                              /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -13                              /* im */
    }, { 5,                            /* re */
      -9                               /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      -11                              /* im */
    }, { 5,                            /* re */
      -1                               /* im */
    }, { 5,                            /* re */
      -3                               /* im */
    }, { 5,                            /* re */
      -7                               /* im */
    }, { 5,                            /* re */
      -5                               /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      15                               /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      13                               /* im */
    }, { 5,                            /* re */
      9                                /* im */
    }, { 5,                            /* re */
      0                                /* im */
    }, { 0,                            /* re */
      11                               /* im */
    }, { 5,                            /* re */
      1                                /* im */
    }, { 5,                            /* re */
      3                                /* im */
    }, { 5,                            /* re */
      7                                /* im */
    }, { 5,                            /* re */
      5                                /* im */
    } };

  struct_T r;
  double varargin_1[352];
  double channel_resp_im;
  double channel_resp_re;
  double dRef_im;
  double dRef_re;
  double im;
  double numTrainSymbols;
  double re;
  double ref_im;
  double ref_re;
  double symbolCounter;
  double weightUpdateCounter;
  int idx;
  int p;
  if (!isInitialized_HS_EWL_LineEqualizer) {
    HS_EWL_LineEqualizer_initialize();
  }

  if (!LineEqualizerStr_not_empty) {
    LineEqualizerStr.isInitialized = 0;
    LineEqualizerStr.StepSize = 0.0001;
    LineEqualizerStr_not_empty = true;
  }

  /* EqualizedData = zeros(buf_size, 1); */
  if (LineEqualizerStr.isInitialized != 1) {
    LineEqualizerStr.isInitialized = 1;
    LineEqualizerStr.TrainingDelayBuffer.Length = 3.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[0].re = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[0].im = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[1].re = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[1].im = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[2].re = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[2].im = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Pointer = 1.0;
    LineEqualizerStr.ActiveDelay = 3.0;
    memset(&LineEqualizerStr.Weights[0], 0, 13U * sizeof(creal_T));
    memset(&LineEqualizerStr.TapDelayLine[0], 0, 13U * sizeof(creal_T));
    LineEqualizerStr.SymbolCounter = 0.0;
    LineEqualizerStr.NumTrainingSymbols = 0.0;
    LineEqualizerStr.WeightUpdateCounter = 0.0;
    LineEqualizerStr.PreviousTrainingFlag = false;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[0].re = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[0].im = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[1].re = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[1].im = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[2].re = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Buffer[2].im = 0.0;
    LineEqualizerStr.TrainingDelayBuffer.Pointer = 1.0;
    memset(&LineEqualizerStr.Weights[0], 0, 13U * sizeof(creal_T));
    LineEqualizerStr.ReferenceSymbol.re = 2.2204460492503131E-16;
    LineEqualizerStr.ReferenceSymbol.im = 0.0;
  }

  symbolCounter = LineEqualizerStr.SymbolCounter;
  weightUpdateCounter = LineEqualizerStr.WeightUpdateCounter;
  numTrainSymbols = LineEqualizerStr.NumTrainingSymbols;
  if (trainingFlag) 
  {
    if (!LineEqualizerStr.PreviousTrainingFlag) 
    {
      numTrainSymbols = LineEqualizerStr.ActiveDelay + 256.0;
      symbolCounter = 0.0;
    } 
    else 
    {
      numTrainSymbols = LineEqualizerStr.NumTrainingSymbols + 256.0;
    }
  }

  LineEqualizerStr.PreviousTrainingFlag = trainingFlag;
  ref_re = LineEqualizerStr.ReferenceSymbol.re;
  ref_im = LineEqualizerStr.ReferenceSymbol.im;
  memcpy(&channel_resp[0], &LineEqualizerStr.Weights[0], 13U * sizeof(creal_T));
  r = LineEqualizerStr.TrainingDelayBuffer;

  for (p = 0; p < 256; p++) 
  {
    for (idx = 0; idx < 12; idx++) 
    {
      LineEqualizerStr.TapDelayLine[12 - idx] = LineEqualizerStr.TapDelayLine[11
        - idx];
    }

    LineEqualizerStr.TapDelayLine[0] = input_buf[p];
    symbolCounter++;
    weightUpdateCounter++;
    channel_resp_re = 0.0;
    channel_resp_im = 0.0;
    for (idx = 0; idx < 13; idx++) 
    {
      ref_re = channel_resp[idx].re;
      ref_im = -channel_resp[idx].im;
      dRef_re = LineEqualizerStr.TapDelayLine[idx].re;
      dRef_im = LineEqualizerStr.TapDelayLine[idx].im;
      channel_resp_re += ref_re * dRef_re - ref_im * dRef_im;
      channel_resp_im += ref_re * dRef_im + ref_im * dRef_re;
    }

    if (symbolCounter <= numTrainSymbols) 
    {
      if (LineEqualizerStr.TrainingDelayBuffer.Length > 0.0) 
      {
        ref_re = r.Buffer[(int)r.Pointer - 1].re;
        ref_im = r.Buffer[(int)r.Pointer - 1].im;
        r.Buffer[(int)r.Pointer - 1] = training_buf[p];
        r.Pointer++;
        if (r.Pointer > LineEqualizerStr.TrainingDelayBuffer.Length) 
        {
          r.Pointer = 1.0;
        }
      } 
      else 
      {
        ref_re = training_buf[p].re;
        ref_im = training_buf[p].im;
      }

      if (symbolCounter <= LineEqualizerStr.ActiveDelay) 
      {
        for (idx = 0; idx < 352; idx++) 
        {
          varargin_1[idx] = rt_hypotd_snf((double)sigConst[idx].re -
            channel_resp_re, (double)sigConst[idx].im - channel_resp_im);
        }

        coder::internal::minimum_for_line_equlation(varargin_1, &ref_re, &idx);
        ref_re = icv[idx - 1].re;
        ref_im = icv[idx - 1].im;
      }
    } 
    else 
    {
      for (idx = 0; idx < 352; idx++) 
      {
        varargin_1[idx] = rt_hypotd_snf((double)sigConst[idx].re -
          channel_resp_re, (double)sigConst[idx].im - channel_resp_im);
      }

      coder::internal::minimum_for_line_equlation(varargin_1, &ref_re, &idx);
      ref_re = icv[idx - 1].re;
      ref_im = icv[idx - 1].im;
    }

    if (trainingFlag || (weightUpdateCounter == 1.0)) 
    {
      weightUpdateCounter = 0.0;
      dRef_re = ref_re - channel_resp_re;
      dRef_im = -(ref_im - channel_resp_im);
      for (idx = 0; idx < 13; idx++) 
      {
        re = LineEqualizerStr.StepSize * LineEqualizerStr.TapDelayLine[idx].re;
        im = LineEqualizerStr.StepSize * LineEqualizerStr.TapDelayLine[idx].im;
        channel_resp[idx].re += re * dRef_re - im * dRef_im;
        channel_resp[idx].im += re * dRef_im + im * dRef_re;
      }
    }

    EqualizedData[p].re = channel_resp_re;
    EqualizedData[p].im = channel_resp_im;
  }

  LineEqualizerStr.TrainingDelayBuffer = r;
  memcpy(&LineEqualizerStr.Weights[0], &channel_resp[0], 13U * sizeof(creal_T));
  LineEqualizerStr.ReferenceSymbol.re = ref_re;
  LineEqualizerStr.ReferenceSymbol.im = ref_im;
  LineEqualizerStr.SymbolCounter = symbolCounter;
  LineEqualizerStr.WeightUpdateCounter = weightUpdateCounter;
  LineEqualizerStr.NumTrainingSymbols = numTrainSymbols;
}

/*
 * Arguments    : void
 * Return Type  : void
 */
void HS_EWL_LineEqualizer_init(void)
{
  LineEqualizerStr_not_empty = false;
}

/*
 * File trailer for HS_EWL_LineEqualizer.c
 *
 * [EOF]
 */
