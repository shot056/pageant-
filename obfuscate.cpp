﻿/**
   obfuscate.cpp

   Copyright (c) 2017 zmatsuo

   This software is released under the MIT License.
   http://opensource.org/licenses/mit-license.php
*/


#include <string.h>

#include "ssh.h"	// base64_decode_atom()
#include "obfuscate.h"

/* generate random number by MT(http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/mt19937ar.html) */

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static void random_mask(char* buffer, size_t length)
{
    /* static variables of MT */
    unsigned long mt[N]; /* the array for the state vector  */
    int mti=N+1; /* mti==N+1 means mt[N] is not initialized */
    unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    char* dst = buffer;
    unsigned long s = 0xf92fedb5UL;
    /* initializes mt[N] with a seed */
    /* void init_genrand(unsigned long s) */
    {
		mt[0]= s & 0xffffffffUL;
		for (mti=1; mti<N; mti++) {
			mt[mti] = 
				(1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
			/* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
			/* In the previous versions, MSBs of the seed affect   */
			/* only MSBs of the array mt[].                        */
			/* 2002/01/09 modified by Makoto Matsumoto             */
			mt[mti] &= 0xffffffffUL;
			/* for >32 bit machines */
		}
    }
    while (length-- > 0) {
		unsigned long y;
		/* generates a random number on [0,0xffffffff]-interval */
		/* unsigned long genrand_int32(void) */
		{
			/* unsigned long y; */
			/* static unsigned long mag01[2]={0x0UL, MATRIX_A}; */
			/* mag01[x] = x * MATRIX_A  for x=0,1 */

			if (mti >= N) { /* generate N words at one time */
				int kk;

				/* if (mti == N+1)   /* if init_genrand() has not been called, */
				/* init_genrand(5489UL); /* a default initial seed is used */

				for (kk=0;kk<N-M;kk++) {
					y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
					mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
				}
				for (;kk<N-1;kk++) {
					y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
					mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
				}
				y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
				mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

				mti = 0;
			}
  
			y = mt[mti++];

			/* Tempering */
			y ^= (y >> 11);
			y ^= (y << 7) & 0x9d2c5680UL;
			y ^= (y << 15) & 0xefc60000UL;
			y ^= (y >> 18);

			/* return y; */
		}
        *dst++ ^= (y >> 4);
    }
}

size_t encrypto(char* original, char* buffer)
{
    size_t length = strlen(original);
    if (buffer != nullptr) {
		random_mask(original, length);
		while (length > 0) {
			size_t n = (length < 3 ? length : 3);
			base64_encode_atom((unsigned char *)original, (int)n, buffer);
			original += n;
			length -= n;
			buffer += 4;
		}
		*buffer = '\0';
    }
    return (length + 2) / 3 * 4;
}

size_t decrypto(const char* encrypted, char* buffer)
{
    size_t encrypted_len = strlen(encrypted);
    if (buffer == nullptr) {
        return encrypted_len * 3 / 4;
    }else{
		const char* src = encrypted;
		const char* src_end = encrypted + encrypted_len;
		char* dst = buffer;
		while (src < src_end) {
			int k = base64_decode_atom((char*) src, (unsigned char *)dst);
			src += 4;
			dst += k;
		}
        random_mask(buffer, dst - buffer);
		*dst = '\0';
		return dst - buffer;
    }
}

sstring decrypto(const sstring &s)
{
	size_t original_len = decrypto(s.c_str(), nullptr);
	sstring decrypted_str(original_len+1, '\0');
	original_len = decrypto(s.c_str(), &decrypted_str[0]);
	decrypted_str.resize(original_len);
	return decrypted_str;
}

sstring encrypto(const sstring &s)
{
	size_t plen = s.length() + 1;
	if (plen == 0) {
		return sstring();
	}
	sstring buf = s;
	buf.resize(buf.length()+1);		// add EOS space
    size_t encrypted_len = encrypto(&buf[0], nullptr);
	sstring encrpted_str(encrypted_len + 1, '\0');
    encrypto(&buf[0], &encrpted_str[0]);
	encrpted_str.pop_back();		// remove EOS
	return encrpted_str;
}

// Local Variables:
// mode: c++
// coding: utf-8-with-signature
// tab-width: 4
// End:
