/*
 * File:     general_RS.c
 * Title:    Encoder/decoder for RS codes in C
 * Authors:  Robert Morelos-Zaragoza (robert@spectra.eng.hawaii.edu)
 *           and Hari Thirumoorthy (harit@spectra.eng.hawaii.edu)
 * Date:     Aug 1995
 *
 *
 * ===============  Encoder/Decoder for RS codes in C =================
 *
 *
 * The encoding and decoding methods used in this program are based on the
 * book "Error Control Coding: Fundamentals and Applications", by Lin and
 * Costello, Prentice Hall, 1983. Most recently on "Theory and Practice of
 * Error Control Codes", by R.E. Blahut.
 *
 *
 * NOTE:
 *          The authors are not responsible for any malfunctioning of
 *          this program, nor for any damage caused by it. Please include the
 *          original program along with these comments in any redistribution.
 *
 * Portions of this program are from a Reed-Solomon encoder/decoder
 * in C, written by Simon Rockliff (simon@augean.ua.oz.au) on 21/9/89.
 *
 * COPYRIGHT NOTICE: This computer program is free for non-commercial purposes.
 * You may implement this program for any non-commercial application. You may
 * also implement this program for commercial purposes, provided that you
 * obtain my written permission. Any modification of this program is covered
 * by this copyright.
 *
 * Copyright (c) 1995.  Robert Morelos-Zaragoza and Hari Thirumoorthy.
 *                      All rights reserved.
 *
 */

/*
  Program computes the generator polynomial of a RS code. Also performs
encoding and decoding of the RS code or a shortened RS code. Compile
using one of the following options:

  In the commands below, using  the [-DNO_PRINT] option will ensure that
 the program runs without printing a lot of detail that is generally
 unnecessary. If the program is compiled without using the option a lot
 of detail will be printed. Some other options, for example changing the
 probability of symbol error are available by directly editing the program
 itself.

 1. cc general_RS.c -DNO_PRINT -lm -o rsgp
        "rsgp" computes the generator polynomial of a RS code whose
        length is "nn", dimension is "kk" and error correcting capability
        is "tt". The generator polynomial is displayed in two formats.
        In one the coefficients of the generator polynomial are given
        as exponents of the primitive element @ of the field. In the other
        the coefficients are given by their representations in the
        basis {@^7,...,@,1}.

 2. cc general_RS.c -DDEBUG -DNO_PRINT -lm -o rsfield
        "rsfield" does all that rsgp does and in addition it also
        displays the field GF(n) and the two representations of the
        field elements namely the "power of @" representation and
        the "Basis" representation.

 3. cc general_RS.c -DENCODE -DNO_PRINT -lm -o rsenc
        "rsenc" does all that rsgp does and in addition it prompts the
        user for the following data. (1) The length of the shortened
        RS code. Of course, if shortening is not desired then, the integer
        nn is entered as the length. (2) The number of codewords that are
        to be generated. (3) The name of the file into which they are to
        be stored. (4) Random number seeds that are needed to ensure true
        randomness in generation of the codewords. (The DowJones closing
        index is a good number to enter!).  These codewords can then be
        transmitted over a noisy channel and decoded using the rsdec program.

        The error correcting capability of the code cannot be set by running
        the program. It must be set in this file "general_RS.c" by changing
        the "tt" parameter. Each time the "tt" is
        changed, the dimension "kk" of the unshortened RS code changes and
        this is given by kk = nn - 2 * tt. This value of kk must be set in
        the program below. After both "tt" and "kk" are set, the program must
        be recompiled.


 4.  cc general_RS.c -DDECODE -DNO_PRINT -lm -o rsdec
        "rsdec" does all that rsgp does and in addition it prompts the user
        for the following data. (1) The length of the RS code. (2) The
        file from which the codewords are to be read. (3) The number of code
        words to read. (4) The probability of symbol error. The program then
        reads in codewords and transmits each codeword over a discrete
        memoryless channel that has a symbol error probability specified
        above. The received corrupted word is then decoded and the results
        of decoding displayed. This basically verifies that the decoding
        procedure is capable of producing the transmitted codeword provided
        the channel caused at most tt errors. Almost always when the channel
        causes more than tt errors, the decoder fails to produce a codeword.
        In such a case the decoder outputs the uncorrected information bytes
        as they were received from the channel.

5.  cc general_RS.c -DDECODE -DDECODER_DEBUG -lm -o rsddebug
        "rsddebug" is similar to rsdec and can be used to debug the
        the decoder and verify its operation. It reads in one codeword
        from a file specified by the user and allows the user to specify
        the number of errors to be caused and their locations. Then the
        decoder transmits the codeword and causes the specified number
        of errors at the specified locations. The actual value of the
        error byte is randomnly generated. The received word is the sum
        of the transmitted codeword and the error word. The decoder then
        decodes this received word and displays all the intermediate
        results and the final result as explained below.

        (1) The syndrome of the received word is shown. This consists of
        2*tt bytes {S(1),S(2), ... ,S(2*tt)}. Each S(i) is an element in
        the field GF(n). NOTE: Iff all the S(i)'s are zero, the received
        word is a codeword in the code. In such a case, the channel
        presumably caused no errors.

  INPUT:
    Change the Galois Field and error correcting capability by
    setting the appropriate global variables below including
    1. primitive polynomial 2. error correcting capability
    3. dimension 4. length.

  FUNCTIONS:
        generate_gf() generates the field.
        gen_poly() generates the generator polynomial.
        encode_rs() encodes in systematic form.
        decode_rs() errors-only decodes a vector assumed to be encoded in systematic form.
            eras_dec_rs() errors+erasures decoding of a vector assumed to be encoded in
                  systematic form.
*/

#include "rs_decoder.h"

inline int max(const int a, const int b) { return (a < b) ? b : a; }

/**** Primitive polynomial ****/
int pp [mm+1] = { 1, 0, 1, 1, 1, 0, 0, 0, 1}; /* 1+x^2+x^3+x^4+x^8 */
/* int pp [mm+1] = { 1, 1, 0, 0, 1}; */ /* 1 + x + x^4 */
/* int pp[mm+1] = {1,1,0,1}; */ /* 1 + x + x^3 */

/* generator polynomial, tables for Galois field */
unsigned char alpha_to[nn+1];
unsigned char index_of[nn+1];

int b0;  /* g(x) has roots @**b0, @**(b0+1), ... ,@^(b0+2*tt-1) */

/* data[] is the info vector, bb[] is the parity vector, recd[] is the noise corrupted received vector */
//int recd[nn];
//int data[Ks];
//int bb[nn-Ks] ;

/* generate GF(2**m) from the irreducible polynomial p(X) in p[0]..p[m]
   lookup tables:  index->polynomial form   alpha_to[] contains j=alpha**i;
                   polynomial form -> index form  index_of[j=alpha**i] = i
   alpha=2 is the primitive element of GF(2**m)
   HARI's COMMENT: (4/13/94) alpha_to[] can be used as follows:
        Let @ represent the primitive element commonly called "alpha" that
   is the root of the primitive polynomial p(x). Then in GF(2^m), for any
   0 <= i <= 2^m-2,
        @^i = a(0) + a(1) @ + a(2) @^2 + ... + a(m-1) @^(m-1)
   where the binary vector (a(0),a(1),a(2),...,a(m-1)) is the representation
   of the integer "alpha_to[i]" with a(0) being the LSB and a(m-1) the MSB. Thus for
   example the polynomial representation of @^5 would be given by the binary
   representation of the integer "alpha_to[5]".
                   Similarily, index_of[] can be used as follows:
        As above, let @ represent the primitive element of GF(2^m) that is
   the root of the primitive polynomial p(x). In order to find the power
   of @ (alpha) that has the polynomial representation
        a(0) + a(1) @ + a(2) @^2 + ... + a(m-1) @^(m-1)
   we consider the integer "i" whose binary representation with a(0) being LSB
   and a(m-1) MSB is (a(0),a(1),...,a(m-1)) and locate the entry
   "index_of[i]". Now, @^index_of[i] is that element whose polynomial
    representation is (a(0),a(1),a(2),...,a(m-1)).
   NOTE:
        The element alpha_to[2^m-1] = 0 always signifying that the
   representation of "@^infinity" = 0 is (0,0,0,...,0).
        Similarily, the element index_of[0] = -1 always signifying
   that the power of alpha which has the polynomial representation
   (0,0,...,0) is "infinity".
*/

void generate_gf()
{
    int i, mask;

    mask = 1 ;
    alpha_to[mm] = 0 ;
    for (i = 0; i < mm; i++)
    {
        alpha_to[i] = mask ;
        index_of[alpha_to[i]] = i ;
        if (pp[i]!=0) /* If pp[i] == 1 then, term @^i occurs in poly-repr of @^mm */
            alpha_to[mm] ^= mask ;  /* Bit-wise EXOR operation */
        mask <<= 1 ; /* single left-shift */
    }
    index_of[alpha_to[mm]] = mm ;
    /* Have obtained poly-repr of @^mm. Poly-repr of @^(i+1) is given by
     poly-repr of @^i shifted left one-bit and accounting for any @^mm
     term that may occur when poly-repr of @^i is shifted. */
    mask >>= 1 ;
    for (i = mm+1; i < nn; i++)
    {
        if (alpha_to[i-1] >= mask)
            alpha_to[i] = alpha_to[mm] ^ ((alpha_to[i-1]^mask)<<1) ;
        else alpha_to[i] = alpha_to[i-1]<<1 ;
        index_of[alpha_to[i]] = i ;
    }
    index_of[0] = 0xff;
}


/* Obtain the generator polynomial of the tt-error correcting, length
  nn=(2**mm -1) Reed Solomon code from the product of (X+@**(b0+i)), i = 0, ... ,(2*tt-1)
  Examples: 	If b0 = 1, tt = 1. deg(g(x)) = 2*tt = 2.
    g(x) = (x+@) (x+@**2)
        If b0 = 0, tt = 2. deg(g(x)) = 2*tt = 4.
    g(x) = (x+1) (x+@) (x+@**2) (x+@**3)

* @param[in] tt - number of errors that can be corrected
* @param[out] *g - pointer to 'generator polynomial'
*/
void gen_poly(unsigned short tt, unsigned char *g)
{
    int i, j;

    b0 = 1;
    g[0] = alpha_to[b0];//g[0] = 2;
    g[1] = 1;     /* g(x) = (X+@**b0) initially */
    for (i = 2; i <= 2*tt; i++)
    {
        g[i] = 1;
        /* Below multiply (g[0]+g[1]*x + ... +g[i]x^i) by (@**(b0+i-1) + x) */
        for (j=i-1; j>0; j--)
            if (g[j] != 0)
                g[j] = g[j-1]^ alpha_to[((index_of[g[j]])+b0+i-1)%nn] ;
            else
                g[j] = g[j-1] ;
        g[0] = alpha_to[((index_of[g[0]])+b0+i-1)%nn] ;     /* g[0] can never be zero */
    }
    /* convert g[] to index form for quicker encoding */
    for (i=0; i <= 2*tt; i++)
        g[i] = index_of[g[i]];
}


