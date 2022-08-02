#ifndef RS_DECODER_H
#define RS_DECODER_H

#include <math.h>
#include <stdio.h>

//#define maxrand 0x7fffffff

#define mm  8           /* RS code over GF(2**mm) - change to suit */
//#define n   256         /* n = size of the field */
#define nn  255         /* nn=2**mm -1   length of codeword */

#define T1  (4)           //#define tt  16          /* number of errors that can be corrected */
#define K1  (nn - 2*T1) //#define kk  223         /* kk = nn-2*tt  */ /* Degree of g(x) = 2*tt */ /* number of data bytes in block */

void generate_gf();
void gen_poly(unsigned short tt, unsigned char *g);
//int decode_rs(unsigned short tt, unsigned short kk, unsigned char *recd);
int decode_rs1(unsigned char *recd);

#endif // RS_DECODER_H
