/*
 * This file is part of the DXX-Rebirth project <http://www.dxx-rebirth.com/>.
 * It is copyright by its individual contributors, as recorded in the
 * project's Git history.  See COPYING.txt at the top level for license
 * terms and a link to the Git history.
 */
/* Maths.h library header file */

#pragma once

#include <stdlib.h>
#include "pstypes.h"



#define D_RAND_MAX 32767

#ifdef __cplusplus
#include <cstddef>
#include "dxxsconf.h"

void d_srand (unsigned int seed);
__attribute_warn_unused_result
int d_rand ();			// Random number function which returns in the range 0-0x7FFF


//=============================== FIXED POINT ===============================

typedef int64_t fix64;		//64 bits int, for timers
typedef int32_t fix;		//16 bits int, 16 bits frac
typedef int16_t fixang;		//angles

typedef struct quadint // integer 64 bit, previously called "quad"
  {
	  union {
		  struct {
    u_int32_t low;
    int32_t high;
		  };
		  int64_t q;
	  };
  }
quadint;


//Convert an int to a fix/fix64 and back
#define i2f(i) ((i)<<16)
#define f2i(f) ((f)>>16)

//Get the int part of a fix, with rounding
#define f2ir(f) (((f)+f0_5)>>16)

//Convert fix to float and float to fix
#define f2fl(f) (static_cast<float>(f) / 65536.0)
#define f2db(f) (static_cast<double>(f) / 65536.0)
#define fl2f(f) static_cast<fix>((f) * 65536)

//Some handy constants
#define f0_0	0
#define f1_0	0x10000
#define f2_0	0x20000
#define f3_0	0x30000
#define f10_0	0xa0000

#define f0_5 0x8000
#define f0_1 0x199a

#define F0_0	f0_0
#define F1_0	f1_0
#define F2_0	f2_0
#define F3_0	f3_0
#define F10_0	f10_0

#define F0_5 	f0_5
#define F0_1 	f0_1

//multiply two fixes, return a fix(64)
__attribute_warn_unused_result
fix64 fixmul64 (fix a, fix b);

/* On x86/amd64 for Windows/Linux, truncating fix64->fix is free. */
__attribute_warn_unused_result
static inline fix fixmul(fix a, fix b)
{
	return static_cast<fix>(fixmul64(a, b));
}

//divide two fixes, return a fix
__attribute_warn_unused_result
fix fixdiv (fix a, fix b);

//multiply two fixes, then divide by a third, return a fix
__attribute_warn_unused_result
fix fixmuldiv (fix a, fix b, fix c);

//multiply two fixes, and add 64-bit product to a quadint
static inline void fixmulaccum (quadint * q, const fix &a, const fix &b)
{
	q->q += static_cast<int64_t>(a) * static_cast<int64_t>(b);
}

//extract a fix from a quadint product
__attribute_warn_unused_result
static inline fix fixquadadjust (const quadint *q)
{
	return q->q >> 16;
}

//negate a quadint
static inline void fixquadnegate (quadint * q)
{
	q->q = -q->q;
}

//computes the square root of a long, returning a short
__attribute_warn_unused_result
ushort long_sqrt (int32_t a);

//computes the square root of a quadint, returning a long
__attribute_warn_unused_result
u_int32_t quad_sqrt (quadint);

//computes the square root of a fix, returning a fix
__attribute_warn_unused_result
fix fix_sqrt (fix a);

//compute sine and cosine of an angle, filling in the variables
//either of the pointers can be NULL
void fix_sincos (fix a, fix * s, fix * c);	//with interpolation
void fix_sincos(fix, std::nullptr_t, std::nullptr_t) = delete;

__attribute_warn_unused_result
static inline fix fix_sin(fix a)
{
	fix s;
	return fix_sincos(a, &s, nullptr), s;
}

__attribute_warn_unused_result
static inline fix fix_cos(fix a)
{
	fix c;
	return fix_sincos(a, nullptr, &c), c;
}

void fix_fastsincos (fix a, fix * s, fix * c);	//no interpolation

//compute inverse sine & cosine
__attribute_warn_unused_result
fixang fix_asin (fix v);

__attribute_warn_unused_result
fixang fix_acos (fix v);

//given cos & sin of an angle, return that angle.
//parms need not be normalized, that is, the ratio of the parms cos/sin must
//equal the ratio of the actual cos & sin for the result angle, but the parms 
//need not be the actual cos & sin.  
//NOTE: this is different from the standard C atan2, since it is left-handed.
__attribute_warn_unused_result
fixang fix_atan2 (fix cos, fix sin);

__attribute_warn_unused_result
int checkmuldiv(fix *r,fix a,fix b,fix c);

extern const ubyte guess_table[256];
extern const short sincos_table[321];
extern const ushort asin_table[258];
extern const ushort acos_table[258];


static inline void clamp_fix_lh(fix& f, const fix& low, const fix& high)
{
	if (f < low)
		f = low;
	else if (high < f)
		f = high;
}

static inline void clamp_fix_symmetric(fix& f, const fix& bound)
{
	clamp_fix_lh(f, -bound, bound);
}
#endif