/* @brief Performs ERRORS-ONLY decoding of RS codes. If decoding is successful,
*         writes the codeword into recd[] itself. Otherwise recd[] is unaltered.
*         If channel caused no more than "tt" errors, the tranmitted codeword will
*         be recovered.
* @param *recd - pointer to 255 bytes of received data to be rs-decoded
*/
int decode_rs1(unsigned char* recd)
    {
    int i,j;
    /* переводим recd[i] в индексную форму */
    for(i = 0; i < nn; i++)
        recd[i] = index_of[recd[i]];

/*
   1) формируем синдромы ошибок в s[]
*/
    bool syn_error = false;
    int s[2*T1+1];
    for(i = 1; i <= 2*T1; i++)
        {
        s[i] = 0;
        for(j = 0; j < nn; j++)
            if(recd[j] != 0xff)
                s[i] ^= alpha_to[(recd[j] + i*j)%nn]; /* recd[j] в индексной форме */
        if(s[i] != 0)
            syn_error = true;        /* установить флаг ошибки */
        /* преобразовать синдром из полиномиальной в индексную форму */
        s[i] = index_of[s[i]];
        }

    if(!syn_error)       /* нет ненулевых синдромов => ошибок нет */
        {
        for(i = 0; i < nn; i++)        /* преобразвать recd[] обратно */
            recd[i] = alpha_to[recd[i]];
        return 0;
        }

    /* есть ошибки, пытаемся исправить */

/*
   2) Вычисляем полином локатора ошибки с помощью итеративного алгоритма Берлекампа

   d[u]   'мю'-тое расхождение, где u='мю'+1 и 'мю' номер шага от -1 до 2*T1,
   l[u]   степень elp на данном шаге, и
   u_l[u] разница между номером шага и степенью elp.
*/

    int elp[2*T1+2][2*T1], d[2*T1+2], l[2*T1+2], u_lu[2*T1+2];
    /* инициализация таблицы */
    d[0] = 0;			/* индексная форма */
    d[1] = s[1];		/* индексная форма */
    elp[0][0] = 0;		/* индексная форма */
    elp[1][0] = 1;		/* полиномиальная форма */
    for(i = 1; i < 2*T1; i++)
        {
        elp[0][i] = -1;	/* индексная форма */
        elp[1][i] = 0;	/* полиномиальная форма */
        }
    l[0] = 0;
    l[1] = 0;
    u_lu[0] = -1;
    u_lu[1] = 0;

    int u = 0;
    do
        {
        u++;
        if(d[u] == 0xff)
            {
            l[u+1] = l[u];
            for(i = 0; i <= l[u]; i++)
                {
                elp[u+1][i] = elp[u][i];
                elp[u][i] = index_of[elp[u][i]];
                }
            }
        else		/* ищем слова с наибольшим u_lu[q] для которых d[q]!=0 */
            {
            int q = u-1;
            while((d[q] == 0xff) && (q > 0))
                q--;
            /* найден первый ненулевой d[q]  */
            if(q > 0)
                {
                int j = q;
                do
                    {
                    j--;
                    if((d[j] != -1) && (u_lu[q] < u_lu[j]))
                        q = j;
                    } while(j > 0);
                }

            /* найден q такой, что d[u]!=0 и u_lu[q] максимально */
            /* запишем степень нового elp полинома */
            l[u+1] = max(l[u], l[q]+u-q);

            /* формируем новый elp(x) */
            for(i = 0; i < 2*T1; i++)
                elp[u+1][i] = 0;
            for(i = 0; i <= l[q]; i++)
                if(elp[q][i] != -1)
                    elp[u+1][i+u-q] = alpha_to[(d[u] + nn - d[q] + elp[q][i])%nn];
            for(i = 0; i <= l[u]; i++)
                {
                elp[u+1][i] ^= elp[u][i];
                elp[u][i] = index_of[elp[u][i]];  /* старый elp - в индексную форму */
                }
            }
        u_lu[u+1] = u-l[u+1];

        /* формируем 'мю'-тое расхождение */
        if(u < 2*T1)    /* на последней итерации расхождение не было обнаружено */
            {
            if(s[u+1] != 0xff)
                d[u+1] = alpha_to[s[u+1]];
            else
                d[u+1] = 0;
            for(i = 1; i <= l[u+1]; i++)
                if((s[u+1-i] != 0xff) && (elp[u+1][i] != 0))
                    d[u+1] ^= alpha_to[(s[u+1-i]+index_of[elp[u+1][i]])%nn];
            d[u+1] = index_of[d[u+1]];	/* d[u+1] в индексную форму */
            }
        } while((u < 2*T1) && (l[u+1] <= T1));

/* Вычисление локатора ошибки завершено */

    ++u;
    if(l[u] > T1)		/* степень elp >T1 - решение невозможно */
        {
        for(i = 0; i < nn; i++)
            recd[i] = alpha_to[recd[i]];
        return 3;		/* флаг ошибки */
        }

    /* можно исправить ошибки */
    for(i = 0; i <= l[u]; i++)	/* elp в индексную форму */
        elp[u][i] = index_of[elp[u][i]];

/*
   3) находим корни полинома локатора ошибки
 */

    int reg[T1+1];
    for(i = 1; i <= l[u]; i++)
        reg[i] = elp[u][i];

    int count = 0;
    int root[T1], loc[T1];
    for(i = 1; i <= nn; i++)
        {
        int q = 1;
        for(j = 1; j <= l[u]; j++)
            if(reg[j] != 0xff)
                {
                reg[j] = (reg[j]+j)%nn;
                q ^= alpha_to[reg[j]];
                }
        if(!q)        /* записываем корень и индекс номера локатора ошибки */
            {
            root[count] = i;
            loc[count] = nn-i;
            count++;
            }
        }
    if(count != l[u])    /* количество корней != степени elp => >T1 ошибок, решение невозможно */
        {
        for(i = 0; i < nn; i++)
            recd[i] = alpha_to[recd[i]];	/* преобразвать recd[] обратно */
        return 2;
        }

    /* количество корней == степени elp следовательно кол. ошибок <= T1 */

    /* формируем полином z(x) */
    int z[T1+1];
    for(i = 1; i <= l[u]; i++)	/* Z[0] = 1 всегда */
        {
        if((s[i] != 0xff) && (elp[u][i] != -1))
            z[i] = alpha_to[s[i]] ^ alpha_to[elp[u][i]];
        else if((s[i] != 0xff) && (elp[u][i] == -1))
            z[i] = alpha_to[s[i]];
        else if((s[i] == 0xff) && (elp[u][i] != -1))
            z[i] = alpha_to[elp[u][i]];
        else
            z[i] = 0;
        for(j = 1; j < i; j++)
            if((s[j] != 0xff) && (elp[u][i-j] != -1))
                z[i] ^= alpha_to[(elp[u][i-j] + s[j])%nn];
        z[i] = index_of[z[i]];		/* в индексную форму */
        }

/*
   4) вычисляем ошибки в позициях loc[i] и исправляем их
*/

    int err[nn];
    for(i = 0; i < nn; i++)
        {
        err[i] = 0;
        recd[i] = alpha_to[recd[i]];
        }
    for(i = 0; i < l[u]; i++)	/* сначала вычисляем числитель ошибки */
        {
        err[loc[i]] = 1;			/* accounts for z[0] */
        for(j = 1; j <= l[u]; j++)
            if(z[j] != 0xff)
                err[loc[i]] ^= alpha_to[(z[j]+j*root[i])%nn];
        if(err[loc[i]] != 0)
            {
            err[loc[i]] = index_of[err[loc[i]]];
            int q = 0;				/* формируем знаменатель */
            for(j = 0; j < l[u]; j++)
                if(j != i)
                    q += index_of[1^alpha_to[(loc[j]+root[i])%nn]];
            q = q % nn;
            err[loc[i]] = alpha_to[(err[loc[i]]-q+nn)%nn];
            recd[loc[i]] ^= err[loc[i]]; /* исправляем ошибку */
            }
        }
    return 1;
    }
