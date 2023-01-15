
#include <debug.h>

#include "multiply.h"

struct mat4 mat4_multiply(
	struct mat4 a,
	struct mat4 b)
{
	return (struct mat4) {
		a.a * b.a + a.b * b.e + a.c * b.i + a.d * b.m,
		a.a * b.b + a.b * b.f + a.c * b.j + a.d * b.n,
		a.a * b.c + a.b * b.g + a.c * b.k + a.d * b.o,
		a.a * b.d + a.b * b.h + a.c * b.l + a.d * b.p,
		
		a.e * b.a + a.f * b.e + a.g * b.i + a.h * b.m,
		a.e * b.b + a.f * b.f + a.g * b.j + a.h * b.n,
		a.e * b.c + a.f * b.g + a.g * b.k + a.h * b.o,
		a.e * b.d + a.f * b.h + a.g * b.l + a.h * b.p,
		
		a.i * b.a + a.j * b.e + a.k * b.i + a.l * b.m,
		a.i * b.b + a.j * b.f + a.k * b.j + a.l * b.n,
		a.i * b.c + a.j * b.g + a.k * b.k + a.l * b.o,
		a.i * b.d + a.j * b.h + a.k * b.l + a.l * b.p,
		
		a.m * b.a + a.n * b.e + a.o * b.i + a.p * b.m,
		a.m * b.b + a.n * b.f + a.o * b.j + a.p * b.n,
		a.m * b.c + a.n * b.g + a.o * b.k + a.p * b.o,
		a.m * b.d + a.n * b.h + a.o * b.l + a.p * b.p,
	};
}

struct vec4 mat4_multiply_with_vec4(
	struct mat4 a,
	struct vec4 b)
{
	return (struct vec4) {
		a.a * b.x + a.b * b.y + a.c * b.z + a.d * b.w,
		
		a.e * b.x + a.f * b.y + a.g * b.z + a.h * b.w,
		
		a.i * b.x + a.j * b.y + a.k * b.z + a.l * b.w,
		
		a.m * b.x + a.n * b.y + a.o * b.z + a.p * b.w,
	};
}











