
#include <debug.h>

#include "add.h"

struct mat4 mat4_add(
	struct mat4 x,
	struct mat4 y)
{
	return (struct mat4) {
		x.a + y.a,
		x.b + y.b,
		x.c + y.c,
		x.d + y.d,
		
		x.e + y.e,
		x.f + y.f,
		x.g + y.g,
		x.h + y.h,
		
		x.i + y.h,
		x.j + y.j,
		x.k + y.k,
		x.l + y.l,
		
		x.m + y.m,
		x.n + y.n,
		x.o + y.o,
		x.p + y.p,
	};
}
